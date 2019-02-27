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
 *      AUTHORS: see AUTHORS file
 *
 *      COPYRIGHT: ©2019 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axrproject.org
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#include <stack>
#include <QSharedPointer>
#include "HSSCodeParser.h"
#include "AXRBuffer.h"
#include "AXRController.h"
#include "AXRDocument.h"
#include "AXRError.h"
#include "AXRWarning.h"
#include "AXRLoggerManager.h"
#include "HSSAttrFunction.h"
#include "HSSComment.h"
#include "HSSCommentNode.h"
#include "HSSContainer.h"
#include "HSSCombinator.h"
#include "HSSDivision.h"
#include "HSSDisplayObject.h"
#include "HSSEventSelector.h"
#include "HSSFlag.h"
#include "HSSFlagFunction.h"
#include "HSSInsertFunction.h"
#include "HSSInstruction.h"
#include "HSSInvalidNode.h"
#include "HSSKeywordConstant.h"
#include "HSSLogFunction.h"
#include "HSSMultiplication.h"
#include "HSSNumberConstant.h"
#include "HSSObject.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSOverrideFunction.h"
#include "HSSParentSelector.h"
#include "HSSParserReceiver.h"
#include "HSSPercentageConstant.h"
#include "HSSPropertyDefinition.h"
#include "HSSPropertyPath.h"
#include "HSSPropertyPathNode.h"
#include "HSSSelFunction.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelector.h"
#include "HSSStartTimerFunction.h"
#include "HSSStatement.h"
#include "HSSStringConstant.h"
#include "HSSStopTimerFunction.h"
#include "HSSSubtraction.h"
#include "HSSSum.h"
#include "HSSSymbolNode.h"
#include "HSSRefFunction.h"
#include "HSSRgb.h"
#include "HSSRule.h"
#include "HSSRootSelector.h"
#include "HSSThisSelector.h"
#include "HSSToggleTimerFunction.h"
#include "HSSToken.h"
#include "HSSTokenizer.h"
#include "HSSUniversalSelector.h"
#include "HSSValueToken.h"
#include "HSSWhitespaceNode.h"

using namespace AXR;

namespace AXR
{
    class HSSCodeParserPrivate
    {
        friend class HSSCodeParser;
        
        HSSCodeParserPrivate()
        : hasLoadedFile(), file(), tokenizer(), line(1), column(1), controller(), receiver()
        {
        }

        AXRController * controller;
        HSSParserReceiver * receiver;

        bool hasLoadedFile;
        
        QSharedPointer<HSSTokenizer> tokenizer;
        QSharedPointer<AXRBuffer> file;
        QList<QSharedPointer<AXRBuffer> > loadedFiles;
        
        QSharedPointer<HSSToken> currentToken;
        qint64 line;
        qint64 column;
        std::vector<HSSParserContext> currentContext;
        std::stack<QSharedPointer<HSSObject> > currentObjectContext;
        AXRString _lastObjectType;
        QSharedPointer<HSSContainer> _containerContextObj;
    };
}

HSSCodeParser::HSSCodeParser(HSSParserReceiver * receiver)
: d(new HSSCodeParserPrivate)
{
    QSharedPointer<HSSTokenizer> tokenizer = QSharedPointer<HSSTokenizer>(new HSSTokenizer());
    d->tokenizer = tokenizer;
    d->currentContext.push_back(HSSParserContextRoot);
    d->_containerContextObj = QSharedPointer<HSSContainer>(new HSSContainer(NULL));
    d->receiver = receiver;
}

HSSCodeParser::~HSSCodeParser()
{
    delete d;
}

void HSSCodeParser::reset()
{
    d->tokenizer->reset();
    d->hasLoadedFile = false;
    d->file.clear();
}

bool HSSCodeParser::isFileLoaded() const
{
    return d->hasLoadedFile;
}

QSharedPointer<AXRBuffer> HSSCodeParser::file() const
{
    return d->file;
}

void HSSCodeParser::setFile(QSharedPointer<AXRBuffer> file)
{
    d->file = file;
    QUrl filePath = file->sourceUrl();
}

AXRController * HSSCodeParser::controller()
{
    return d->controller;
}

void HSSCodeParser::setController(AXRController * controller)
{
    d->controller = controller;
    d->_containerContextObj->setController(controller);
}

bool HSSCodeParser::loadFile(QSharedPointer<AXRBuffer> file)
{
    axr_log(LoggerChannelOverview, "HSSParser: loading file " + file->sourceUrl().toString());

    //check if the file has been loaded already
    if (d->loadedFiles.contains(file))
    {
        AXRError("HSSParser", "Failed loading file " + file->sourceUrl().toString() + " because of circular reference").raise();
        return false;
    }

    this->parseHssFile(file);
    
    return true;
}

bool HSSCodeParser::parseHssFile(QSharedPointer<AXRBuffer> buffer)
{
    //needs reset on next load
    d->hasLoadedFile = true;
    d->file = buffer;
    
    d->tokenizer->setFile(buffer);
    d->tokenizer->readNextChar();
    
    this->readNextToken();
    
    QSharedPointer<HSSStatement> statement;
    
    bool done = this->atEndOfSource();
    
    if(!done) this->skip(HSSWhitespace);
    
    while (!done)
    {
        if (statement)
        {
            statement.clear();
        }
        if (this->atEndOfSource())
        {
            done = true;
        }
        else
        {
            try
            {
                this->readNextStatement();
            }
            catch (const AXRError &e)
            {
                e.raise();
                this->readNextToken();
                if (!this->atEndOfSource())
                {
                    this->skip(HSSWhitespace);
                }
            }
        }
    }
    
    return true;
}

bool HSSCodeParser::readNextStatement()
{
    bool ret = true;
    
    //the file was empty
    if (this->atEndOfSource())
        return false;
    
    switch (d->currentToken->getType())
    {
        case HSSInstructionSign:
        {
            QSharedPointer<HSSInstruction> theInstr = this->readInstruction();
            if (!theInstr)
            {
                ret = false;
                break;
            }

            switch (theInstr->getInstructionType())
            {
                case HSSGrayscale1Instruction:
                case HSSGrayscale2Instruction:
                case HSSRGBInstruction:
                case HSSRGBAInstruction:
                case HSSRGBAAInstruction:
                case HSSRRGGBBInstruction:
                case HSSRRGGBBAInstruction:
                case HSSRRGGBBAAInstruction:
                {
                    QSharedPointer<HSSObjectDefinition> theObj = this->getObjectFromInstruction(theInstr);
                    if (this->atEndOfSource())
                        return false;

                    this->skip(HSSWhitespace);
                    if (this->atEndOfSource())
                        return false;
                    

                    if (d->currentToken->isA(HSSIdentifier))
                    {
                        theObj->getObject()->setName(VALUE_TOKEN(d->currentToken)->getString());

                        this->readNextToken();
                        if (this->atEndOfSource())
                            return false;
                    }

                    this->recursiveAddObjectDefinition(theObj);
                    
                    ret = true;
                    this->skip(HSSEndOfStatement);
                    if (this->atEndOfSource())
                        return false;

                    this->skip(HSSWhitespace);
                    if (this->atEndOfSource())
                        return false;
                    break;
                }
                    
                case HSSImportInstruction:
                {
                    d->receiver->receiveParserEvent(HSSParserEventInstruction, theInstr);
                    ret = true;
                    
                    break;
                }
                    
                default:
                    break;
            }
            
            break;
        }
            
            
            //if the statement starts with an object sign, it is an object definition
        case HSSObjectSign:
        {
            //create an object definition
            bool done = false;
            while (!done)
            {
                done = true;
                QSharedPointer<HSSObjectDefinition> theObj = this->readObjectDefinition("");
                if (theObj)
                {
                    this->recursiveAddObjectDefinition(theObj);
                }
                else
                {
                    return false;
                }
                
                if (!this->atEndOfSource() && d->currentToken->isA(HSSAmpersand))
                {
                    done = false;
                }
            }
            
            break;
        }
            
            //if the statement starts with an identifier, universal selector or combinator it is a rule
        case HSSIdentifier:
        {
            QSharedPointer<HSSRule> theRule = this->readRule();
            if (theRule)
            {
                d->receiver->receiveParserEvent(HSSParserEventRule, theRule);
            }
            break;
        }
        case HSSSymbol:
        {
            if (VALUE_TOKEN(d->currentToken)->equals(HSSSymbol, "*") || this->isCombinator())
            {
                QSharedPointer<HSSRule> theRule = this->readRule();
                if (theRule)
                {
                    d->receiver->receiveParserEvent(HSSParserEventRule, theRule);
                }
                break;
            }
            else
            {
                d->receiver->receiveParserEvent(HSSParserEventOther, HSSSymbolNode::createSymbol(VALUE_TOKEN(d->currentToken)->getString(), d->controller));
            }
        }
            //if it is a comment
        case HSSBlockComment:
        case HSSLineComment:
        {
            QSharedPointer<HSSCommentNode> theComment = HSSCommentNode::createComment(VALUE_TOKEN(d->currentToken)->getString(), d->controller);
            d->receiver->receiveParserEvent(HSSParserEventComment, theComment);
            this->readNextToken();
            if (!this->atEndOfSource())
                this->skip(HSSWhitespace);
            ret = true;
            break;
        }
        
        case HSSEndOfStatement:
        {
            //some sort of error in the code
            this->skip(HSSEndOfStatement);
            return false;
        }
            
        default:
        {
            if (!this->atEndOfSource())
            {
                this->skip(d->currentToken->getType());
            }
            return false;
        }
    }
    
    return ret;
}

