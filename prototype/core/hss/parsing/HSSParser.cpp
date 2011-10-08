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
 *      Last changed: 2011/10/06
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 15
 *
 ********************************************************************/

#include "../hss.h"
#include "HSSValueToken.h"
#include "HSSConstants.h"
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "../../axr/AXRDebugging.h"
#include "../../axr/AXRController.h"
#include <boost/pointer_cast.hpp>
#include "HSSExpressions.h"
#include "../objects/HSSRgba.h"
#include "HSSFunctionCall.h"
#include "../objects/HSSFunctions.h"

using namespace AXR;

HSSParser::HSSParser(AXRController * theController)
{
    this->controller = theController;
    this->tokenizer = HSSTokenizer::p(new HSSTokenizer());
    
    this->currentContext.push_back(HSSParserContextRoot);
    std_log1("creating hss parser");
}

//HSSParser::HSSParser(HSSTokenizer::buf_p buffer, unsigned buflen, std::string filename)
//{
//    this->tokenizer = HSSTokenizer::p(new HSSTokenizer(buffer, buflen));
//    this->filename = filename;
//    
//    this->currentContext.push_back(HSSParserContextRoot);
//    //FIXME: will there be a root object? Now defaults to container
//    this->currentObjectContext.push(HSSContainer::p(new HSSContainer()));
//    
//    this->readNextToken();
//}

HSSParser::~HSSParser()
{
    std_log1("destructing hss parser");
    unsigned i;
    for (i=0; i<this->currentObjectContext.size(); i++){
        this->currentObjectContext.pop();
    }
}

void HSSParser::reset()
{
    //clear the tokenizer
    this->tokenizer->reset();
    
    //clear the current object context
    unsigned i;
    for (i=0; i<this->currentObjectContext.size(); i++){
        this->currentObjectContext.pop();
    }
    //clear the current context
    this->currentContext.clear();
    this->filename = std::string();
    this->currentToken.reset();
    
    //initialize the new values
    this->currentContext.push_back(HSSParserContextRoot);
    //FIXME: will there be a root object? Now defaults to container
    this->currentObjectContext.push(HSSContainer::p(new HSSContainer()));
    
}

bool HSSParser::loadFile(std::string filepath)
{
    security_brake_init();
    //get the filename from the path
    this->filename = filepath.substr(filepath.rfind("/", filepath.size())+1);
    
    //open the file for reading
    FILE * hssfile = fopen(filepath.c_str(), "r");
    //read the file into the buffer of the tokenizer
    HSSTokenizer::buf_p hssbuffer = this->tokenizer->getBuffer();
    int len = (int)fread(hssbuffer.get(), 1, AXR_HSS_BUFFER_SIZE, hssfile);
    if (ferror(hssfile)) {
        fclose(hssfile);
        return false;
    }
    //initialize
    this->tokenizer->setBufferLength(len);
    this->tokenizer->readNextChar();
    
    //FIXME: what if the file is longer than the buffer?
    fclose(hssfile);
    
    this->readNextToken();
    
    HSSStatement::p statement;

    bool done = this->atEndOfSource();
    
    if (!done) this->skip(HSSWhitespace);
    
    while (!done) {
        std_log1("read statement");
        if(statement){
            statement.reset();
        }
        try {
            statement = this->readNextStatement();
        }
        catch(AXR::HSSUnexpectedTokenException e){
            std::cout << e.toString() << std::endl;
            continue;
        }
        catch(AXR::HSSUnexpectedEndOfSourceException e){
            std::cout << e.toString() << std::endl;
        }
        catch(AXR::HSSUnexpectedObjectTypeException e){
            std::cout << e.toString() << std::endl;
        }
        catch(AXR::HSSExpectedTokenException e){
            std::cout << e.toString() << std::endl;
        }
        catch(AXR::HSSWrongHexLengthException e){
            std::cout << e.toString() << std::endl;
        }
        
        
        if(!statement){
            done = true;
        } else {
//            std::cout << std::endl << "-----------------------------" << std::endl
//            <<  statement->toString() << std::endl << "-----------------------------" << std::endl;
            switch (statement->getType()) {
                case HSSStatementTypeRule:
                {
                    HSSRule::p theRule = boost::static_pointer_cast<HSSRule>(statement);
                    this->controller->rulesAdd(theRule);
                    break; 
                }
                    
                case HSSStatementTypeObjectDefinition:
                {
                    HSSObject::p theObj = boost::static_pointer_cast<HSSObjectDefinition>(statement)->getObject();
                    this->controller->objectTreeAdd(theObj);
                    break;
                }
                    
                case HSSStatementTypeComment:
                {
                    std_log1(statement->toString());
                    break;
                }
                
                default:
                    std_log1("unknown statement");
                    break;
            }
            this->controller->statementsAdd(statement);
        }
        
        security_brake();
    }
    std_log1("reached end of source");
    std_log1("\n\n\n\n");
    
    return true;
}

