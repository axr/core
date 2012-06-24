/********************************************************************
 *             a  A                                                        
 *            AM\/MA                                                         
 *           (MA:MMD                                                         
 *            :: VD
 *           ::  º                                                         
 *          ::                                                              
 *         ::   **      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6             
 +       6::Z. TMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6            
 *      6M:AMMJMMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6              
 *      ::  TMMTMC         ___MMMM     VMMMMMMM6       MMMM                   
 *     MMM  TMMMTTM,     AMMMMMMMM      VMMMMM6        MMMM                  
 *    :: MM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM                   
 *   ::   MMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM                   
 *  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM                   
 *         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM                   
 *        TMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM                   
 *       TMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM                   
 *      TMMM MMMM                                                           
 *     TMMM  .MMM                                         
 *     TMM   .MMD       ARBITRARY·······XML········RENDERING                           
 *     TMM    MMA       ====================================                              
 *     TMN    MM                               
 *      MN    ZM                       
 *            MM,
 *
 * 
 *      AUTHORS: Miro Keller
 *      
 *      COPYRIGHT: ©2011 - All Rights Reserved
 *
 *      LICENSE: see License.txt file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2012/03/26
 *      HSS version: 1.0
 *      Core version: 0.46
 *      Revision: 12
 *
 ********************************************************************/

/**
 *  @todo do something meaningful with the error codes
 */

#include "../hss.h"
#include "../parsing/HSSParserExceptions.h"
#include <iostream>
#include <sstream>
#include "../../axr/AXRDebugging.h"
#include "../../axr/errors/errors.h"

using namespace AXR;

HSSTokenizer::HSSTokenizer()
{
    //these start out empty
	this->currentChar = NULL;
	
	//initialize the currentTokenText
	this->currentTokenText = std::string();
	
	//we start out with an empty buffer
	this->buflen = 0;
	//we start at the beginning of the buffer
	this->bufpos = 0;
    //the peeking offset is 0
    this->peekpos = this->peekColumn = this->peekLine =  0;
    //we start at line 1 and column 1
    this->currentLine = this->currentColumn = 1;
    
    //by default, numbers are read as real numbers and A-F will be an identifier
    this->preferHex = false;
}

HSSTokenizer::~HSSTokenizer()
{
    
}

void HSSTokenizer::reset()
{
    
    this->file.reset();
    //this starts out empty
	this->currentChar = NULL;
	
	//initialize the currentTokenText
	this->currentTokenText = std::string();
	
	//we start out at the beginning of an empty buffer
	this->buflen = this->bufpos = this->peekpos = 0;
    //we start at line 0 and column 0
    this->currentLine = this->currentColumn = 1;
    
    //by default, numbers are read as real numbers and A-F will be an identifier
    this->preferHex = false;
}

void HSSTokenizer::setFile(AXRFile::p file) { this->file = file; }
AXRFile::p HSSTokenizer::getFile() { return this->file; }

HSS_TOKENIZING_STATUS HSSTokenizer::readNextChar()
{
	if(this->buflen == this->bufpos){
		this->currentChar = '\0';
	} else {
        char * buffer = this->file->getBuffer();
		this->currentChar = buffer[this->bufpos];
	}
#if AXR_DEBUG_LEVEL > 3
    std::ostringstream tempstream;
    tempstream << this->bufpos;
    std_log4("read charachter "+tempstream.str()+"|" << this->currentChar << "|");
#endif
	this->bufpos++;
    this->currentColumn++;
    
	
	return HSSTokenizerOK;
}