QSharedPointer<HSSRule> HSSCodeParser::readRule()
{
    QSharedPointer<HSSRule> errorState;
    bool valid = true;
    
    //if we had a weird token here, abort
    if (d->currentToken->isA(HSSDot) || d->currentToken->isA(HSSEndOfStatement))
    {
        this->skip(d->currentToken->getType());
        return errorState;
    }
    
    //initialize the rule
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
    selectorChains = this->readSelectorChains(HSSBlockOpen);
    if (atEndOfSource())
        return errorState;
    
    
    //if we have a end of statement here, we're done
    if (d->currentToken->isA(HSSEndOfStatement))
    {
        this->readNextToken();
        if (atEndOfSource())
            return errorState;
        
        //skip any whitespace
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
        
        QSharedPointer<HSSRule> ret = QSharedPointer<HSSRule>(new HSSRule(d->controller));
        ret->setSelectorChains(selectorChains);
        return ret;
    }
    
    QSharedPointer<HSSRule> newRule = QSharedPointer<HSSRule>(new HSSRule(d->controller));
    newRule->setSelectorChains(selectorChains);
    
    //we expect a block to open
    this->skip(HSSBlockOpen);
    if (atEndOfSource())
        return errorState;
    
    //skip any whitespace
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    //now we're inside the block
    d->currentObjectContext.push(d->_containerContextObj);
    d->currentContext.push_back(HSSParserContextBlock);
    
    //read the inner part of the block
    while (!this->atEndOfSource() && valid && !d->currentToken->isA(HSSBlockClose))
    {
        switch (d->currentToken->getType())
        {
            case HSSIdentifier:
            case HSSObjectSign:
            {
                bool isShorthand;
                if (this->isPropertyDefinition(&isShorthand))
                {
                    QSharedPointer<HSSPropertyDefinition> propertyDefinition = this->readPropertyDefinition(true, isShorthand);
                    if (propertyDefinition)
                    {
                        newRule->propertiesAdd(propertyDefinition);
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    QSharedPointer<HSSRule> theRule = this->readRule();
                    if (theRule)
                    {
                        newRule->childrenAdd(theRule);
                    }
                    else
                    {
                        break;
                    }
                }
                break;
            }

            case HSSDoubleDot:
            case HSSNegator:
            case HSSColon:
            {
                QSharedPointer<HSSRule> theRule = this->readRule();
                if (theRule)
                {
                    newRule->childrenAdd(theRule);
                }
                else
                {
                    break;
                }
                break;
            }
                
            case HSSInstructionSign:
            {
                QSharedPointer<HSSRule> childRule = this->readInstructionRule();
                if (childRule)
                {
                    newRule->childrenAdd(childRule);
                }
                else
                {
                    valid = false;
                    break;
                }
                break;
            }
            case HSSSymbol:
            {
                if (VALUE_TOKEN(d->currentToken)->getString() == "*")
                {
                    QSharedPointer<HSSRule> theRule = this->readRule();
                    if (theRule)
                    {
                        newRule->childrenAdd(theRule);
                    }
                    else
                    {
                        break;
                    }
                    break;
                }
                else
                {
                    printf("Error: found unexpected symbol while reading rule\n");
                    d->receiver->receiveParserEvent(HSSParserEventInvalid, HSSInvalidNode::createInvalidNode(VALUE_TOKEN(d->currentToken)->getString(), d->controller));
                    this->readNextToken();
                    if (this->atEndOfSource())
                        return errorState;
                    this->skip(HSSWhitespace);
                    if (this->atEndOfSource())
                        return errorState;
                    valid = false;
                }
                break;
            }

            default:
            {
                AXRWarning("HSSParser", "Unexpected token of type " + HSSToken::tokenStringRepresentation(d->currentToken->getType()) + " while reading rule", d->file->sourceUrl(), d->line, d->column).raise();
                this->skip(d->currentToken->getType());
                if(this->atEndOfSource())
                    break;
                
                this->skip(HSSWhitespace);
                if (this->atEndOfSource())
                    break;
                break;
            }
        }
        
        //        if (this->atEndOfSource())
        //        {
        //            QSharedPointer<HSSNameSelector> sbjct = selectorChains.back()->subject()->getName();
        //            AXRString lmntnm;
        //            if (sbjct)
        //            {
        //                lmntnm = sbjct->getElementName();
        //            }
        //            else
        //            {
        //                lmntnm = "unknown element";
        //            }
        //
        //            AXRWarning("HSSParser", "Auto closing block of rule targeting " + lmntnm + " because of unexpected end of file", d->file->sourceUrl(), d->line, d->column).raise();
        //            //leave the block context
        //            d->currentContext.pop_back();
        //            return newRule;
        //        }
    }
    
    //reset the index of the shorthand properties
    d->currentObjectContext.top()->shorthandReset();
    
    //leave the block context
    d->currentContext.pop_back();
    d->currentObjectContext.pop();
    
    if (!this->atEndOfSource())
    {
        this->skip(HSSBlockClose);
    }
    if (!this->atEndOfSource())
    {
        //ignore all the whitespace after the block
        this->skip(HSSWhitespace);
    }
    if (valid)
    {
        return newRule;
    }
    return errorState;
}

std::vector<QSharedPointer<HSSSelectorChain> > HSSCodeParser::readSelectorChains(HSSTokenType stopOn)
{
    std::vector<QSharedPointer<HSSSelectorChain> > errorState;
    std::vector<QSharedPointer<HSSSelectorChain> > retvect;
    QSharedPointer<HSSSelectorChain> ret = QSharedPointer<HSSSelectorChain>(new HSSSelectorChain(d->controller));
    bool done = false;
    bool rootContext = false;
    
    //at root context, we don't want to prepend with @this
    if(d->currentContext.back() == HSSParserContextRoot){
        rootContext = true;
    }
    
    //set the current context
    d->currentContext.push_back(HSSParserContextSelectorChain);
    if(!rootContext){
        QSharedPointer<HSSCombinator> beginning_combinator = this->readCombinator();
        if (beginning_combinator)
        {
            QSharedPointer<HSSSimpleSelector> newSs = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(d->controller));
            newSs->setName(QSharedPointer<HSSThisSelector>(new HSSThisSelector(d->controller)));
            ret->add(newSs);
            ret->add(beginning_combinator);
        }
    }
    
    while (!done)
    {
        QSharedPointer<HSSSimpleSelector> ss = this->readSimpleSelector();
        if (ss)
        {
            ret->add(ss);
        } else {
            break;
        }
        if (this->atEndOfSource())
            break;
        
        QSharedPointer<HSSCombinator> cc = this->readChildrenCombinatorOrSkip();
        if (cc)
        {
            ret->add(cc);
        }
        if (this->atEndOfSource())
            break;
        
        QSharedPointer<HSSCombinator> combinator = this->readCombinator();
        if (combinator)
        {
            ret->add(combinator);
        }
        if (this->atEndOfSource())
            break;
        
        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            break;
        
        if (d->currentToken->isA(HSSComma))
        {
            retvect.push_back(ret);
            
            this->readNextToken();
            if (this->atEndOfSource())
                break;
            
            this->skip(HSSWhitespace);
            if (this->atEndOfSource())
                break;
            
            if (!d->currentToken->isA(stopOn))
            {
                ret = QSharedPointer<HSSSelectorChain>(new HSSSelectorChain(d->controller));
            }
            else
            {
                ret = QSharedPointer<HSSSelectorChain>();
            }
        }
        if (d->currentToken->isA(stopOn) || d->currentToken->isA(HSSEndOfStatement))
        {
            done = true;
        }
    }
    
    if (ret)
    {
        retvect.push_back(ret);
    }
    
    //we're not in a selector anymore
    d->currentContext.pop_back();
    
    return retvect;
}

QSharedPointer<HSSSimpleSelector> HSSCodeParser::readSimpleSelector()
{
    QSharedPointer<HSSSimpleSelector> ret;
    bool isNegating = false;
    isNegating = this->isNegator();
    switch (d->currentToken->getType())
    {
        case HSSIdentifier:
        {
            ret = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(d->controller));
            ret->setName(this->readNameSelector(isNegating));
            
            //we're done negating for now
            isNegating = false;
            break;
        }
            
        case HSSSymbol:
        {
            const char currentTokenValue = *(VALUE_TOKEN(d->currentToken)->getString()).toStdString().c_str();
            switch (currentTokenValue)
            {
                case '*':
                {
                    ret = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(d->controller));
                    QSharedPointer<HSSUniversalSelector> uniSel = QSharedPointer<HSSUniversalSelector>(new HSSUniversalSelector(d->controller));
                    ret->setName(uniSel);
                    
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventUniversalSelector, uniSel);
                    
                    this->readNextToken();
                    break;
                }
            }
            
            break;
        }
            
        case HSSColon:
        {
            ret = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(d->controller));
            QSharedPointer<HSSUniversalSelector> uniSel = QSharedPointer<HSSUniversalSelector>(new HSSUniversalSelector(d->controller));
            ret->setName(uniSel);
            break;
        }
            
        case HSSObjectSign:
        {
            QSharedPointer<HSSNameSelector> selector = this->readObjectSelector();
            
            if (!selector)
            {
                break;
            }
            
            ret = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(d->controller));
            ret->setName(selector);
        }
            
        default:
            break;
    }
    
    bool done = false;
    while (!done)
    {
        if (!isNegating)
        {
            isNegating = this->isNegator();
        }
        
        QSharedPointer<HSSFilter> filter = this->readFilter();
        if (filter)
        {
            filter->setNegating(isNegating);
            //we're done negating for now
            isNegating = false;
            ret->filtersAdd(filter);
        }
        else
        {
            done = true;
        }
    }
    return ret;
}

QSharedPointer<HSSNameSelector> HSSCodeParser::readObjectSelector()
{
    QSharedPointer<HSSNameSelector> ret;
    
    switch (d->currentToken->getType())
    {
        case HSSObjectSign:
        {
            this->skip(HSSObjectSign);
            if (d->currentToken->isA(HSSIdentifier))
            {
                AXRString objtype = VALUE_TOKEN(d->currentToken)->getString();
                if (objtype == "this")
                {
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventObjectType, HSSObjectNameConstant::createConstant(objtype, d->controller));

                    ret = QSharedPointer<HSSThisSelector>(new HSSThisSelector(d->controller));
                    this->readNextToken();
                    break;
                }
                else if (objtype == "super")
                {
                    /**
                     *  @todo implement \@super
                     */
                    AXRError("HSSParser", "@super not implemented yet").raise();
                }
                else if (objtype == "parent")
                {
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventObjectType, HSSObjectNameConstant::createConstant(objtype, d->controller));

                    ret = QSharedPointer<HSSParentSelector>(new HSSParentSelector(d->controller));
                    this->readNextToken();
                    break;
                }
                else if (objtype == "root")
                {
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventObjectType, HSSObjectNameConstant::createConstant(objtype, d->controller));

                    ret = QSharedPointer<HSSRootSelector>(new HSSRootSelector(d->controller));
                    this->readNextToken();
                    break;
                }
                else if (objtype == "event")
                {
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventObjectType, HSSObjectNameConstant::createConstant(objtype, d->controller));
                    
                    ret = QSharedPointer<HSSEventSelector>(new HSSEventSelector(d->controller));
                    this->readNextToken();
                    break;
                }
                else
                {
                    AXRWarning("HSSParser", "No objects other than @this, @super, @parent or @root are supported in selectors.", d->file->sourceUrl(), d->line, d->column).raise();
                    return QSharedPointer<HSSNameSelector>();
                }
            }
            else if (d->currentToken->isA(HSSWhitespace) || d->currentToken->isA(HSSBlockOpen) || d->currentToken->isA(HSSColon))
            {
                ret = QSharedPointer<HSSThisSelector>(new HSSThisSelector(d->controller));
                this->skip(HSSWhitespace);
                break;
            }
        }
            
        default:
            break;
    }
    return ret;
}

bool HSSCodeParser::isNegator()
{
    if (d->currentToken->isA(HSSNegator))
    {
        this->readNextToken();
        return true;
    }
    
    return false;
}

QSharedPointer<HSSFilter> HSSCodeParser::readFilter()
{
    QSharedPointer<HSSFilter> errorState;
    if (d->currentToken->isA(HSSColon))
    {
        this->skip(HSSColon);
        if (this->atEndOfSource())
            return errorState;
        
        if (d->currentToken->isA(HSSColon))
        {
            this->skip(HSSColon);
            if (this->atEndOfSource())
                return errorState;
            
            AXRString flagName = VALUE_TOKEN(d->currentToken)->getString();
            QSharedPointer<HSSFlag> theFlag = QSharedPointer<HSSFlag>(new HSSFlag(d->controller));
            theFlag->setName(flagName);
            d->receiver->receiveParserEvent(HSSParserEventFlagName, HSSObjectNameConstant::createConstant(flagName, d->controller));
            this->readNextToken();
            return theFlag;
        }
        else if (d->currentToken->isA(HSSIdentifier))
        {
            AXRString filterName = VALUE_TOKEN(d->currentToken)->getString();
            QSharedPointer<HSSFilter> theFlag = HSSFilter::newFilterWithStringType(filterName, d->controller);
            d->receiver->receiveParserEvent(HSSParserEventFilterName, HSSObjectNameConstant::createConstant(filterName, d->controller));
            this->readNextToken();
            return theFlag;
        }
    }
    
    return errorState;
}

QSharedPointer<HSSCombinator> HSSCodeParser::readCombinator()
{
    QSharedPointer<HSSCombinator> ret;
    
    switch (d->currentToken->getType())
    {
        case HSSSymbol:
        {
            const char currentTokenValue = *(VALUE_TOKEN(d->currentToken)->getString()).toStdString().c_str();
            switch (currentTokenValue)
            {
                case '=':
                case '-':
                case '+':
                case '>':
                    /**
                     *  @todo special handling for text selection combinators?
                     */
                    ret = this->readSymbolCombinator();
                    break;
                    
                    //huh? we didn't expect any other symbol
                default:
                    break;
            }
            break;
        }
            
        case HSSDot:
        {
            //ret = QSharedPointer<HSSCombinator>(new HSSCombinator(HSSCombinatorTypeNearestDescendants, d->controller));
            this->skip(HSSDot);
            break;
        }
        case HSSDoubleDot:
        {
            ret = QSharedPointer<HSSCombinator>(new HSSCombinator(HSSCombinatorTypeDescendants, d->controller));
            this->readNextToken();
            break;
        }
            
        default:
            break;
    }
    
    return ret;
    
}

bool HSSCodeParser::isCombinator()
{
    return this->isCombinator(d->currentToken);
}

bool HSSCodeParser::isCombinator(QSharedPointer<HSSToken> token)
{
    //are we in a context that accepts combinators?
    HSSParserContext context = d->currentContext.back();
    if (context == HSSParserContextExpression)
    {
        return false;
    }
    //all combinators are symbols
    switch (token->getType())
    {
        case HSSSymbol:
        {
            const char currentTokenChar = *(VALUE_TOKEN(token).data()->getString()).toStdString().c_str();
            switch (currentTokenChar)
            {
                case '=':
                case '-':
                case '+':
                case '>':
                    return true;
                    
                default:
                    return false;
            }
            break;
        }
        case HSSDot:
        case HSSDoubleDot:
            return true;
        case HSSWhitespace:
            return this->isChildrenCombinator();
        default:
            break;
    }
    return false;
}