HSSStatement::p HSSParser::readNextStatement()
{
    HSSStatement::p ret;
    if(this->currentContext.back() == HSSParserContextRoot)
    {
        //the file was empty
        if(this->atEndOfSource())
            return ret;
        
        
        switch (this->currentToken->getType()) {
            case HSSInstructionSign:
            {
                HSSInstruction::p instruction = this->readInstruction();
                //FIXME: what do do here?
                std_log1("******************** unimplemented ***********************");
                break;
            }
            
            //if the statement starts with an object sign, it is an object definition
            case HSSObjectSign:
            {
                //create an object definition
                ret = this->readObjectDefinition();
                break;
            }
            
            //if the statement starts with an identifier, universal selector or combinator it is a rule
            case HSSIdentifier:
            {
                ret = this->readRule();
                break;
            }
            case HSSSymbol:
            {
                if(VALUE_TOKEN(this->currentToken)->equals(HSSSymbol, "*") 	|| this->isCombinator()){
                    ret = this->readRule();
                    break;
                }
            }
            //if it is a comment
            case HSSBlockComment:
            case HSSLineComment:
            {
                ret = HSSComment::p(new HSSComment(VALUE_TOKEN(this->currentToken)->getString()));
                this->readNextToken();
                if(this->atEndOfSource())
                    return ret;
                this->skip(HSSWhitespace);
                break;
            }
                
                
            default:
                break;
        }
        
        return ret;
    } else {
        std_log1("reading in anything other than root context is not implemented yet");
        return ret;
    }
}

