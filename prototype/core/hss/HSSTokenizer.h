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

#ifndef HSSTOKENIZER_H
#define HSSTOKENIZER_H

#import "HSSToken.h"
#import "HSSValueToken.h"

#import <string>
using namespace std;

//the return states any part of the tokenizer will report
enum HSS_TOKENIZING_STATUS {
	HSSTokenizerUnknownError = -1,
	HSSTokenizerDone = 0,
	HSSTokenizerOK = 1,
};

class HSSTokenizer
{
public:
	//initialize with a pointer to the buffer where the HSS code is stored, and the lenght of the buffer
	HSSTokenizer(char * buffer, unsigned buflen);
	//destructor
	~HSSTokenizer();
	//reads and returns a pointer to the next token in the buffer or NULL if the buffer was empty
    //the caller acquires ownership of the pointer
	HSSToken * readNextToken();
    
    //reads and returns a pointer to the next token in the buffer or NULL if the buffer was empty
    //BUT doesn't advance the current position of the normal reading flow. Call resetPeek() after
    //you finish peeking
    //the caller acquires ownership of the pointer
    HSSToken * peekNextToken();
    void resetPeek();
    
    //if you are expecting a hexadecimal number, set this to true
    //don't forget to reset it afterwards
    bool preferHex;
    
    //the current line and column position we're on
    long int currentLine;
    long int currentColumn;
	
protected:
	//the current character that has been read
	char currentChar;
	//here's where the tokens are stored
	HSSToken * tokenList;
	//here's where the current token's text is stored
	string currentTokenText;
	//pointer to the buffer where it will be read
	char * buffer;
	//how long is the buffer?
	unsigned buflen;
	//the position into the buffer
	unsigned bufpos;
    //the position into the buffer when peeking - it is an offset from bufpos
    int peekpos;
	
	//reads a the next character and stores it, also keeps track of the position in the buffer
	//it will also set the appropriate state if the end of the buffer is reached, but expects
	//more source
	HSS_TOKENIZING_STATUS readNextChar();
	//skips over any whitespace
	HSS_TOKENIZING_STATUS skipWhitespace();
	//checks if we are at the end of the source
	bool atEndOfSource();
	//to create tokens that have more than one char, this will store it and continue reading
	HSS_TOKENIZING_STATUS storeCurrentCharAndReadNext();
    //in some cases, storing specific chars in the currentTokenText is needed
	HSS_TOKENIZING_STATUS storeChar(char value);
	//returns the stored token text and clears the variable
	string extractCurrentTokenText();
	//reads and returns a whitespace token
    //the caller acquires ownership of the pointer
	HSSToken * readWhitespace();
	//reads and returns an identifier token
    //the caller acquires ownership of the pointer
	HSSToken * readIdentifier();
	//reads and returns a hexadecimal number
    //the caller acquires ownership of the pointer
    HSSToken * readHex();
	//reads and returns a hexadecimal number or an identifier token
    //the caller acquires ownership of the pointer
    HSSToken * readHexOrIdentifier();
	//reads and returns either a number or a percentage token
    //the caller acquires ownership of the pointer
	HSSToken * readNumberOrPercentage();
	//reads and returns either a single quoted or double quoted string token
    //the caller acquires ownership of the pointer
	HSSToken * readString();
	//reads and returns an object type token
    //the caller acquires ownership of the pointer
	HSSToken * readObjectType();
    //reads and returns a comment or a symbol token
    //the caller acquires ownership of the pointer
    HSSToken * readCommentOrSymbol();
	//reads and returns a symbol token
    //the caller acquires ownership of the pointer
	HSSToken * readSymbol();
};



#endif