bool HSSCodeParser::isChildrenCombinator()
{
    //if the next token is anything other than a combinator, an open block or an object sign the whitespace means children combinator
    QSharedPointer<HSSToken> peekToken = d->tokenizer->peekNextToken();
    if(!peekToken){
        return false;
    }
    bool ret = !this->isCombinator(peekToken) && !peekToken->isA(HSSBlockOpen) && !peekToken->isA(HSSObjectSign);
    d->tokenizer->resetPeek();
    return ret;
}

bool HSSCodeParser::isPropertyDefinition(bool * isShorthand)
{
    
    bool ret = true;
    *isShorthand = false;
    
    QSharedPointer<HSSToken> peekToken;
    if (d->currentToken->isA(HSSInstructionSign))
    {
        bool currentPref = d->tokenizer->isHexPreferred();
        d->tokenizer->setHexPreferred(true);
        
        peekToken = d->tokenizer->peekNextToken();
        ret = peekToken->isA(HSSHexNumber);
        d->tokenizer->setHexPreferred(currentPref);
        
    }
    else
    {
        peekToken = d->tokenizer->peekNextToken();
        bool done = false;
        while (peekToken && !done) {
            //we assume we are done
            done = true;
            //skip all whitespace and comments
            while (peekToken && (peekToken->isA(HSSWhitespace) || peekToken->isA(HSSBlockComment) || peekToken->isA(HSSLineComment)))
            {
                peekToken = d->tokenizer->peekNextToken();
            }
            //if the next token is a colon, it is either a property definition or a filter
            if (peekToken->isA(HSSColon))
            {
                //we'll peek until we find a end of statement, a closing block or an opening one
                peekToken = d->tokenizer->peekNextToken();
                //if we find a whitespace or an object sign here, we can be sure it's a property definition
                if (peekToken->isA(HSSWhitespace) || peekToken->isA(HSSObjectSign))
                {
                    ret = true;
                }
                else
                {
                    while (peekToken && !peekToken->isA(HSSEndOfStatement) && !peekToken->isA(HSSBlockClose) && !peekToken->isA(HSSBlockOpen))
                    {
                        peekToken = d->tokenizer->peekNextToken();
                    }
                    //if we find an opening block, we're dealing with a selector
                    if (peekToken && peekToken->isA(HSSBlockOpen))
                    {
                        ret = false;
                    }
                }
            }
            //either property grouping or a rule
            else if (peekToken->isA(HSSComma))
            {
                peekToken = d->tokenizer->peekNextToken();
                //skip all whitespace and comments
                while (peekToken && (peekToken->isA(HSSWhitespace) || peekToken->isA(HSSBlockComment) || peekToken->isA(HSSLineComment)))
                {
                    peekToken = d->tokenizer->peekNextToken();
                }
                if (peekToken->isA(HSSIdentifier))
                {
                    peekToken = d->tokenizer->peekNextToken();
                    done = false;
                    continue;
                }
            }
            //this might be dot notation
            else if(peekToken->isA(HSSDot))
            {
                peekToken = d->tokenizer->peekNextToken();
                if(peekToken->isA(HSSIdentifier)){
                    //we are still in the property chain, continue
                    peekToken = d->tokenizer->peekNextToken();
                    done = false;
                    continue;
                }
                else
                {
                    //this can't be a property definition -- either a syntax error or a selector chain
                    ret = false;
                    *isShorthand = false;
                }
            }
            else
            {
                if (d->currentToken->isA(HSSObjectSign))
                {
                    if (peekToken->isA(HSSIdentifier))
                    {
                        AXRString objtype = VALUE_TOKEN(peekToken)->getString();
                        if ((objtype == "this")
                            || (objtype == "super")
                            || (objtype == "parent")
                            || (objtype == "root"))
                        {
                            //it is a selector
                            ret = false;
                            *isShorthand = false;
                        }
                        else
                        {
                            //it is a shorthand with an object definition
                            ret = true;
                            *isShorthand = true;
                        }
                    }
                    else
                    {
                        //it is a shorthand with an object definition
                        ret = true;
                        *isShorthand = true;
                    }
                    
                }
                else
                {
                    //no colon, it may be a rule -- we peek until we find the end of the statement
                    //or we can conclude it actually is a rule
                    bool done = false;
                    while (!done)
                    {
                        switch (peekToken->getType())
                        {
                            case HSSBlockClose:
                            case HSSEndOfStatement:
                                //it is a shorthand
                                ret = true;
                                done = true;
                                *isShorthand = true;
                                break;
                                
                            case HSSObjectSign:
                            {
                                peekToken = d->tokenizer->peekNextToken();
                                if (peekToken->isA(HSSIdentifier))
                                {
                                    AXRString objtype = VALUE_TOKEN(peekToken)->getString();
                                    if ((objtype == "this")
                                        || (objtype == "super")
                                        || (objtype == "parent")
                                        || (objtype == "root"))
                                    {
                                        //it is a selector, continue because it may be a selector inside a function
                                    }
                                    else
                                    {
                                        //this is a regular object definition, so we can conclude it is not a rule
                                        ret = true;
                                        *isShorthand = true;
                                        done = true;
                                    }
                                }
                                else
                                {
                                    //unexpected token, abort
                                    ret = false;
                                    *isShorthand = false;
                                    done = true;
                                }
                                break;
                            }
                                
                            case HSSBlockOpen:
                                //it is a
                                ret = false;
                                done = true;
                                break;
                                
                            default:
                                break;
                        }
                        
                        if (!done)
                        {
                            peekToken = d->tokenizer->peekNextToken();
                            if (!peekToken)
                            {
                                //we probably reached end of source, abort
                                done = true;
                            }
                        }
                    }
                }
            }
        } //end while !done
    }
    
    d->tokenizer->resetPeek();
    return ret;
}

QSharedPointer<HSSCombinator> HSSCodeParser::readChildrenCombinatorOrSkip()
{
    QSharedPointer<HSSCombinator> errorState;
    //are we dealing with whitespace?
    if (d->currentToken->isA(HSSWhitespace))
    {
        if (this->isChildrenCombinator())
        {
            QSharedPointer<HSSCombinator> newCombinator = QSharedPointer<HSSCombinator>(new HSSCombinator(HSSCombinatorTypeChildren, d->controller));
            this->skip(HSSWhitespace);
            return newCombinator;
        }
        else
        {
            //alright, ignore it
            this->skip(HSSWhitespace);
            return errorState;
        }
    }
    else
    {
        //done, nothing to see here, move along...
        return errorState;
    }
}

//this expects the current token to be a symbol

QSharedPointer<HSSCombinator> HSSCodeParser::readSymbolCombinator()
{
    /**
     *  @todo check the context
     */
    QSharedPointer<HSSCombinator> errorState;
    QSharedPointer<HSSCombinator> ret;
    const char currentTokenChar = *(VALUE_TOKEN(d->currentToken)->getString()).toStdString().c_str();
    switch (currentTokenChar)
    {
        case '=':
            ret = QSharedPointer<HSSCombinator>(new HSSCombinator(HSSCombinatorTypeSiblings, d->controller));
            break;
        case '-':
            ret = QSharedPointer<HSSCombinator>(new HSSCombinator(HSSCombinatorTypePreviousSiblings, d->controller));
            break;
        case '+':
            ret = QSharedPointer<HSSCombinator>(new HSSCombinator(HSSCombinatorTypeNextSiblings, d->controller));
            break;
        default:
            return errorState;
    }
    
    this->readNextToken();
    if (this->atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    return ret;
}

QSharedPointer<HSSNameSelector> HSSCodeParser::readNameSelector(bool isNegating)
{
    AXRString theValue = VALUE_TOKEN(d->currentToken)->getString();
    QSharedPointer<HSSNameSelector> ret = QSharedPointer<HSSNameSelector>(new HSSNameSelector(theValue, d->controller));
    ret->setNegating(isNegating);
    
    //notify the receiver
    d->receiver->receiveParserEvent(HSSParserEventNameSelector, ret);
    
    this->readNextToken();
    
    return ret;
}

//this assumes currentToken is an object sign or an ampersand
///@todo propertyName here doesn't deal at all with dot notation
QSharedPointer<HSSObjectDefinition> HSSCodeParser::readObjectDefinition(AXRString propertyName)
{
    QSharedPointer<HSSObjectDefinition> errorState;
    AXRString objtype;
    QSharedPointer<HSSObject> obj;
    
    //set the current context
    d->currentContext.push_back(HSSParserContextObjectDefinition);
    
    //we either deal with a @ or an & symbol
    if (d->currentToken->isA(HSSObjectSign))
    {
        this->skip(HSSObjectSign);
        if (this->atEndOfSource())
            return errorState;
        
        //first we need to know what type of object it is
        if (d->currentToken->isA(HSSWhitespace) || d->currentToken->isA(HSSBlockOpen))
        {
            //damn, we'll have to derive that from the context
            if (d->currentObjectContext.size() > 0)
            {
                if (propertyName == "")
                {
                    objtype = d->currentObjectContext.top()->defaultObjectType();
                }
                else
                {
                    objtype = d->currentObjectContext.top()->defaultObjectType(propertyName);
                }
                
            }
            else
            {
                objtype = "container";
            }
        }
        else if (d->currentToken->isA(HSSIdentifier))
        {
            //alright, we've got a type, look it up
            objtype = VALUE_TOKEN(d->currentToken)->getString();
            d->receiver->receiveParserEvent(HSSParserEventObjectName, HSSObjectNameConstant::createConstant(objtype, d->controller));
            this->readNextToken();
            if (this->atEndOfSource())
                return errorState;
        }
        else
        {
            //Unexpected token while reading object definition
            return errorState;
        }
        
        //try to create an object of that type
        obj = HSSObject::newObjectWithType(objtype, d->controller);
        if(!obj)
        {
            if (d->currentObjectContext.size() > 0)
            {
                if (propertyName == "")
                {
                    objtype = d->currentObjectContext.top()->defaultObjectType();
                }
                else
                {
                    objtype = d->currentObjectContext.top()->defaultObjectType(propertyName);
                }
            }
            else
            {
                objtype = "value";
            }
            
            obj = HSSObject::newObjectWithType(objtype, d->controller);
            if(!obj)
            {
                objtype = "value";
                obj = HSSObject::newObjectWithType(objtype, d->controller);
            }
        }
        
        d->_lastObjectType = objtype;
        
    }
    else
    {
        this->skip(HSSAmpersand);
        if (this->atEndOfSource())
            return errorState;
        obj = HSSObject::newObjectWithType(d->_lastObjectType, d->controller);
        objtype = d->_lastObjectType;
        
        if (d->currentToken->isA(HSSObjectSign))
        {
            AXRWarning("HSSCodeParser", "Using &@ is obsolete, you shouldn't do that anymore", d->file->sourceUrl(), d->line, d->column).raise();
            this->readNextToken();
            if (this->atEndOfSource())
                return errorState;
        }
    }
    
    //get the name of the object
    if (d->currentToken->isA(HSSWhitespace))
    {
        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            return errorState;
    }
    
    switch (d->currentToken->getType())
    {
        case HSSIdentifier:
        {
            AXRString name = VALUE_TOKEN(d->currentToken)->getString();
            obj->setName(name);
            obj->_debugName = name.toStdString();
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventObjectName, HSSObjectNameConstant::createConstant(name, d->controller));
            this->readNextToken();
            if (this->atEndOfSource())
                return errorState;
            break;
        }
        case HSSBlockOpen:
            //it is the opening curly brace, therefore an anonymous object:
            //do nothing
            break;
        case HSSEndOfStatement:
        case HSSBlockClose:
        case HSSParenthesisClose:
        {
            //the property definition ends here
            return QSharedPointer<HSSObjectDefinition>(new HSSObjectDefinition(obj, d->controller));
        }
            
        default:
            //Unexpected token while reading object definition
            return errorState;
    }
    
    QSharedPointer<HSSObjectDefinition> newObjDef = QSharedPointer<HSSObjectDefinition>(new HSSObjectDefinition(obj, d->controller));
    this->skip(HSSWhitespace);
    if (this->atEndOfSource())
        return errorState;
    this->skip(HSSBlockOpen);
    if (this->atEndOfSource())
        return errorState;
    this->skip(HSSWhitespace);
    if (this->atEndOfSource())
        return errorState;
    
    //now we're inside the block
    d->currentContext.push_back(HSSParserContextBlock);
    
    //read the inner part of the block
    d->currentObjectContext.push(obj);
    
    while (!d->currentToken->isA(HSSBlockClose))
    {
        switch (d->currentToken->getType())
        {
            case HSSSymbol:
            {
                QSharedPointer<HSSRule> theRule = this->readRule();
                if (theRule)
                    newObjDef->rulesAdd(theRule);
                break;
            }
                
            case HSSAmpersand:
            {
                d->_lastObjectType = objtype;
                QSharedPointer<HSSObjectDefinition> childDef = this->readObjectDefinition(propertyName);
                if (childDef)
                {
                    childDef->setParentNode(newObjDef);
                    newObjDef->childrenAdd(childDef);
                }
                else
                {
                    return newObjDef;
                }
                break;
            }
                
            case HSSIdentifier:
            case HSSColon:
            case HSSObjectSign:
            case HSSNegator:
            {
                bool isShorthand;
                if (this->isPropertyDefinition(&isShorthand))
                {
                    QSharedPointer<HSSPropertyDefinition> propertyDefinition = this->readPropertyDefinition(true, isShorthand);
                    if (propertyDefinition)
                    {
                        newObjDef->propertiesAdd(propertyDefinition);
                    }
                    else
                    {
                        return errorState;
                    }
                }
                else
                {
                    QSharedPointer<HSSRule> theRule = this->readRule();
                    if (theRule)
                    {
                        newObjDef->rulesAdd(theRule);
                    }
                    else
                    {
                        return errorState;
                    }
                }
                break;
            }
                
            case HSSInstructionSign:
            {
                bool isShorthand;
                if (this->isPropertyDefinition(&isShorthand))
                {
                    QSharedPointer<HSSPropertyDefinition> propertyDefinition = this->readPropertyDefinition(true, isShorthand);
                    if (propertyDefinition)
                    {
                        newObjDef->propertiesAdd(propertyDefinition);
                    }
                    else
                    {
                        return errorState;
                    }
                }
                else
                {
                    QSharedPointer<HSSRule> childRule = this->readInstructionRule();
                    if (childRule)
                    {
                        newObjDef->rulesAdd(childRule);
                    }
                    else
                    {
                        return errorState;
                    }
                }
                break;
            }
                
            default:
            {
                bool isShorthand;
                if (this->isPropertyDefinition(&isShorthand))
                {
                    QSharedPointer<HSSPropertyDefinition> propertyDefinition = this->readPropertyDefinition(true, isShorthand);
                    if (propertyDefinition)
                    {
                        newObjDef->propertiesAdd(propertyDefinition);
                    }
                    else
                    {
                        return errorState;
                    }
                }
                else
                {
                    //Unexpected token
                    this->readNextToken();
                    if (this->atEndOfSource())
                        return errorState;
                    this->skip(HSSWhitespace);
                    if (this->atEndOfSource())
                        return errorState;
                }
                break;
            }
        }
        if (this->atEndOfSource())
        {
            AXRWarning("HSSCodeParser", "Auto closing block of object definition with name " + obj->getName() + " because of unexpected end of file", d->file->sourceUrl(), d->line, d->column).raise();
            //leave the block context
            d->currentContext.pop_back();
            return errorState;
        }
    }
    
    //reset the index of the shorthand properties
    obj->shorthandReset();
    //out we are
    d->currentObjectContext.pop();
    
    //we're out of the block, we expect a closing brace
    this->skip(HSSBlockClose);
    //leave the block context
    d->currentContext.pop_back();
    //leave the object definition context
    d->currentContext.pop_back();
    d->_lastObjectType = objtype;
    if (!this->atEndOfSource())
    {
        //ignore all the whitespace after the block
        this->skip(HSSWhitespace);
    }
    
    return newObjDef;
}

