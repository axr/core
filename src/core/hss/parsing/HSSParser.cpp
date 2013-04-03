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
 *      COPYRIGHT: ©2013 - All Rights Reserved
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

#include <cstdio>
#include <cstdlib>
#include <QUrl>
#include "AXRBuffer.h"
#include "AXRController.h"
#include "AXRDocument.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSAttrFunction.h"
#include "HSSCombinator.h"
#include "HSSComment.h"
#include "HSSContainer.h"
#include "HSSDivision.h"
#include "HSSFilter.h"
#include "HSSFlag.h"
#include "HSSFlagFunction.h"
#include "HSSInstruction.h"
#include "HSSKeywordConstant.h"
#include "HSSMultiplication.h"
#include "HSSNegation.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSParentSelector.h"
#include "HSSParser.h"
#include "HSSPercentageConstant.h"
#include "HSSPropertyDefinition.h"
#include "HSSRefFunction.h"
#include "HSSRgb.h"
#include "HSSRootSelector.h"
#include "HSSRule.h"
#include "HSSSelectorChain.h"
#include "HSSSelFunction.h"
#include "HSSSimpleSelector.h"
#include "HSSStringConstant.h"
#include "HSSSubtraction.h"
#include "HSSSum.h"
#include "HSSThisSelector.h"
#include "HSSTokenizer.h"
#include "HSSUniversalSelector.h"
#include "HSSValue.h"
#include "HSSValueToken.h"

using namespace AXR;

HSSParser::HSSParser(AXRController * theController)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSParser: creating HSS parser");

    this->controller = theController;
    this->tokenizer = QSharedPointer<HSSTokenizer>(new HSSTokenizer());

    this->currentContext.push_back(HSSParserContextRoot);
    this->_genericContext = QSharedPointer<HSSValue>(new HSSValue(this->controller));
    this->currentObjectContextAdd(this->_genericContext);
    this->_lastObjectType = "value";

    this->line = 1;
    this->column = 1;
}

//HSSParser::HSSParser(HSSTokenizer::buf_p buffer, unsigned buflen, AXRString filename)
//{
//    this->tokenizer = QSharedPointer<HSSTokenizer>(new HSSTokenizer(buffer, buflen));
//    this->filename = filename;
//
//    this->currentContext.push_back(HSSParserContextRoot);
//    //FIXME: will there be a root object? Now defaults to container
//    this->currentObjectContext.push(QSharedPointer<HSSContainer>(new HSSContainer()));
//
//    this->readNextToken();
//}

HSSParser::~HSSParser()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSParser: destructing HSS parser");
    unsigned i;
    for (i = 0; i<this->currentObjectContext.size(); ++i)
    {
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
    for (i = 0; i<this->currentObjectContext.size(); ++i)
    {
        this->currentObjectContext.pop();
    }
    //clear the current context
    this->currentContext.clear();
    this->loadedFiles.clear();
    this->currentToken.clear();
    this->currentFile.clear();

    //initialize the new values
    this->currentContext.push_back(HSSParserContextRoot);
    //set the new default object context
    this->currentObjectContextAdd(this->_genericContext);

}

bool HSSParser::loadFile(QSharedPointer<AXRBuffer> file)
{
    axr_log(LoggerChannelOverview, "HSSParser: loading file " + file->sourceUrl().toString());

    security_brake_init();

    //check if the file has been loaded already
    if (this->loadedFiles.contains(file))
    {
        AXRError("HSSParser", "Failed loading file " + file->sourceUrl().toString() + " because of circular reference").raise();
        return false;
    }

    this->loadedFiles.append(file);
    this->currentFile = file;

    //propagate the file to the tokenizer
    this->tokenizer->setFile(file);
    this->tokenizer->readNextChar();

    this->readNextToken();

    QSharedPointer<HSSStatement> statement;

    bool done = this->atEndOfSource();

    if (!done) this->skip(HSSWhitespace);

    while (!done)
    {
        if (statement)
        {
            statement.clear();
        }
        try
        {
            axr_log(LoggerChannelHSSParser, "\nHSSParser: reading next statement");

            done = !this->readNextStatement();
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

        security_brake();
    }

    axr_log(LoggerChannelOverview, "HSSParser: finished parsing " + file->sourceUrl().toString());

    return true;
}

bool HSSParser::readNextStatement()
{
    bool ret = true;
    //if(this->currentContext.back() == HSSParserContextRoot)
    //{
    //the file was empty
    if (this->atEndOfSource())
        return false;

    AXRController * controller = this->getController();

    switch (this->currentToken->getType())
    {
    case HSSInstructionSign:
    {
        QSharedPointer<HSSInstruction> theInstr = this->readInstruction();

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
            this->skip(HSSWhitespace);
            if (this->currentToken->isA(HSSIdentifier))
            {
                theObj->getObject()->setName(VALUE_TOKEN(this->currentToken)->getString());
                this->readNextToken(true);
            }

            axr_log(LoggerChannelHSSParser, "HSSParser: adding object definition to object tree");
            this->recursiveAddObjectDefinition(theObj);
            controller->parserTreeAdd(theObj);
            ret = true;

            this->skipExpected(HSSEndOfStatement);
            if (!this->atEndOfSource())
            {
                this->skip(HSSWhitespace);
            }
            break;
        }

        case HSSImportInstruction:
        {
            //save
            QSharedPointer<HSSTokenizer> currentTokenizer = this->tokenizer;
            QSharedPointer<AXRBuffer> currentFile = this->currentFile;
            qint64 currentLine = this->line;
            qint64 currentColumn = this->column;
            QSharedPointer<HSSToken> currentCurrentToken = this->currentToken;
            std::vector<HSSParserContext> currentCurrentContext = this->currentContext;
            std::stack<QSharedPointer<HSSObject> > currentCurrentObjectContext = this->currentObjectContext;

            //load
            this->tokenizer = QSharedPointer<HSSTokenizer>(new HSSTokenizer());
            this->line = 1;
            this->column = 1;
            QSharedPointer<AXRBuffer> theFile;
            try
            {
                QUrl url(theInstr->getValue());
                if (url.isRelative())
                {
                    theFile = controller->document()->getFile(currentFile->sourceUrl().resolved(url));
                }
                else
                {
                    theFile = controller->document()->getFile(url);
                }

                if (theFile)
                {
                    this->loadFile(theFile);
                }
            }
            catch (const AXRError &e)
            {
                e.raise();
                //restore
                this->tokenizer = currentTokenizer;
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
            this->currentFile = currentFile;
            this->line = currentLine;
            this->column = currentColumn;
            this->currentToken = currentCurrentToken;
            this->currentContext = currentCurrentContext;
            this->currentObjectContext = currentCurrentObjectContext;

            controller->parserTreeAdd(theInstr);
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
                axr_log(LoggerChannelHSSParser, "HSSParser: adding object definition to object tree");
                this->recursiveAddObjectDefinition(theObj);
                controller->parserTreeAdd(theObj);
            }

            if (!this->atEndOfSource() && this->currentToken->isA(HSSAmpersand))
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
            axr_log(LoggerChannelHSSParser, "HSSParser: adding rule");
            controller->rulesAdd(theRule);
            controller->parserTreeAdd(theRule);
        }
        break;
    }
    case HSSSymbol:
    {
        if (VALUE_TOKEN(this->currentToken)->equals(HSSSymbol, "*") || this->isCombinator())
        {
            QSharedPointer<HSSRule> theRule = this->readRule();
            if (theRule)
            {
                axr_log(LoggerChannelHSSParser, "HSSParser: adding rule");
                controller->rulesAdd(theRule);
                controller->parserTreeAdd(theRule);
            }
            break;
        }
        else
        {
            throw AXRError("HSSParser", "Unexpected symbol " + VALUE_TOKEN(this->currentToken)->getString(), this->currentFile->sourceUrl(), this->line, this->column);
        }
    }
        //if it is a comment
    case HSSBlockComment:
    case HSSLineComment:
    {
        QSharedPointer<HSSComment> theComment = QSharedPointer<HSSComment>(new HSSComment(VALUE_TOKEN(this->currentToken)->getString(), controller));
        this->readNextToken();
        if (!this->atEndOfSource())
            this->skip(HSSWhitespace);
        axr_log(LoggerChannelHSSParser, "HSSParser: adding comment to parser tree");
        controller->parserTreeAdd(theComment);
        ret = true;
        break;
    }


    default:
        throw AXRWarning("HSSParser", "Unexpected token of type " + HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->sourceUrl(), this->line, this->column);
        break;
    }

    return ret;
    //    } else {
    //        axr_log(LoggerChannelObsolete1, "reading in anything other than root context is not implemented yet");
    //        return ret;
    //    }
}