HSSToken::p HSSTokenizer::readNextToken()
{
    HSSToken::p ret;
    
	//if we're at the end of the source
	if (this->atEndOfSource()) {
		return ret;
	}
	
	char cc = this->currentChar;
    
	//identifiers can start with a letter or an underscore
	if (isalpha(cc) || cc == '_') {
        if(this->preferHex){
            return this->readHexOrIdentifier();
        } else {
            return this->readIdentifier();
        }
	}
	
    //the \302 is the character that appears when pressing
    //alt+space in TextMate, a weird unicode space
	if (isspace(cc) || cc == '\302') {
		return this->readWhitespace();
	}
	
	//if it starts with a number it is either a number or a percentage
	if (isdigit(cc)) {
        if(this->preferHex){
            return this->readHexOrIdentifier();
        } else {
            return this->readNumberOrPercentage();
        }
	}
	
	switch (cc) {
		//if it starts with quotes, either single or double, it is a string
		case '"':
		case '\'':
			return this->readString();
        case '#':
            ret = HSSToken::p(new HSSToken(HSSInstructionSign, this->currentLine, this->currentColumn -1));
            this->readNextChar();
            return ret;
		case '@':
			ret = HSSToken::p(new HSSToken(HSSObjectSign, this->currentLine, this->currentColumn -1));
			this->readNextChar();
			return ret;
		case '{':
			ret = HSSToken::p(new HSSToken(HSSBlockOpen, this->currentLine, this->currentColumn -1));
			this->readNextChar();
			return ret;
		case '}':
			ret = HSSToken::p(new HSSToken(HSSBlockClose, this->currentLine, this->currentColumn -1));
			this->readNextChar();
			return ret;
        case ',':
			ret = HSSToken::p(new HSSToken(HSSComma, this->currentLine, this->currentColumn -1));
			this->readNextChar();
			return ret;
        case ':':
			ret = HSSToken::p(new HSSToken(HSSColon, this->currentLine, this->currentColumn -1));
			this->readNextChar();
			return ret;
		case ';':
			ret = HSSToken::p(new HSSToken(HSSEndOfStatement, this->currentLine, this->currentColumn -1));
			this->readNextChar();
			return ret;
		case '(':
			ret = HSSToken::p(new HSSToken(HSSParenthesisOpen, this->currentLine, this->currentColumn -1));
			this->readNextChar();
			return ret;
		case ')':
			ret = HSSToken::p(new HSSToken(HSSParenthesisClose, this->currentLine, this->currentColumn -1));
			this->readNextChar();
			return ret;
        case '/':
            return this->readCommentOrSymbol();
		default:
			return this->readSymbol();
	}
}

HSSToken::p HSSTokenizer::peekNextToken()
{
    std_log4("bufpos: " << this->bufpos);
    //store the current position in the buffer
    int curpos = this->bufpos;
    unsigned curline = this->currentLine;
    unsigned curcol = this->currentColumn;
    HSSToken::p ret = this->readNextToken();
    //store the new offset
    this->peekpos += (this->bufpos - curpos);
    this->peekLine += (this->currentLine - curline);
    this->peekColumn += (this->currentColumn - curcol);
    
    std_log4("new peekpos: " << this->peekpos << " because bufpos: " << this->bufpos << " and curpos: " << curpos);
    
    return ret;
}

void HSSTokenizer::resetPeek()
{
    //restore the position in the buffer
    //we start one position before we were before, since we are re-reading the char
    this->bufpos -= this->peekpos + 1;
    this->currentLine -= this->peekLine;
    this->currentColumn -= this->peekColumn+1;
    std_log4("end bufpos: " << this->bufpos);
    this->readNextChar();
    
    //reset the offset to 0
    this->peekpos = 0;
    this->peekLine = 0;
    this->peekColumn = 0;
}

void HSSTokenizer::setBufferLength(unsigned length)
{
    this->buflen = length;
}

HSS_TOKENIZING_STATUS HSSTokenizer::skipWhitespace()
{
	while (isspace(this->currentChar) || this->currentChar == '\302'){
        //if there is this weird space char, skip another byte
        if (this->currentChar == '\302') {
            this->readNextChar();
        }
        if(this->currentChar == '\n' || this->currentChar == '\r'){
            this->currentLine++;
            this->currentColumn = 1;
        }
		this->readNextChar();
	}
	
	return HSSTokenizerOK;
}

bool HSSTokenizer::atEndOfSource()
{
	return this->currentChar == '\0';
}

