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
 *      Last changed: 2012/03/29
 *      HSS version: 1.0
 *      Core version: 0.46
 *      Revision: 26
 *
 ********************************************************************/

#include "../hss.h"
#include "../tokenizing/HSSValueToken.h"
#include "HSSConstants.h"
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "../../axr/AXRDebugging.h"
#include "../../axr/errors/errors.h"
#include "../../axr/AXRController.h"
#include "../../axr/AXRWrapper.h"
#include <boost/pointer_cast.hpp>
#include "HSSExpressions.h"
#include "../objects/HSSRgb.h"
#include "HSSThisSelector.h"
#include "HSSFunctions.h"
#include "HSSFilter.h"
#include "HSSNegation.h"
#include "HSSFlag.h"

using namespace AXR;

HSSParser::HSSParser(AXRController * theController, AXRWrapper * wrapper)
{
    this->controller = theController;
    this->wrapper = wrapper;
    this->tokenizer = HSSTokenizer::p(new HSSTokenizer());
    
    this->currentContext.push_back(HSSParserContextRoot);
    std_log4("creating hss parser");
    this->line = 1;
    this->column = 1;
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
    std_log4("destructing hss parser");
    unsigned i;
    for (i=0; i<this->currentObjectContext.size(); i++){
        this->currentObjectContext.pop();
    }
    this->loadedFiles.clear();
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
    this->basepath = "";
    this->loadedFiles.clear();
    this->currentToken.reset();
    this->currentFile.reset();
    
    //initialize the new values
    this->currentContext.push_back(HSSParserContextRoot);
    //FIXME: will there be a root object? Now defaults to container
    this->currentObjectContext.push(HSSContainer::p(new HSSContainer()));
    
}