QSharedPointer<HSSRule> HSSParser::readRule()
{
    axr_log(LoggerChannelHSSParser, "HSSParser: reading rule");
    security_brake_init()

    AXRController * controller = this->getController();

    //throw error if at end of source
    this->checkForUnexpectedEndOfSource();

    //initialize the rule
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
    try
    {
        selectorChains = this->readSelectorChains(HSSBlockOpen);
        //if we have a end of statement here, we're done
        if (this->currentToken->isA(HSSEndOfStatement))
        {
            this->readNextToken(true);
            //skip any whitespace
            this->skip(HSSWhitespace, true);
            QSharedPointer<HSSRule> ret = QSharedPointer<HSSRule>(new HSSRule(controller));
            ret->setSelectorChains(selectorChains);
            return ret;
        }

    }
    catch (const AXRError &e)
    {
        this->readNextToken();
        this->checkForUnexpectedEndOfSource();
        this->skip(HSSWhitespace);

        e.raise();

        //return an empty rule
        QSharedPointer<HSSRule> ret;
        return ret;
    }

    QSharedPointer<HSSRule> ret = QSharedPointer<HSSRule>(new HSSRule(controller));
    ret->setSelectorChains(selectorChains);

    //we expect a block to open
    this->skipExpected(HSSBlockOpen, true);
    //skip any whitespace
    this->skip(HSSWhitespace, true);

    //now we're inside the block
    this->currentObjectContextAdd(QSharedPointer<HSSContainer>(new HSSContainer(controller)));
    this->currentContext.push_back(HSSParserContextBlock);

    //read the inner part of the block
    while (!this->currentToken->isA(HSSBlockClose))
    {
        try
        {
            switch (this->currentToken->getType())
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
                        ret->propertiesAdd(propertyDefinition);
                    }
                }
                else
                {
                    QSharedPointer<HSSRule> theRule = this->readRule();
                    if (theRule)
                        ret->childrenAdd(theRule);
                }
                break;
            }

            case HSSSymbol:
            case HSSNegator:
            case HSSColon:
            {
                QSharedPointer<HSSRule> theRule = this->readRule();
                if (theRule)
                    ret->childrenAdd(theRule);
                break;
            }

            case HSSInstructionSign:
            {
                QSharedPointer<HSSRule> childRule = this->readInstructionRule();
                if (childRule)
                    ret->childrenAdd(childRule);
                break;
            }

            default:
            {
                AXRWarning("HSSParser", "Unexpected token of type " + HSSToken::tokenStringRepresentation(this->currentToken->getType()) + " while reading rule", this->currentFile->sourceUrl(), this->line, this->column).raise();
                this->readNextToken();
                this->skip(HSSWhitespace);
                break;
            }
            }

        }
        catch (const AXRError &e)
        {
            AXRWarning("HSSParser", "Invalid value for " + e.getMessage(), this->currentFile->sourceUrl(), this->line, this->column).raise();
            if (!this->atEndOfSource())
            {
                this->skipUntilEndOfStatement();
                this->readNextToken();
                this->checkForUnexpectedEndOfSource();
                this->skip(HSSWhitespace);
            }
        }

        if (this->atEndOfSource())
        {
            QSharedPointer<HSSNameSelector> sbjct = selectorChains.back()->subject()->getName();
            AXRString lmntnm;
            if (sbjct)
            {
                lmntnm = sbjct->getElementName();
            }
            else
            {
                lmntnm = "unknown element";
            }

            AXRWarning("HSSParser", "Auto closing block of rule targeting " + lmntnm + " because of unexpected end of file", this->currentFile->sourceUrl(), this->line, this->column).raise();
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
    this->currentObjectContextRemoveLast();
    if (!this->atEndOfSource())
    {
        //ignore all the whitespace after the block
        this->skip(HSSWhitespace);
    }

    return ret;
}

std::vector<QSharedPointer<HSSSelectorChain> > HSSParser::readSelectorChains(HSSTokenType stopOn)
{
    axr_log(LoggerChannelHSSParser, "HSSParser: reading selector chains");
    security_brake_init();

    AXRController * controller = this->getController();

    std::vector<QSharedPointer<HSSSelectorChain> > retvect;
    QSharedPointer<HSSSelectorChain> ret = QSharedPointer<HSSSelectorChain>(new HSSSelectorChain(controller));
    bool done = false;
    bool rootContext = false;

    //at root context, we don't want to prepend with @this
    if(this->currentContext.back() == HSSParserContextRoot){
        rootContext = true;
    }

    //set the current context
    this->currentContext.push_back(HSSParserContextSelectorChain);
    if(!rootContext){
        QSharedPointer<HSSCombinator> beginning_combinator = this->readCombinator();
        if (beginning_combinator)
        {
            QSharedPointer<HSSSimpleSelector> newSs = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(controller));
            newSs->setName(QSharedPointer<HSSThisSelector>(new HSSThisSelector(controller)));
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
        }

        QSharedPointer<HSSCombinator> cc = this->readChildrenCombinatorOrSkip();
        if (cc)
        {
            ret->add(cc);
        }
        QSharedPointer<HSSCombinator> combinator = this->readCombinator();
        if (combinator)
        {
            ret->add(combinator);
        }
        this->skip(HSSWhitespace);
        if (this->currentToken->isA(HSSComma))
        {
            retvect.push_back(ret);

            this->readNextToken(true);
            this->skip(HSSWhitespace);
            if (!this->currentToken->isA(stopOn))
            {
                ret = QSharedPointer<HSSSelectorChain>(new HSSSelectorChain(controller));
            }
            else
            {
                ret = QSharedPointer<HSSSelectorChain>();
            }
        }
        if (this->currentToken->isA(stopOn) || this->currentToken->isA(HSSEndOfStatement))
        {
            done = true;
        }

        security_brake();
    }
    security_brake_reset();

    if (ret)
    {
        retvect.push_back(ret);
    }

    //we're not in a selector anymore
    this->currentContext.pop_back();

    return retvect;
}

QSharedPointer<HSSSimpleSelector> HSSParser::readSimpleSelector()
{
    axr_log(LoggerChannelHSSParser, "HSSParser: reading simple selector");
    security_brake_init();

    AXRController * controller = getController();

    QSharedPointer<HSSSimpleSelector> ret;
    bool isNegating = false;
    isNegating = this->isNegator();
    switch (this->currentToken->getType())
    {
    case HSSIdentifier:
    {
        ret = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(controller));
        ret->setName(this->readNameSelector(isNegating));

        //we're done negating for now
        isNegating = false;
        break;
    }

    case HSSSymbol:
    {
        const char currentTokenValue = *(VALUE_TOKEN(this->currentToken)->getString()).toStdString().c_str();
        switch (currentTokenValue)
        {
        case '*':
        {
            ret = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(controller));
            ret->setName(QSharedPointer<HSSUniversalSelector>(new HSSUniversalSelector(controller)));
            this->readNextToken();
            break;
        }
        }

        break;
    }

    case HSSColon:
    {
        ret = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(controller));
        ret->setName(QSharedPointer<HSSUniversalSelector>(new HSSUniversalSelector(controller)));
        break;
    }

    case HSSObjectSign:
    {
        QSharedPointer<HSSNameSelector> selector = this->readObjectSelector();

        if (!selector)
        {
            break;
        }

        ret = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(controller));
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
        security_brake();
    }
    security_brake_reset();
    return ret;
}

QSharedPointer<HSSNameSelector> HSSParser::readObjectSelector()
{
    QSharedPointer<HSSNameSelector> ret;
    AXRController * controller = this->getController();
    switch (this->currentToken->getType())
    {
    case HSSObjectSign:
    {
        this->readNextToken(true);
        if (this->currentToken->isA(HSSIdentifier))
        {
            AXRString objtype = VALUE_TOKEN(this->currentToken)->getString();
            if (objtype == "this")
            {
                ret = QSharedPointer<HSSThisSelector>(new HSSThisSelector(controller));
                this->readNextToken(true);
            }
            else if (objtype == "super")
            {
                /**
                 *  @todo implement \@super
                 */
                axr_log(LoggerChannelObsolete0, "@super not implemented yet");
            }
            else if (objtype == "parent")
            {
                ret = QSharedPointer<HSSParentSelector>(new HSSParentSelector(controller));
                this->readNextToken(true);
            }
            else if (objtype == "root")
            {
                ret = QSharedPointer<HSSRootSelector>(new HSSRootSelector(controller));
                this->readNextToken(true);
                break;
            }
            else
            {
                AXRWarning("HSSParser", "No objects other than @this, @super, @parent or @root are supported in selectors.", this->currentFile->sourceUrl(), this->line, this->column).raise();
                return QSharedPointer<HSSNameSelector>();
            }
        }
        else if (this->currentToken->isA(HSSWhitespace) || this->currentToken->isA(HSSBlockOpen) || this->currentToken->isA(HSSColon))
        {
            ret = QSharedPointer<HSSThisSelector>(new HSSThisSelector(controller));
            this->skip(HSSWhitespace, true);
            break;
        }
    }

    default:
        break;
    }
    return ret;
}

bool HSSParser::isNegator()
{
    if (this->currentToken->isA(HSSNegator))
    {
        this->readNextToken();
        return true;
    }

    return false;
}

QSharedPointer<HSSFilter> HSSParser::readFilter()
{
    axr_log(LoggerChannelHSSParser, "HSSParser: reading filter");

    QSharedPointer<HSSFilter> ret;
    if (this->currentToken->isA(HSSColon))
    {
        this->readNextToken();

        if (this->currentToken->isA(HSSColon))
        {
            this->readNextToken();
            AXRString flagName = VALUE_TOKEN(this->currentToken)->getString();
            QSharedPointer<HSSFlag> theFlag = QSharedPointer<HSSFlag>(new HSSFlag(this->getController()));
            theFlag->setName(flagName);
            ret = theFlag;
            this->readNextToken();
            this->checkForUnexpectedEndOfSource();

        }
        else
        {
            this->expect(HSSIdentifier);

            AXRString filterName = VALUE_TOKEN(this->currentToken)->getString();
            ret = HSSFilter::newFilterWithStringType(filterName, this->getController());

            this->readNextToken();
            this->checkForUnexpectedEndOfSource();
        }
    }

    return ret;
}

QSharedPointer<HSSCombinator> HSSParser::readCombinator()
{
    axr_log(LoggerChannelHSSParser, "HSSParser: reading combinator");
    QSharedPointer<HSSCombinator> ret;

    if (this->currentToken->isA(HSSSymbol))
    {
        const char currentTokenValue = *(VALUE_TOKEN(this->currentToken)->getString()).toStdString().c_str();
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

        case '.':
            //we need to check if it is really a combinator or just a syntax error
            if (VALUE_TOKEN(this->currentToken)->getString() == "..")
            {
                ret = this->readSymbolCombinator();
                break;
            }

            //huh? we didn't expect any other symbol
        default:
            break;
        }
    }

    return ret;

}