void HSSCodeParser::recursiveAddObjectDefinition(QSharedPointer<HSSObjectDefinition> objDef)
{
    d->receiver->receiveParserEvent(HSSParserEventObjectDefinition, objDef);
    const std::vector<QSharedPointer<HSSObjectDefinition> >children = objDef->getChildren();
    for (size_t i = 0; i < children.size(); ++i)
    {
        QSharedPointer<HSSObjectDefinition> child = children[i];
        
        // prepend backwards
        std::deque<QSharedPointer<HSSPropertyDefinition> > properties = objDef->getProperties();
        for (size_t i = properties.size(); i > 0; i--)
        {
            child->propertiesPrepend(properties[i - 1]);
        }
        
        const std::deque<QSharedPointer<HSSRule> > rules = objDef->getRules();
        for (std::deque<QSharedPointer<HSSRule> >::const_reverse_iterator it = rules.rbegin(); it != rules.rend(); ++it)
        {
            child->rulesPrepend((*it));
        }
        
        this->recursiveAddObjectDefinition(child);
    }
}

QSharedPointer<HSSPropertyDefinition> HSSCodeParser::readPropertyDefinition()
{
    return this->readPropertyDefinition(false, false);
}

QSharedPointer<HSSPropertyDefinition> HSSCodeParser::readPropertyDefinition(bool shorthandChecked, bool isShorthand)
{
    
    QVector<QSharedPointer<HSSPropertyPath> > propertyPaths;
    
    QSharedPointer<HSSPropertyDefinition> errorState;
    bool valid = true;
    
    switch (d->currentToken->getType())
    {
        case HSSIdentifier:
        {
            bool isPropertyDef = true;
            if (!shorthandChecked)
            {
                isPropertyDef = this->isPropertyDefinition(&isShorthand);
            }
            if (isPropertyDef)
            {
                if (!isShorthand)
                {
                    bool done = false;
                    while (!done)
                    {
                        done = true;
                        AXRString propertyName = VALUE_TOKEN(d->currentToken)->getString();
                        QSharedPointer<HSSPropertyPath> path(new HSSPropertyPath(d->controller));
                        path->add(propertyName);
                        
                        //notify the receiver
                        d->receiver->receiveParserEvent(HSSParserEventPropertyPath, QSharedPointer<HSSPropertyPathNode>(new HSSPropertyPathNode(propertyName, d->controller)));
                        
                        this->readNextToken();
                        if (this->atEndOfSource())
                            return errorState;
                        
                        //allow whitespace before colon
                        this->skip(HSSWhitespace);
                        if (this->atEndOfSource())
                            return errorState;
                        
                        if (d->currentToken->isA(HSSDot))
                        {
                            //notify the receiver
                            d->receiver->receiveParserEvent(HSSParserEventPropertyPath, HSSSymbolNode::createSymbol(".", d->controller));
                            this->readNextToken();
                            if (this->atEndOfSource())
                                return errorState;
                            
                            bool done2 = false;
                            while (!done2)
                            {
                                done2 = true;
                                if (!d->currentToken->isA(HSSIdentifier))
                                {
                                    AXRError("HSSCodeParser", "Unexpected token after dot in property expression", d->file->sourceUrl(), d->line, d->column).raise();
                                    continue;
                                }
                                AXRString prop = VALUE_TOKEN(d->currentToken)->getString();
                                path->add(prop);

                                //notify the receiver
                                d->receiver->receiveParserEvent(HSSParserEventPropertyPath, QSharedPointer<HSSPropertyPathNode>(new HSSPropertyPathNode(prop, d->controller)));
                                this->readNextToken();
                                if (this->atEndOfSource())
                                    return errorState;
                                
                                if (d->currentToken->isA(HSSDot))
                                {
                                    done2 = false;
                                    //notify the receiver
                                    d->receiver->receiveParserEvent(HSSParserEventPropertyPath, HSSSymbolNode::createSymbol(".", d->controller));
                                    this->readNextToken();
                                    if (this->atEndOfSource())
                                        return errorState;
                                }
                            }
                        }
                        propertyPaths.append(path);
                        if (d->currentToken->isA(HSSComma))
                        {
                            //notify the receiver
                            d->receiver->receiveParserEvent(HSSParserEventOther, HSSSymbolNode::createSymbol(",", d->controller));
                            
                            this->readNextToken();
                            if (this->atEndOfSource())
                                return errorState;
                            
                            this->skip(HSSWhitespace);
                            if (this->atEndOfSource())
                                return errorState;
                            
                            if (d->currentToken->isA(HSSIdentifier))
                            {
                                done = false;
                                continue;
                            }
                        }
                        //now must come a colon
                        this->skip(HSSColon);
                        if (this->atEndOfSource())
                            return errorState;
                        
                        //we don't give a f$%# about whitespace
                        this->skip(HSSWhitespace);
                        if (this->atEndOfSource())
                            return errorState;
                        
                        d->currentObjectContext.top()->shorthandSkip(propertyName);
                    }
                    break;
                }
                
                //fall through to 'default:'
                
            }
            else
            {
                //it is not a property definition, abort
                return errorState;
            }
            
            //fall through to 'default:'
        }
            
        default:
        {
            //we assume we are dealing with shorthand notation
            //get the property name for the current value
            QSharedPointer<HSSPropertyPath> path(new HSSPropertyPath(d->controller));
            path->add(d->currentObjectContext.top()->getPropertyForCurrentValue());
            propertyPaths.push_back(path);
            //consume the property
            d->currentObjectContext.top()->shorthandNext();
            break;
        }
    }

    QSharedPointer<HSSPropertyDefinition> newPropeDef = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(propertyPaths, d->controller));

    this->readValues(newPropeDef, propertyPaths.front()->front()->getPropertyName());
    
    if (!this->atEndOfSource())
    {
        //skip the semicolon
        this->skip(HSSEndOfStatement);
        if (this->atEndOfSource())
            return errorState;
        
        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            return errorState;
    }
    if (!valid)
    {
        return errorState;
    }
    return newPropeDef;
}

bool HSSCodeParser::readValues(const QSharedPointer<HSSParserNode> &target, AXRString propertyName)
{
    bool done = false;
    bool valid = true;
    bool hasValue = false;

    while (!done && valid)
    {
        //now comes either an object definition, a literal value or an expression
        //object
        if (d->currentToken->isA(HSSObjectSign))
        {
            bool objdefDone = false;
            while (!objdefDone)
            {
                objdefDone = true;
                QSharedPointer<HSSObjectDefinition> objdef = this->readObjectDefinition(propertyName); ///@todo: what to do if the property names give different default objects?
                if (objdef)
                {
                    target->addValue(objdef);
                    hasValue = true;
                }
                else
                {
                    valid = false;
                }
                
                if (this->atEndOfSource())
                {
                    objdefDone = true;
                    done = true;
                }
                else if (valid && d->currentToken->isA(HSSAmpersand))
                {
                    objdefDone = false;
                }
            }
            //this->readNextToken();
            
        }
        else if (d->currentToken->isA(HSSSingleQuoteString) || d->currentToken->isA(HSSDoubleQuoteString))
        {
            AXRString theString = VALUE_TOKEN(d->currentToken)->getString();
            target->addValue(QSharedPointer<HSSStringConstant>(new HSSStringConstant(theString, d->controller)));
            hasValue = true;

            //notify the receiver
            if (d->currentToken->isA(HSSSingleQuoteString))
            {
                d->receiver->receiveParserEvent(HSSParserEventStringConstant, HSSStringConstant::stringToConstant(theString, d->controller));
            }
            else
            {
                d->receiver->receiveParserEvent(HSSParserEventStringConstant, HSSStringConstant::stringToConstant(theString, d->controller));
            }
            
            this->readNextToken();
            if (this->atEndOfSource())
                return false;
            
            this->skip(HSSWhitespace);
            if (this->atEndOfSource())
                return false;
            
            //number literal
        }
        else if (d->currentToken->isA(HSSNumber) || d->currentToken->isA(HSSPercentageNumber) || d->currentToken->isA(HSSParenthesisOpen))
        {
            /**
             *  @todo parse the number and see if it is an int or a float
             */
            QSharedPointer<HSSParserNode> exp = this->readExpression();
            if (exp)
            {
                target->addValue(exp);
                hasValue = true;
            }
            else
            {
                valid = false;
            }
            
            
        }
        else if (d->currentToken->isA(HSSIdentifier))
        {
            //this is either a function, a keyword or an object name
            
            AXRString valuestr = VALUE_TOKEN(d->currentToken)->getString();
            //check if it is a function
            QSharedPointer<HSSObject> objectContext = d->currentObjectContext.top();
            
            if (objectContext->isFunction(valuestr, propertyName)) ///@todo: what to do if the property names give different functions?
            {
                QSharedPointer<HSSParserNode> exp = this->readExpression();
                if (exp)
                {
                    target->addValue(exp);
                    hasValue = true;
                }
                else
                {
                    valid = false;
                }
                
                //check if it is a keyword
            }
            else if (objectContext->isKeyword(valuestr, propertyName)) ///@todo: what to do if the property names give different keywords?
            {
                QSharedPointer<HSSKeywordConstant> kwConstant = QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(valuestr, d->controller));
                target->addValue(kwConstant);
                hasValue = true;
                
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParseEventKeywordConstant, kwConstant);
                
                this->readNextToken();
                if (this->atEndOfSource())
                    return false;
                //we assume it is an object name at this point
            }
            else
            {
                QSharedPointer<HSSObjectNameConstant> objName = QSharedPointer<HSSObjectNameConstant>(new HSSObjectNameConstant(valuestr, d->controller));
                target->addValue(objName);
                hasValue = true;
                
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParseEventObjectNameConstant, objName);
                
                this->readNextToken();
                if (this->atEndOfSource())
                    return false;
            }
            /*
             //this is either a function, a keyword or an object name, all of which can be
             //part of an expression
             QSharedPointer<HSSParserNode> exp = this->readExpression();
             if (exp){
             ret->addValue(exp);
             } else {
             valid = false;
             }
             */
            
        }
        else if (d->currentToken->isA(HSSInstructionSign))
        {
            QSharedPointer<HSSInstruction> theInstruction;
            
            theInstruction = this->readInstruction();
            
            if (theInstruction)
            {
                target->addValue(this->getObjectFromInstruction(theInstruction));
                hasValue = true;
            }
            else
            {
                valid = false;
            }
        }
        else if (d->currentToken->isA(HSSEndOfStatement))
        {
            done = true;
            
            if (!hasValue)
            {
                AXRError("HSSCodeParser", "No value set for the current property").raise();
                valid = false;
            }
        }
        else if (d->currentToken->isA(HSSBlockClose))
        {
            done = true;
            
            if (!hasValue)
            {
                AXRError("HSSCodeParser", "No value set for the current property").raise();
                valid = false;
            }
        }
        else
        {
            valid = false;
        }
        
        if (!valid)
        {
            done = true;
        }
        
        if (valid && !this->atEndOfSource())
        {
            this->skip(HSSWhitespace);
            if (this->atEndOfSource())
                return false;
            
            if (d->currentToken->isA(HSSComma))
            {
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParserEventOther, HSSSymbolNode::createSymbol(",", d->controller));
                
                this->readNextToken();
                if (this->atEndOfSource())
                    return false;
                
                this->skip(HSSWhitespace);
                if (this->atEndOfSource())
                    return false;
            }
            else
            {
                done = true;
            }
        }
    }
    return valid;
}

