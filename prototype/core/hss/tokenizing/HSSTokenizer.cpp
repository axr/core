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
 *      Last changed: 2011/09/17
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 6
 *
 ********************************************************************/

//FIXME: do something meaningful with the error codes

#include "../hss.h"
#include "../parsing/HSSParserExceptions.h"
#include <iostream>
#include <sstream>
#include "../../axr/AXRDebugging.h"

using namespace AXR;

HSSTokenizer::HSSTokenizer()
{
    //create a new, empty buffer - it will be filled later
    HSSTokenizer::buf_p aBuf = HSSTokenizer::buf_p(new char[AXR_HSS_BUFFER_SIZE]);
    this->bufferlist.push_back(aBuf);
    //these start out empty
	this->currentChar = NULL;
	
	//initialize the currentTokenText
	this->currentTokenText = std::string();
	
	//store the pointer to the buffer
	this->buffer = aBuf;
	//we start out with an empty buffer
	this->buflen = 0;
	//we start at the beginning of the buffer
	this->bufpos = 0;
    //the peeking position is the same as the bufpos
    this->peekpos = 0;
    //we start at line 0 and column 0
    this->currentLine = this->currentColumn = 1;
    
    //by default, numbers are read as real numbers and A-F will be an identifier
    this->preferHex = false;
}

HSSTokenizer::HSSTokenizer(HSSTokenizer::buf_p buffer, unsigned buflen)
{
	//these start out empty
	this->currentChar = NULL;
	
	//initialize the currentTokenText
	this->currentTokenText = std::string();
	
	//store the pointer to the buffer
	this->buffer = HSSTokenizer::buf_p(buffer);
	//store the length of the buffer
	this->buflen = buflen;
	//we start at the beginning of the buffer
	this->bufpos = 0;
    //the peeking position is the same as the bufpos
    this->peekpos = 0;
    //we start at line 0 and column 0
    this->currentLine = this->currentColumn = 1;
	
	//start by reading the first character
	this->readNextChar();
    
    //by default, numbers are read as real numbers and A-F will be an identifier
    this->preferHex = false;
}

HSSTokenizer::~HSSTokenizer()
{
    this->bufferlist.clear();
}

void HSSTokenizer::reset()
{
    //empty the buffer list
    this->bufferlist.clear();
    
    //create a new, empty buffer - it will be filled later
    HSSTokenizer::buf_p aBuf = HSSTokenizer::buf_p(new char[AXR_HSS_BUFFER_SIZE]);
    this->bufferlist.push_back(aBuf);
    //this starts out empty
	this->currentChar = NULL;
	
	//initialize the currentTokenText
	this->currentTokenText = std::string();
	
	//store the pointer to the buffer
	this->buffer = aBuf;
	//we start out at the beginning of an empty buffer
	this->buflen = this->bufpos = this->peekpos = 0;
    //we start at line 0 and column 0
    this->currentLine = this->currentColumn = 1;
    
    //by default, numbers are read as real numbers and A-F will be an identifier
    this->preferHex = false;
}