//std::vector<QSharedPointer<HSSSelectorChain>> HSSParser::readSelectorChains(HSSTokenType stopOn)
//{
//    axr_log(LoggerChannelHSSParser, "HSSParser: reading selector chain");
//    inc_output_indent();
//    security_brake_init();
//
//    std::vector<QSharedPointer<HSSSelectorChain>> retvect;
//    QSharedPointer<HSSSelectorChain> ret = QSharedPointer<HSSSelectorChain>(new HSSSelectorChain());
//
//    //first we need to look at the selector chain
//    //set the current context
//    this->currentContext.push_back(HSSParserContextSelectorChain);
//    //parse the selector chain until we find the token to stop on
//    while (this->currentToken && !this->currentToken->isA(stopOn)) {
//
//
//        switch (this->currentToken->getType()) {
//            case HSSIdentifier:
//            {
//                //if it's an identifier, it's a simple selector
//                ret->add(this->readSelector());
//                //adds only if needed
//                QSharedPointer<HSSCombinator> childrenCombinator(this->readChildrenCombinatorOrSkip());
//                if(childrenCombinator){
//                    ret->add(childrenCombinator);
//                }
//                break;
//            }
//
//            case HSSSymbol:
//            {
//                //a symbol, probably a combinator
//                const char currentTokenValue = *(VALUE_TOKEN(this->currentToken)->getString()).c_str();
//                switch (currentTokenValue) {
//                    case '=':
//                    case '-':
//                    case '+':
//                    case '>':
//                        /**
//                         *  @todo special handling for text selection combinators?
//                         */
//                        ret->add(this->readSymbolCombinator());
//                        break;
//                    case '*':
//                        ret->add(QSharedPointer<HSSUniversalSelector>(new HSSUniversalSelector()));
//                        this->readNextToken();
//                        //adds only if needed
//                        ret->add(this->readChildrenCombinatorOrSkip());
//
//                        break;
//                    case '.':
//                        //we need to check if it is really a combinator or just a syntax error
//                        if (VALUE_TOKEN(this->currentToken)->getString() == ".."){
//                            ret->add(this->readSymbolCombinator());
//                            break;
//                        }
//
//                    case '!':
//                    {
//                        //it's a negation
//                        ret->add(QSharedPointer<HSSNegation>(new HSSNegation()));
//                        this->readNextToken();
//                        break;
//                    }
//
//                        //huh? we didn't expect any other symbol
//                    default:
//                        throw AXRError::p(new AXRWarning("HSSParser", "Unexpected token of type "+HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->getFileName(), this->line, this->column));
//                }
//                break;
//            }
//
//            case HSSColon:
//            {
//                //if there is another colon, it is a flag
//                this->readNextToken(true);
//                if(this->currentToken->isA(HSSColon)){
//                    this->readNextToken(true);
//                    QSharedPointer<HSSParserNode> theFlag = this->readFlag();
//                    if(theFlag){
//                        ret->add(theFlag);
//                    }
//
//                } else {
//                    //it is a filter
//                    QSharedPointer<HSSParserNode> theFilter = this->readFilter();
//                    if(theFilter){
//                        ret->add(theFilter);
//                    }
//                }
//                //adds only if needed
//                ret->add(this->readChildrenCombinatorOrSkip());
//                break;
//            }
//
//            case HSSComma:
//            {
//                retvect.push_back(ret);
//
//                this->readNextToken(true);
//                this->skip(HSSWhitespace);
//                if(!this->currentToken->isA(stopOn)){
//                    ret = QSharedPointer<HSSSelectorChain>(new HSSSelectorChain());
//                } else {
//                    ret = QSharedPointer<HSSSelectorChain>();
//                }
//                break;
//            }
//
//            case HSSObjectSign:
//            {
//                this->readNextToken(true);
//                if(this->currentToken->isA(HSSIdentifier)){
//                    AXRString objtype = VALUE_TOKEN(this->currentToken)->getString();
//                    if (objtype == "this") {
//                        ret->add(QSharedPointer<HSSThisSelector>(new HSSThisSelector()));
//                        this->readNextToken(true);
//                        //adds only if needed
//                        QSharedPointer<HSSCombinator> childrenCombinator(this->readChildrenCombinatorOrSkip());
//                        if(childrenCombinator){
//                            ret->add(childrenCombinator);
//                        }
//                        break;
//                    } else if (objtype == "super"){
//                        /**
//                         *  @todo implement \@super
//                         */
//                        std_log("@super not implemented yet");
//                    } else if (objtype == "parent"){
//                        /**
//                         *  @todo implement \@parent
//                         */
//                        std_log("@parent not implemented yet");
//                    } else if (objtype == "root"){
//                        ret->add(QSharedPointer<HSSRootSelector>(new HSSRootSelector()));
//                        this->readNextToken(true);
//                        //adds only if needed
//                        QSharedPointer<HSSCombinator> childrenCombinator(this->readChildrenCombinatorOrSkip());
//                        if(childrenCombinator){
//                            ret->add(childrenCombinator);
//                        }
//                        break;
//                    } else {
//                        throw AXRError::p(new AXRWarning("HSSParser", "No objects other than @this, @super, @parent or @root are supported in selectors.", this->currentFile->getFileName(), this->line, this->column));
//                    }
//                } else if(this->currentToken->isA(HSSWhitespace) || this->currentToken->isA(HSSBlockOpen) || this->currentToken->isA(HSSColon)){
//                    ret->add(QSharedPointer<HSSThisSelector>(new HSSThisSelector()));
//                    this->skip(HSSWhitespace, true);
//                    break;
//                }
//
//                //fall through
//            }
//
//            default:
//            {
//                //we didn't expect any other type of token
//                throw AXRError::p(new AXRWarning("HSSParser", "Unexpected token of type "+HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->getFileName(), this->line, this->column));
//            }
//        }
//
//        security_brake();
//    }
//    security_brake_reset();
//
//    if(ret) {
//        retvect.push_back(ret);
//    }
//
//    //we're not in a selector anymore
//    this->currentContext.pop_back();
//
//    //we expect a block to open
//    this->skipExpected(stopOn);
//    //if the file ends here, fuuuuuuu[...]
//    this->checkForUnexpectedEndOfSource();
//    //skip any whitespace
//    this->skip(HSSWhitespace);
//    //if the file ends here, fuuuuuuu[...]
//    this->checkForUnexpectedEndOfSource();
//
//    dec_output_indent();
//    return retvect;
//}

bool HSSParser::isCombinator()
{
    return this->isCombinator(this->currentToken);
}

bool HSSParser::isCombinator(QSharedPointer<HSSToken> token)
{
    //are we in a context that accepts combinators?
    HSSParserContext context = this->currentContext.back();
    if (context == HSSParserContextExpression)
    {
        return false;
    }
    //all combinators are symbols
    if (token->isA(HSSSymbol))
    {
        const char currentTokenChar = *(VALUE_TOKEN(token).data()->getString()).toStdString().c_str();
        switch (currentTokenChar)
        {
        case '=':
        case '-':
        case '+':
        case '.':
        case '>':
            return true;

        default:
            return false;
        }
    }
    else if (token->isA(HSSWhitespace))
    {
        return this->isChildrenCombinator();
    }
    return false;
}

//this function assumes currentToken to be whitespace

bool HSSParser::isChildrenCombinator()
{
    //if the next token is anything other than a combinator, an open block or an object sign the whitespace means children combinator
    QSharedPointer<HSSToken> peekToken = this->tokenizer->peekNextToken();
    bool ret = !this->isCombinator(peekToken) && !peekToken->isA(HSSBlockOpen) && !peekToken->isA(HSSObjectSign);
    this->tokenizer->resetPeek();
    return ret;
}

//wether it is or not a shorthand will be passed to the isShorthand pointer

bool HSSParser::isPropertyDefinition(bool * isShorthand)
{
    axr_log(LoggerChannelHSSParser, "HSSParser: checking if property definition");
    bool ret = true;
    *isShorthand = false;

    //property definitions always start with identifiers (the property)
    //    if(!this->currentToken->isA(HSSIdentifier)){
    //        return false;
    //    }

    QSharedPointer<HSSToken> peekToken;
    if (this->currentToken->isA(HSSInstructionSign))
    {
        bool currentPref = this->tokenizer->isHexPreferred();
        this->tokenizer->setHexPreferred(true);

        peekToken = this->tokenizer->peekNextToken();
        ret = peekToken->isA(HSSHexNumber);
        this->tokenizer->setHexPreferred(currentPref);

    }
    else
    {
        peekToken = this->tokenizer->peekNextToken();

        //skip all whitespace and comments
        while (peekToken && (peekToken->isA(HSSWhitespace) || peekToken->isA(HSSBlockComment) || peekToken->isA(HSSLineComment)))
        {
            peekToken = this->tokenizer->peekNextToken();
        }
        //if the next token is a colon, it is either a property definition or a filter
        if (peekToken->isA(HSSColon))
        {
            //we'll peek until we find a end of statement, a closing block or an opening one
            peekToken = this->tokenizer->peekNextToken();
            //if we find a whitespace or an object sign here, we can be sure it's a property definition
            if (peekToken->isA(HSSWhitespace) || peekToken->isA(HSSObjectSign))
            {
                ret = true;
            }
            else
            {
                while (!peekToken->isA(HSSEndOfStatement) && !peekToken->isA(HSSBlockClose) && !peekToken->isA(HSSBlockOpen))
                {
                    peekToken = this->tokenizer->peekNextToken();
                    this->checkForUnexpectedEndOfSource();
                }
                //if we find an opening block, we're dealing with a selector
                if (peekToken->isA(HSSBlockOpen))
                {
                    ret = false;
                }
            }
        }
        else
        {
            if (this->currentToken->isA(HSSObjectSign))
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
                        this->checkForUnexpectedEndOfSource();
                        peekToken = this->tokenizer->peekNextToken();
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
                        peekToken = this->tokenizer->peekNextToken();
                        this->checkForUnexpectedEndOfSource();
                    }
                }
            }
        }
    }

    this->tokenizer->resetPeek();
    return ret;
}