QSharedPointer<HSSPropertyPath> HSSCodeParser::readPropertyPath()
{
    QSharedPointer<HSSPropertyPath> ret(new HSSPropertyPath(d->controller));
    if (!d->currentToken->isA(HSSIdentifier))
    {
        AXRError("HSSCodeParser", "Unexpected token after dot in property path", d->file->sourceUrl(), d->line, d->column).raise();
        return ret;
    }
    AXRString propertyName = VALUE_TOKEN(d->currentToken)->getString();
    QSharedPointer<HSSPropertyPathNode> ppn(new HSSPropertyPathNode(propertyName, d->controller));
    ret->add(ppn);

    //notify the receiver
    d->receiver->receiveParserEvent(HSSParserEventPropertyPath, ppn);
    
    this->readNextToken();
    if (d->currentToken->isA(HSSDot))
    {
        this->readNextToken();
        bool done = false;
        while (!done)
        {
            done = true;
            if (d->currentToken->isA(HSSDot))
            {
                done = false;
                this->skip(HSSDot);
            }
            
            if (!d->currentToken->isA(HSSIdentifier))
            {
                AXRError("HSSCodeParser", "Unexpected token after dot in property path", d->file->sourceUrl(), d->line, d->column).raise();
                continue;
            }
            QSharedPointer<HSSPropertyPathNode> ppn(new HSSPropertyPathNode(VALUE_TOKEN(d->currentToken)->getString(), d->controller));
            ret->add(ppn);

            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventPropertyPath, ppn);

            this->readNextToken();
        }
    }
    return ret;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readValue(AXRString propertyName, bool &valid)
{
    bool isValid = true;
    QSharedPointer<HSSParserNode> errorState;
    //now comes either an object definition, a literal value or an expression
    //object
    if (d->currentToken->isA(HSSObjectSign))
    {
        QSharedPointer<HSSObjectDefinition> objdef = this->readObjectDefinition(propertyName);
        if (objdef)
        {
            return objdef;
        }
        else
        {
            isValid = false;
        }
        //this->readNextToken();
        
    }
    else if (d->currentToken->isA(HSSSingleQuoteString) || d->currentToken->isA(HSSDoubleQuoteString))
    {
        QSharedPointer<HSSStringConstant> ret = QSharedPointer<HSSStringConstant>(new HSSStringConstant(VALUE_TOKEN(d->currentToken)->getString(), d->controller));
        this->readNextToken();
        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            return errorState;
        
        //number literal
        return ret;
    }
    else if (d->currentToken->isA(HSSNumber) || d->currentToken->isA(HSSPercentageNumber) || d->currentToken->isA(HSSParenthesisOpen))
    {
        /**
         *  @todo parse the number and see if it is an int or a float
         */
        QSharedPointer<HSSParserNode> exp = this->readExpression();
        if (exp)
        {
            return exp;
        }
        else
        {
            isValid = false;
        }
    }
    else if (d->currentToken->isA(HSSIdentifier))
    {
        //this is either a function, a keyword or an object name
        
        AXRString valuestr = VALUE_TOKEN(d->currentToken)->getString();
        //check if it is a function
        QSharedPointer<HSSObject> objectContext = d->currentObjectContext.top();
        
        if (objectContext->isFunction(valuestr, propertyName))
        {
            QSharedPointer<HSSParserNode> exp = this->readExpression();
            if (exp)
            {
                return exp;
            }
        }
        //check if it is a keyword
        else if (objectContext->isKeyword(valuestr, propertyName))
        {
            QSharedPointer<HSSKeywordConstant> ret = QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(valuestr, d->controller));
            this->readNextToken();
            if (this->atEndOfSource())
                return errorState;
            //we assume it is an object name at this point
            return ret;
        }
        else
        {
            QSharedPointer<HSSObjectNameConstant> ret = QSharedPointer<HSSObjectNameConstant>(new HSSObjectNameConstant(valuestr, d->controller));
            this->readNextToken();
            if (this->atEndOfSource())
                return errorState;
            return ret;
        }
    }
    else if (d->currentToken->isA(HSSInstructionSign))
    {
        QSharedPointer<HSSInstruction> theInstruction;
        
        theInstruction = this->readInstruction();
        
        if (theInstruction)
        {
            return this->getObjectFromInstruction(theInstruction);
        }
        
    }
    return errorState;
}

QSharedPointer<HSSInstruction> HSSCodeParser::readInstruction()
{
    return this->readInstruction(true);
}

QSharedPointer<HSSInstruction> HSSCodeParser::readInstruction(bool preferHex)
{
    QSharedPointer<HSSInstruction> errorState;
    AXRString currentval;
    
    //set preference
    d->tokenizer->setHexPreferred(preferHex);
    //skip the instruction sign -- this will automatically read the next token
    this->skip(HSSInstructionSign);
    if (this->atEndOfSource())
        return errorState;
    
    //restore
    d->tokenizer->setHexPreferred(false);
    
    //we are looking at
    //if it starts with a number, we are looking at a color instruction
    if (d->currentToken->isA(HSSHexNumber))
    {
        currentval = VALUE_TOKEN(d->currentToken)->getString();
        QSharedPointer<HSSInstruction> ret;
        switch (currentval.length())
        {
            case 1:
                ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSGrayscale1Instruction, currentval, d->controller));
                this->readNextToken();
                if (this->atEndOfSource())
                    return errorState;
                break;
            case 2:
                ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSGrayscale2Instruction, currentval, d->controller));
                this->readNextToken();
                if (this->atEndOfSource())
                    return errorState;
                break;
            case 3:
                ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSRGBInstruction, currentval, d->controller));
                this->readNextToken();
                if (this->atEndOfSource())
                    return errorState;
                break;
            case 4:
                ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSRGBAInstruction, currentval, d->controller));
                this->readNextToken();
                if (this->atEndOfSource())
                    return errorState;
                break;
            case 5:
                ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSRGBAAInstruction, currentval, d->controller));
                this->readNextToken();
                if (this->atEndOfSource())
                    return errorState;
                break;
            case 6:
                ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSRRGGBBInstruction, currentval, d->controller));
                this->readNextToken();
                if (this->atEndOfSource())
                    return errorState;
                break;
            case 7:
                ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSRRGGBBAInstruction, currentval, d->controller));
                this->readNextToken();
                if (this->atEndOfSource())
                    return errorState;
                break;
            case 8:
                ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSRRGGBBAAInstruction, currentval, d->controller));
                this->readNextToken();
                if (this->atEndOfSource())
                    return errorState;
                break;
                
            default:
                AXRWarning("HSSCodeParser", "Wrong length for hexadecimal number (should be between 1 and 8 digits long, inclusive)", d->file->sourceUrl(), d->line, d->column).raise();
                return errorState;
        }
        
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventColor, ret);
        
        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            return errorState;
        return ret;
    }
    else if (d->currentToken->isA(HSSIdentifier))
    {
        QSharedPointer<HSSInstruction> ret;
        currentval = VALUE_TOKEN(d->currentToken)->getString();
        if (currentval == "new")
        {
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSNewInstruction, currentval, d->controller));
            
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventInstruction, ret);
            
            this->readNextToken();
            if (atEndOfSource())
                return errorState;
            this->skip(HSSWhitespace);
            if (atEndOfSource())
                return errorState;
            if (d->currentToken->isA(HSSParenthesisOpen))
            {
                this->skip(HSSParenthesisOpen);
                this->skip(HSSWhitespace);
                if (d->currentToken->isA(HSSNumber))
                {
                    HSSUnit number = VALUE_TOKEN(d->currentToken)->getLong();
                    ret->setArgument(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(number, d->controller)));
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventOther, HSSNumberConstant::number(number, d->controller));
                    this->readNextToken();
                    this->skip(HSSWhitespace);
                    this->skip(HSSParenthesisClose);
                    this->skip(HSSWhitespace);
                }
                else
                {
                    return errorState;
                }
            }
        }
        else if (currentval == "ensure")
        {
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSEnsureInstruction, d->controller));
            this->readNextToken();
        }
        else if (currentval == "import")
        {
            
            this->readNextToken();
            this->skip(HSSWhitespace);
            
            if (d->currentToken->isA(HSSDoubleQuoteString) || d->currentToken->isA(HSSSingleQuoteString))
            {
                AXRString theString = VALUE_TOKEN(d->currentToken)->getString();
                ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSImportInstruction, theString, d->controller));
            }
            else if (d->currentToken->isA(HSSIdentifier))
            {
                AXRString instructionKw = VALUE_TOKEN(d->currentToken)->getString();
                if (instructionKw == "UIFramework")
                {
                    QUrl url;
                    url.setScheme(HSSFRAMEWORK_PROTOCOL);
                    url.setPath("framework/UIFramework.hss");
                    ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSImportInstruction, url.toString(), d->controller));
                }
                else
                {
                    return errorState;
                }
            }
            else
            {
                return errorState;
            }
            
            this->readNextToken();
            if (!this->atEndOfSource())
            {
                this->skip(HSSWhitespace);
            }
            if (!this->atEndOfSource())
            {
                this->skip(HSSEndOfStatement);
            }
            
        }
        else if (currentval == "move")
        {
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSMoveInstruction, d->controller));
            this->readNextToken();
        }
        else if (currentval == "delete")
        {
            
            this->readNextToken();
            this->skip(HSSWhitespace);
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSDeleteInstruction, d->controller));
        }
        else
        {
            return errorState;
        }
        return ret;
        
    }
    else
    {
        return errorState;
    }
    
    if (!this->atEndOfSource())
    {
        this->skip(HSSWhitespace);
    }
    return errorState;
}