HSSRule::p HSSParser::readRule()
{
    security_brake_init()
    
    //throw error if at end of source
    this->checkForUnexpectedEndOfSource();
    
    //initialize the rule
    HSSSelectorChain::p selectorChain = this->readSelectorChain(HSSBlockOpen);
    HSSRule::p ret = HSSRule::p(new HSSRule(selectorChain));
    
    //now we're inside the block
    this->currentContext.push_back(HSSParserContextBlock);
    
    //read the inner part of the block
    while (!this->currentToken->isA(HSSBlockClose))
    {
        //std_log1(this->currentToken->toString());
        //if we find an identifier, we must peek forward to see if it is a property name
        if(this->currentToken->isA(HSSIdentifier)){
            if (this->isPropertyDefinition()){
                HSSPropertyDefinition::p propertyDefinition = this->readPropertyDefinition();
                ret->propertiesAdd(propertyDefinition);
            } else {
                //recursive omg!
                ret->childrenAdd(this->readRule());
            }
        } else if(this->currentToken->isA(HSSSymbol)){
            ret->childrenAdd(this->readRule());
        } else if(this->currentToken->isA(HSSLineComment) || this->currentToken->isA(HSSBlockComment)){
            //ignore
            this->readNextToken();
            this->skip(HSSWhitespace);
        } else if (this->currentToken->isA(HSSInstructionSign)){
            HSSRule::p childRule = this->readInstructionRule();
            if(childRule){
                ret->childrenAdd(childRule);
            }
            
        } else {
            throw HSSUnexpectedTokenException(this->currentToken->getType(), this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
        }
        
        security_brake()
    }
    security_brake_reset()
    
    //we're out of the block, read next token
    this->readNextToken();
    //leave the block context
    this->currentContext.pop_back();
    if(!this->atEndOfSource()){
        //ignore all the whitespace after the block
        this->skip(HSSWhitespace);
    }
    
    return ret;
}

HSSSelectorChain::p HSSParser::readSelectorChain(HSSTokenType stopOn)
{
    security_brake_init();
    
    HSSSelectorChain::p ret = HSSSelectorChain::p(new HSSSelectorChain());
    
    //first we need to look at the selector chain
    //set the appropriate context
    this->currentContext.push_back(HSSParserContextSelectorChain);
    //parse the selector chain until we find the block
    while (this->currentToken && !this->currentToken->isA(stopOn)) {
        std_log3(this->currentToken->toString());
        
        //if it's an identifier, it's a simple selector
        if (this->currentToken->isA(HSSIdentifier)){
            ret->add(this->readSelector());
            //adds only if needed
            HSSCombinator::p childrenCombinator(this->readChildrenCombinatorOrSkip());
            if(childrenCombinator){
                ret->add(childrenCombinator);
            }
            
            //a symbol, probably a combinator
        } else if (this->currentToken->isA(HSSSymbol)) {
            const char currentTokenValue = *(VALUE_TOKEN(this->currentToken)->getString()).c_str();
            switch (currentTokenValue) {
                case '=':
                case '-':
                case '+':
                case '>': //FIXME: special handling for text selection combinators?
                    ret->add(this->readSymbolCombinator());
                    break;
                case '*':
                    ret->add(HSSUniversalSelector::p(new HSSUniversalSelector()));
                    this->readNextToken();
                    //adds only if needed
                    ret->add(this->readChildrenCombinatorOrSkip());
                    
                    break;
                case '.':
                    //we need to check if it is really a combinator or just a syntax error
                    if(   VALUE_TOKEN(this->currentToken)->getString() == ".."
                       || VALUE_TOKEN(this->currentToken)->getString() == "..."  ){
                        ret->add(this->readSymbolCombinator());
                        break;
                    }
                    //huh? we didn't expect any other symbol
                default:
                    throw HSSUnexpectedTokenException(HSSSymbol, std::string(1,currentTokenValue), this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
            }
            //we didn't expect any other type of token
        } else {
            throw HSSUnexpectedTokenException(this->currentToken->getType(), this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
        }
        
        security_brake();
    }
    security_brake_reset();
    
    //we're not in a selector anymore
    this->currentContext.pop_back();
    
    //we expect a block to open
    this->skipExpected(stopOn);
    //if the file ends here, fuuuuuuu[...]
    this->checkForUnexpectedEndOfSource();
    //skip any whitespace
    this->skip(HSSWhitespace);
    //if the file ends here, fuuuuuuu[...]
    this->checkForUnexpectedEndOfSource();
    
    return ret;
}

bool HSSParser::isCombinator()
{
    return this->isCombinator(this->currentToken);
}

bool HSSParser::isCombinator(HSSToken::p token)
{
    //are we in a context that accepts combinators?
    HSSParserContext context = this->currentContext.back();
    if (context == HSSParserContextExpression) {
        return false;
    }
    //all combinators are symbols
    if(token->isA(HSSSymbol) ){
        const char currentTokenChar = *(VALUE_TOKEN(token).get()->getString()).c_str();
        switch (currentTokenChar) {
            case '=':
            case '-':
            case '+':
            case '.':
            case '>':
                return true;
                //fixme
                
            default:
                return false;
        }
    } else if ( token->isA(HSSWhitespace) ){
        return this->isChildrenCombinator();
    }
    return false;
}

//this function assumes currentToken to be whitespace
bool HSSParser::isChildrenCombinator()
{
    std_log4("----- peeking ------ ");
    //if the next token is anything other than a combinator or an open block the whitespace means children combinator
    HSSToken::p peekToken = this->tokenizer->peekNextToken();
    std_log4(peekToken->toString());
    bool ret = !this->isCombinator(peekToken) && !peekToken->isA(HSSBlockOpen);
    this->tokenizer->resetPeek();
    std_log4("----- finished peeking ------ ");
    return ret;
}

//this function assumes currentToken to be an identifier
bool HSSParser::isPropertyDefinition()
{
    bool ret = true;
    
    std_log4("----- peeking ------ ");
    HSSToken::p peekToken = this->tokenizer->peekNextToken();
    std_log4(peekToken->toString());
    //if the next token is a colon, it is either a property definition or a filter
    if(peekToken->isA(HSSColon)){
        //we'll peek until we find a end of statement, a closing block or an opening one
        peekToken = this->tokenizer->peekNextToken();
        //if we find a whitespace or an object sign here, we can be sure it's a property definition
        if( peekToken->isA(HSSWhitespace) || peekToken->isA(HSSObjectSign) ){
            ret = true;
        } else {
            while(! peekToken->isA(HSSEndOfStatement) && !peekToken->isA(HSSBlockClose) && !peekToken->isA(HSSBlockOpen))
            {
                std_log1(peekToken->toString());
                peekToken = this->tokenizer->peekNextToken();
                this->checkForUnexpectedEndOfSource();
            }
            //if we find an opening block, we're dealing with a selector
            if(peekToken->isA(HSSBlockOpen)){
                ret = false;
            }
        }
        
        
//        peekToken = this->tokenizer->peekNextToken();
//        //now, if we're dealing with an identifier it may be a filter
//        if(peekToken->isA(HSSIdentifier)) {
//            //if it is, after the identifier can only come a parenthesis open or a whitespace
//            peekToken = this->tokenizer->peekNextToken();
//            if(! peekToken->isA(HSSParenthesisOpen) && ! peekToken->isA(HSSWhitespace) )
//            {
//                //we still don't know... continue peeking
//                peekToken = this->tokenizer->peekNextToken();
//                
//            }
//        }
    } else {
        //no colon, no property definiton
        ret = false;
    }
    std_log4("----- finished peeking ------ ");
    this->tokenizer->resetPeek();
    return ret;
}

HSSCombinator::p HSSParser::readChildrenCombinatorOrSkip()
{
    HSSCombinator::p ret;
    //are we dealing with whitespace?
    if(this->currentToken->isA(HSSWhitespace)){
        if(this->isChildrenCombinator()){
            HSSCombinator::p newCombinator = HSSCombinator::p(new HSSCombinator(HSSCombinatorTypeChildren));
            this->readNextToken();
            return newCombinator;
        } else {
            //alright, ignore it
            this->skip(HSSWhitespace);
            return ret;
        }
    } else {
        //done, nothing to see here, move along...
        return ret;
    }
}

//this expects the current token to be a symbol
HSSCombinator::p HSSParser::readSymbolCombinator()
{
    //FIXME: check the context
    HSSCombinator::p ret;
    const char currentTokenChar = *(VALUE_TOKEN(this->currentToken)->getString()).c_str();
    switch (currentTokenChar) {
        case '=':
            ret = HSSCombinator::p(new HSSCombinator(HSSCombinatorTypeSiblings));
            break;
        case '-':
            ret = HSSCombinator::p(new HSSCombinator(HSSCombinatorTypePreviousSiblings));
            break;
        case '+':
            ret = HSSCombinator::p(new HSSCombinator(HSSCombinatorTypeNextSiblings));
            break;
        case '.':
            if(VALUE_TOKEN(this->currentToken)->getString() == ".."){
                ret = HSSCombinator::p(new HSSCombinator(HSSCombinatorTypeDescendants));
            } else if (VALUE_TOKEN(this->currentToken)->getString() == "..."){
                ret = HSSCombinator::p(new HSSCombinator(HSSCombinatorTypeAllDescendants));
            }
            
            break;
        default:
            throw HSSUnexpectedTokenException(HSSSymbol, std::string(1, currentTokenChar), this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
            return ret;
    }
    
    this->readNextToken();
    this->skip(HSSWhitespace);
    return ret;
}

//this assumes that the currentToken is an identifier
HSSSelector::p HSSParser::readSelector()
{
    std::string theValue = VALUE_TOKEN(this->currentToken)->getString();
    HSSSelector::p ret = HSSSelector::p(new HSSSelector(theValue));
    this->readNextToken();
    return ret;
}


//this assumes currentToken is an object sign
HSSObjectDefinition::p HSSParser::readObjectDefinition()
{
    HSSObjectDefinition::p ret;
    std::string objtype;
    HSSObject::p obj;
    
    //yeah, yeah, we know the @ already
    this->skipExpected(HSSObjectSign);
    //end of file would be fatal
    this->checkForUnexpectedEndOfSource();
    
    //store the current context for later use
    HSSParserContext outerContext = this->currentContext.back();
    //set the appropriate context
    this->currentContext.push_back(HSSParserContextObjectDefinition);
    
    //first we need to know what type of object it is
    if (this->currentToken->isA(HSSWhitespace)) {
        //damn, we'll have to derive that from the context
        if (outerContext == HSSParserContextRoot){
            objtype = "container";
        } else {
            //FIXME
            std_log1("deriving object types from context is only supported in root context yet");
            objtype = "object";
        }
    } else if(this->currentToken->isA(HSSIdentifier)){
        //alright, we've got a type, look it up
        objtype = VALUE_TOKEN(this->currentToken)->getString();
        this->readNextToken();
        this->checkForUnexpectedEndOfSource();
    } else {
        throw HSSUnexpectedTokenException(this->currentToken->getType(), this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
    }
    
    //try to create an object of that type
    try {
        obj = HSSObject::newObjectWithType(objtype);
    } catch (HSSUnknownObjectTypeException e) {
        throw HSSUnexpectedObjectTypeException(e.type, this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
    }
    
    
    //get the name of the object
    if (this->currentToken->isA(HSSWhitespace)) {
        this->skip(HSSWhitespace);
    }
    if (this->currentToken->isA(HSSIdentifier)) {
        obj->setName(VALUE_TOKEN(this->currentToken)->getString());
        std_log3("setting its name to "+VALUE_TOKEN(this->currentToken)->getString());
        this->readNextToken();
    } else if (this->currentToken->isA(HSSBlockOpen)){
        //it is the opening curly brace, therefore an annonymous object:
        //do nothing
    } else {
        throw HSSUnexpectedTokenException(this->currentToken->getType(), this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
    }
    
    ret = HSSObjectDefinition::p(new HSSObjectDefinition(obj));
    this->skip(HSSWhitespace);
    this->skipExpected(HSSBlockOpen);
    this->skip(HSSWhitespace);
    
    //now we're inside the block
    this->currentContext.push_back(HSSParserContextBlock);
    
    //if the file ends here, fuuuuuuu[...]
    this->checkForUnexpectedEndOfSource();
    
    //read the inner part of the block
    this->currentObjectContext.push(obj);
    while (!this->currentToken->isA(HSSBlockClose)){
        const HSSPropertyDefinition::p &property = this->readPropertyDefinition();
        ret->propertiesAdd(property);
    }
    this->currentObjectContext.pop();
    
    //we're out of the block, we expect a closing brace
    this->skipExpected(HSSBlockClose);
    //leave the block context
    this->currentContext.pop_back();
    //leave the object definition context
    this->currentContext.pop_back();
    if(!this->atEndOfSource()){
        //ignore all the whitespace after the block
        this->skip(HSSWhitespace);
    }
    
    return ret;
}

HSSPropertyDefinition::p HSSParser::readPropertyDefinition()
{
    std::string propertyName;
    
    //end of source is no good
    this->checkForUnexpectedEndOfSource();
    
    HSSPropertyDefinition::p ret;
    
    if (this->currentToken->isA(HSSIdentifier)){
        propertyName = VALUE_TOKEN(this->currentToken)->getString();
        ret = HSSPropertyDefinition::p(new HSSPropertyDefinition(propertyName));
        this->readNextToken();
        //now must come a colon
        this->skipExpected(HSSColon);
        //we don't give a f$%# about whitespace
        this->skip(HSSWhitespace);
        //now comes either an object definition, a literal value or an expression
        //object
        if (this->currentToken->isA(HSSObjectSign)){
            ret->setValue(this->readObjectDefinition());
            //this->readNextToken();
            
        } else if (this->currentToken->isA(HSSSingleQuoteString) || this->currentToken->isA(HSSDoubleQuoteString)){
            ret->setValue(HSSStringConstant::p(new HSSStringConstant(VALUE_TOKEN(this->currentToken)->getString())));
            this->checkForUnexpectedEndOfSource();
            this->readNextToken();
            
        //number literal
        } else if (this->currentToken->isA(HSSNumber) || this->currentToken->isA(HSSPercentageNumber) || this->currentToken->isA(HSSParenthesisOpen)){
            
            //FIXME: parse the number and see if it is an int or a float
            HSSParserNode::p exp = this->readExpression();
            ret->setValue(exp);
            
        } else if (this->currentToken->isA(HSSIdentifier)){
            //this is either a function, a keyword or an object name
            
            std::string valuestr = VALUE_TOKEN(this->currentToken)->getString();
            //check if it is a function
            HSSObject::p objectContext = this->currentObjectContext.top();
            
            if (objectContext->isFunction(valuestr, propertyName)){
                ret->setValue(this->readExpression());
            //check if it is a keyword
            } else if (objectContext->isKeyword(valuestr, propertyName)){
                ret->setValue(HSSKeywordConstant::p(new HSSKeywordConstant(valuestr))); 
                this->readNextToken();
            //we assume it is an object name at this point
            } else {
                //FIXME (?)
                ret->setValue(HSSObjectNameConstant::p(new HSSObjectNameConstant(valuestr)));
                this->readNextToken();
            }
            
        } else if (this->currentToken->isA(HSSInstructionSign)){
            ret->setValue(this->getObjectFromInstruction(this->readInstruction()));
            
        } else {
            throw HSSUnexpectedTokenException(this->currentToken->getType(), this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
        }
        
        this->skip(HSSWhitespace);
        //expect a semicolon or the closing brace
        if(this->currentToken->isA(HSSEndOfStatement)){
            this->readNextToken();
            this->skip(HSSWhitespace);
            
        } else if (this->currentToken->isA(HSSBlockClose)){
            //alright, this is the end of the property definition
            std_log3("end of property definition");
        } else {
            throw HSSUnexpectedTokenException(this->currentToken->getType(), this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
        }
        
        return ret;
    } else {
        throw HSSUnexpectedTokenException(this->currentToken->getType(), this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
        this->readNextToken();
    }
    return ret;
}


HSSInstruction::p HSSParser::readInstruction()
{
    HSSInstruction::p ret;
    std::string currentval;
    
    this->tokenizer->preferHex = true;
    
    this->skipExpected(HSSInstructionSign);
    this->checkForUnexpectedEndOfSource();
    //we are looking at
    //if it starts with a number, we are looking at a color instruction
    if (this->currentToken->isA(HSSHexNumber)){
        currentval = VALUE_TOKEN(this->currentToken)->getString();
        switch (currentval.length()) {
            //1 digit grayscale
            case 1:
                ret = HSSInstruction::p(new HSSInstruction(HSSGrayscale1Instruction, currentval));
                this->readNextToken();
                break;
            //2 digit grayscale
            case 2:
                ret = HSSInstruction::p(new HSSInstruction(HSSGrayscale2Instruction, currentval));
                this->readNextToken();
                break;
            //rgb
            case 3:
                ret = HSSInstruction::p(new HSSInstruction(HSSRGBInstruction, currentval));
                this->readNextToken();
                break;
            //rgba
            case 4:
                ret = HSSInstruction::p(new HSSInstruction(HSSRGBAInstruction, currentval));
                this->readNextToken();
                break;
            //rrggbb
            case 6:
                ret = HSSInstruction::p(new HSSInstruction(HSSRRGGBBInstruction, currentval));
                this->readNextToken();
                break;
            //rrggbbaa
            case 8:
                ret = HSSInstruction::p(new HSSInstruction(HSSRRGGBBAAInstruction, currentval));
                this->readNextToken();
                break;
                
            default:
                throw HSSWrongHexLengthException(currentval.length(), this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
                return ret;
        }
                
    } else if (this->currentToken->isA(HSSIdentifier)){
        currentval = VALUE_TOKEN(this->currentToken)->getString();
        if (currentval == "new"){
            ret = HSSInstruction::p(new HSSInstruction(HSSNewInstruction));
        } else if (currentval == "ensure") {
            ret = HSSInstruction::p(new HSSInstruction(HSSEnsureInstruction));
        } else if (currentval == "import") {
            ret = HSSInstruction::p(new HSSInstruction(HSSImportInstruction));
        } else if (currentval == "move") {
            ret = HSSInstruction::p(new HSSInstruction(HSSMoveInstruction));
        } else {
            throw HSSUnexpectedTokenException(this->currentToken->getType(), this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
            return ret;
        }
        this->readNextToken();
        
    } else {
        throw HSSUnexpectedTokenException(this->currentToken->getType(), this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
        return ret;
    }
    
    this->tokenizer->preferHex = false;
    
    this->checkForUnexpectedEndOfSource();
    this->skip(HSSWhitespace);
    return ret;
}

HSSObjectDefinition::p HSSParser::getObjectFromInstruction(HSSInstruction::p instruction)
{
    HSSObjectDefinition::p ret;
    HSSInstructionType instructionType = instruction->getInstructionType();
    switch (instructionType) {
        case HSSGrayscale1Instruction:
        case HSSGrayscale2Instruction:
        {
            //try to create an object of that type
            HSSRgba::p obj = HSSRgba::p(new HSSRgba());
            
            unsigned int hexValue;
            std::string tempstr = instruction->getValue();
            if(instructionType == HSSGrayscale1Instruction){
                tempstr = tempstr + tempstr;
            }
            sscanf(tempstr.c_str(), "%X", &hexValue);
            
            obj->setDRed(HSSNumberConstant::p(new HSSNumberConstant(hexValue)));
            obj->setDGreen(HSSNumberConstant::p(new HSSNumberConstant(hexValue)));
            obj->setDBlue(HSSNumberConstant::p(new HSSNumberConstant(hexValue)));
            
            ret = HSSObjectDefinition::p(new HSSObjectDefinition(obj));
            break;
        }
            
        case HSSRGBInstruction:
        case HSSRGBAInstruction:
        case HSSRRGGBBInstruction:
        case HSSRRGGBBAAInstruction:
        {
            //try to create an object of that type
            HSSRgba::p obj = HSSRgba::p(new HSSRgba());
            
            std::string red;
            unsigned int redHex;
            std::string green;
            unsigned int greenHex;
            std::string blue;
            unsigned int blueHex;
            
            unsigned int alphaHex;
            std::string alpha;
            
            switch (instructionType) {
                case HSSRGBInstruction:
                    red = instruction->getValue().substr(0, 1);
                    red = red + red;
                    green = instruction->getValue().substr(1, 1);
                    green = green + green;
                    blue = instruction->getValue().substr(2, 1);
                    blue = blue + blue;
                    alpha = "FF";
                    
                    break;
                case HSSRGBAInstruction:
                    red = instruction->getValue().substr(0, 1);
                    red = red + red;
                    green = instruction->getValue().substr(1, 1);
                    green = green + green;
                    blue = instruction->getValue().substr(2, 1);
                    blue = blue + blue;
                    alpha = instruction->getValue().substr(3, 1);
                    alpha = alpha + alpha;
                    
                    break;
                    
                case HSSRRGGBBInstruction:
                    red = instruction->getValue().substr(0, 2);
                    green = instruction->getValue().substr(2, 2);
                    blue = instruction->getValue().substr(4, 2);
                    alpha = "FF";
                    
                    break;
                
                case HSSRRGGBBAAInstruction:
                    red = instruction->getValue().substr(0, 2);
                    green = instruction->getValue().substr(2, 2);
                    blue = instruction->getValue().substr(4, 2);
                    alpha = instruction->getValue().substr(6, 2);
                    
                    break;
                
                default:
                    break;
            }
            
            sscanf(red.c_str(), "%X", &redHex);
            sscanf(green.c_str(), "%X", &greenHex);
            sscanf(blue.c_str(), "%X", &blueHex);
            sscanf(alpha.c_str(), "%X", &alphaHex);
            
            obj->setDRed(HSSNumberConstant::p(new HSSNumberConstant(redHex)));
            obj->setDGreen(HSSNumberConstant::p(new HSSNumberConstant(greenHex)));
            obj->setDBlue(HSSNumberConstant::p(new HSSNumberConstant(blueHex)));
            obj->setDAlpha(HSSNumberConstant::p(new HSSNumberConstant(alphaHex)));
            
            ret = HSSObjectDefinition::p(new HSSObjectDefinition(obj));
            break;
        }
            
        default:
            std_log1("*********** eror: unknown instruction type ****************");
            break;
    }
    return ret;
}

//this function assumes currentToken is a instruction sign
HSSRule::p HSSParser::readInstructionRule()
{
    HSSInstruction::p instruction = this->readInstruction();
    HSSRule::p ret;
    switch (instruction->getInstructionType()) {
        case HSSNewInstruction:
        case HSSEnsureInstruction:
        case HSSMoveInstruction:
        {
            ret = this->readRule();
            ret->setInstruction(instruction);
            break;
        }
            
        default:
            std_log1("*********** eror: unknown instruction type ****************");
            break;
    }
    
    return ret;
}

HSSParserNode::p HSSParser::readExpression()
{
    return this->readAdditiveExpression();
}

HSSParserNode::p HSSParser::readAdditiveExpression()
{
    security_brake_init();
    
    this->checkForUnexpectedEndOfSource();
    HSSParserNode::p left = this->readMultiplicativeExpression();
    while (!this->atEndOfSource() && this->currentToken->isA(HSSSymbol)) {
        const char currentTokenChar = *(VALUE_TOKEN(this->currentToken)->getString()).c_str();
        switch (currentTokenChar) {
            case '+':
            {
                this->readNextToken();
                this->skip(HSSWhitespace);
                left = HSSSum::p(new HSSSum(left, this->readMultiplicativeExpression()));
                break;
            }
                
            case '-':
            {
                this->readNextToken();
                this->skip(HSSWhitespace);
                left = HSSSubtraction::p(new HSSSubtraction(left, this->readMultiplicativeExpression()));
                break;
            }
                
            default:
                return left;
                break;
        }
        
        security_brake();
    }
    
    return left;
}

HSSParserNode::p HSSParser::readMultiplicativeExpression()
{
    security_brake_init();
    
    this->checkForUnexpectedEndOfSource();
    HSSParserNode::p left = this->readBaseExpression();
    while (!this->atEndOfSource() && this->currentToken->isA(HSSSymbol)) {
        
        const char currentTokenChar = *(VALUE_TOKEN(this->currentToken)->getString()).c_str();
        switch (currentTokenChar) {
            case '*':
            {
                this->readNextToken();
                this->skip(HSSWhitespace);
                left = HSSMultiplication::p(new HSSMultiplication(left, this->readBaseExpression()));
                break;
            }
                
            case '/':
            {
                this->readNextToken();
                this->skip(HSSWhitespace);
                left = HSSDivision::p(new HSSDivision(left, this->readBaseExpression()));
                break;
            }
                
            default:
                return left;
                break;
        }
        
        security_brake();
    }
    
    return left;
}

HSSParserNode::p HSSParser::readBaseExpression()
{
    this->checkForUnexpectedEndOfSource();
    HSSParserNode::p left;
    
    switch (this->currentToken->getType()) {
        case HSSNumber:
        {
            left = HSSNumberConstant::p(new HSSNumberConstant(strtold(VALUE_TOKEN(this->currentToken)->getString().c_str(), NULL)));
            this->readNextToken();
            this->skip(HSSWhitespace);
            break;
        }
        
        case HSSPercentageNumber:
        {
            left = HSSPercentageConstant::p(new HSSPercentageConstant(strtold(VALUE_TOKEN(this->currentToken)->getString().c_str(), NULL)));
            this->readNextToken();
            this->skip(HSSWhitespace);
            break;
        }
            
        case HSSParenthesisOpen:
        {
            this->readNextToken();
            this->skip(HSSWhitespace);
            left = this->readExpression();
            this->skipExpected(HSSParenthesisClose);
            this->skip(HSSWhitespace);
            break;
        }
        
        case HSSIdentifier:
        {
            left = this->readFunction();
            break;
        }
        
        default:
            throw "Unknown token type while parsing base expression";
            break;
    }
    
    return left;
}

void HSSParser::readNextToken()
{
    //read next one
    this->currentToken = this->tokenizer->readNextToken();
}

HSSParserNode::p HSSParser::readFunction()
{
    HSSParserNode::p ret;
    
    this->checkForUnexpectedEndOfSource();
    if (this->currentToken->isA(HSSIdentifier)){
        //create new function
        std::string name = VALUE_TOKEN(this->currentToken)->getString();
        if(name == "ref"){
            HSSFunctionCall::p functionCall = HSSFunctionCall::p(new HSSFunctionCall());
            HSSRefFunction::p refFunction = HSSRefFunction::p(new HSSRefFunction(this->controller));
            
            this->checkForUnexpectedEndOfSource();
            this->readNextToken();
            this->skip(HSSWhitespace);
            this->skipExpected(HSSParenthesisOpen);
            this->skip(HSSWhitespace);
            //read the arguments
            //first, we expect either "min", "max", "avg" or a property name
            if (! this->currentToken->isA(HSSIdentifier)){
                std_log1("HSSParser: unexpected token while reading ref function "+name);
            } else {
                std::string firstValue = VALUE_TOKEN(this->currentToken)->getString();
                if (   firstValue == "min"
                    || firstValue == "max"
                    || firstValue == "avg"   ){
                    
                    refFunction->setModifier(firstValue);
                    
                    //after this there comes the property name
                    this->checkForUnexpectedEndOfSource();
                    this->readNextToken();
                    this->skip(HSSWhitespace);
                    if (! this->currentToken->isA(HSSIdentifier)){
                        std_log1("HSSParser: unexpected token while reading ref function: "+HSSToken::tokenStringRepresentation(this->currentToken->getType()));
                    } else {
                        refFunction->setPropertyName(HSSObservable::observablePropertyFromString(VALUE_TOKEN(this->currentToken)->getString()));
                    }
                    
                    
                } else {
                    //just the property name
                    refFunction->setPropertyName(HSSObservable::observablePropertyFromString(firstValue));
                }
            }
            
            this->checkForUnexpectedEndOfSource();
            this->readNextToken();
            this->skipExpected(HSSWhitespace);
            if (!this->currentToken->isA(HSSIdentifier) || VALUE_TOKEN(this->currentToken)->getString() != "of"){
                std_log1("HSSParser: unexpected token while reading ref function: "+HSSToken::tokenStringRepresentation(this->currentToken->getType()));
            }
            this->checkForUnexpectedEndOfSource();
            this->readNextToken();
            this->skipExpected(HSSWhitespace);
            
            
            //now read the selector chain
            refFunction->setSelectorChain(this->readSelectorChain(HSSParenthesisClose));
            
            
            
            functionCall->setFunction(refFunction);
            ret = functionCall;
            
        } else if (name == "sel") {
            
            
        } else if (name == "min") {
            
        } else if (name == "max") {
            
        } else if (name == "ceil") {
            
        } else if (name == "floor") {
            
        } else if (name == "round") {
            
        } else {
            std_log1("HSSParser: unexpected function name "+name);
        }

        
        
        
        
        
    } else {
        throw HSSUnexpectedTokenException(this->currentToken->getType(), this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
    }
    
    return ret;
}

bool HSSParser::atEndOfSource()
{
    if(!this->currentToken){
        return true;
    } else {
        return false;
    }
}

void HSSParser::checkForUnexpectedEndOfSource()
{
    if (this->atEndOfSource()) {
        throw HSSUnexpectedEndOfSourceException(this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
    }
}

void HSSParser::skipExpected(HSSTokenType type)
{
    this->checkForUnexpectedEndOfSource();
    if (!this->currentToken->isA(type)) {
        throw HSSExpectedTokenException(type, this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
    }
    this->readNextToken();
}

void HSSParser::skipExpected(HSSTokenType type, std::string value)
{
    this->checkForUnexpectedEndOfSource();
    //FIXME: I'm not sure if this works as expected
    HSSValueToken::p currentToken = HSSValueToken::p(VALUE_TOKEN(this->currentToken));
    if (!currentToken->equals(type, value)) {
        throw HSSExpectedTokenException(type, value, this->filename, this->tokenizer->currentLine, this->tokenizer->currentColumn);
    }
    this->readNextToken();
}

void HSSParser::skip(HSSTokenType type)
{
    if(this->currentToken->isA(type)){
        this->readNextToken();
    }
}

void HSSParser::currentObjectContextRemoveLast()
{
    this->currentObjectContext.pop();
}

unsigned int HSSParser::currentObjectContextSize()
{
    return this->currentObjectContext.size();
}

void HSSParser::currentObjectContextAdd(HSSObject::p theObject)
{
    this->currentObjectContext.push(theObject);
}