QSharedPointer<HSSCombinator> HSSParser::readChildrenCombinatorOrSkip()
{
    QSharedPointer<HSSCombinator> ret;
    //are we dealing with whitespace?
    if (this->currentToken->isA(HSSWhitespace))
    {
        if (this->isChildrenCombinator())
        {
            QSharedPointer<HSSCombinator> newCombinator = QSharedPointer<HSSCombinator>(new HSSCombinator(HSSCombinatorTypeChildren, this->getController()));
            this->readNextToken();
            return newCombinator;
        }
        else
        {
            //alright, ignore it
            this->skip(HSSWhitespace);
            return ret;
        }
    }
    else
    {
        //done, nothing to see here, move along...
        return ret;
    }
}

//this expects the current token to be a symbol

QSharedPointer<HSSCombinator> HSSParser::readSymbolCombinator()
{
    axr_log(LoggerChannelHSSParser, "HSSParser: reading symbol combinator");

    /**
     *  @todo check the context
     */
    QSharedPointer<HSSCombinator> ret;
    const char currentTokenChar = *(VALUE_TOKEN(this->currentToken)->getString()).toStdString().c_str();
    switch (currentTokenChar)
    {
    case '=':
        ret = QSharedPointer<HSSCombinator>(new HSSCombinator(HSSCombinatorTypeSiblings, this->getController()));
        break;
    case '-':
        ret = QSharedPointer<HSSCombinator>(new HSSCombinator(HSSCombinatorTypePreviousSiblings, this->getController()));
        break;
    case '+':
        ret = QSharedPointer<HSSCombinator>(new HSSCombinator(HSSCombinatorTypeNextSiblings, this->getController()));
        break;
    case '.':
        if (VALUE_TOKEN(this->currentToken)->getString() == "..")
        {
            ret = QSharedPointer<HSSCombinator>(new HSSCombinator(HSSCombinatorTypeDescendants, this->getController()));
        }

        break;
    default:
        throw AXRWarning("HSSParser", "Unexpected token of type " + HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->sourceUrl(), this->line, this->column);
    }

    this->readNextToken();
    this->skip(HSSWhitespace);

    return ret;
}

//this assumes that the currentToken is an identifier

QSharedPointer<HSSNameSelector> HSSParser::readNameSelector(bool isNegating)
{
    axr_log(LoggerChannelHSSParser, "HSSParser: reading name selector");

    AXRString theValue = VALUE_TOKEN(this->currentToken)->getString();
    QSharedPointer<HSSNameSelector> ret = QSharedPointer<HSSNameSelector>(new HSSNameSelector(theValue, this->getController()));
    ret->setNegating(isNegating);
    this->readNextToken();

    return ret;
}


//this assumes currentToken is an object sign or an ampersand

QSharedPointer<HSSObjectDefinition> HSSParser::readObjectDefinition(AXRString propertyName)
{
    axr_log(LoggerChannelHSSParser, "HSSParser: reading object definition");

    QSharedPointer<HSSObjectDefinition> ret;
    AXRString objtype;
    QSharedPointer<HSSObject> obj;

    //set the current context
    this->currentContext.push_back(HSSParserContextObjectDefinition);

    //we either deal with a @ or an & symbol
    if (this->currentToken->isA(HSSObjectSign))
    {
        this->skipExpected(HSSObjectSign, true);

        //first we need to know what type of object it is
        if (this->currentToken->isA(HSSWhitespace) || this->currentToken->isA(HSSBlockOpen))
        {
            //damn, we'll have to derive that from the context
            if (this->currentObjectContext.size() > 0)
            {
                if (propertyName == "")
                {
                    objtype = this->currentObjectContext.top()->defaultObjectType();
                }
                else
                {
                    objtype = this->currentObjectContext.top()->defaultObjectType(propertyName);
                }

            }
            else
            {
                objtype = "container";
            }
        }
        else if (this->currentToken->isA(HSSIdentifier))
        {
            //alright, we've got a type, look it up
            objtype = VALUE_TOKEN(this->currentToken)->getString();
            this->readNextToken();
            this->checkForUnexpectedEndOfSource();
        }
        else
        {
            throw AXRError("HSSParser", "Unexpected token while reading object definition: " + HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->sourceUrl(), this->line, this->column);
        }

        //try to create an object of that type
        try
        {
            obj = HSSObject::newObjectWithType(objtype, this->controller);
        }
        catch (const AXRError &e)
        {
            AXRWarning("HSSParser", "Invalid object name " + e.getMessage(), this->currentFile->sourceUrl(), this->line, this->column).raise();
            if (this->currentObjectContext.size() > 0)
            {
                if (propertyName == "")
                {
                    objtype = this->currentObjectContext.top()->defaultObjectType();
                }
                else
                {
                    objtype = this->currentObjectContext.top()->defaultObjectType(propertyName);
                }
            }
            else
            {
                objtype = "value";
            }

            try
            {
                obj = HSSObject::newObjectWithType(objtype, this->controller);
            }
            catch (const AXRError &)
            {
                objtype = "value";
                obj = HSSObject::newObjectWithType(objtype, this->controller);
            }
        }

        this->_lastObjectType = objtype;

    }
    else
    {
        this->skipExpected(HSSAmpersand, true);
        obj = HSSObject::newObjectWithType(this->_lastObjectType, this->controller);
        objtype = this->_lastObjectType;

        if (this->currentToken->isA(HSSObjectSign))
        {
            AXRWarning("HSSParser", "Using &@ is obsolete, you shouldn't do that anymore", this->currentFile->sourceUrl(), this->line, this->column).raise();
            this->readNextToken(true);
        }
    }

    //get the name of the object
    if (this->currentToken->isA(HSSWhitespace))
    {
        this->skip(HSSWhitespace);
    }

    AXRController * controller = this->getController();

    switch (this->currentToken->getType())
    {
    case HSSIdentifier:
        obj->setName(VALUE_TOKEN(this->currentToken)->getString());
        axr_log(LoggerChannelObsolete3, "setting its name to " + VALUE_TOKEN(this->currentToken)->getString());
        this->readNextToken();
        break;
    case HSSBlockOpen:
        //it is the opening curly brace, therefore an anonymous object:
        //do nothing
        break;
    case HSSEndOfStatement:
    case HSSBlockClose:
    case HSSParenthesisClose:
    {
        //the property definition ends here
        ret = QSharedPointer<HSSObjectDefinition>(new HSSObjectDefinition(obj, controller));
        return ret;
    }

    default:
        throw AXRError("HSSParser", "Unexpected token while reading object definition: " + HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->sourceUrl(), this->line, this->column);
        break;
    }

    ret = QSharedPointer<HSSObjectDefinition>(new HSSObjectDefinition(obj, controller));
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
        try
        {
            switch (this->currentToken->getType())
            {
            case HSSSymbol:
            {
                QSharedPointer<HSSRule> theRule = this->readRule();
                if (theRule)
                    ret->rulesAdd(theRule);
                break;
            }

            case HSSAmpersand:
            {
                QSharedPointer<HSSObjectDefinition> childDef = this->readObjectDefinition(propertyName);
                childDef->setParentNode(ret);
                ret->childrenAdd(childDef);
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
                        ret->propertiesAdd(propertyDefinition);
                    }
                }
                else
                {
                    QSharedPointer<HSSRule> theRule = this->readRule();
                    if (theRule)
                        ret->rulesAdd(theRule);
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
                        ret->propertiesAdd(propertyDefinition);
                    }
                }
                else
                {
                    QSharedPointer<HSSRule> childRule = this->readInstructionRule();
                    if (childRule)
                        ret->rulesAdd(childRule);
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
                        ret->propertiesAdd(propertyDefinition);
                    }
                }
                else
                {
                    AXRWarning("HSSParser", "Unexpected token of type " + HSSToken::tokenStringRepresentation(this->currentToken->getType()) + " while reading object definition", this->currentFile->sourceUrl(), this->line, this->column).raise();
                    this->readNextToken();
                    this->skip(HSSWhitespace);
                }
                break;
            }
            }

        }
        catch (const AXRError &e)
        {
            AXRWarning("HSSParser", "Invalid value for " + e.getMessage(), this->currentFile->sourceUrl(), this->line, this->column).raise();
            if (!this->atEndOfSource())
            {
                this->skipUntilEndOfStatement();
                this->readNextToken();
                this->checkForUnexpectedEndOfSource();
                this->skip(HSSWhitespace);
            }
        }

        if (this->atEndOfSource())
        {
            AXRWarning("HSSParser", "Auto closing block of object definition with name " + obj->getName() + " because of unexpected end of file", this->currentFile->sourceUrl(), this->line, this->column).raise();
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
    this->_lastObjectType = objtype;
    if (!this->atEndOfSource())
    {
        //ignore all the whitespace after the block
        this->skip(HSSWhitespace);
    }

    return ret;
}