QSharedPointer<HSSObjectDefinition> HSSCodeParser::getObjectFromInstruction(QSharedPointer<HSSInstruction> instruction)
{
    QSharedPointer<HSSObjectDefinition> ret;
    HSSInstructionType instructionType = instruction->getInstructionType();
    
    switch (instructionType)
    {
        case HSSGrayscale1Instruction:
        case HSSGrayscale2Instruction:
        {
            //try to create an object of that type
            QSharedPointer<HSSRgb> obj = QSharedPointer<HSSRgb>(new HSSRgb(d->controller));
            
            unsigned int hexValue;
            AXRString tempstr = instruction->getValue();
            if (instructionType == HSSGrayscale1Instruction)
            {
                tempstr = tempstr + tempstr;
            }
            
            hexValue = tempstr.toUInt(NULL, 16);
            
            ret = QSharedPointer<HSSObjectDefinition>(new HSSObjectDefinition(obj, d->controller));
            
            QSharedPointer<HSSPropertyDefinition> newRed = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(d->controller));
            QSharedPointer<HSSPropertyPath> redPath(new HSSPropertyPath(d->controller));
            redPath->add("red");
            QVector<QSharedPointer<HSSPropertyPath> > redPaths;
            redPaths.push_back(redPath);
            newRed->setPaths(redPaths);
            newRed->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(hexValue, d->controller)));
            ret->propertiesAdd(newRed);
            
            QSharedPointer<HSSPropertyDefinition> newGreen = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(d->controller));
            QSharedPointer<HSSPropertyPath> greenPath(new HSSPropertyPath(d->controller));
            greenPath->add("green");
            QVector<QSharedPointer<HSSPropertyPath> > greenPaths;
            greenPaths.push_back(greenPath);
            newGreen->setPaths(greenPaths);
            newGreen->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(hexValue, d->controller)));
            ret->propertiesAdd(newGreen);
            
            QSharedPointer<HSSPropertyDefinition> newBlue = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(d->controller));
            QSharedPointer<HSSPropertyPath> bluePath(new HSSPropertyPath(d->controller));
            bluePath->add("blue");
            QVector<QSharedPointer<HSSPropertyPath> > bluePaths;
            bluePaths.push_back(bluePath);
            newBlue->setPaths(bluePaths);
            newBlue->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(hexValue, d->controller)));
            ret->propertiesAdd(newBlue);
            
            break;
        }
            
        case HSSRGBInstruction:
        case HSSRGBAInstruction:
        case HSSRGBAAInstruction:
        case HSSRRGGBBInstruction:
        case HSSRRGGBBAInstruction:
        case HSSRRGGBBAAInstruction:
        {
            //try to create an object of that type
            QSharedPointer<HSSRgb> obj = QSharedPointer<HSSRgb>(new HSSRgb(d->controller));
            
            AXRString red;
            unsigned int redHex;
            AXRString green;
            unsigned int greenHex;
            AXRString blue;
            unsigned int blueHex;
            
            unsigned int alphaHex;
            AXRString alpha;
            
            switch (instructionType)
            {
                case HSSRGBInstruction:
                    red = instruction->getValue().mid(0, 1);
                    red += red;
                    green = instruction->getValue().mid(1, 1);
                    green += green;
                    blue = instruction->getValue().mid(2, 1);
                    blue += blue;
                    alpha = "FF";
                    break;
                case HSSRGBAInstruction:
                    red = instruction->getValue().mid(0, 1);
                    red += red;
                    green = instruction->getValue().mid(1, 1);
                    green += green;
                    blue = instruction->getValue().mid(2, 1);
                    blue += blue;
                    alpha = instruction->getValue().mid(3, 1);
                    alpha += alpha;
                    break;
                case HSSRGBAAInstruction:
                    red = instruction->getValue().mid(0, 1);
                    red += red;
                    green = instruction->getValue().mid(1, 1);
                    green += green;
                    blue = instruction->getValue().mid(2, 1);
                    blue += blue;
                    alpha = instruction->getValue().mid(3, 2);
                    break;
                case HSSRRGGBBInstruction:
                    red = instruction->getValue().mid(0, 2);
                    green = instruction->getValue().mid(2, 2);
                    blue = instruction->getValue().mid(4, 2);
                    alpha = "FF";
                    break;
                case HSSRRGGBBAInstruction:
                    red = instruction->getValue().mid(0, 2);
                    green = instruction->getValue().mid(2, 2);
                    blue = instruction->getValue().mid(4, 2);
                    alpha = instruction->getValue().mid(6, 1);
                    alpha += alpha;
                    break;
                case HSSRRGGBBAAInstruction:
                    red = instruction->getValue().mid(0, 2);
                    green = instruction->getValue().mid(2, 2);
                    blue = instruction->getValue().mid(4, 2);
                    alpha = instruction->getValue().mid(6, 2);
                    break;
                default:
                    break;
            }
            
            redHex = red.toUInt(NULL, 16);
            greenHex = green.toUInt(NULL, 16);
            blueHex = blue.toUInt(NULL, 16);
            alphaHex = alpha.toUInt(NULL, 16);
            
            ret = QSharedPointer<HSSObjectDefinition>(new HSSObjectDefinition(obj, d->controller));
            
            QSharedPointer<HSSPropertyDefinition> newRed = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(d->controller));
            QSharedPointer<HSSPropertyPath> redPath(new HSSPropertyPath(d->controller));
            redPath->add("red");
            QVector<QSharedPointer<HSSPropertyPath> > redPaths;
            redPaths.push_back(redPath);
            newRed->setPaths(redPaths);
            newRed->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(redHex, d->controller)));
            ret->propertiesAdd(newRed);
            
            QSharedPointer<HSSPropertyDefinition> newGreen = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(d->controller));
            QSharedPointer<HSSPropertyPath> greenPath(new HSSPropertyPath(d->controller));
            greenPath->add("green");
            QVector<QSharedPointer<HSSPropertyPath> > greenPaths;
            greenPaths.push_back(greenPath);
            newGreen->setPaths(greenPaths);
            newGreen->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(greenHex, d->controller)));
            ret->propertiesAdd(newGreen);
            
            QSharedPointer<HSSPropertyDefinition> newBlue = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(d->controller));
            QSharedPointer<HSSPropertyPath> bluePath(new HSSPropertyPath(d->controller));
            bluePath->add("blue");
            QVector<QSharedPointer<HSSPropertyPath> > bluePaths;
            bluePaths.push_back(bluePath);
            newBlue->setPaths(bluePaths);
            newBlue->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(blueHex, d->controller)));
            ret->propertiesAdd(newBlue);
            
            QSharedPointer<HSSPropertyDefinition> newAlpha = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(d->controller));
            QSharedPointer<HSSPropertyPath> alphaPath(new HSSPropertyPath(d->controller));
            alphaPath->add("alpha");
            QVector<QSharedPointer<HSSPropertyPath> > alphaPaths;
            alphaPaths.push_back(alphaPath);
            newAlpha->setPaths(alphaPaths);
            newAlpha->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(alphaHex, d->controller)));
            ret->propertiesAdd(newAlpha);
            
            break;
        }
            
        default:
            AXRError("HSSParser", "Unknown instruction type", d->file->sourceUrl(), d->line, d->column).raise();
            break;
    }
    return ret;
}

QSharedPointer<HSSRule> HSSCodeParser::readInstructionRule()
{
    QSharedPointer<HSSInstruction> instruction = this->readInstruction(false);
    QSharedPointer<HSSRule> errorState;
    if (!instruction)
        return errorState;
    switch (instruction->getInstructionType())
    {
        case HSSNewInstruction:
        case HSSEnsureInstruction:
        case HSSMoveInstruction:
        {
            QSharedPointer<HSSRule> ret = this->readRule();
            if (ret)
            {
                ret->setInstruction(instruction);
            }
            return ret;
        }
            
        case HSSDeleteInstruction:
        {
            //initialize the rule
            std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
            selectorChains = this->readSelectorChains(HSSEndOfStatement);
            if (atEndOfSource())
                return errorState;
            
            //we expect the end of the statement here
            this->skip(HSSEndOfStatement);
            if (atEndOfSource())
                return errorState;
            
            //skip any whitespace
            this->skip(HSSWhitespace);
            if (atEndOfSource())
                return errorState;
            
            selectorChains = this->readSelectorChains(HSSEndOfStatement);
            if (atEndOfSource())
                return errorState;
            
            //we expect the end of the statement here
            this->skip(HSSEndOfStatement);
            if (atEndOfSource())
                return errorState;
            
            //skip any whitespace
            this->skip(HSSWhitespace);
            if (atEndOfSource())
                return errorState;
            
            QSharedPointer<HSSRule> ret = QSharedPointer<HSSRule>(new HSSRule(d->controller));
            ret->setSelectorChains(selectorChains);
            ret->setInstruction(instruction);
            break;
        }
            
        case HSSImportInstruction:
        {
            /**
             *  @todo handle this case?
             */
            break;
        }
            
        default:
            AXRError("HSSParser", "Unknown instruction type", d->file->sourceUrl(), d->line, d->column).raise();
            break;
    }
    
    return errorState;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readExpression()
{
    QSharedPointer<HSSParserNode> ret = this->readAdditiveExpression();
    
    return ret;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readAdditiveExpression()
{
    QSharedPointer<HSSParserNode> left = this->readMultiplicativeExpression();
    while (!this->atEndOfSource() && d->currentToken->isA(HSSSymbol))
    {
        const char currentTokenChar = *(VALUE_TOKEN(d->currentToken)->getString()).toStdString().c_str();
        switch (currentTokenChar)
        {
            case '+':
            {
                this->skip(d->currentToken->getType());
                if (this->atEndOfSource())
                    return left;
                this->skip(HSSWhitespace);
                if (this->atEndOfSource())
                    return left;
                left = QSharedPointer<HSSSum>(new HSSSum(left, this->readMultiplicativeExpression(), d->controller));
                break;
            }
                
            case '-':
            {
                this->skip(d->currentToken->getType());
                if (this->atEndOfSource())
                    return left;
                this->skip(HSSWhitespace);
                if (this->atEndOfSource())
                    return left;
                left = QSharedPointer<HSSSubtraction>(new HSSSubtraction(left, this->readMultiplicativeExpression(), d->controller));
                break;
            }
                
            default:
                return left;
                break;
        }
    }
    return left;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readMultiplicativeExpression()
{
    QSharedPointer<HSSParserNode> left = this->readBaseExpression();
    
    while (!this->atEndOfSource() && d->currentToken->isA(HSSSymbol))
    {
        
        const char currentTokenChar = *(VALUE_TOKEN(d->currentToken)->getString()).toStdString().c_str();
        switch (currentTokenChar)
        {
            case '*':
            {
                this->skip(d->currentToken->getType());
                if (this->atEndOfSource())
                    return left;
                
                this->skip(HSSWhitespace);
                if (this->atEndOfSource())
                    return left;
                
                left = QSharedPointer<HSSMultiplication>(new HSSMultiplication(left, this->readBaseExpression(), d->controller));
                break;
            }
            case '/':
            {
                this->skip(d->currentToken->getType());
                if (this->atEndOfSource())
                    return left;
                
                this->skip(HSSWhitespace);
                if (this->atEndOfSource())
                    return left;
                
                left = QSharedPointer<HSSDivision>(new HSSDivision(left, this->readBaseExpression(), d->controller));
                break;
            }
            default:
                return left;
                break;
        }
    }
    return left;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readBaseExpression()
{
    QSharedPointer<HSSParserNode> errorState;
    
    switch (d->currentToken->getType())
    {
        case HSSNumber:
        {
            QSharedPointer<HSSParserNode> ret = QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(VALUE_TOKEN(d->currentToken)->getLong(), d->controller));
            
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventNumberConstant, ret);
            
            this->readNextToken();
            if (atEndOfSource())
                return errorState;
            
            this->skip(HSSWhitespace);
            if (atEndOfSource())
                return errorState;
            
            return ret;
        }
            
        case HSSPercentageNumber:
        {
            QSharedPointer<HSSParserNode> ret = QSharedPointer<HSSPercentageConstant>(new HSSPercentageConstant(VALUE_TOKEN(d->currentToken)->getLong(), d->controller));
            
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventPercentageConstant, ret);
            
            this->readNextToken();
            if (atEndOfSource())
                return errorState;
            
            this->skip(HSSWhitespace);
            if (atEndOfSource())
                return errorState;
            
            return ret;
        }
            
        case HSSParenthesisOpen:
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventOther, HSSSymbolNode::createSymbol("(", d->controller));
            
            this->readNextToken();
            if (atEndOfSource())
                return errorState;
            
            this->skip(HSSWhitespace);
            if (atEndOfSource())
                return errorState;
            
            QSharedPointer<HSSParserNode> ret = this->readExpression();
            if (atEndOfSource())
                return errorState;
            
            this->skip(HSSParenthesisClose);
            if (atEndOfSource())
                return errorState;
            
            this->skip(HSSWhitespace);
            if (atEndOfSource())
                return errorState;
            
            return ret;
        }
            
        case HSSIdentifier:
        {
            return this->readFunction();
        }
            
        default:
            return errorState;
    }
    return errorState;
}

