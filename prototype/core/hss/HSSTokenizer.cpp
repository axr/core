/********************************************************************
 *             a  A                                                        
 *            AM\/MA                                                         
 *           (MMMMMD                                                         
 *            :: VD
 *           ::  .                                                         
 *          ::                                                              
 *          M   NZ      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6             
 +       .MMZ. MMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6            
 *      AMMMMMDJMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6              
 *      MM  MMMMMC         ___MMMM     VMMMMMMM6       MMMM                   
 *      MM  MMMMMMM,     AMMMMMMMM      VMMMMM6        MMMM                  
 *     :MM AMMMMMMMD    MMMMMMMMMM       MMMMMM        MMMM                   
 *    :: MMMMMMMMMM    MMMMMMMMMMM      AMMMMMMD       MMMM                   
 *   .     MMMMM~     MMMM    MMMM     AMMMMMMMMD      MMMM                   
 *         MMMMM      MMMM    MMMM    AMMM6  MMMMD     MMMM                   
 *        MMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM                   
 *       MMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM                   
 *      MMMM MMMM                                                           
 *     AMMM  .MMM                                         
 *     MMM   .MMD       ARBITRARY·······XML········RENDERING                           
 *     MMM    MMA       ====================================                              
 *     DMN    MM                               
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
 *      Last changed: 2011/04/04
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 ********************************************************************/

//FIXME: do something meaningful with the error codes

#include "HSSTokenizer.h"
#include "HSSToken.h"
#include "HSSValueToken.h"
#include <iostream>
#include "AXR.h"
#include <sstream>

HSSTokenizer::HSSTokenizer(char * buffer, unsigned buflen)
{
	//these start out empty
	this->currentChar = NULL;
	this->tokenList = NULL;
	
	//initialize the currentTokenText
	this->currentTokenText = string();
	
	//store the pointer to the buffer
	this->buffer = buffer;
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
	
}


HSSToken * HSSTokenizer::readNextToken()
{	
	//if we're at the end of the source
	if (this->atEndOfSource()) {
		return NULL;
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
            return this->readHex();
        } else {
            return this->readNumberOrPercentage();
        }
	}
	
	HSSToken * ret;
	
	switch (cc) {
		//if it starts with quotes, either single or double, it is a string
		case '"':
		case '\'':
			return this->readString();
        case '#':
            ret = new HSSToken(HSSInstructionSign);
            this->readNextChar();
            return ret;
		case '@':
			ret = new HSSToken(HSSObjectSign);
			this->readNextChar();
			return ret;
		case '{':
			ret = new HSSToken(HSSBlockOpen);
			this->readNextChar();
			return ret;
		case '}':
			ret = new HSSToken(HSSBlockClose);
			this->readNextChar();
			return ret;
        case ':':
			ret = new HSSToken(HSSColon);
			this->readNextChar();
			return ret;
		case ';':
			ret = new HSSToken(HSSEndOfStatement);
			this->readNextChar();
			return ret;
		case '(':
			ret = new HSSToken(HSSParenthesisOpen);
			this->readNextChar();
			return ret;
		case ')':
			ret = new HSSToken(HSSParenthesisClose);
			this->readNextChar();
			return ret;
        case '/':
            return this->readCommentOrSymbol();
		default:
			return this->readSymbol();
	}
}


HSSToken * HSSTokenizer::peekNextToken()
{
    std_log4("bufpos: " << this->bufpos);
    //store the current position in the buffer
    int curpos = this->bufpos;
    //offset the position
    //FIXME: check off-bounds offsets
    this->bufpos += this->peekpos;  
    std_log4("peekpos: " << this->peekpos << " -> new bufpos: " << this->bufpos);
    HSSToken * ret = this->readNextToken();
    //store the new offset
    this->peekpos = (this->bufpos - curpos);
    std_log4("new peekpos: " << this->peekpos << " because bufpos: " << this->bufpos << " and curpos: " << curpos);
    
    //restore the position in the buffer
    //we start one position before we were before, since we are re-reading the char
    this->bufpos = curpos - 1;
    std_log4("end bufpos: " << this->bufpos);
    this->readNextChar();
    
    return ret;
}

void HSSTokenizer::resetPeek()
{
    //just reset the offset to 0
    this->peekpos = 0;
}