bool HSSParser::loadFile(AXRFile::p file)
{
    security_brake_init();
    
    //check if the file has been loaded already
    if (this->loadedFiles.find(file) != this->loadedFiles.end()){
        AXRError::p(new AXRError("HSSParser", "Failed loading file "+file->fileName+" because of circular reference"))->raise();
        return false;
    }
    
    this->loadedFiles.insert(file);
    this->currentFile = file;
    
    //propagate the file to the tokenizer
    this->tokenizer->setFile(file);
    //read the file
    int len = this->wrapper->readFile(file);
    
    //initialize
    this->tokenizer->setBufferLength(len);
    this->tokenizer->readNextChar();
    
    //FIXME: what if the file is longer than the buffer?
    this->wrapper->closeFile(file);
    
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
        catch (AXR::AXRError::p e){
            e->raise();
            this->readNextToken();
            if (!this->atEndOfSource()){
                this->skip(HSSWhitespace);
            }
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
            if (this->atEndOfSource()){
                done = true;
            }
            
        } else {
            switch (statement->getType()) {
                case HSSStatementTypeRule:
                {
                    HSSRule::p theRule = boost::static_pointer_cast<HSSRule>(statement);
                    this->controller->rulesAdd(theRule);
                    this->controller->parserTreeAdd(statement);
                    break; 
                }
                    
                case HSSStatementTypeObjectDefinition:
                {
                    HSSObjectDefinition::p theObj = boost::static_pointer_cast<HSSObjectDefinition>(statement);
                    this->recursiveAddObjectDefinition(theObj);
                    this->controller->parserTreeAdd(statement);
                    break;
                }
                    
                case HSSStatementTypeComment:
                {
                    std_log1(statement->toString());
                    this->controller->parserTreeAdd(statement);
                    break;
                }
                    
                case HSSStatementTypeInstruction:
                {
                    HSSInstruction::p theInstr = boost::static_pointer_cast<HSSInstruction>(statement);
                    switch (theInstr->getInstructionType()) {
                        case HSSImportInstruction:
                        {
                            //save
                            HSSTokenizer::p currentTokenizer = this->tokenizer;
                            std::string currentBasepath = this->basepath;
                            AXRFile::p currentFile = this->currentFile;
                            unsigned currentLine = this->line;
                            unsigned currentColumn = this->column;
                            HSSToken::p currentCurrentToken = this->currentToken;
                            std::vector<HSSParserContext> currentCurrentContext = this->currentContext;
                            std::stack<HSSObject::p> currentCurrentObjectContext = this->currentObjectContext;
                            
                            //load
                            this->tokenizer = HSSTokenizer::p(new HSSTokenizer());
                            this->line = 1;
                            this->column = 1;
                            try {
                                AXRFile::p theFile = this->wrapper->getFile("file://"+this->basepath+"/"+theInstr->getValue());
                                this->loadFile(theFile);
                                
                            } catch (AXRError::p e) {
                                e->raise();
                                //restore
                                this->tokenizer = currentTokenizer;
                                this->basepath = currentBasepath;
                                this->currentFile = currentFile;
                                this->line = currentLine;
                                this->column = currentColumn;
                                this->currentToken = currentCurrentToken;
                                this->currentContext = currentCurrentContext;
                                this->currentObjectContext = currentCurrentObjectContext;
                                
                                break;
                            }
                            
                            
                            //restore
                            this->tokenizer = currentTokenizer;
                            this->basepath = currentBasepath;
                            this->currentFile = currentFile;
                            this->line = currentLine;
                            this->column = currentColumn;
                            this->currentToken = currentCurrentToken;
                            this->currentContext = currentCurrentContext;
                            this->currentObjectContext = currentCurrentObjectContext;
                            
                            this->controller->parserTreeAdd(statement);
                            
                            break;
                        }
                        default:
                            std_log("Error: Unknown instruction type");
                            break;
                    }
                    break;
                }
                
                default:
                    std_log("Error: Unknown statement type");
                    break;
            }
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
    //if(this->currentContext.back() == HSSParserContextRoot)
    //{
        //the file was empty
        if(this->atEndOfSource())
            return ret;
        
        
        switch (this->currentToken->getType()) {
            case HSSInstructionSign:
            {
                HSSInstruction::p theinstr = this->readInstruction();
                
                switch (theinstr->getInstructionType()) {
                    case HSSGrayscale1Instruction:
                    case HSSGrayscale2Instruction:
                    case HSSRGBInstruction:
                    case HSSRRGGBBInstruction:
                    case HSSRGBAInstruction:
                    case HSSRRGGBBAAInstruction:
                    {
                        HSSObjectDefinition::p objdef = this->getObjectFromInstruction(theinstr);
                        this->skip(HSSWhitespace);
                        if(this->currentToken->isA(HSSIdentifier)){
                            objdef->getObject()->setName(VALUE_TOKEN(this->currentToken)->getString());
                            this->readNextToken(true);
                        }
                        ret = objdef;
                        this->skipExpected(HSSEndOfStatement);
                        if(!this->atEndOfSource()){
                            this->skip(HSSWhitespace);
                        }
                        
                        break;
                    }
                        
                    default:
                        ret = theinstr;
                        break;
                }
                
                break;
            }
            
            //if the statement starts with an object sign, it is an object definition
            case HSSObjectSign:
            {
                //create an object definition
                ret = this->readObjectDefinition("");
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
                throw AXRWarning::p(new AXRWarning("HSSParser", "Unexpected token of type "+HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->fileName, this->line, this->column));
                break;
        }
        
        return ret;
//    } else {
//        std_log1("reading in anything other than root context is not implemented yet");
//        return ret;
//    }
}

HSSRule::p HSSParser::readRule()
{
    security_brake_init()
    
    //throw error if at end of source
    this->checkForUnexpectedEndOfSource();
    
    //initialize the rule
    HSSSelectorChain::p selectorChain;
    try {
        selectorChain = this->readSelectorChain(HSSBlockOpen);
    } catch (AXRError::p e) {
        this->readNextToken();
        this->checkForUnexpectedEndOfSource();
        this->skip(HSSWhitespace);
        
        e->raise();
        
        //return an empty rule
        HSSRule::p ret;
        return ret;
    }
    
    HSSRule::p ret = HSSRule::p(new HSSRule());
    ret->setSelectorChain(selectorChain);
    
    //now we're inside the block
    this->currentContext.push_back(HSSParserContextBlock);
    
    //read the inner part of the block
    while (!this->currentToken->isA(HSSBlockClose))
    {
        try {
            switch (this->currentToken->getType()) {
                case HSSIdentifier:
                case HSSColon:
                case HSSObjectSign:
                {
                    bool isShorthand;
                    if (this->isPropertyDefinition(&isShorthand)){
                        HSSPropertyDefinition::p propertyDefinition = this->readPropertyDefinition(true, isShorthand);
                        if (propertyDefinition){
                            ret->propertiesAdd(propertyDefinition);
                        }
                    } else {
                        this->currentObjectContextAdd(HSSObject::p(new HSSContainer()));
                        HSSRule::p theRule = this->readRule();
                        if(theRule)
                            ret->childrenAdd(theRule);
                        this->currentObjectContextRemoveLast();
                    }
                    break;
                }
                    
                case HSSSymbol:
                {
                    HSSRule::p theRule = this->readRule();
                    if(theRule)
                        ret->childrenAdd(theRule);
                    break;
                }
                    
                case HSSInstructionSign:
                {
                    this->currentObjectContextAdd(HSSObject::p(new HSSContainer()));
                    HSSRule::p childRule = this->readInstructionRule();
                    if(childRule)
                        ret->childrenAdd(childRule);
                    this->currentObjectContextRemoveLast();
                    break;
                }
                    
                default:
                {
                    AXRWarning::p(new AXRWarning("HSSParser", "Unexpected token of type "+HSSToken::tokenStringRepresentation(this->currentToken->getType())+" while reading rule", this->currentFile->fileName, this->line, this->column))->raise();
                    this->readNextToken();
                    this->skip(HSSWhitespace);
                    break;
                }
            }
            
        } catch (AXRError::p e) {
            AXRWarning::p(new AXRWarning("HSSParser", "Invalid value for "+e->getMessage(), this->currentFile->fileName, this->line, this->column))->raise();
            if(!this->atEndOfSource()){
                this->skipUntilEndOfStatement();
                this->readNextToken();
                this->checkForUnexpectedEndOfSource();
                this->skip(HSSWhitespace);
            }
        }
        
        if (this->atEndOfSource()) {
            HSSSelector::p sbjct = selectorChain->subject();
            std::string lmntnm;
            if(sbjct){
                lmntnm = sbjct->getElementName();
            } else {
                lmntnm = "unknown element";
            }
            
            AXRWarning::p(new AXRWarning("HSSParser", "Auto closing block of rule targeting "+lmntnm+" because of unexpected end of file", this->currentFile->fileName, this->line, this->column))->raise();
            //leave the block context
            this->currentContext.pop_back();
            return ret;
        }
        
        security_brake()
    }
    security_brake_reset()
    
    //reset the index of the shorthand properties
    this->currentObjectContext.top()->shorthandReset();
    
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
    //parse the selector chain until we find the token to stop on
    while (this->currentToken && !this->currentToken->isA(stopOn)) {
        
        
        switch (this->currentToken->getType()) {
            case HSSIdentifier:
            {
                //if it's an identifier, it's a simple selector
                ret->add(this->readSelector());
                //adds only if needed
                HSSCombinator::p childrenCombinator(this->readChildrenCombinatorOrSkip());
                if(childrenCombinator){
                    ret->add(childrenCombinator);
                }
                break;
            }
            
            case HSSSymbol:
            {
                //a symbol, probably a combinator
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
                        if (VALUE_TOKEN(this->currentToken)->getString() == ".."){
                            ret->add(this->readSymbolCombinator());
                            break;
                        }
                        
                    case '!':
                    {
                        //it's a negation
                        ret->add(HSSNegation::p(new HSSNegation()));
                        this->readNextToken();
                        break;
                    }
                        
                        //huh? we didn't expect any other symbol
                    default:
                        throw AXRError::p(new AXRWarning("HSSParser", "Unexpected token of type "+HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->fileName, this->line, this->column));
                }
                break;
            }
                
            case HSSColon:
            {
                //if there is another colon, it is a flag
                this->readNextToken(true);
                if(this->currentToken->isA(HSSColon)){
                    this->readNextToken(true);
                    HSSParserNode::p theFlag = this->readFlag();
                    if(theFlag){
                        ret->add(theFlag);
                    }
                    
                } else {
                    //it is a filter
                    HSSParserNode::p theFilter = this->readFilter();
                    if(theFilter){
                        ret->add(theFilter);
                    }
                }
                //adds only if needed
                ret->add(this->readChildrenCombinatorOrSkip());
                break;
            }
                
            case HSSObjectSign:
            {
                this->readNextToken(true);
                if(this->currentToken->isA(HSSIdentifier)){
                    std::string objtype = VALUE_TOKEN(this->currentToken)->getString();
                    if (objtype == "this") {
                        ret->add(HSSThisSelector::p(new HSSThisSelector()));
                        this->readNextToken(true);
                        //adds only if needed
                        HSSCombinator::p childrenCombinator(this->readChildrenCombinatorOrSkip());
                        if(childrenCombinator){
                            ret->add(childrenCombinator);
                        }
                        break;
                    } else if (objtype == "super"){
                        //FIXME
                        std_log("@super not implemented yet");
                    } else if (objtype == "parent"){
                        //FIXME
                        std_log("@parent not implemented yet");
                    } else if (objtype == "root"){
                        //FIXME
                        std_log("@root not implemented yet");
                    } else {
                        throw AXRError::p(new AXRWarning("HSSParser", "No objects other than @this, @super, @parent or @root are supported in selectors.", this->currentFile->fileName, this->line, this->column));
                    }
                } else if(this->currentToken->isA(HSSWhitespace) || this->currentToken->isA(HSSBlockOpen) || this->currentToken->isA(HSSColon)){
                    ret->add(HSSThisSelector::p(new HSSThisSelector()));
                    this->skip(HSSWhitespace, true);
                    break;
                }
                
                //fall through
            }
                
            default:
            {
                //we didn't expect any other type of token
                throw AXRError::p(new AXRWarning("HSSParser", "Unexpected token of type "+HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->fileName, this->line, this->column));
            }
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

//wether it is or not a shorthand will be passed to the isShorthand pointer
bool HSSParser::isPropertyDefinition(bool * isShorthand)
{
    bool ret = true;
    *isShorthand = false;
    
    //property definitions always start with identifiers (the property)
//    if(!this->currentToken->isA(HSSIdentifier)){
//        return false;
//    }
    
    std_log4("----- peeking ------ ");
    HSSToken::p peekToken = this->tokenizer->peekNextToken();
    std_log4(peekToken->toString());
    //skip all whitespace and comments
    while (peekToken && (peekToken->isA(HSSWhitespace) || peekToken->isA(HSSBlockComment) || peekToken->isA(HSSLineComment)))
    {
        peekToken = this->tokenizer->peekNextToken();
    }
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
                std_log4(peekToken->toString());
                peekToken = this->tokenizer->peekNextToken();
                this->checkForUnexpectedEndOfSource();
            }
            //if we find an opening block, we're dealing with a selector
            if(peekToken->isA(HSSBlockOpen)){
                ret = false;
            }
        }
    } else {
        if(this->currentToken->isA(HSSObjectSign)){
            if(peekToken->isA(HSSIdentifier)){
                std::string objtype = VALUE_TOKEN(peekToken)->getString();
                if (    (objtype == "this")
                    ||  (objtype == "super")
                    ||  (objtype == "parent")
                    ||  (objtype == "root") ){
                    //it is a selector
                    ret = false;
                    *isShorthand = false;
                } else {
                    //it is a shorthand with an object definition
                    ret = true;
                    *isShorthand = true;
                }
            } else {
                //it is a shorthand with an object definition
                ret = true;
                *isShorthand = true;
            }
            
        } else {
            //no colon, it may be a rule -- we peek until we find the end of the statement
            //or we can conclude it actually is a rule
            bool done = false;
            while(!done)
            {
                switch (peekToken->getType()) {
                    case HSSBlockClose:
                    case HSSEndOfStatement:
                        //it is a shorthand
                        ret = true;
                        done = true;
                        *isShorthand = true;
                        break;
                        
                    case HSSBlockOpen:
                        //it is a 
                        ret = false;
                        done = true;
                        break;
                        
                    default:
                        break;
                }
                
                if(!done){
                    peekToken = this->tokenizer->peekNextToken();
                    this->checkForUnexpectedEndOfSource();
                }
            }
        }
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
            throw AXRError::p(new AXRWarning("HSSParser", "Unexpected token of type "+HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->fileName, this->line, this->column));
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
HSSObjectDefinition::p HSSParser::readObjectDefinition(std::string propertyName)
{
    HSSObjectDefinition::p ret;
    std::string objtype;
    HSSObject::p obj;
    
    //yeah, yeah, we know the @ already
    this->skipExpected(HSSObjectSign);
    //end of file would be fatal
    this->checkForUnexpectedEndOfSource();
    
    //set the appropriate context
    this->currentContext.push_back(HSSParserContextObjectDefinition);
    
    //first we need to know what type of object it is
    if (this->currentToken->isA(HSSWhitespace) || this->currentToken->isA(HSSBlockOpen) ) {
        //damn, we'll have to derive that from the context
        if (this->currentObjectContext.size() > 0){
            if (propertyName == ""){
                objtype = this->currentObjectContext.top()->defaultObjectType();
            } else {
                objtype = this->currentObjectContext.top()->defaultObjectType(propertyName);
            }
            
        } else {
            objtype = "container";
        }
    } else if(this->currentToken->isA(HSSIdentifier)){
        //alright, we've got a type, look it up
        objtype = VALUE_TOKEN(this->currentToken)->getString();
        this->readNextToken();
        this->checkForUnexpectedEndOfSource();
    } else {
        throw AXRError::p(new AXRError("HSSParser", "Unexpected token while reading object definition: "+HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->fileName, this->line, this->column));
    }
    
    //try to create an object of that type
    try {
        obj = HSSObject::newObjectWithType(objtype);
    } catch (AXRError::p e) {
        AXRWarning::p(new AXRWarning("HSSParser", "Invalid object name "+e->getMessage(), this->currentFile->fileName, this->line, this->column))->raise();
        if (this->currentObjectContext.size() > 0){
            if (propertyName == ""){
                objtype = this->currentObjectContext.top()->defaultObjectType();
            } else {
                objtype = this->currentObjectContext.top()->defaultObjectType(propertyName);
            }
        } else {
            objtype = "value";
        }
        
        try {
            obj = HSSObject::newObjectWithType(objtype);
        } catch (AXRError::p e) {
            obj = HSSObject::newObjectWithType("value");
        }
    }
    obj->setController(this->controller);
    
    
    //get the name of the object
    if (this->currentToken->isA(HSSWhitespace)) {
        this->skip(HSSWhitespace);
    }
    
    switch (this->currentToken->getType()) {
        case HSSIdentifier:
            obj->setName(VALUE_TOKEN(this->currentToken)->getString());
            std_log3("setting its name to "+VALUE_TOKEN(this->currentToken)->getString());
            this->readNextToken();
            break;
        case HSSBlockOpen:
            //it is the opening curly brace, therefore an annonymous object:
            //do nothing
            break;
        case HSSEndOfStatement:
        case HSSBlockClose:
        case HSSParenthesisClose:
        {
            //the property definition ends here
            ret = HSSObjectDefinition::p(new HSSObjectDefinition(obj));
            return ret;
        }
            
        default:
            throw AXRError::p(new AXRError("HSSParser", "Unexpected token while reading object definition: "+HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->fileName, this->line, this->column));
            break;
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
    
    security_brake_init()
    while (!this->currentToken->isA(HSSBlockClose))
    {
        try {
            switch (this->currentToken->getType()) {
                case HSSSymbol:
                {
                    if(VALUE_TOKEN(this->currentToken)->getString() == "&"){
                        HSSToken::p peekToken = this->tokenizer->peekNextToken();
                        if(peekToken->isA(HSSObjectSign)){
                            this->tokenizer->resetPeek();
                            this->readNextToken(true);
                            HSSObjectDefinition::p childDef = this->readObjectDefinition(propertyName);
                            childDef->setParentNode(ret);
                            ret->childrenAdd(childDef);
                            break;
                        } else {
                            this->tokenizer->resetPeek();
                            //fall through (to default, or else FIXME)
                        }
                    } else {
                        HSSRule::p theRule = this->readRule();
                        if(theRule)
                            ret->rulesAdd(theRule);
                        break;
                    }
                }
                    
                case HSSIdentifier:
                case HSSColon:
                case HSSObjectSign:
                {
                    bool isShorthand;
                    if (this->isPropertyDefinition(&isShorthand)){
                        HSSPropertyDefinition::p propertyDefinition = this->readPropertyDefinition(true, isShorthand);
                        if (propertyDefinition){
                            ret->propertiesAdd(propertyDefinition);
                        }
                    } else {
                        this->currentObjectContextAdd(HSSObject::p(new HSSContainer()));
                        HSSRule::p theRule = this->readRule();
                        if(theRule)
                            ret->rulesAdd(theRule);
                        this->currentObjectContextRemoveLast();
                    }
                    break;
                }
                
                case HSSInstructionSign:
                {
                    bool isShorthand;
                    if (this->isPropertyDefinition(&isShorthand)){
                        HSSPropertyDefinition::p propertyDefinition = this->readPropertyDefinition(true, isShorthand);
                        if (propertyDefinition){
                            ret->propertiesAdd(propertyDefinition);
                        }
                    } else {
                        HSSRule::p childRule = this->readInstructionRule();
                        if(childRule)
                            ret->rulesAdd(childRule);
                    }
                    break;
                }
                    
                default:
                {
                    bool isShorthand;
                    if (this->isPropertyDefinition(&isShorthand)){
                        HSSPropertyDefinition::p propertyDefinition = this->readPropertyDefinition(true, isShorthand);
                        if (propertyDefinition){
                            ret->propertiesAdd(propertyDefinition);
                        }
                    } else {
                        AXRWarning::p(new AXRWarning("HSSParser", "Unexpected token of type "+HSSToken::tokenStringRepresentation(this->currentToken->getType())+" while reading object definition", this->currentFile->fileName, this->line, this->column))->raise();
                        this->readNextToken();
                        this->skip(HSSWhitespace);
                    }
                    break;
                }
            }
            
        } catch (AXRError::p e) {
            AXRWarning::p(new AXRWarning("HSSParser", "Invalid value for "+e->getMessage(), this->currentFile->fileName, this->line, this->column))->raise();
            if(!this->atEndOfSource()){
                this->skipUntilEndOfStatement();
                this->readNextToken();
                this->checkForUnexpectedEndOfSource();
                this->skip(HSSWhitespace);
            }
        }
        
        if (this->atEndOfSource()) {
            AXRWarning::p(new AXRWarning("HSSParser", "Auto closing block of object definition with name "+obj->getName()+" because of unexpected end of file", this->currentFile->fileName, this->line, this->column))->raise();
            //leave the block context
            this->currentContext.pop_back();
            return ret;
        }
        
        security_brake()
    }
    security_brake_reset()
    
    //reset the index of the shorthand properties
    obj->shorthandReset();
    //out we are
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


void HSSParser::recursiveAddObjectDefinition(HSSObjectDefinition::p objDef)
{
    this->controller->objectTreeAdd(objDef);
    unsigned i, size;
    const std::vector<HSSObjectDefinition::p>children = objDef->getChildren();
    for (i=0, size=children.size(); i<size; i++) {
        HSSObjectDefinition::p child = children[i];
        std::deque<HSSPropertyDefinition::p> properties = objDef->getProperties();
        unsigned i;
        //prepend backwards
        for (i=properties.size(); i>0; i--){
            child->propertiesPrepend(properties[i-1]);
        }
        std::deque<HSSRule::p>::const_reverse_iterator it;
        const std::deque<HSSRule::p> rules = objDef->getRules();
        for (it=rules.rbegin(); it!=rules.rend(); it++) {
            child->rulesPrepend((*it));
        }
        this->recursiveAddObjectDefinition(child);
    }
}

HSSPropertyDefinition::p HSSParser::readPropertyDefinition()
{
    return this->readPropertyDefinition(false, false);
}

HSSPropertyDefinition::p HSSParser::readPropertyDefinition(bool shorthandChecked, bool isShorthand)
{
    std::string propertyName;
    
    //end of source is no goodf
    this->checkForUnexpectedEndOfSource();
    
    HSSPropertyDefinition::p ret;
    bool valid = true;
    
    switch (this->currentToken->getType()) {
        case HSSIdentifier:
        {
            bool isPropertyDef = true;
            if (!shorthandChecked){
                isPropertyDef = this->isPropertyDefinition(&isShorthand);
            }
            if (isPropertyDef) {
                if (!isShorthand){
                    try {
                        propertyName = VALUE_TOKEN(this->currentToken)->getString();
                        this->readNextToken();
                        //now must come a colon
                        this->skipExpected(HSSColon);
                        //we don't give a f$%# about whitespace
                        this->skip(HSSWhitespace);
                        this->currentObjectContext.top()->shorthandSkip(propertyName);
                    } catch (AXRError::p e) {
                        e->raise();
                        valid = false;
                    }
                    break;
                }
                
                //fall through to 'default:'
                
            } else {
                //it is not a property definition, abort
                throw AXRError::p(new AXRError("HSSParser", "Failed to read property definition", this->currentFile->fileName, this->line, this->column));
            }
            
            //fall through to 'default:'
        }
        
        default:
        {
            //we assume we are dealing with shorthand notation
            //get the property name for the current value
            try {
                propertyName = this->currentObjectContext.top()->getPropertyForCurrentValue();
                //consume the property
                this->currentObjectContext.top()->shorthandNext();
            } catch (AXRError::p e) {
                AXRError::p(new AXRError("HSSParser", "Could not get property for current value"))->raise();
                valid = false;
            }
            break;
        }
    }
    
    bool done = false;
    ret = HSSPropertyDefinition::p(new HSSPropertyDefinition(propertyName));
    
    while (!done && valid) {
        try {
            //now comes either an object definition, a literal value or an expression
            //object
            if (this->currentToken->isA(HSSObjectSign)){
                HSSObjectDefinition::p objdef = this->readObjectDefinition(propertyName);
                if(objdef){
                    ret->addValue(objdef);
                } else {
                    valid = false;
                }
                //this->readNextToken();
                
            } else if (this->currentToken->isA(HSSSingleQuoteString) || this->currentToken->isA(HSSDoubleQuoteString)){
                ret->addValue(HSSStringConstant::p(new HSSStringConstant(VALUE_TOKEN(this->currentToken)->getString())));
                this->checkForUnexpectedEndOfSource();
                this->readNextToken();
                this->skip(HSSWhitespace);
                
                //number literal
            } else if (this->currentToken->isA(HSSNumber) || this->currentToken->isA(HSSPercentageNumber) || this->currentToken->isA(HSSParenthesisOpen)){
                //FIXME: parse the number and see if it is an int or a float
                HSSParserNode::p exp = this->readExpression();
                if(exp){
                    ret->addValue(exp);
                } else {
                    valid = false;
                }
                                
                
            } else if (this->currentToken->isA(HSSIdentifier)){
                //this is either a function, a keyword or an object name
                
                std::string valuestr = VALUE_TOKEN(this->currentToken)->getString();
                //check if it is a function
                HSSObject::p objectContext = this->currentObjectContext.top();
                
                if (objectContext->isFunction(valuestr, propertyName)){
                    HSSParserNode::p exp = this->readExpression();
                    if (exp){
                        ret->addValue(exp);
                    } else {
                        valid = false;
                    }
                    
                    //check if it is a keyword
                } else if (objectContext->isKeyword(valuestr, propertyName)){
                    ret->addValue(HSSKeywordConstant::p(new HSSKeywordConstant(valuestr))); 
                    this->readNextToken();
                    //we assume it is an object name at this point
                } else {
                    //FIXME (?)
                    ret->addValue(HSSObjectNameConstant::p(new HSSObjectNameConstant(valuestr)));
                    this->readNextToken();
                }
                
                
            } else if (this->currentToken->isA(HSSInstructionSign)){
                HSSInstruction::p theInstruction;
                
                theInstruction = this->readInstruction();
                
                if (theInstruction){
                    ret->addValue(this->getObjectFromInstruction(theInstruction));
                } else {
                    valid = false;
                }
                
            } else {
                valid = false;
            }
            
            if (!valid){
                done = true;
            }
        } catch (AXRError::p e) {
            e->raise();
            valid = false;
            done = true;
        }
        
        if(valid){
            this->skip(HSSWhitespace, true);
            if(this->currentToken->isA(HSSComma)){
                this->readNextToken();
                this->skip(HSSWhitespace, true);
            } else {
                done = true;
            }
        }
    }
    if (valid) {
        //skip the semicolon
        this->skip(HSSEndOfStatement);
        this->skip(HSSWhitespace);
    } else if (!this->atEndOfSource()){
        this->skipUntilEndOfStatement();
    }
    
    if (!valid) {
        throw AXRError::p(new AXRError("HSSParser", "Errors found while reading "+propertyName, this->currentFile->fileName, this->line, this->column));
    }
    return ret;
}


HSSInstruction::p HSSParser::readInstruction()
{
    return this->readInstruction(true);
}

HSSInstruction::p HSSParser::readInstruction(bool preferHex)
{
    HSSInstruction::p ret;
    std::string currentval;
    
    //set preference
    this->tokenizer->preferHex = preferHex;
    //skip the instruction sign -- this will automatically read the next token
    this->skipExpected(HSSInstructionSign);
    //restore
    this->tokenizer->preferHex = false;
    
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
                //balk
                AXRError::p(new AXRWarning("HSSParser", "Wrong length for hexadecimal number (should be 1, 2, 3, 4, 6 or 8 digits long)", this->currentFile->fileName, this->line, this->column))->raise();
                return ret;
        }
        
        this->skip(HSSWhitespace, true);
                
    } else if (this->currentToken->isA(HSSIdentifier)){
        currentval = VALUE_TOKEN(this->currentToken)->getString();
        if (currentval == "new"){
            ret = HSSInstruction::p(new HSSInstruction(HSSNewInstruction));
            this->readNextToken(true);
            if (this->currentToken->isA(HSSParenthesisOpen)) {
                this->readNextToken(true);
                this->skip(HSSWhitespace);
                if (this->currentToken->isA(HSSNumber)) {
                    long double number = VALUE_TOKEN(this->currentToken)->getLong();
                    ret->setArgument(HSSNumberConstant::p(new HSSNumberConstant(number)));
                    this->readNextToken(true);
                    this->skip(HSSWhitespace);
                    this->skipExpected(HSSParenthesisClose);
                    this->skip(HSSWhitespace);
                } else {
                    throw AXRError::p(new AXRError("HSSParser", "Unknown value in argument of new statement", this->currentFile->fileName, this->line, this->column));
                }
            }
        } else if (currentval == "ensure") {
            ret = HSSInstruction::p(new HSSInstruction(HSSEnsureInstruction));
            this->readNextToken();
        } else if (currentval == "import") {
            
            this->readNextToken();
            this->checkForUnexpectedEndOfSource();
            this->skip(HSSWhitespace);
            
            if (this->currentToken->isA(HSSDoubleQuoteString) || this->currentToken->isA(HSSSingleQuoteString)) {
                std::string theString = VALUE_TOKEN(this->currentToken)->getString();
                ret = HSSInstruction::p(new HSSInstruction(HSSImportInstruction, theString));
                
            } else {
                throw AXRError::p(new AXRError("HSSParser", "Expected string", this->currentFile->fileName, this->line, this->column));
            }
            
            this->readNextToken();
            if (!this->atEndOfSource()) {
                this->skip(HSSWhitespace);
            }
            if (!this->atEndOfSource()) {
                this->skip(HSSEndOfStatement);
            }
            
        } else if (currentval == "move") {
            ret = HSSInstruction::p(new HSSInstruction(HSSMoveInstruction));
            this->readNextToken();
        } else if (currentval == "delete"){
            
            this->readNextToken();
            this->checkForUnexpectedEndOfSource();
            this->skip(HSSWhitespace);
            ret = HSSInstruction::p(new HSSInstruction(HSSDeleteInstruction));
            
        } else {
            //balk
            throw AXRWarning::p(new AXRWarning("HSSParser", "Unknown instruction "+currentval, this->currentFile->fileName, this->line, this->column));
        }
        
    } else {
        //balk
        throw AXRWarning::p(new AXRWarning("HSSParser", "Unexpected token "+HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->fileName, this->line, this->column));
    }
    
    if(!this->atEndOfSource()){
        this->skip(HSSWhitespace);
    }
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
            HSSRgb::p obj = HSSRgb::p(new HSSRgb());
            
            unsigned int hexValue;
            std::string tempstr = instruction->getValue();
            if(instructionType == HSSGrayscale1Instruction){
                tempstr = tempstr + tempstr;
            }
            sscanf(tempstr.c_str(), "%X", &hexValue);
            
            ret = HSSObjectDefinition::p(new HSSObjectDefinition(obj));
            
            HSSPropertyDefinition::p newRed = HSSPropertyDefinition::p(new HSSPropertyDefinition());
            newRed->setName("red");
            newRed->setValue(HSSNumberConstant::p(new HSSNumberConstant(hexValue)));
            ret->propertiesAdd(newRed);
            
            HSSPropertyDefinition::p newGreen = HSSPropertyDefinition::p(new HSSPropertyDefinition());
            newGreen->setName("green");
            newGreen->setValue(HSSNumberConstant::p(new HSSNumberConstant(hexValue)));
            ret->propertiesAdd(newGreen);
            
            HSSPropertyDefinition::p newBlue = HSSPropertyDefinition::p(new HSSPropertyDefinition());
            newBlue->setName("blue");
            newBlue->setValue(HSSNumberConstant::p(new HSSNumberConstant(hexValue)));
            ret->propertiesAdd(newBlue);
            
            break;
        }
            
        case HSSRGBInstruction:
        case HSSRGBAInstruction:
        case HSSRRGGBBInstruction:
        case HSSRRGGBBAAInstruction:
        {
            //try to create an object of that type
            HSSRgb::p obj = HSSRgb::p(new HSSRgb());
            
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
            
            ret = HSSObjectDefinition::p(new HSSObjectDefinition(obj));
            
            HSSPropertyDefinition::p newRed = HSSPropertyDefinition::p(new HSSPropertyDefinition());
            newRed->setName("red");
            newRed->setValue(HSSNumberConstant::p(new HSSNumberConstant(redHex)));
            ret->propertiesAdd(newRed);
            
            HSSPropertyDefinition::p newGreen = HSSPropertyDefinition::p(new HSSPropertyDefinition());
            newGreen->setName("green");
            newGreen->setValue(HSSNumberConstant::p(new HSSNumberConstant(greenHex)));
            ret->propertiesAdd(newGreen);
            
            HSSPropertyDefinition::p newBlue = HSSPropertyDefinition::p(new HSSPropertyDefinition());
            newBlue->setName("blue");
            newBlue->setValue(HSSNumberConstant::p(new HSSNumberConstant(blueHex)));
            ret->propertiesAdd(newBlue);
            
            HSSPropertyDefinition::p newAlpha = HSSPropertyDefinition::p(new HSSPropertyDefinition());
            newAlpha->setName("alpha");
            newAlpha->setValue(HSSNumberConstant::p(new HSSNumberConstant(alphaHex)));
            ret->propertiesAdd(newAlpha);
            
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
    HSSInstruction::p instruction = this->readInstruction(false);
    HSSRule::p ret;
    switch (instruction->getInstructionType()) {
        case HSSNewInstruction:
        case HSSEnsureInstruction:
        case HSSMoveInstruction:
        {
            ret = this->readRule();
            if(ret){
                ret->setInstruction(instruction);
            }
            
            break;
        }
            
        case HSSDeleteInstruction:
        {
            //initialize the rule
            HSSSelectorChain::p selectorChain;
            try {
                selectorChain = this->readSelectorChain(HSSEndOfStatement);
            } catch (AXRError::p e) {
                this->readNextToken();
                this->checkForUnexpectedEndOfSource();
                this->skip(HSSWhitespace);
                
                e->raise();
                
                //return an empty rule
                HSSRule::p ret;
                return ret;
            }
            
            ret = HSSRule::p(new HSSRule());
            ret->setSelectorChain(selectorChain);
            ret->setInstruction(instruction);
            break;
        }
            
        case HSSImportInstruction:
        {
            std_log("hello");
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
            left = HSSNumberConstant::p(new HSSNumberConstant(VALUE_TOKEN(this->currentToken)->getLong()));
            this->readNextToken();
            this->skip(HSSWhitespace);
            break;
        }
        
        case HSSPercentageNumber:
        {
            left = HSSPercentageConstant::p(new HSSPercentageConstant(VALUE_TOKEN(this->currentToken)->getLong()));
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
            throw AXRError::p(new AXRError("HSSParser", "Unknown token type "+HSSToken::tokenStringRepresentation(this->currentToken->getType())+" while parsing base expression", this->currentFile->fileName, this->line, this->column));
    }
    
    return left;
}

//this method expects the currentToken to be an identifier
HSSParserNode::p HSSParser::readFilter()
{
    HSSFilter::p ret;
    this->expect(HSSIdentifier);
    
    std::string filterName = VALUE_TOKEN(this->currentToken)->getString();
    ret = HSSFilter::newFilterWithStringType(filterName);
    
    this->readNextToken();
    this->checkForUnexpectedEndOfSource();
    
    return ret;
}

//this method expects the currentToken to be an identifier
HSSParserNode::p HSSParser::readFlag()
{
    HSSFlag::p ret;
    this->expect(HSSIdentifier);
    
    std::string flagName = VALUE_TOKEN(this->currentToken)->getString();
    ret = HSSFlag::p(new HSSFlag());
    ret->setName(flagName);
    
    this->readNextToken();
    this->checkForUnexpectedEndOfSource();
    
    return ret;
}

HSSParserNode::p HSSParser::readFunction()
{
    HSSParserNode::p ret;
    
    this->checkForUnexpectedEndOfSource();
    if (this->currentToken->isA(HSSIdentifier)){
        //create new function
        std::string name = VALUE_TOKEN(this->currentToken)->getString();
        if(name == "ref"){
            HSSRefFunction::p refFunction = HSSRefFunction::p(new HSSRefFunction());
            refFunction->setController(this->controller);
            
            this->readNextToken(true);
            this->skip(HSSWhitespace, true);
            this->skipExpected(HSSParenthesisOpen, true);
            this->skip(HSSWhitespace, true);
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
            this->skip(HSSWhitespace, true);
            //if shorthand notation -- assumes 'of @this'
            HSSSelectorChain::p selectorChain;
            if(this->currentToken->isA(HSSParenthesisClose)){
                selectorChain = HSSSelectorChain::p(new HSSSelectorChain());
                selectorChain->add(HSSThisSelector::p(new HSSThisSelector()));
                this->readNextToken(true);
                this->skip(HSSWhitespace, true);
                
            } else {
                if (!this->currentToken->isA(HSSIdentifier) || VALUE_TOKEN(this->currentToken)->getString() != "of"){
                    std_log1("HSSParser: unexpected token while reading ref function: "+HSSToken::tokenStringRepresentation(this->currentToken->getType()));
                }
                this->checkForUnexpectedEndOfSource();
                this->readNextToken(true);
                this->skipExpected(HSSWhitespace, true);
                
                
                //now read the selector chain
                selectorChain = this->readSelectorChain(HSSParenthesisClose);
            }
            
            refFunction->setSelectorChain(selectorChain);
            ret = refFunction;
            
        } else if (name == "sel") {
            this->readNextToken(true);
            this->skip(HSSWhitespace, true);
            this->skipExpected(HSSParenthesisOpen, true);
            this->skip(HSSWhitespace, true);
            //read the selector chain
            HSSSelectorChain::p selectorChain;
            try {
                selectorChain = this->readSelectorChain(HSSParenthesisClose);
            } catch (AXRError::p e) {
                this->readNextToken(true);
                this->skip(HSSWhitespace);
                
                e->raise();
                
                //return an empty function
                return ret;
            }
            
            HSSSelFunction::p selFunction = HSSSelFunction::p(new HSSSelFunction());
            selFunction->setController(this->controller);
            selFunction->setSelectorChain(selectorChain);
            
            ret = selFunction;
            
        } else if(
                    name == "flag"
                ||  name == "unflag"
                ||  name == "toggleFlag"
            ){
            
            HSSFlagFunction::p flagFunction = HSSFlagFunction::p(new HSSFlagFunction(HSSFlagFunction::flagFunctionTypeFromString(name)));
            flagFunction->setController(this->controller);
            
            this->readNextToken(true);
            this->skip(HSSWhitespace, true);
            this->skipExpected(HSSParenthesisOpen, true);
            this->skip(HSSWhitespace, true);
            //read the arguments
            //first, we expect the name
            if (! this->currentToken->isA(HSSIdentifier)){
                std_log1("HSSParser: unexpected token while reading ref function "+name);
            } else {
                flagFunction->setName(VALUE_TOKEN(this->currentToken)->getString());
            }
            
            this->checkForUnexpectedEndOfSource();
            this->readNextToken();
            this->skip(HSSWhitespace, true);
            //if shorthand notation -- assumes 'of @this'
            HSSSelectorChain::p selectorChain;
            if(this->currentToken->isA(HSSParenthesisClose)){
                selectorChain = HSSSelectorChain::p(new HSSSelectorChain());
                selectorChain->add(HSSThisSelector::p(new HSSThisSelector()));
                this->readNextToken(true);
                this->skip(HSSWhitespace, true);
                
            } else {
                if (!this->currentToken->isA(HSSIdentifier) || VALUE_TOKEN(this->currentToken)->getString() != "of"){
                    std_log1("HSSParser: unexpected token while reading ref function: "+HSSToken::tokenStringRepresentation(this->currentToken->getType()));
                }
                this->checkForUnexpectedEndOfSource();
                this->readNextToken(true);
                this->skipExpected(HSSWhitespace, true);
                
                
                //now read the selector chain
                selectorChain = this->readSelectorChain(HSSParenthesisClose);
            }
            
            flagFunction->setSelectorChain(selectorChain);
            ret = flagFunction;
            
        } else if (name == "min") {
            
        } else if (name == "max") {
            
        } else if (name == "ceil") {
            
        } else if (name == "floor") {
            
        } else if (name == "round") {
            
        } else {
            throw AXRError::p(new AXRError("HSSParser", "Unexpected function name: "+name, this->currentFile->fileName, this->line, this->column));
        }
        
    } else {
        throw AXRError::p(new AXRError("HSSParser", "Unexpected token while reading function: "+HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->fileName, this->line, this->column));
    }
    
    return ret;
}

void HSSParser::readNextToken()
{
    this->readNextToken(false);
}

void HSSParser::readNextToken(bool checkForUnexpectedEndOfSource)
{
    //read next one
    try {
        HSSToken::p theToken = this->tokenizer->readNextToken();
        if(theToken && (theToken->isA(HSSBlockComment) || theToken->isA(HSSLineComment))){
            this->readNextToken();
        } else {
            this->currentToken = theToken;
        }
        
    } catch (AXRError::p e) {
        this->currentToken = HSSToken::p();
        throw;
    }
    
    if (!this->atEndOfSource()) {
        this->line = this->currentToken->line;
        this->column = this->currentToken->column;
    } else {
        this->line = this->tokenizer->currentLine;
        this->column = this->tokenizer->currentColumn - 1;
    }
    
    if(checkForUnexpectedEndOfSource) this->checkForUnexpectedEndOfSource();
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
        throw AXRError::p(new AXRError("HSSParser", "The file ended unexpectedly", this->currentFile->fileName, this->line, this->column));
    }
}

void HSSParser::skipExpected(HSSTokenType type)
{
    this->skipExpected(type, false);
}

void HSSParser::skipExpected(HSSTokenType type, bool checkForUnexpectedEndOfSource)
{
    this->checkForUnexpectedEndOfSource();
    if (!this->currentToken->isA(type)) {
        throw AXRError::p(new AXRError("HSSParser", "Expected token of type "+HSSToken::tokenStringRepresentation(type), this->currentFile->fileName, this->line, this->column));
    }
    this->readNextToken(checkForUnexpectedEndOfSource);
}

void HSSParser::skipExpected(HSSTokenType type, std::string value)
{
    this->skipExpected(type, value, false);
}

void HSSParser::skipExpected(HSSTokenType type, std::string value, bool checkForUnexpectedEndOfSource)
{
    this->checkForUnexpectedEndOfSource();
    //FIXME: I'm not sure if this works as expected
    HSSValueToken::p currentToken = HSSValueToken::p(VALUE_TOKEN(this->currentToken));
    if (!currentToken->equals(type, value)) {
        throw AXRError::p(new AXRError("HSSParser", "Expected token of type "+HSSToken::tokenStringRepresentation(type)+" and value "+value, this->currentFile->fileName, this->line, this->column));
    }
    this->readNextToken(checkForUnexpectedEndOfSource);
}

void HSSParser::skip(HSSTokenType type)
{
    this->skip(type, false);
}

void HSSParser::skip(HSSTokenType type, bool checkForUnexpectedEndOfSource)
{
    if(this->currentToken->isA(type)){
        if(type == HSSWhitespace){
            while (!this->atEndOfSource() && this->currentToken->isA(type)) {
                this->readNextToken();
            }
            if (checkForUnexpectedEndOfSource) {
                this->checkForUnexpectedEndOfSource();
            }
        } else {
            this->readNextToken(checkForUnexpectedEndOfSource);
        }
    }
}

void HSSParser::skipUntilEndOfStatement()
{
    while (!this->currentToken->isA(HSSEndOfStatement) && !this->currentToken->isA(HSSBlockClose)) {
        this->readNextToken();
        if (this->currentToken->isA(HSSBlockOpen)) {
            this->skipUntilEndOfStatement();
            this->readNextToken();
        }
        this->checkForUnexpectedEndOfSource();
    }
}

void HSSParser::expect(HSSTokenType type){
    if (!this->currentToken->isA(type))
        throw AXRError::p(new AXRError("HSSParser", "Expected token of type "+HSSToken::tokenStringRepresentation(type), this->currentFile->fileName, this->line, this->column));
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

void HSSParser::setBasePath(std::string value)
{
    this->basepath = value;
}

std::string HSSParser::getBasePath()
{
    return this->basepath;
}