HSS_TOKENIZING_STATUS HSSTokenizer::storeCurrentCharAndReadNext()
{
	this->currentTokenText.append(1, this->currentChar);
	this->readNextChar();
	
	return HSSTokenizerOK;
}

HSS_TOKENIZING_STATUS HSSTokenizer::storeChar(char value)
{
	this->currentTokenText.append(1, value);
	return HSSTokenizerOK;
}

std::string HSSTokenizer::extractCurrentTokenText()
{
	std::string tempctt = this->currentTokenText;
	this->currentTokenText.clear();
	return tempctt;
}


//reads and returns a whitespace token
HSSToken::p HSSTokenizer::readWhitespace()
{
    unsigned line = this->currentLine;
    unsigned column = this->currentColumn - 1;
    
	this->skipWhitespace();
	return HSSToken::p(new HSSToken(HSSWhitespace, line, column));
}

//reads and returns an identifier token
HSSToken::p HSSTokenizer::readIdentifier()
{
    unsigned line = this->currentLine;
    unsigned column = this->currentColumn - 1;
    
	while (isalnum(this->currentChar) || this->currentChar == '_') {
		this->storeCurrentCharAndReadNext();
	}
	return HSSValueToken::p(new HSSValueToken(HSSIdentifier, this->extractCurrentTokenText(), line, column));
}

//reads and returns a hexadecimal number
//HSSToken::p HSSTokenizer::readHex()
//{
//    security_brake_init();
//	while (1) {
//        switch (this->currentChar) {
//            case 'a':
//            case 'A':
//            case 'b':
//            case 'B':
//            case 'c':
//            case 'C':
//            case 'd':
//            case 'D':
//            case 'e':
//            case 'E':
//            case 'f':
//            case 'F':
//                this->storeCurrentCharAndReadNext();
//                continue;
//                
//            default:
//                if (isdigit(this->currentChar)){
//                    this->storeCurrentCharAndReadNext();
//                    continue;
//                    
//                } else {
//                    break;
//                }
//        }
//        security_brake();
//	}
//    
//    return HSSValueToken::p(new HSSValueToken(HSSHexNumber, this->extractCurrentTokenText()));
//}

//reads and returns a hexadecimal number or an identifier
HSSToken::p HSSTokenizer::readHexOrIdentifier()
{
    unsigned line = this->currentLine;
    unsigned column = this->currentColumn -1;
    
    security_brake_init();
    bool done = false;
    this->currentTokenText.clear();
	while (!done) {
        switch (this->currentChar) {
            case 'a':
            case 'A':
            case 'b':
            case 'B':
            case 'c':
            case 'C':
            case 'd':
            case 'D':
            case 'e':
            case 'E':
            case 'f':
            case 'F':
                this->storeCurrentCharAndReadNext();
                continue;
                
            default:
                if (isdigit(this->currentChar)){
                    this->storeCurrentCharAndReadNext();
                    continue;
                } else {
                    if(isalpha(this->currentChar)){
                        done = true;
                        break;
                    } else if (this->currentTokenText.size() > 0){
                        return HSSValueToken::p(new HSSValueToken(HSSHexNumber, this->extractCurrentTokenText(), line, column));
                    } else {
                        done = true;
                        break;
                    }
                }
        }
        security_brake();
	}
    
    //if we reached this far, it is an identifier - finish reading it
    return this->readIdentifier();
}

//reads and returns either a number or a percentage token
//the currentChar is assumed to be a number
HSSToken::p HSSTokenizer::readNumberOrPercentage()
{
    unsigned line = this->currentLine;
    unsigned column = this->currentColumn -1;
    bool dotFound = false;
	while (isdigit(this->currentChar) || this->currentChar == '.'){
        if(this->currentChar == '.'){
            if(dotFound){
                break; 
            } else {
                dotFound = true;
            }
        }
        
		this->storeCurrentCharAndReadNext();
	}
    HSSToken::p ret;
   if(currentChar == '%'){
	   ret = HSSValueToken::p(new HSSValueToken(HSSPercentageNumber, this->extractCurrentTokenText(), line, column));
       this->readNextChar();
   } else {
	   ret = HSSValueToken::p(new HSSValueToken(HSSNumber, this->extractCurrentTokenText(), line, column));
   }
	return ret;
}