void HSSParser::recursiveAddObjectDefinition(QSharedPointer<HSSObjectDefinition> objDef)
{
    this->controller->objectTreeAdd(objDef);
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

QSharedPointer<HSSPropertyDefinition> HSSParser::readPropertyDefinition()
{
    return this->readPropertyDefinition(false, false);
}

QSharedPointer<HSSPropertyDefinition> HSSParser::readPropertyDefinition(bool shorthandChecked, bool isShorthand)
{
    axr_log(LoggerChannelHSSParser, "HSSParser: reading property definition");

    AXRString propertyName;

    //end of source is no good
    this->checkForUnexpectedEndOfSource();

    QSharedPointer<HSSPropertyDefinition> ret;
    bool valid = true;

    switch (this->currentToken->getType())
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
                try
                {
                    propertyName = VALUE_TOKEN(this->currentToken)->getString();
                    this->readNextToken();
                    //allow whitespace before colon
                    this->skip(HSSWhitespace);
                    //now must come a colon
                    this->skipExpected(HSSColon);
                    //we don't give a f$%# about whitespace
                    this->skip(HSSWhitespace);
                    this->currentObjectContext.top()->shorthandSkip(propertyName);
                }
                catch (const AXRError &e)
                {
                    e.raise();
                    valid = false;
                }
                break;
            }

            //fall through to 'default:'

        }
        else
        {
            //it is not a property definition, abort
            throw AXRError("HSSParser", "Failed to read property definition", this->currentFile->sourceUrl(), this->line, this->column);
        }

        //fall through to 'default:'
    }

    default:
    {
        //we assume we are dealing with shorthand notation
        //get the property name for the current value
        try
        {
            propertyName = this->currentObjectContext.top()->getPropertyForCurrentValue();
            //consume the property
            this->currentObjectContext.top()->shorthandNext();
        }
        catch (const AXRError &)
        {
            AXRError("HSSParser", "Could not get property for current value").raise();
            valid = false;
        }
        break;
    }
    }

    bool done = false;
    AXRController * controller = this->getController();
    ret = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(propertyName, controller));

    while (!done && valid)
    {
        try
        {
            //now comes either an object definition, a literal value or an expression
            //object
            if (this->currentToken->isA(HSSObjectSign))
            {
                bool objdefDone = false;
                while (!objdefDone)
                {
                    objdefDone = true;
                    try
                    {
                        QSharedPointer<HSSObjectDefinition> objdef = this->readObjectDefinition(propertyName);
                        if (objdef)
                        {
                            ret->addValue(objdef);
                        }
                    }
                    catch (const AXRError &e)
                    {
                        e.raise();
                    }

                    if (this->currentToken->isA(HSSAmpersand))
                    {
                        objdefDone = false;
                    }

                }
                //this->readNextToken();

            }
            else if (this->currentToken->isA(HSSSingleQuoteString) || this->currentToken->isA(HSSDoubleQuoteString))
            {
                ret->addValue(QSharedPointer<HSSStringConstant>(new HSSStringConstant(VALUE_TOKEN(this->currentToken)->getString(), controller)));
                this->checkForUnexpectedEndOfSource();
                this->readNextToken();
                this->skip(HSSWhitespace);

                //number literal
            }
            else if (this->currentToken->isA(HSSNumber) || this->currentToken->isA(HSSPercentageNumber) || this->currentToken->isA(HSSParenthesisOpen))
            {
                /**
                 *  @todo parse the number and see if it is an int or a float
                 */
                QSharedPointer<HSSParserNode> exp = this->readExpression();
                if (exp)
                {
                    ret->addValue(exp);
                }
                else
                {
                    valid = false;
                }


            }
            else if (this->currentToken->isA(HSSIdentifier))
            {
                //this is either a function, a keyword or an object name

                AXRString valuestr = VALUE_TOKEN(this->currentToken)->getString();
                //check if it is a function
                QSharedPointer<HSSObject> objectContext = this->currentObjectContext.top();

                if (objectContext->isFunction(valuestr, propertyName))
                {
                    QSharedPointer<HSSParserNode> exp = this->readExpression();
                    if (exp)
                    {
                        ret->addValue(exp);
                    }
                    else
                    {
                        valid = false;
                    }

                    //check if it is a keyword
                }
                else if (objectContext->isKeyword(valuestr, propertyName))
                {
                    ret->addValue(QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(valuestr, controller)));
                    this->readNextToken();
                    //we assume it is an object name at this point
                }
                else
                {
                    ret->addValue(QSharedPointer<HSSObjectNameConstant>(new HSSObjectNameConstant(valuestr, controller)));
                    this->readNextToken();
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
            else if (this->currentToken->isA(HSSInstructionSign))
            {
                QSharedPointer<HSSInstruction> theInstruction;

                theInstruction = this->readInstruction();

                if (theInstruction)
                {
                    ret->addValue(this->getObjectFromInstruction(theInstruction));
                }
                else
                {
                    valid = false;
                }
            }
            else if (this->currentToken->isA(HSSEndOfStatement))
            {
                done = true;

            }
            else
            {
                valid = false;
            }

            if (!valid)
            {
                done = true;
            }
        }
        catch (const AXRError &e)
        {
            e.raise();
            valid = false;
            done = true;
        }

        if (valid)
        {
            this->skip(HSSWhitespace, true);
            if (this->currentToken->isA(HSSComma))
            {
                this->readNextToken();
                this->skip(HSSWhitespace, true);
            }
            else
            {
                done = true;
            }
        }
    }
    if (valid)
    {
        //skip the semicolon
        this->skip(HSSEndOfStatement);
        this->skip(HSSWhitespace);
    }
    else if (!this->atEndOfSource())
    {
        this->skipUntilEndOfStatement();
    }

    if (!valid)
    {
        throw AXRError("HSSParser", "Errors found while reading " + propertyName, this->currentFile->sourceUrl(), this->line, this->column);
    }

    return ret;
}

QSharedPointer<HSSParserNode> HSSParser::readValue(AXRString propertyName, bool &valid)
{
    axr_log(LoggerChannelHSSParser, "HSSParser: reading value");

    bool isValid = true;
    QSharedPointer<HSSParserNode> ret;
    try
    {
        //now comes either an object definition, a literal value or an expression
        //object
        if (this->currentToken->isA(HSSObjectSign))
        {
            QSharedPointer<HSSObjectDefinition> objdef = this->readObjectDefinition(propertyName);
            if (objdef)
            {
                ret = objdef;
            }
            else
            {
                isValid = false;
            }
            //this->readNextToken();

        }
        else if (this->currentToken->isA(HSSSingleQuoteString) || this->currentToken->isA(HSSDoubleQuoteString))
        {
            ret = QSharedPointer<HSSStringConstant>(new HSSStringConstant(VALUE_TOKEN(this->currentToken)->getString(), this->getController()));
            this->checkForUnexpectedEndOfSource();
            this->readNextToken();
            this->skip(HSSWhitespace);

            //number literal
        }
        else if (this->currentToken->isA(HSSNumber) || this->currentToken->isA(HSSPercentageNumber) || this->currentToken->isA(HSSParenthesisOpen))
        {
            /**
             *  @todo parse the number and see if it is an int or a float
             */
            QSharedPointer<HSSParserNode> exp = this->readExpression();
            if (exp)
            {
                ret = exp;
            }
            else
            {
                isValid = false;
            }


        }
        else if (this->currentToken->isA(HSSIdentifier))
        {
            //this is either a function, a keyword or an object name

            AXRString valuestr = VALUE_TOKEN(this->currentToken)->getString();
            //check if it is a function
            QSharedPointer<HSSObject> objectContext = this->currentObjectContext.top();

            if (objectContext->isFunction(valuestr, propertyName))
            {
                QSharedPointer<HSSParserNode> exp = this->readExpression();
                if (exp)
                {
                    ret = exp;
                }
                else
                {
                    isValid = false;
                }

                //check if it is a keyword
            }
            else if (objectContext->isKeyword(valuestr, propertyName))
            {
                ret = QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(valuestr, this->getController()));
                this->readNextToken();
                //we assume it is an object name at this point
            }
            else
            {
                ret = QSharedPointer<HSSObjectNameConstant>(new HSSObjectNameConstant(valuestr, this->getController()));
                this->readNextToken();
            }


        }
        else if (this->currentToken->isA(HSSInstructionSign))
        {
            QSharedPointer<HSSInstruction> theInstruction;

            theInstruction = this->readInstruction();

            if (theInstruction)
            {
                ret = this->getObjectFromInstruction(theInstruction);
            }
            else
            {
                isValid = false;
            }

        }
        else
        {
            isValid = false;
        }
    }
    catch (const AXRError &e)
    {
        e.raise();
        isValid = false;
    }

    valid = isValid;

    return ret;
}

QSharedPointer<HSSInstruction> HSSParser::readInstruction()
{
    return this->readInstruction(true);
}