//this method expects the currentToken to be an identifier
QSharedPointer<HSSParserNode> HSSCodeParser::readFlag()
{
    QSharedPointer<HSSFlag> ret;
    
    AXRString flagName = VALUE_TOKEN(d->currentToken)->getString();
    ret = QSharedPointer<HSSFlag>(new HSSFlag(d->controller));
    ret->setName(flagName);
    
    this->readNextToken();
    
    return ret;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readFunction()
{
    QSharedPointer<HSSParserNode> errorState;
    
    if (d->currentToken->isA(HSSIdentifier))
    {
        //create new function
        AXRString name = VALUE_TOKEN(d->currentToken)->getString();
        if (name == "ref")
        {
            return this->readRefFunction();
        }
        else if (name == "sel")
        {
            return this->readSelFunction();
        }
        else if (HSSFlag::isFlag(name))
        {
            return this->readFlagFunction();
        }
        else if (name == "attr")
        {
            return this->readAttrFunction();
        }
        else if (name == "min")
        {
            return this->readMinFunction();
        }
        else if (name == "max")
        {
            return this->readMaxFunction();
        }
        else if (name == "ceil")
        {
            return this->readCeilFunction();
        }
        else if (name == "floor")
        {
            return this->readFloorFunction();
        }
        else if (name == "round")
        {
            return this->readRoundFunction();
        }
        else if (name == "log")
        {
            return this->readLogFunction();
        }
        else if (name == "override")
        {
            return this->readOverrideFunction();
        }
        else if (name == "startTimer")
        {
            return this->readStartTimerFunction();
        }
        else if (name == "stopTimer")
        {
            return this->readStopTimerFunction();
        }
        else if (name == "toggleTimer")
        {
            return this->readToggleTimerFunction();
        }
        else if (name == "insert")
        {
            return this->readInsertFunction();
        }
        else if (d->controller->document()->isCustomFunction(name))
        {
            return this->readCustomFunction();
        }
        else
        {
            return errorState;
        }
    }
    else
    {
        return errorState;
    }
    return errorState;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readRefFunction()
{
    QSharedPointer<HSSParserNode> errorState;
    QSharedPointer<HSSRefFunction> refFunction = QSharedPointer<HSSRefFunction>(new HSSRefFunction(d->controller));
    
    //notify the receiver
    d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant("ref", d->controller)));
    
    this->readNextToken();
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSParenthesisOpen);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    //read the arguments
    //first, we expect either "min", "max", "avg" or a property path
    if (!d->currentToken->isA(HSSIdentifier))
    {
        return errorState;
    }
    else
    {
        AXRString firstValue = VALUE_TOKEN(d->currentToken)->getString();
        if (firstValue == "min"
            || firstValue == "max"
            || firstValue == "avg")
        {
            refFunction->setModifier(firstValue);

            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventFunctionModifier, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(firstValue, d->controller)));

            //after this there comes the property path
            this->readNextToken();
            if (atEndOfSource())
                return errorState;
            
            this->skip(HSSWhitespace);
            if (atEndOfSource())
                return errorState;
            
            if (!d->currentToken->isA(HSSIdentifier))
            {
                return errorState;
            }
            else
            {
                refFunction->setPropertyPath(this->readPropertyPath());
            }
        }
        else
        {
            //just the property path
            refFunction->setPropertyPath(this->readPropertyPath());
        }
    }
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    //if shorthand notation -- assumes 'of @this'
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
    if (d->currentToken->isA(HSSParenthesisClose))
    {
        QSharedPointer<HSSSelectorChain> selectorChain;
        selectorChain = QSharedPointer<HSSSelectorChain>(new HSSSelectorChain(d->controller));
        QSharedPointer<HSSSimpleSelector> newSs = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(d->controller));
        newSs->setName(QSharedPointer<HSSThisSelector>(new HSSThisSelector(d->controller)));
        selectorChain->add(newSs);
        selectorChains.push_back(selectorChain);

        this->skip(HSSParenthesisClose);
        if (atEndOfSource())
            return errorState;

        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
    }
    else
    {
        if (!d->currentToken->isA(HSSIdentifier) || VALUE_TOKEN(d->currentToken)->getString() != "of")
        {
            return errorState;
        }
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionModifier, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant("of", d->controller)));

        this->readNextToken();
        if (atEndOfSource())
            return errorState;
        
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
        
        //now read the selector chain
        selectorChains = this->readSelectorChains(HSSParenthesisClose);
        if (atEndOfSource())
            return errorState;
        
        //we expect the closing parenthesis here
        this->skip(HSSParenthesisClose);
        if (atEndOfSource())
            return errorState;
        
        //skip any whitespace
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
    }
    
    refFunction->setSelectorChains(selectorChains);
    return refFunction;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readSelFunction()
{
    QSharedPointer<HSSParserNode> errorState;

    //notify the receiver
    d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant("sel", d->controller)));
    
    this->readNextToken();
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSParenthesisOpen);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    //read the selector chain
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
    selectorChains = this->readSelectorChains(HSSParenthesisClose);
    if (atEndOfSource())
        return errorState;
    
    //we expect the closing parenthesis here
    this->skip(HSSParenthesisClose);
    if (atEndOfSource())
        return errorState;
    
    //skip any whitespace
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    QSharedPointer<HSSSelFunction> selFunction = QSharedPointer<HSSSelFunction>(new HSSSelFunction(d->controller));
    selFunction->setSelectorChains(selectorChains);
    return selFunction;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readFlagFunction()
{
    QSharedPointer<HSSParserNode> errorState;
    AXRString name = VALUE_TOKEN(d->currentToken)->getString();
    QSharedPointer<HSSFlagFunction> flagFunction = QSharedPointer<HSSFlagFunction>(new HSSFlagFunction(HSSFlagFunction::flagFunctionTypeFromString(name), d->controller));

    //notify the receiver
    d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));

    this->readNextToken();
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSParenthesisOpen);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    //read the arguments
    //first, we expect the name
    if (d->currentToken->isA(HSSIdentifier))
    {
        AXRString flagName = VALUE_TOKEN(d->currentToken)->getString();
        flagFunction->setName(flagName);
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFlagName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(flagName, d->controller)));
    }
    else if (d->currentToken->isA(HSSSymbol) && VALUE_TOKEN(d->currentToken)->getString() == "*")
    {
        flagFunction->setName("*");
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventOther, HSSSymbolNode::createSymbol("*", d->controller));
    }
    else
    {
        AXRError("HSSCodeParser", "unexpected token while reading flagging function " + name).raise();
        return errorState;
    }
    
    this->readNextToken();
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    if (name == "replaceFlag" && d->currentToken->isA(HSSIdentifier))
    {
        if (VALUE_TOKEN(d->currentToken)->getString() != "with")
        {
            AXRError("HSSCodeParser", "Unexpected token while reading flagging function: " + HSSToken::tokenStringRepresentation(d->currentToken->getType())).raise();
            return errorState;
        }
        
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionModifier, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant("with", d->controller)));
        
        this->readNextToken();
        if (atEndOfSource())
            return errorState;
        
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;

        if (d->currentToken->isA(HSSIdentifier))
        {
            AXRString flagName = VALUE_TOKEN(d->currentToken)->getString();
            flagFunction->setName2(flagName);
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventFlagName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(flagName, d->controller)));
        }
        else
        {
            AXRError("HSSCodeParser", "unexpected token while reading flagging function " + name).raise();
            return errorState;
        }
        
        this->readNextToken();
        if (atEndOfSource())
            return errorState;
        
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
    }
    
    //if shorthand notation
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
    if (d->currentToken->isA(HSSParenthesisClose))
    {
        QSharedPointer<HSSSelectorChain> selectorChain;
        selectorChain = QSharedPointer<HSSSelectorChain>(new HSSSelectorChain(d->controller));
        QSharedPointer<HSSSimpleSelector> newSs = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(d->controller));
        if(name == "takeFlag")
        {
            //assumes 'of *'
            newSs->setName(QSharedPointer<HSSUniversalSelector>(new HSSUniversalSelector(d->controller)));
        }
        else
        {
            //assumes 'of @this'
            newSs->setName(QSharedPointer<HSSThisSelector>(new HSSThisSelector(d->controller)));
        }
        selectorChain->add(newSs);
        selectorChains.push_back(selectorChain);
        
        this->skip(HSSParenthesisClose);
        if (atEndOfSource())
            return errorState;
        
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
    }
    else
    {
        if (!d->currentToken->isA(HSSIdentifier) || VALUE_TOKEN(d->currentToken)->getString() != "of")
        {
            AXRError("HSSCodeParser", "Unexpected token while reading flagging function: " + HSSToken::tokenStringRepresentation(d->currentToken->getType())).raise();
            return errorState;
        }

        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionModifier, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant("of", d->controller)));

        this->readNextToken();
        if (atEndOfSource())
            return errorState;

        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;

        //now read the selector chain
        selectorChains = this->readSelectorChains(HSSParenthesisClose);
        if (atEndOfSource())
            return errorState;
        
        //we expect the closing parenthesis here
        this->skip(HSSParenthesisClose);
        if (atEndOfSource())
            return errorState;

        //skip any whitespace
        this->skip(HSSWhitespace);
    }
    
    flagFunction->setSelectorChains(selectorChains);
    return flagFunction;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readAttrFunction()
{
    QSharedPointer<HSSParserNode> errorState;
    AXRString name = VALUE_TOKEN(d->currentToken)->getString();
    QSharedPointer<HSSAttrFunction> attrFunction = QSharedPointer<HSSAttrFunction>(new HSSAttrFunction(d->controller));
    
    //notify the receiver
    d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));
    
    this->readNextToken();
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSParenthesisOpen);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    //read the arguments
    //first, we expect an identifier
    if (!d->currentToken->isA(HSSIdentifier))
    {
        AXRError("HSSCodeParser", "Unexpected token while reading attr function " + name).raise();
        return errorState;
    }
    else
    {
        attrFunction->setAttributeName(VALUE_TOKEN(d->currentToken)->getString());
    }
    
    this->readNextToken();
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    //if shorthand notation -- assumes 'of @this'
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
    if (d->currentToken->isA(HSSParenthesisClose))
    {
        QSharedPointer<HSSSelectorChain> selectorChain;
        selectorChain = QSharedPointer<HSSSelectorChain>(new HSSSelectorChain(d->controller));
        QSharedPointer<HSSSimpleSelector> newSs = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(d->controller));
        newSs->setName(QSharedPointer<HSSThisSelector>(new HSSThisSelector(d->controller)));
        selectorChain->add(newSs);
        selectorChains.push_back(selectorChain);
        
        this->readNextToken();
        if (atEndOfSource())
            return errorState;
        
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
    }
    else
    {
        if (!d->currentToken->isA(HSSIdentifier) || VALUE_TOKEN(d->currentToken)->getString() != "of")
        {
            AXRError("HSSCodeParser", "Unexpected token while reading attr function " + name).raise();
        }
        
        this->readNextToken();
        if (atEndOfSource())
            return errorState;
        
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
        
        //now read the selector chain
        selectorChains = this->readSelectorChains(HSSParenthesisClose);
        if (atEndOfSource())
            return errorState;
        
        //we expect the closing parenthesis here
        this->skip(HSSParenthesisClose);
        if (atEndOfSource())
            return errorState;
        
        //skip any whitespace
        this->skip(HSSWhitespace);
    }
    
    attrFunction->setSelectorChains(selectorChains);
    return attrFunction;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readMinFunction()
{
    AXRError("HSSCodeParser", "the min() function is still unimplemented").raise();
    QSharedPointer<HSSParserNode> ret;
    return ret;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readMaxFunction()
{
    AXRError("HSSCodeParser", "the max() function is still unimplemented").raise();
    QSharedPointer<HSSParserNode> ret;
    return ret;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readCeilFunction()
{
    AXRError("HSSCodeParser", "the ceil() function is still unimplemented").raise();
    QSharedPointer<HSSParserNode> ret;
    return ret;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readFloorFunction()
{
    AXRError("HSSCodeParser", "the floor() function is still unimplemented").raise();
    QSharedPointer<HSSParserNode> ret;
    return ret;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readRoundFunction()
{
    AXRError("HSSCodeParser", "the round() function is still unimplemented").raise();
    QSharedPointer<HSSParserNode> ret;
    return ret;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readLogFunction()
{
    QSharedPointer<HSSParserNode> errorState;
    AXRString name = VALUE_TOKEN(d->currentToken)->getString();
    QSharedPointer<HSSLogFunction> logFunction = QSharedPointer<HSSLogFunction>(new HSSLogFunction(d->controller));

    //notify the receiver
    d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));

    this->readNextToken();
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSParenthesisOpen);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    //read the arguments
    bool done = false;
    while (!done)
    {
        done = true;
        bool valueValid = false;
        QSharedPointer<HSSParserNode> value = this->readValue("value", valueValid); //FIXME: don't know the property name here
        if(valueValid){
            logFunction->add(value);
        }
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
        
        if (d->currentToken->isA(HSSComma))
        {
            done = false;
            this->readNextToken();
            if (atEndOfSource())
                return errorState;
            
        }
    }
    this->skip(HSSParenthesisClose);
    return logFunction;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readOverrideFunction()
{
    QSharedPointer<HSSParserNode> errorState;
    AXRString name = VALUE_TOKEN(d->currentToken)->getString();

    QSharedPointer<HSSOverrideFunction> overrideFunction = QSharedPointer<HSSOverrideFunction>(new HSSOverrideFunction(d->controller));
    
    //notify the receiver
    d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));
    
    this->readNextToken();
    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSParenthesisOpen);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains = this->readSelectorChains(HSSParenthesisClose);
    overrideFunction->setSelectorChains(selectorChains);

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    this->skip(HSSParenthesisClose);
    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    //we expect a block to open
    this->skip(HSSBlockOpen);
    if (atEndOfSource())
        return errorState;

    //skip any whitespace
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    bool valid = true;
    while (!this->atEndOfSource() && valid && !d->currentToken->isA(HSSBlockClose)) {
        QSharedPointer<HSSPropertyDefinition> propertyDefinition = this->readPropertyDefinition(true, false);
        if (propertyDefinition)
        {
            overrideFunction->propertiesAdd(propertyDefinition);
        }
        else
        {
            valid = false;
        }
    }
    if (!this->atEndOfSource())
    {
        this->skip(HSSBlockClose);
    }
    if (!this->atEndOfSource())
    {
        //ignore all the whitespace after the block
        this->skip(HSSWhitespace);
    }
    if (valid)
    {
        return overrideFunction;
    }
    return errorState;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readStartTimerFunction()
{
    QSharedPointer<HSSParserNode> errorState;
    AXRString name = VALUE_TOKEN(d->currentToken)->getString();
    
    QSharedPointer<HSSStartTimerFunction> startTimerFunction = QSharedPointer<HSSStartTimerFunction>(new HSSStartTimerFunction(d->controller));
    
    //notify the receiver
    d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));
    
    this->readNextToken();
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSParenthesisOpen);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    if (d->currentToken->isA(HSSIdentifier))
    {
        AXRString id = VALUE_TOKEN(d->currentToken)->getString();
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventObjectName, QSharedPointer<HSSObjectNameConstant>(new HSSObjectNameConstant(id, d->controller)));
        
        startTimerFunction->setTimerName(id);

        this->readNextToken();
        if (atEndOfSource())
            return errorState;

        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;

        this->skip(HSSComma);
        if (atEndOfSource())
            return errorState;

        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
    }
    else
    {
        return errorState;
    }
    if (d->currentToken->isA(HSSIdentifier))
    {
        QSharedPointer<HSSParserNode> fun = this->readFunction();
        startTimerFunction->setFunction(fun);

        if (atEndOfSource())
            return errorState;

        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;

        this->skip(HSSComma);
        if (atEndOfSource())
            return errorState;

        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
    }
    else
    {
        return errorState;
    }
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    //milliseconds
    QSharedPointer<HSSParserNode> ms = this->readExpression();
    if (ms)
    {
        startTimerFunction->setMs(ms);
    }
    else
    {
        return errorState;
    }

    this->skip(HSSParenthesisClose);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    
    return startTimerFunction;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readStopTimerFunction()
{
    QSharedPointer<HSSParserNode> errorState;
    AXRString name = VALUE_TOKEN(d->currentToken)->getString();

    QSharedPointer<HSSStopTimerFunction> stopTimerFunction = QSharedPointer<HSSStopTimerFunction>(new HSSStopTimerFunction(d->controller));

    //notify the receiver
    d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));

    this->readNextToken();
    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    this->skip(HSSParenthesisOpen);
    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    if (d->currentToken->isA(HSSIdentifier))
    {
        AXRString id = VALUE_TOKEN(d->currentToken)->getString();
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventObjectName, QSharedPointer<HSSObjectNameConstant>(new HSSObjectNameConstant(id, d->controller)));

        stopTimerFunction->setTimerName(id);

        this->readNextToken();
        if (atEndOfSource())
            return errorState;

        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;

        this->skip(HSSComma);
        if (atEndOfSource())
            return errorState;

        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
    }
    else
    {
        return errorState;
    }

    this->skip(HSSParenthesisClose);
    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    return stopTimerFunction;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readToggleTimerFunction()
{
    QSharedPointer<HSSParserNode> errorState;
    AXRString name = VALUE_TOKEN(d->currentToken)->getString();
    
    QSharedPointer<HSSToggleTimerFunction> toggleTimerFunction = QSharedPointer<HSSToggleTimerFunction>(new HSSToggleTimerFunction(d->controller));
    
    //notify the receiver
    d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));
    
    this->readNextToken();
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSParenthesisOpen);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    if (d->currentToken->isA(HSSIdentifier))
    {
        AXRString id = VALUE_TOKEN(d->currentToken)->getString();
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventObjectName, QSharedPointer<HSSObjectNameConstant>(new HSSObjectNameConstant(id, d->controller)));
        
        toggleTimerFunction->setTimerName(id);
        
        this->readNextToken();
        if (atEndOfSource())
            return errorState;
        
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
        
        this->skip(HSSComma);
        if (atEndOfSource())
            return errorState;
        
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
    }
    else
    {
        return errorState;
    }
    
    if (d->currentToken->isA(HSSIdentifier))
    {
        QSharedPointer<HSSParserNode> fun = this->readFunction();
        toggleTimerFunction->setFunction(fun);
        
        if (atEndOfSource())
            return errorState;
        
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
        
        this->skip(HSSComma);
        if (atEndOfSource())
            return errorState;
        
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
    }
    else
    {
        return errorState;
    }
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    //milliseconds
    QSharedPointer<HSSParserNode> ms = this->readExpression();
    if (ms)
    {
        toggleTimerFunction->setMs(ms);
    }
    else
    {
        return errorState;
    }

    this->skip(HSSParenthesisClose);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    return toggleTimerFunction;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readInsertFunction()
{
    QSharedPointer<HSSParserNode> errorState;
    AXRString name = VALUE_TOKEN(d->currentToken)->getString();

    QSharedPointer<HSSInsertFunction> insertFunction = QSharedPointer<HSSInsertFunction>(new HSSInsertFunction(d->controller));
    
    //notify the receiver
    d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));

    this->readNextToken();
    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    this->skip(HSSParenthesisOpen);
    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    bool valueValid = this->readValues(insertFunction, "value");
    if (!valueValid)
        return errorState;

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    //if shorthand notation
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
    if (d->currentToken->isA(HSSParenthesisClose))
    {
        QSharedPointer<HSSSelectorChain> selectorChain;
        selectorChain = QSharedPointer<HSSSelectorChain>(new HSSSelectorChain(d->controller));
        QSharedPointer<HSSSimpleSelector> newSs = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(d->controller));

        //assumes 'of @this'
        newSs->setName(QSharedPointer<HSSThisSelector>(new HSSThisSelector(d->controller)));

        selectorChain->add(newSs);
        selectorChains.push_back(selectorChain);
        
        this->skip(HSSParenthesisClose);
        if (atEndOfSource())
            return errorState;
        
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
    }
    else
    {
        if (!d->currentToken->isA(HSSIdentifier) || VALUE_TOKEN(d->currentToken)->getString() != "of")
        {
            AXRError("HSSCodeParser", "Unexpected token while reading flagging function: " + HSSToken::tokenStringRepresentation(d->currentToken->getType())).raise();
            return errorState;
        }
        
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionModifier, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant("of", d->controller)));
        
        this->readNextToken();
        if (atEndOfSource())
            return errorState;
        
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
        
        //now read the selector chain
        selectorChains = this->readSelectorChains(HSSParenthesisClose);
        if (atEndOfSource())
            return errorState;
        
        //we expect the closing parenthesis here
        this->skip(HSSParenthesisClose);
        if (atEndOfSource())
            return errorState;
        
        //skip any whitespace
        this->skip(HSSWhitespace);
    }
    
    insertFunction->setSelectorChains(selectorChains);

    this->skip(HSSParenthesisClose);
    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    return insertFunction;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readCustomFunction()
{
    QSharedPointer<HSSParserNode> errorState;
    AXRString name = VALUE_TOKEN(d->currentToken)->getString();
    this->readNextToken();
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSParenthesisOpen);
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;
    
    std::deque<QSharedPointer<HSSParserNode> > arguments;
    while (!d->currentToken->isA(HSSParenthesisClose) && !this->atEndOfSource())
    {
        bool valid;
        arguments.push_back(this->readValue("", valid));
        if (d->currentToken->isA(HSSComma))
        {
            this->skip(HSSComma);
            if (atEndOfSource())
                return errorState;
        }
    }
    QSharedPointer<HSSFunction> theFunction = QSharedPointer<HSSFunction>(new HSSFunction(HSSFunctionTypeCustom, d->controller));
    theFunction->setArguments(arguments);
    theFunction->setName(name);
    this->readNextToken();
    if (atEndOfSource())
        return errorState;
    
    this->skip(HSSWhitespace);
    return theFunction;
}