HSS_TOKENIZING_STATUS HSSTokenizer::readNextChar()
{
	if(this->buflen == this->bufpos){
		this->currentChar = '\0';
	} else {
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
	
	if (isspace(cc)) {
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
            ret = HSSToken::p(new HSSToken(HSSInstructionSign));
            this->readNextChar();
            return ret;
		case '@':
			ret = HSSToken::p(new HSSToken(HSSObjectSign));
			this->readNextChar();
			return ret;
		case '{':
			ret = HSSToken::p(new HSSToken(HSSBlockOpen));
			this->readNextChar();
			return ret;
		case '}':
			ret = HSSToken::p(new HSSToken(HSSBlockClose));
			this->readNextChar();
			return ret;
        case ':':
			ret = HSSToken::p(new HSSToken(HSSColon));
			this->readNextChar();
			return ret;
		case ';':
			ret = HSSToken::p(new HSSToken(HSSEndOfStatement));
			this->readNextChar();
			return ret;
		case '(':
			ret = HSSToken::p(new HSSToken(HSSParenthesisOpen));
			this->readNextChar();
			return ret;
		case ')':
			ret = HSSToken::p(new HSSToken(HSSParenthesisClose));
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
    HSSToken::p ret = this->readNextToken();
    //store the new offset
    this->peekpos += (this->bufpos - curpos);
    std_log4("new peekpos: " << this->peekpos << " because bufpos: " << this->bufpos << " and curpos: " << curpos);
    
    return ret;
}

void HSSTokenizer::resetPeek()
{
    //restore the position in the buffer
    //we start one position before we were before, since we are re-reading the char
    this->bufpos -= this->peekpos + 1;
    std_log4("end bufpos: " << this->bufpos);
    this->readNextChar();
    
    //reset the offset to 0
    this->peekpos = 0;
}


HSSTokenizer::buf_p HSSTokenizer::getBuffer()
{
    return this->buffer;
}

void HSSTokenizer::setBufferLength(unsigned length)
{
    this->buflen = length;
}

HSS_TOKENIZING_STATUS HSSTokenizer::skipWhitespace()
{
	while (isspace(this->currentChar)){
        if(this->currentChar == '\n' || this->currentChar == '\r'){
            this->currentLine++;
            this->currentColumn = 0;
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
	this->skipWhitespace();
	return HSSToken::p(new HSSToken(HSSWhitespace));
}

//reads and returns an identifier token
HSSToken::p HSSTokenizer::readIdentifier()
{
	while (isalnum(this->currentChar)) {
		this->storeCurrentCharAndReadNext();
	}
	return HSSValueToken::p(new HSSValueToken(HSSIdentifier, this->extractCurrentTokenText()));
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
    security_brake_init();
	while (1) {
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
                    return HSSValueToken::p(new HSSValueToken(HSSHexNumber, this->extractCurrentTokenText()));
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
	while (isnumber(this->currentChar)){
		this->storeCurrentCharAndReadNext();
	}
    HSSToken::p ret;
   if(currentChar == '%'){
	   ret = HSSValueToken::p(new HSSValueToken(HSSPercentageNumber, this->extractCurrentTokenText()));
       this->readNextChar();
   } else {
	   ret = HSSValueToken::p(new HSSValueToken(HSSNumber, this->extractCurrentTokenText()));
   }
	return ret;
}

//reads and returns either a single quoted or double quoted string token
//the currentChar is assumed to be either " or '
HSSToken::p HSSTokenizer::readString()
{
    HSSToken::p ret;
    if(this->currentChar == '"'){
        this->readNextChar();
        while (this->currentChar != '"') {
            if(this->atEndOfSource()){
                //FIXME
                throw HSSUnexpectedEndOfSourceException("unknown", this->currentLine, this->currentColumn);
                return ret;
            }
            this->storeCurrentCharAndReadNext();
        }
        ret = HSSValueToken::p(new HSSValueToken(HSSDoubleQuoteString, this->extractCurrentTokenText()));
    } else if(this->currentChar == '\'') {
        this->readNextChar();
        while (this->currentChar != '\'') {
            if(this->atEndOfSource()){
                //FIXME
                throw HSSUnexpectedEndOfSourceException("unknown", this->currentLine, this->currentColumn);
                return ret;
            }
            this->storeCurrentCharAndReadNext();
        }
        ret = HSSValueToken::p(new HSSValueToken(HSSSingleQuoteString, this->extractCurrentTokenText()));
    }
    
    this->readNextChar();
	return ret;
}

//reads and returns a comment or a symbol token
//assumes currentChar == '/'
HSSToken::p HSSTokenizer::readCommentOrSymbol()
{
    HSSValueToken::p ret;
    this->readNextChar();
    if (this->currentChar == '/'){
        this->readNextChar(); // skip '/'
        //read all chars until end of line
        while (this->currentChar != '\n' && this->currentChar != '\r' && this->currentChar != '\f' && !this->atEndOfSource()) {
            this->storeCurrentCharAndReadNext();
        }
        ret = HSSValueToken::p(new HSSValueToken(HSSLineComment, this->extractCurrentTokenText()));
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
        ret = HSSValueToken::p(new HSSValueToken(HSSBlockComment, this->extractCurrentTokenText()));
        readNextChar();
    } else {
        ret = HSSValueToken::p(new HSSValueToken(HSSSymbol, '/'));
    }
    
    return ret;
}

//reads and returns a symbol token
HSSToken::p HSSTokenizer::readSymbol()
{
    HSSToken::p ret;
    
    switch (this->currentChar) {
        case '.':
            this->storeCurrentCharAndReadNext();
            if(this->currentChar == '.'){
                this->storeCurrentCharAndReadNext();
                if(this->currentChar == '.'){
                    this->storeCurrentCharAndReadNext();
                }
                
                ret = HSSValueToken::p(new HSSValueToken(HSSSymbol, this->extractCurrentTokenText()));
                return ret;
            }
            
            ret = HSSValueToken::p(new HSSValueToken(HSSSymbol, this->extractCurrentTokenText()));
            readNextChar();
            return ret;
                        
        default:
            ret = HSSValueToken::p(new HSSValueToken(HSSSymbol, this->currentChar));
            this->readNextChar();
            return ret;
    }
}

