QSharedPointer<HSSInstruction> HSSParser::readInstruction(bool preferHex)
{
    axr_log(LoggerChannelHSSParser, "HSSParser: reading instruction");

    QSharedPointer<HSSInstruction> ret;
    AXRString currentval;

    //set preference
    this->tokenizer->setHexPreferred(preferHex);
    //skip the instruction sign -- this will automatically read the next token
    this->skipExpected(HSSInstructionSign);
    //restore
    this->tokenizer->setHexPreferred(false);

    this->checkForUnexpectedEndOfSource();
    //we are looking at
    //if it starts with a number, we are looking at a color instruction
    AXRController * controller = this->getController();
    if (this->currentToken->isA(HSSHexNumber))
    {
        currentval = VALUE_TOKEN(this->currentToken)->getString();
        switch (currentval.length())
        {
        case 1:
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSGrayscale1Instruction, currentval, controller));
            this->readNextToken();
            break;
        case 2:
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSGrayscale2Instruction, currentval, controller));
            this->readNextToken();
            break;
        case 3:
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSRGBInstruction, currentval, controller));
            this->readNextToken();
            break;
        case 4:
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSRGBAInstruction, currentval, controller));
            this->readNextToken();
            break;
        case 5:
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSRGBAAInstruction, currentval, controller));
            this->readNextToken();
            break;
        case 6:
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSRRGGBBInstruction, currentval, controller));
            this->readNextToken();
            break;
        case 7:
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSRRGGBBAInstruction, currentval, controller));
            this->readNextToken();
            break;
        case 8:
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSRRGGBBAAInstruction, currentval, controller));
            this->readNextToken();
            break;

        default:
            AXRWarning("HSSParser", "Wrong length for hexadecimal number (should be between 1 and 8 digits long, inclusive)", this->currentFile->sourceUrl(), this->line, this->column).raise();
            return ret;
        }

        this->skip(HSSWhitespace, true);

    }
    else if (this->currentToken->isA(HSSIdentifier))
    {
        AXRController * controller = this->getController();
        currentval = VALUE_TOKEN(this->currentToken)->getString();
        if (currentval == "new")
        {
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSNewInstruction, controller));
            this->readNextToken(true);
            if (this->currentToken->isA(HSSParenthesisOpen))
            {
                this->readNextToken(true);
                this->skip(HSSWhitespace);
                if (this->currentToken->isA(HSSNumber))
                {
                    HSSUnit number = VALUE_TOKEN(this->currentToken)->getLong();
                    ret->setArgument(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(number, controller)));
                    this->readNextToken(true);
                    this->skip(HSSWhitespace);
                    this->skipExpected(HSSParenthesisClose);
                    this->skip(HSSWhitespace);
                }
                else
                {
                    throw AXRError("HSSParser", "Unknown value in argument of new statement", this->currentFile->sourceUrl(), this->line, this->column);
                }
            }
        }
        else if (currentval == "ensure")
        {
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSEnsureInstruction, controller));
            this->readNextToken();
        }
        else if (currentval == "import")
        {

            this->readNextToken();
            this->checkForUnexpectedEndOfSource();
            this->skip(HSSWhitespace);

            if (this->currentToken->isA(HSSDoubleQuoteString) || this->currentToken->isA(HSSSingleQuoteString))
            {
                AXRString theString = VALUE_TOKEN(this->currentToken)->getString();
                ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSImportInstruction, theString, controller));
            }
            else if (this->currentToken->isA(HSSIdentifier))
            {
                AXRString instructionKw = VALUE_TOKEN(this->currentToken)->getString();
                if (instructionKw == "UIFramework")
                {
                    QUrl url;
                    url.setScheme(HSSFRAMEWORK_PROTOCOL);
                    url.setPath("framework/UIFramework.hss");
                    ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSImportInstruction, url.toString(), controller));
                }
                else
                {
                    throw AXRWarning("HSSParser", "Unknown keyword for import instruction.");
                }
            }
            else
            {
                throw AXRError("HSSParser", "Expected string", this->currentFile->sourceUrl(), this->line, this->column);
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
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSMoveInstruction, controller));
            this->readNextToken();
        }
        else if (currentval == "delete")
        {

            this->readNextToken();
            this->checkForUnexpectedEndOfSource();
            this->skip(HSSWhitespace);
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSDeleteInstruction, controller));

        }
        else
        {
            //balk
            throw AXRWarning("HSSParser", "Unknown instruction " + currentval, this->currentFile->sourceUrl(), this->line, this->column);
        }

    }
    else
    {
        //balk
        throw AXRWarning("HSSParser", "Unexpected token " + HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->sourceUrl(), this->line, this->column);
    }

    if (!this->atEndOfSource())
    {
        this->skip(HSSWhitespace);
    }

    return ret;
}

QSharedPointer<HSSObjectDefinition> HSSParser::getObjectFromInstruction(QSharedPointer<HSSInstruction> instruction)
{
    QSharedPointer<HSSObjectDefinition> ret;
    HSSInstructionType instructionType = instruction->getInstructionType();
    AXRController * controller = this->getController();
    switch (instructionType)
    {
    case HSSGrayscale1Instruction:
    case HSSGrayscale2Instruction:
    {
        //try to create an object of that type
        QSharedPointer<HSSRgb> obj = QSharedPointer<HSSRgb>(new HSSRgb(this->controller));

        unsigned int hexValue;
        AXRString tempstr = instruction->getValue();
        if (instructionType == HSSGrayscale1Instruction)
        {
            tempstr = tempstr + tempstr;
        }

        hexValue = tempstr.toUInt(NULL, 16);

        ret = QSharedPointer<HSSObjectDefinition>(new HSSObjectDefinition(obj, controller));

        AXRController * controller = this->getController();

        QSharedPointer<HSSPropertyDefinition> newRed = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(controller));
        newRed->setName("red");
        newRed->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(hexValue, controller)));
        ret->propertiesAdd(newRed);

        QSharedPointer<HSSPropertyDefinition> newGreen = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(controller));
        newGreen->setName("green");
        newGreen->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(hexValue, controller)));
        ret->propertiesAdd(newGreen);

        QSharedPointer<HSSPropertyDefinition> newBlue = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(controller));
        newBlue->setName("blue");
        newBlue->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(hexValue, controller)));
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
        QSharedPointer<HSSRgb> obj = QSharedPointer<HSSRgb>(new HSSRgb(this->controller));

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

        ret = QSharedPointer<HSSObjectDefinition>(new HSSObjectDefinition(obj, controller));

        QSharedPointer<HSSPropertyDefinition> newRed = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(controller));
        newRed->setName("red");
        newRed->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(redHex, controller)));
        ret->propertiesAdd(newRed);

        QSharedPointer<HSSPropertyDefinition> newGreen = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(controller));
        newGreen->setName("green");
        newGreen->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(greenHex, controller)));
        ret->propertiesAdd(newGreen);

        QSharedPointer<HSSPropertyDefinition> newBlue = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(controller));
        newBlue->setName("blue");
        newBlue->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(blueHex, controller)));
        ret->propertiesAdd(newBlue);

        QSharedPointer<HSSPropertyDefinition> newAlpha = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(controller));
        newAlpha->setName("alpha");
        newAlpha->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(alphaHex, controller)));
        ret->propertiesAdd(newAlpha);

        break;
    }

    default:
        axr_log(LoggerChannelObsolete1, "*********** eror: unknown instruction type ****************");
        break;
    }
    return ret;
}

//this function assumes currentToken is a instruction sign