void HSSCodeParser::readNextToken()
{
    //read next one
    QSharedPointer<HSSToken> theToken = d->tokenizer->readNextToken();
    if (theToken && (theToken->isA(HSSBlockComment) || theToken->isA(HSSLineComment)))
    {
        d->receiver->receiveParserEvent(HSSParserEventComment, HSSCommentNode::createComment(VALUE_TOKEN(theToken)->getString(), d->controller));
        this->readNextToken();
    }
    else
    {
        d->currentToken = theToken;
    }
    
    if (!this->atEndOfSource())
    {
        d->line = d->currentToken->line;
        d->column = d->currentToken->column;
    }
    else
    {
        d->line = d->tokenizer->currentLine();
        d->column = d->tokenizer->currentColumn() - 1;
    }
}

bool HSSCodeParser::atEndOfSource()
{
    if (!d->currentToken && d->tokenizer->atEndOfSource())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void HSSCodeParser::skip(HSSTokenType type)
{
    if (d->currentToken->isA(type))
    {
        if (type == HSSWhitespace)
        {
            QSharedPointer<HSSWhitespaceNode> theNode = HSSWhitespaceNode::createWhitespace(VALUE_TOKEN(d->currentToken)->getString(), d->controller);
            d->receiver->receiveParserEvent(HSSParserEventWhitespace, theNode);
            this->readNextToken();
            if (!this->atEndOfSource() && d->currentToken->isA(type))
            {
                this->skip(type);
            }
        }
        else if (type == HSSNumber)
        {
            QSharedPointer<HSSValueToken> valueToken = VALUE_TOKEN(d->currentToken);
            d->receiver->receiveParserEvent(HSSParserEventOther, HSSSymbolNode::createSymbol(QString::number(valueToken->getLong()), d->controller));
            this->readNextToken();
        }
        else
        {
            QSharedPointer<HSSValueToken> valueToken = VALUE_TOKEN(d->currentToken);
            d->receiver->receiveParserEvent(HSSParserEventOther, HSSSymbolNode::createSymbol(valueToken->getString(), d->controller));
            this->readNextToken();
        }
    }
}

void HSSCodeParser::skipUntilEndOfStatement()
{
    while (!this->atEndOfSource() &&!d->currentToken->isA(HSSEndOfStatement) && !d->currentToken->isA(HSSBlockClose))
    {
        this->readNextToken();
        if (this->atEndOfSource()) return;
        if (d->currentToken->isA(HSSBlockOpen))
        {
            this->skipUntilEndOfStatement();
            if (this->atEndOfSource()) return;
            this->readNextToken();
            if (this->atEndOfSource()) return;
        }
    }
    this->readNextToken();
    if (this->atEndOfSource()) return;
    this->skip(HSSWhitespace);
}

void HSSCodeParser::currentObjectContextRemoveLast()
{
    d->currentObjectContext.pop();
}

size_t HSSCodeParser::currentObjectContextSize() const
{
    return d->currentObjectContext.size();
}

void HSSCodeParser::currentObjectContextAdd(QSharedPointer<HSSObject> theObject)
{
    d->currentObjectContext.push(theObject);
}