HSS_TOKENIZING_STATUS HSSTokenizer::readNextChar()
{
	if(this->buflen == this->bufpos){
		this->currentChar = '\0';
	} else {
		this->currentChar = buffer[this->bufpos];
	}
    std::ostringstream tempstream;
    tempstream << this->bufpos;
    std_log4("read charachter "+tempstream.str()+"|" << this->currentChar << "|");
	this->bufpos++;
    this->currentColumn++;
    
	
	return HSSTokenizerOK;
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

string HSSTokenizer::extractCurrentTokenText()
{
	string tempctt = this->currentTokenText;
	this->currentTokenText.clear();
	return tempctt;
}


//reads and returns a whitespace token
HSSToken * HSSTokenizer::readWhitespace()
{
	this->skipWhitespace();
	return new HSSToken(HSSWhitespace);
}

//reads and returns an identifier token
HSSToken * HSSTokenizer::readIdentifier()
{
	while (isalnum(this->currentChar)) {
		this->storeCurrentCharAndReadNext();
	}
	return new HSSValueToken(HSSIdentifier, this->extractCurrentTokenText());
}

//reads and returns a hexadecimal number
HSSToken * HSSTokenizer::readHex()
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
                    break;
                }
        }
        security_brake();
	}
    
    return new HSSValueToken(HSSHexNumber, this->extractCurrentTokenText());
}

//reads and returns a hexadecimal number or an identifier
HSSToken * HSSTokenizer::readHexOrIdentifier()
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
                    break;
                }
        }
        security_brake();
	}
    
    //if we reached this far, it is an identifier - finish reading it
    return this->readIdentifier();
}

//reads and returns either a number or a percentage token
//the currentChar is assumed to be a number
HSSToken * HSSTokenizer::readNumberOrPercentage()
{
	while (isnumber(this->currentChar)){
		this->storeCurrentCharAndReadNext();
	}
	HSSToken * ret;
   if(currentChar == '%'){
	   ret = new HSSValueToken(HSSPercentageNumber, this->extractCurrentTokenText());
       this->readNextChar();
   } else {
	   ret = new HSSValueToken(HSSNumber, this->extractCurrentTokenText());
   }
	return ret;
}

//reads and returns either a single quoted or double quoted string token
//the currentChar is assumed to be either " or '
HSSToken * HSSTokenizer::readString()
{
    HSSToken * ret;
    if(this->currentChar == '"'){
        this->readNextChar();
        while (this->currentChar != '"') {
            if(this->atEndOfSource()){
                //FIXME
                throw HSSUnexpectedEndOfSourceException("unknown", this->currentLine, this->currentColumn);
                return NULL;
            }
            this->storeCurrentCharAndReadNext();
        }
        ret = new HSSValueToken(HSSDoubleQuoteString, this->extractCurrentTokenText());
    } else if(this->currentChar == '\'') {
        this->readNextChar();
        while (this->currentChar != '\'') {
            if(this->atEndOfSource()){
                //FIXME
                throw HSSUnexpectedEndOfSourceException("unknown", this->currentLine, this->currentColumn);
                return NULL;
            }
            this->storeCurrentCharAndReadNext();
        }
        ret = new HSSValueToken(HSSSingleQuoteString, this->extractCurrentTokenText());
    }
    
    this->readNextChar();
	return ret;
}

//reads and returns a comment or a symbol token
//assumes currentChar == '/'
HSSToken * HSSTokenizer::readCommentOrSymbol()
{
    HSSValueToken * ret;
    this->readNextChar();
    if (this->currentChar == '/'){
        this->readNextChar(); // skip '/'
        //read all chars until end of line
        while (this->currentChar != '\n' && this->currentChar != '\r' && this->currentChar != '\f' && !this->atEndOfSource()) {
            this->storeCurrentCharAndReadNext();
        }
        ret = new HSSValueToken(HSSLineComment, this->extractCurrentTokenText());
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
        ret = new HSSValueToken(HSSBlockComment, this->extractCurrentTokenText());
        readNextChar();
    } else {
        ret = new HSSValueToken(HSSSymbol, '/');
    }
    
    return ret;
}

//reads and returns a symbol token
HSSToken * HSSTokenizer::readSymbol()
{
	HSSToken * ret;
    
    switch (this->currentChar) {
        case '.':
            this->storeCurrentCharAndReadNext();
            if(this->currentChar == '.'){
                this->storeCurrentCharAndReadNext();
                if(this->currentChar == '.'){
                    this->storeCurrentCharAndReadNext();
                }
                
                ret = new HSSValueToken(HSSSymbol, this->extractCurrentTokenText());
                return ret;
            }
            
            ret = new HSSValueToken(HSSSymbol, this->extractCurrentTokenText());
            readNextChar();
            return ret;
                        
        default:
            ret = new HSSValueToken(HSSSymbol, this->currentChar);
            this->readNextChar();
            return ret;
    }
}

