QSharedPointer<HSSRule> HSSParser::readInstructionRule()
{
    axr_log(LoggerChannelHSSParser, "HSSParser: reading instruction rule");

    QSharedPointer<HSSInstruction> instruction = this->readInstruction(false);
    QSharedPointer<HSSRule> ret;
    switch (instruction->getInstructionType())
    {
    case HSSNewInstruction:
    case HSSEnsureInstruction:
    case HSSMoveInstruction:
    {
        ret = this->readRule();
        if (ret)
        {
            ret->setInstruction(instruction);
        }

        break;
    }

    case HSSDeleteInstruction:
    {
        //initialize the rule
        std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
        try
        {
            selectorChains = this->readSelectorChains(HSSEndOfStatement);
            //we expect the end of the statement here
            this->skipExpected(HSSEndOfStatement, true);
            //skip any whitespace
            this->skip(HSSWhitespace, true);
        }
        catch (const AXRError &e)
        {
            this->readNextToken();
            this->checkForUnexpectedEndOfSource();
            this->skip(HSSWhitespace);

            e.raise();

            //return an empty rule
            QSharedPointer<HSSRule> ret;
            return ret;
        }

        ret = QSharedPointer<HSSRule>(new HSSRule(this->getController()));
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
        axr_log(LoggerChannelObsolete1, "*********** eror: unknown instruction type ****************");
        break;
    }

    axr_log(LoggerChannelHSSParser, "HSSParser: reading instruction rule");

    return ret;
}

QSharedPointer<HSSParserNode> HSSParser::readExpression()
{
    axr_log(LoggerChannelHSSParser, "HSSParser: reading expression");

    QSharedPointer<HSSParserNode> ret = this->readAdditiveExpression();

    return ret;
}

QSharedPointer<HSSParserNode> HSSParser::readAdditiveExpression()
{
    security_brake_init();

    this->checkForUnexpectedEndOfSource();
    QSharedPointer<HSSParserNode> left = this->readMultiplicativeExpression();
    AXRController * controller = this->getController();
    while (!this->atEndOfSource() && this->currentToken->isA(HSSSymbol))
    {
        const char currentTokenChar = *(VALUE_TOKEN(this->currentToken)->getString()).toStdString().c_str();
        switch (currentTokenChar)
        {
        case '+':
        {
            this->readNextToken();
            this->skip(HSSWhitespace);
            left = QSharedPointer<HSSSum>(new HSSSum(left, this->readMultiplicativeExpression(), controller));
            break;
        }

        case '-':
        {
            this->readNextToken();
            this->skip(HSSWhitespace);
            left = QSharedPointer<HSSSubtraction>(new HSSSubtraction(left, this->readMultiplicativeExpression(), controller));
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

QSharedPointer<HSSParserNode> HSSParser::readMultiplicativeExpression()
{
    security_brake_init();

    this->checkForUnexpectedEndOfSource();
    QSharedPointer<HSSParserNode> left = this->readBaseExpression();
    AXRController * controller = this->getController();
    while (!this->atEndOfSource() && this->currentToken->isA(HSSSymbol))
    {

        const char currentTokenChar = *(VALUE_TOKEN(this->currentToken)->getString()).toStdString().c_str();
        switch (currentTokenChar)
        {
        case '*':
        {
            this->readNextToken();
            this->skip(HSSWhitespace);
            left = QSharedPointer<HSSMultiplication>(new HSSMultiplication(left, this->readBaseExpression(), controller));
            break;
        }

        case '/':
        {
            this->readNextToken();
            this->skip(HSSWhitespace);
            left = QSharedPointer<HSSDivision>(new HSSDivision(left, this->readBaseExpression(), controller));
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

QSharedPointer<HSSParserNode> HSSParser::readBaseExpression()
{
    this->checkForUnexpectedEndOfSource();
    QSharedPointer<HSSParserNode> left;

    switch (this->currentToken->getType())
    {
    case HSSNumber:
    {
        left = QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(VALUE_TOKEN(this->currentToken)->getLong(), this->getController()));
        this->readNextToken();
        this->skip(HSSWhitespace);
        break;
    }

    case HSSPercentageNumber:
    {
        left = QSharedPointer<HSSPercentageConstant>(new HSSPercentageConstant(VALUE_TOKEN(this->currentToken)->getLong(), this->getController()));
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
        /*
        AXRString valuestr = VALUE_TOKEN(this->currentToken)->getString();
        //check if it is a function
        QSharedPointer<HSSObject> objectContext = this->currentObjectContext.top();
        if (objectContext->isFunction(valuestr, propertyName)){*/
        left = this->readFunction();
        /*
       //check if it is a keyword
       } else if (objectContext->isKeyword(valuestr, propertyName)){
           left = QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(valuestr));
           this->readNextToken();
           this->skip(HSSWhitespace);
           //we assume it is an object name at this point
       } else {
           left = QSharedPointer<HSSObjectNameConstant>(new HSSObjectNameConstant(valuestr));
           this->readNextToken();
           this->skip(HSSWhitespace);
       }*/

        break;
    }

    default:
        throw AXRError("HSSParser", "Unknown token type " + HSSToken::tokenStringRepresentation(this->currentToken->getType()) + " while parsing base expression", this->currentFile->sourceUrl(), this->line, this->column);
    }

    return left;
}

////this method expects the currentToken to be an identifier
//QSharedPointer<HSSParserNode> HSSParser::readFilter()
//{
//    axr_log(LoggerChannelHSSParser, "HSSParser: reading filter");
//    inc_output_indent();
//
//    QSharedPointer<HSSFilter> ret;
//    this->expect(HSSIdentifier);
//
//    AXRString filterName = VALUE_TOKEN(this->currentToken)->getString();
//    ret = HSSFilter::newFilterWithStringType(filterName);
//
//    this->readNextToken();
//    this->checkForUnexpectedEndOfSource();
//
//    dec_output_indent();
//
//    return ret;
//}

//this method expects the currentToken to be an identifier

QSharedPointer<HSSParserNode> HSSParser::readFlag()
{
    axr_log(LoggerChannelHSSParser, "HSSParser: reading flag");

    QSharedPointer<HSSFlag> ret;
    this->expect(HSSIdentifier);

    AXRString flagName = VALUE_TOKEN(this->currentToken)->getString();
    ret = QSharedPointer<HSSFlag>(new HSSFlag(this->getController()));
    ret->setName(flagName);

    this->readNextToken();
    this->checkForUnexpectedEndOfSource();

    return ret;
}

QSharedPointer<HSSParserNode> HSSParser::readFunction()
{
    axr_log(LoggerChannelHSSParser, "HSSParser: reading function");

    QSharedPointer<HSSParserNode> ret;

    this->checkForUnexpectedEndOfSource();
    AXRController * controller = this->getController();
    if (this->currentToken->isA(HSSIdentifier))
    {
        //create new function
        AXRString name = VALUE_TOKEN(this->currentToken)->getString();
        if (name == "ref")
        {
            ret = this->readRefFunction();

        }
        else if (name == "sel")
        {
            ret = this->readSelFunction();
        }
        else if (
                name == "flag"
                || name == "unflag"
                || name == "toggleFlag"
                || name == "takeFlag"
                )
        {
            ret = this->readFlagFunction();
        }
        else if (name == "attr")
        {
            ret = this->readAttrFunction();
        }
        else if (name == "min")
        {
            ret = this->readMinFunction();
        }
        else if (name == "max")
        {
            ret = this->readMaxFunction();
        }
        else if (name == "ceil")
        {
            ret = this->readCeilFunction();
        }
        else if (name == "floor")
        {
            ret = this->readFloorFunction();
        }
        else if (name == "round")
        {
            ret = this->readRoundFunction();
        }
        else if (controller->document()->isCustomFunction(name))
        {
            ret = this->readCustomFunction();
        }
        else
        {
            throw AXRError("HSSParser", "Unexpected function name: " + name, this->currentFile->sourceUrl(), this->line, this->column);
        }
    }
    else
    {
        throw AXRError("HSSParser", "Unexpected token while reading function: " + HSSToken::tokenStringRepresentation(this->currentToken->getType()), this->currentFile->sourceUrl(), this->line, this->column);
    }

    return ret;
}

QSharedPointer<HSSParserNode> HSSParser::readRefFunction()
{
    QSharedPointer<HSSParserNode> ret;
    QSharedPointer<HSSRefFunction> refFunction = QSharedPointer<HSSRefFunction>(new HSSRefFunction(controller));

    this->readNextToken(true);
    this->skip(HSSWhitespace, true);
    this->skipExpected(HSSParenthesisOpen, true);
    this->skip(HSSWhitespace, true);
    //read the arguments
    //first, we expect either "min", "max", "avg" or a property name
    if (!this->currentToken->isA(HSSIdentifier))
    {
        AXRString name = VALUE_TOKEN(this->currentToken)->getString();
        axr_log(LoggerChannelObsolete1, "HSSParser: unexpected token while reading ref function " + name);
    }
    else
    {
        AXRString firstValue = VALUE_TOKEN(this->currentToken)->getString();
        if (firstValue == "min"
            || firstValue == "max"
            || firstValue == "avg")
        {
            refFunction->setModifier(firstValue);

            //after this there comes the property name
            this->checkForUnexpectedEndOfSource();
            this->readNextToken();
            this->skip(HSSWhitespace);
            if (!this->currentToken->isA(HSSIdentifier))
            {
                axr_log(LoggerChannelObsolete1, "HSSParser: unexpected token while reading ref function: " + HSSToken::tokenStringRepresentation(this->currentToken->getType()));
            }
            else
            {
                refFunction->setPropertyName(HSSObservable::observablePropertyFromString(VALUE_TOKEN(this->currentToken)->getString()));
            }
        }
        else
        {
            //just the property name
            refFunction->setPropertyName(HSSObservable::observablePropertyFromString(firstValue));
        }
    }

    this->checkForUnexpectedEndOfSource();
    this->readNextToken();
    this->skip(HSSWhitespace, true);
    //if shorthand notation -- assumes 'of @this'
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
    if (this->currentToken->isA(HSSParenthesisClose))
    {
        QSharedPointer<HSSSelectorChain> selectorChain;
        selectorChain = QSharedPointer<HSSSelectorChain>(new HSSSelectorChain(controller));
        QSharedPointer<HSSSimpleSelector> newSs = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(controller));
        newSs->setName(QSharedPointer<HSSThisSelector>(new HSSThisSelector(controller)));
        selectorChain->add(newSs);
        selectorChains.push_back(selectorChain);
        this->readNextToken(true);
        this->skip(HSSWhitespace, true);

    }
    else
    {
        if (!this->currentToken->isA(HSSIdentifier) || VALUE_TOKEN(this->currentToken)->getString() != "of")
        {
            axr_log(LoggerChannelObsolete1, "HSSParser: unexpected token while reading ref function: " + HSSToken::tokenStringRepresentation(this->currentToken->getType()));
        }
        this->checkForUnexpectedEndOfSource();
        this->readNextToken(true);
        this->skipExpected(HSSWhitespace, true);

        //now read the selector chain
        selectorChains = this->readSelectorChains(HSSParenthesisClose);
        //we expect the closing parenthesis here
        this->skipExpected(HSSParenthesisClose, true);
        //skip any whitespace
        this->skip(HSSWhitespace, true);
    }

    refFunction->setSelectorChains(selectorChains);
    ret = refFunction;
    return ret;
}

QSharedPointer<HSSParserNode> HSSParser::readSelFunction()
{
    QSharedPointer<HSSParserNode> ret;
    this->readNextToken(true);
    this->skip(HSSWhitespace, true);
    this->skipExpected(HSSParenthesisOpen, true);
    this->skip(HSSWhitespace, true);
    //read the selector chain
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
    try
    {
        selectorChains = this->readSelectorChains(HSSParenthesisClose);
        //we expect the closing parenthesis here
        this->skipExpected(HSSParenthesisClose, true);
        //skip any whitespace
        this->skip(HSSWhitespace, true);
    }
    catch (const AXRError &e)
    {
        this->readNextToken(true);
        this->skip(HSSWhitespace);

        e.raise();

        //return an empty function
        return ret;
    }

    QSharedPointer<HSSSelFunction> selFunction = QSharedPointer<HSSSelFunction>(new HSSSelFunction(controller));
    selFunction->setSelectorChains(selectorChains);

    ret = selFunction;
    return ret;
}

QSharedPointer<HSSParserNode> HSSParser::readFlagFunction()
{
    QSharedPointer<HSSParserNode> ret;
    AXRString name = VALUE_TOKEN(this->currentToken)->getString();
    QSharedPointer<HSSFlagFunction> flagFunction = QSharedPointer<HSSFlagFunction>(new HSSFlagFunction(HSSFlagFunction::flagFunctionTypeFromString(name), controller));

    this->readNextToken(true);
    this->skip(HSSWhitespace, true);
    this->skipExpected(HSSParenthesisOpen, true);
    this->skip(HSSWhitespace, true);
    //read the arguments
    //first, we expect the name
    if (this->currentToken->isA(HSSIdentifier))
    {
        flagFunction->setName(VALUE_TOKEN(this->currentToken)->getString());
    }
    else if (this->currentToken->isA(HSSSymbol) && VALUE_TOKEN(this->currentToken)->getString() == "*")
    {
        flagFunction->setName("*");
    }
    else
    {
        axr_log(LoggerChannelObsolete0, "HSSParser: unexpected token while reading flagging function " + name);
    }

    this->checkForUnexpectedEndOfSource();
    this->readNextToken();
    this->skip(HSSWhitespace, true);
    //if shorthand notation
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
    if (this->currentToken->isA(HSSParenthesisClose))
    {
        QSharedPointer<HSSSelectorChain> selectorChain;
        selectorChain = QSharedPointer<HSSSelectorChain>(new HSSSelectorChain(controller));
        QSharedPointer<HSSSimpleSelector> newSs = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(controller));
        if(name == "takeFlag")
        {
            //assumes 'of *'
            newSs->setName(QSharedPointer<HSSUniversalSelector>(new HSSUniversalSelector(controller)));
        }
        else
        {
            //assumes 'of @this'
            newSs->setName(QSharedPointer<HSSThisSelector>(new HSSThisSelector(controller)));
        }
        selectorChain->add(newSs);
        selectorChains.push_back(selectorChain);
        this->readNextToken(true);
        this->skip(HSSWhitespace, true);

    }
    else
    {
        if (!this->currentToken->isA(HSSIdentifier) || VALUE_TOKEN(this->currentToken)->getString() != "of")
        {
            AXRError("HSSParser", "Unexpected token while reading flagging function: " + HSSToken::tokenStringRepresentation(this->currentToken->getType())).raise();
        }
        this->checkForUnexpectedEndOfSource();
        this->readNextToken(true);
        this->skipExpected(HSSWhitespace, true);

        //now read the selector chain
        selectorChains = this->readSelectorChains(HSSParenthesisClose);
        //we expect the closing parenthesis here
        this->skipExpected(HSSParenthesisClose, true);
        //skip any whitespace
        this->skip(HSSWhitespace, true);
    }

    flagFunction->setSelectorChains(selectorChains);
    ret = flagFunction;
    return ret;
}

QSharedPointer<HSSParserNode> HSSParser::readAttrFunction()
{
    QSharedPointer<HSSParserNode> ret;
    AXRString name = VALUE_TOKEN(this->currentToken)->getString();
    QSharedPointer<HSSAttrFunction> attrFunction = QSharedPointer<HSSAttrFunction>(new HSSAttrFunction(controller));

    this->readNextToken(true);
    this->skip(HSSWhitespace, true);
    this->skipExpected(HSSParenthesisOpen, true);
    this->skip(HSSWhitespace, true);
    //read the arguments
    //first, we expect an identifier
    if (!this->currentToken->isA(HSSIdentifier))
    {
        AXRError("HSSParser", "Unexpected token while reading attr function " + name).raise();
    }
    else
    {
        attrFunction->setAttributeName(VALUE_TOKEN(this->currentToken)->getString());
    }

    this->checkForUnexpectedEndOfSource();
    this->readNextToken();
    this->skip(HSSWhitespace, true);
    //if shorthand notation -- assumes 'of @this'
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
    if (this->currentToken->isA(HSSParenthesisClose))
    {
        AXRController * controller = this->getController();
        QSharedPointer<HSSSelectorChain> selectorChain;
        selectorChain = QSharedPointer<HSSSelectorChain>(new HSSSelectorChain(controller));
        QSharedPointer<HSSSimpleSelector> newSs = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(controller));
        newSs->setName(QSharedPointer<HSSThisSelector>(new HSSThisSelector(controller)));
        selectorChain->add(newSs);
        selectorChains.push_back(selectorChain);
        this->readNextToken(true);
        this->skip(HSSWhitespace, true);
    }
    else
    {
        if (!this->currentToken->isA(HSSIdentifier) || VALUE_TOKEN(this->currentToken)->getString() != "of")
        {
            AXRError("HSSParser", "Unexpected token while reading attr function " + name).raise();
        }
        this->checkForUnexpectedEndOfSource();
        this->readNextToken(true);
        this->skipExpected(HSSWhitespace, true);

        //now read the selector chain
        selectorChains = this->readSelectorChains(HSSParenthesisClose);
        //we expect the closing parenthesis here
        this->skipExpected(HSSParenthesisClose, true);
        //skip any whitespace
        this->skip(HSSWhitespace, true);
    }

    attrFunction->setSelectorChains(selectorChains);
    ret = attrFunction;
    return ret;
}

QSharedPointer<HSSParserNode> HSSParser::readMinFunction()
{
    AXRError("HSSParser", "the min() function is still unimplemented").raise();
    QSharedPointer<HSSParserNode> ret;
    return ret;
}

QSharedPointer<HSSParserNode> HSSParser::readMaxFunction()
{
    AXRError("HSSParser", "the max() function is still unimplemented").raise();
    QSharedPointer<HSSParserNode> ret;
    return ret;
}

QSharedPointer<HSSParserNode> HSSParser::readCeilFunction()
{
    AXRError("HSSParser", "the ceil() function is still unimplemented").raise();
    QSharedPointer<HSSParserNode> ret;
    return ret;
}

QSharedPointer<HSSParserNode> HSSParser::readFloorFunction()
{
    AXRError("HSSParser", "the floor() function is still unimplemented").raise();
    QSharedPointer<HSSParserNode> ret;
    return ret;
}

QSharedPointer<HSSParserNode> HSSParser::readRoundFunction()
{
    AXRError("HSSParser", "the round() function is still unimplemented").raise();
    QSharedPointer<HSSParserNode> ret;
    return ret;
}

QSharedPointer<HSSParserNode> HSSParser::readCustomFunction()
{
    QSharedPointer<HSSParserNode> ret;
    AXRString name = VALUE_TOKEN(this->currentToken)->getString();
    this->readNextToken(true);
    this->skip(HSSWhitespace, true);
    this->skipExpected(HSSParenthesisOpen, true);
    this->skip(HSSWhitespace, true);
    std::deque<QSharedPointer<HSSParserNode> > arguments;
    while (!this->currentToken->isA(HSSParenthesisClose) && !this->atEndOfSource())
    {
        bool valid;
        arguments.push_back(this->readValue("", valid));
        if (this->currentToken->isA(HSSComma))
        {
            this->readNextToken(true);
        }
    }
    QSharedPointer<HSSFunction> theFunction = QSharedPointer<HSSFunction>(new HSSFunction(HSSFunctionTypeCustom, controller));
    theFunction->setArguments(arguments);
    theFunction->setName(name);
    ret = theFunction;
    this->readNextToken(true);
    this->skip(HSSWhitespace, true);
    return ret;
}

void HSSParser::readNextToken()
{
    this->readNextToken(false);
}

void HSSParser::readNextToken(bool checkForUnexpectedEndOfSource)
{
    //read next one
    try
    {
        QSharedPointer<HSSToken> theToken = this->tokenizer->readNextToken();
        if (theToken && (theToken->isA(HSSBlockComment) || theToken->isA(HSSLineComment)))
        {
            this->readNextToken();
        }
        else
        {
            this->currentToken = theToken;
        }

    }
    catch (const AXRError &)
    {
        this->currentToken = QSharedPointer<HSSToken>();
        throw;
    }

    if (!this->atEndOfSource())
    {
        this->line = this->currentToken->line;
        this->column = this->currentToken->column;
    }
    else
    {
        this->line = this->tokenizer->currentLine();
        this->column = this->tokenizer->currentColumn() - 1;
    }

    if (checkForUnexpectedEndOfSource) this->checkForUnexpectedEndOfSource();
}

bool HSSParser::atEndOfSource()
{
    if (!this->currentToken)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void HSSParser::checkForUnexpectedEndOfSource()
{
    if (this->atEndOfSource())
    {
        throw AXRError("HSSParser", "The file ended unexpectedly", this->currentFile->sourceUrl(), this->line, this->column);
    }
}

void HSSParser::skipExpected(HSSTokenType type)
{
    this->skipExpected(type, false);
}

void HSSParser::skipExpected(HSSTokenType type, bool checkForUnexpectedEndOfSource)
{
    this->checkForUnexpectedEndOfSource();
    if (!this->currentToken->isA(type))
    {
        throw AXRError("HSSParser", "Expected token of type " + HSSToken::tokenStringRepresentation(type), this->currentFile->sourceUrl(), this->line, this->column);
    }
    this->readNextToken(checkForUnexpectedEndOfSource);
}

void HSSParser::skipExpected(HSSTokenType type, AXRString value)
{
    this->skipExpected(type, value, false);
}

void HSSParser::skipExpected(HSSTokenType type, AXRString value, bool checkForUnexpectedEndOfSource)
{
    this->checkForUnexpectedEndOfSource();
    /**
     *  @todo I'm not sure if this works as expected
     */
    QSharedPointer<HSSValueToken> currentToken = QSharedPointer<HSSValueToken>(VALUE_TOKEN(this->currentToken));
    if (!currentToken->equals(type, value))
    {
        throw AXRError("HSSParser", "Expected token of type " + HSSToken::tokenStringRepresentation(type) + " and value " + value, this->currentFile->sourceUrl(), this->line, this->column);
    }
    this->readNextToken(checkForUnexpectedEndOfSource);
}

void HSSParser::skip(HSSTokenType type)
{
    this->skip(type, false);
}

void HSSParser::skip(HSSTokenType type, bool checkForUnexpectedEndOfSource)
{
    if (this->currentToken->isA(type))
    {
        if (type == HSSWhitespace)
        {
            while (!this->atEndOfSource() && this->currentToken->isA(type))
            {
                this->readNextToken();
            }
            if (checkForUnexpectedEndOfSource)
            {
                this->checkForUnexpectedEndOfSource();
            }
        }
        else
        {
            this->readNextToken(checkForUnexpectedEndOfSource);
        }
    }
}

void HSSParser::skipUntilEndOfStatement()
{
    while (!this->currentToken->isA(HSSEndOfStatement) && !this->currentToken->isA(HSSBlockClose))
    {
        this->readNextToken();
        if (this->currentToken->isA(HSSBlockOpen))
        {
            this->skipUntilEndOfStatement();
            this->readNextToken();
        }
        this->checkForUnexpectedEndOfSource();
    }
}

void HSSParser::expect(HSSTokenType type)
{
    if (!this->currentToken->isA(type))
        throw AXRError("HSSParser", "Expected token of type " + HSSToken::tokenStringRepresentation(type), this->currentFile->sourceUrl(), this->line, this->column);
}

void HSSParser::currentObjectContextRemoveLast()
{
    this->currentObjectContext.pop();
}

size_t HSSParser::currentObjectContextSize() const
{
    return this->currentObjectContext.size();
}

void HSSParser::currentObjectContextAdd(QSharedPointer<HSSObject> theObject)
{
    this->currentObjectContext.push(theObject);
}


AXRController* HSSParser::getController()
{
    return controller;
}

void HSSParser::setController(AXRController *controller)
{
    this->controller = controller;
}