//reads and returns either a single quoted or double quoted string token
//the currentChar is assumed to be either " or '
HSSToken::p HSSTokenizer::readString()
{
    unsigned line = this->currentLine;
    unsigned column = this->currentColumn -1;
    
    HSSToken::p ret;
    if(this->currentChar == '"'){
        this->readNextChar();
        while (this->currentChar != '"') {
            if(this->atEndOfSource()){
                throw AXRError::p(new AXRError("HSSTokenizer", "Unexpected end of source"));
            }
            this->storeCurrentCharAndReadNext();
        }
        ret = HSSValueToken::p(new HSSValueToken(HSSDoubleQuoteString, this->extractCurrentTokenText(), line, column));
    } else if(this->currentChar == '\'') {
        this->readNextChar();
        while (this->currentChar != '\'') {
            if(this->atEndOfSource()){
                throw AXRError::p(new AXRError("HSSTokenizer", "Unexpected end of source"));
            }
            this->storeCurrentCharAndReadNext();
        }
        ret = HSSValueToken::p(new HSSValueToken(HSSSingleQuoteString, this->extractCurrentTokenText(), line, column));
    }
    
    this->readNextChar();
	return ret;
}

//reads and returns a comment or a symbol token
//assumes currentChar == '/'
HSSToken::p HSSTokenizer::readCommentOrSymbol()
{
    unsigned line = this->currentLine;
    unsigned column = this->currentColumn -1;
    
    HSSValueToken::p ret;
    this->readNextChar();
    if (this->currentChar == '/'){
        this->readNextChar(); // skip '/'
        //read all chars until end of line
        while (this->currentChar != '\n' && this->currentChar != '\r' && this->currentChar != '\f' && !this->atEndOfSource()) {
            this->storeCurrentCharAndReadNext();
        }
        ret = HSSValueToken::p(new HSSValueToken(HSSLineComment, this->extractCurrentTokenText(), line, column));
    } else if (this->currentChar == '*'){
        readNextChar(); //skip '*'
        while (1) {
            if(this->currentChar == '*'){
                readNextChar(); //we won't know if it is the end of the comment until we seek further
                if(this->currentChar == '/'){
                    //it is the end, break the loop
                    break;
                } else {
                    //it's not the end of the comment, store the char and continue reading
                    this->storeChar('*');
                }
            } else if(this->atEndOfSource()) {
                break;
            }
            
            this->storeCurrentCharAndReadNext();
        }
        ret = HSSValueToken::p(new HSSValueToken(HSSBlockComment, this->extractCurrentTokenText(), line, column));
        readNextChar();
    } else {
        ret = HSSValueToken::p(new HSSValueToken(HSSSymbol, '/', line, column));
    }
    
    return ret;
}

//reads and returns a symbol token
HSSToken::p HSSTokenizer::readSymbol()
{
    unsigned line = this->currentLine;
    unsigned column = this->currentColumn -1;
    
    HSSToken::p ret;
    
    switch (this->currentChar) {
        case '.':
            this->storeCurrentCharAndReadNext();
            if(this->currentChar == '.'){
                this->storeCurrentCharAndReadNext();
                if(this->currentChar == '.'){
                    this->storeCurrentCharAndReadNext();
                }
                
                ret = HSSValueToken::p(new HSSValueToken(HSSSymbol, this->extractCurrentTokenText(), line, column));
                return ret;
            }
            
            ret = HSSValueToken::p(new HSSValueToken(HSSSymbol, this->extractCurrentTokenText(), line, column));
            readNextChar();
            return ret;
                        
        default:
            ret = HSSValueToken::p(new HSSValueToken(HSSSymbol, this->currentChar, line, column));
            this->readNextChar();
            return ret;
    }
}

















