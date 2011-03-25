/*
 *  HSSTokenizer.cpp
 *  mac
 *
 *  Created by Miro Keller on 11/03/11.
 *  Copyright 2011 Miro Keller. All rights reserved.
 *
 */

//FIXME: do something meaningful with the error codes

#include "HSSTokenizer.h"
#include "HSSToken.h"
#include "HSSValueToken.h"
#include <iostream>
#include "AXR.h"

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
	
	//start by reading the first character
	this->readNextChar();
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
		return this->readIdentifier();
	}
	
	if (isspace(cc)) {
		return this->readWhitespace();
	}
	
	//if it starts with a number it is either a number or a percentage
	if (isdigit(cc)) {
		return this->readNumberOrPercentage();
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
    std_log4("was at position " << this->bufpos);
	if(this->buflen == this->bufpos){
		this->currentChar = '\0';
	} else {
		this->currentChar = buffer[this->bufpos];
	}
    std_log4("read charachter |" << this->currentChar << "|");
	this->bufpos++;
    std_log4("now at position " << this->bufpos);
    
	
	return HSSTokenizerOK;
}

HSS_TOKENIZING_STATUS HSSTokenizer::skipWhitespace()
{
	while (isspace(this->currentChar)){
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
                throw HSSUnexpectedEndOfSourceException();
                return NULL;
            }
            this->storeCurrentCharAndReadNext();
        }
        ret = new HSSValueToken(HSSDoubleQuoteString, this->extractCurrentTokenText());
    } else if(this->currentChar == '\'') {
        this->readNextChar();
        while (this->currentChar != '\'') {
            if(this->atEndOfSource()){
                throw HSSUnexpectedEndOfSourceException();
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

















