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

#include "axr.h"
#include "HSSCodeParser.h"

using namespace AXR;

namespace AXR
{
    class HSSCodeParserPrivate
    {
        friend class HSSCodeParser;

        HSSCodeParserPrivate()
        : hasLoadedFile(), file(), tokenizer(), line(1), column(1), controller(), receiver(), notifiesReceiver(true)
        {
        }

        AXRController * controller;
        HSSParserReceiver * receiver;

        bool hasLoadedFile;

        QSharedPointer<HSSTokenizer> tokenizer;
        QSharedPointer<AXRBuffer> file;
        std::vector<QSharedPointer<AXRBuffer> > loadedFiles;

        QSharedPointer<HSSToken> currentToken;
        size_t line;
        size_t column;
        std::vector<HSSParserContext> currentContext;
        std::stack<QSharedPointer<HSSObject> > currentObjectContext;
        AXRString _lastObjectType;
        QSharedPointer<HSSContainer> _containerContextObj;
        bool notifiesReceiver;
        bool ignoreTokenReadCalls;
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
    d->ignoreTokenReadCalls = false;
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
    axr_log(LoggerChannelOverview, "HSSParser: loading file " + file->sourceUrl());

    //check if the file has been loaded already
    if (std::find(d->loadedFiles.begin(), d->loadedFiles.end(), file) != d->loadedFiles.end())
    {
        AXRError("HSSParser", "Failed loading file " + file->sourceUrl() + " because of circular reference").raise();
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
                this->parseNext();
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

bool HSSCodeParser::parseNext()
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
                        AXRString theName = d->currentToken->getString();
                        theObj->setName(theName);

                        if (d->notifiesReceiver)
                        {
                            //notify the receiver
                            d->receiver->receiveParserEvent(HSSParserEventObjectName, HSSObjectNameConstant::createConstant(theName, d->controller));
                        }

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

            //if the statement starts with an identifier, universal selector or combinator it is a rule,
            //except for variable declarations, which start with "var"
        case HSSIdentifier:
        {
            this->parseIdentifierStatement();
            break;
        }
        case HSSSymbol:
        {
            if (d->currentToken->equals(HSSSymbol, "*") || this->isCombinator())
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
                this->skipInvalidToken();
            }
            break;
        }
            //if it is a comment
        case HSSBlockComment:
        case HSSLineComment:
        {
            QSharedPointer<HSSCommentNode> theComment = HSSCommentNode::createComment(d->currentToken->getString(), d->controller);
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
            AXRError("HSSCodeParser", "Unknown token type", d->file->sourceUrl(), d->currentToken->line, d->currentToken->column).raise();
            if (!this->atEndOfSource())
            {
                this->skip(d->currentToken->getType());
            }
            return false;
        }
    }

    return ret;
}

void HSSCodeParser::parseIdentifierStatement()
{
    bool statementDone = false;
    if (d->currentToken->getString() == "var")
    {
        QSharedPointer<HSSVarDeclaration> varDecl = this->readVarDecl();

        if (varDecl)
        {
            d->receiver->receiveParserEvent(HSSParserEventVariableDeclaration, varDecl);
        }
        if (this->atEndOfSource())
            return;
        this->skip(HSSEndOfStatement);
        if (this->atEndOfSource())
            return;
        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            return;
        return;
    }

    QSharedPointer<HSSParserNode> currentNode;
    while (!statementDone)
    {
        statementDone = true;
        if (this->isPropertyPath())
        {
            QSharedPointer<HSSPropertyPath> thePath = this->readPropertyPath(true);
            if (this->isAssignment())
            {
                QSharedPointer<HSSAssignment> asgmt = this->readAssignment(thePath);
                if (asgmt)
                {
                    d->receiver->receiveParserEvent(HSSParserEventAssignment, asgmt);
                }
            }
            else if (this->isComparison())
            {
                QSharedPointer<HSSComparison> comparison = this->readComparison(thePath->front()->evaluate(), thePath);
                if (comparison)
                {
                    currentNode = comparison;
                    statementDone = false;
                }
            }
        }
        else
        {
            if (this->isAssignment())
            {
                QSharedPointer<HSSToken> identifierToken = d->currentToken;
                if (d->notifiesReceiver)
                {
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventVariableName, HSSObjectNameConstant::createConstant(identifierToken->getString(), d->controller));
                }

                this->readNextToken();
                if (atEndOfSource())
                    return;

                QSharedPointer<HSSAssignment> asgmt = this->readAssignment(identifierToken);
                if (asgmt)
                {
                    d->receiver->receiveParserEvent(HSSParserEventAssignment, asgmt);
                }
                this->skip(HSSEndOfStatement);
                if (atEndOfSource())
                    return;
                this->skip(HSSWhitespace);
                if (atEndOfSource())
                    return;
            }
            else if (this->isComparison())
            {
                QSharedPointer<HSSToken> identifierToken = d->currentToken;
                QSharedPointer<HSSObjectNameConstant> objName = HSSObjectNameConstant::createConstant(identifierToken->getString(), d->controller);
                if (d->notifiesReceiver)
                {
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventVariableName, objName);
                }
                this->readNextToken();
                if (atEndOfSource())
                    return;

                QSharedPointer<HSSComparison> comparison = this->readComparison("", objName);
                if (comparison)
                {
                    currentNode = comparison;
                    statementDone = false;
                }
            }
            else
            {
                QSharedPointer<HSSRule> theRule = this->readRule();
                if (theRule)
                {
                    d->receiver->receiveParserEvent(HSSParserEventRule, theRule);
                }
            }
        }

    }
}

QSharedPointer<HSSVarDeclaration> HSSCodeParser::readVarDecl()
{
    QSharedPointer<HSSVarDeclaration> errorState;

    //skip the "var"
    if (d->currentToken->getString() != "var")
        return errorState;

    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventKeywordConstant, HSSKeywordConstant::keyword("var", d->controller));
    }

    this->readNextToken();

    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    //read the variable name
    if (!d->currentToken->isA(HSSIdentifier))
        return errorState;

    AXRString name = d->currentToken->getString();

    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventVariableName, HSSObjectNameConstant::createConstant(name, d->controller));
    }

    QSharedPointer<HSSVarDeclaration> ret(new HSSVarDeclaration(name, d->controller));

    QSharedPointer<HSSToken> theIdentifier = d->currentToken;

    this->readNextToken();

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    if (d->currentToken->isA(HSSEndOfStatement))
    {
        return ret;
    }
    else
    {
        QSharedPointer<HSSAssignment> asgmt = this->readAssignment(theIdentifier);
        if (!asgmt)
            return errorState;

        ret->setAssignment(asgmt);
    }
    this->skip(HSSWhitespace);

    return ret;
}

bool HSSCodeParser::isBoolean()
{
    if (d->currentToken->isA(HSSIdentifier))
    {
        if (d->currentToken->getString() == "true" || d->currentToken->getString() == "false")
        {
            return true;
        }
    }
    return false;
}

bool HSSCodeParser::isAssignment()
{
    QSharedPointer<HSSToken> peekToken = d->currentToken;

    if (peekToken->isA(HSSIdentifier))
    {
        peekToken = d->tokenizer->peekNextToken();
    }
    if (peekToken->isA(HSSWhitespace))
    {
        peekToken = d->tokenizer->peekNextToken();
    }
    if (peekToken->isA(HSSEqualSign))
    {
        d->tokenizer->resetPeek();
        return true;
    }
    return false;
}

bool HSSCodeParser::isPropertyPath()
{
    QSharedPointer<HSSToken> peekToken;
    if (d->currentToken->isA(HSSIdentifier))
    {
        peekToken = d->tokenizer->peekNextToken();
        if (peekToken->isA(HSSDot))
        {
            peekToken = d->tokenizer->peekNextToken();
            if (peekToken->isA(HSSIdentifier))
            {
                d->tokenizer->resetPeek();
                return true;
            }
        }
        else if (peekToken->isA(HSSParenthesisOpen))
        {
            d->tokenizer->resetPeek();
            return true;
        }
        d->tokenizer->resetPeek();
        return false;
    }
    return false;
}

QSharedPointer<HSSBooleanConstant> HSSCodeParser::readBoolean()
{
    if (!d->currentToken->isA(HSSIdentifier))
        return QSharedPointer<HSSBooleanConstant>();

    bool boolValue = false;
    if (d->currentToken->getString() == "true")
        boolValue = true;

    QSharedPointer<HSSBooleanConstant> ret(new HSSBooleanConstant(boolValue, d->controller));
    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventBooleanConstant, ret);
    }
    this->readNextToken();
    if (this->atEndOfSource())
        return ret;
    this->skip(HSSWhitespace);

    return ret;
}

QSharedPointer<HSSAssignment> HSSCodeParser::readAssignment(QSharedPointer<HSSToken> identifier)
{
    QSharedPointer<HSSPropertyPath> newPath(new HSSPropertyPath(d->controller));
    AXRString varName = identifier->getString();
    QSharedPointer<HSSPropertyPathNode> ppn(new HSSPropertyPathNode(HSSObjectNameConstant::createConstant(varName, d->controller), d->controller));
    newPath->add(ppn);
    return this->readAssignment(newPath);
}

QSharedPointer<HSSAssignment> HSSCodeParser::readAssignment(QSharedPointer<HSSPropertyPath> thePath)
{
    QSharedPointer<HSSAssignment> errorState;

    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    QSharedPointer<HSSAssignment> ret(new HSSAssignment(d->controller));
    ret->setPropertyPath(thePath);

    if (!d->currentToken->isA(HSSEqualSign))
        return errorState;
    this->skip(HSSEqualSign);
    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    //read in strict mode: no value means syntax error
    bool expressionValid = this->readVals(ret, "expression", true, true);
    if (!expressionValid)
        return errorState;

    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    return ret;
}

const bool HSSCodeParser::isComparisonSign() const
{
    if (d->currentToken->isA(HSSSymbol))
    {
        const char currentTokenChar = *(d->currentToken->getString()).data().c_str();
        switch (currentTokenChar)
        {
            case '&':
            case '|':
            {
                return true;
            }
            default:
                return false;
        }
    }
    return false;
}

//this supposes we've alread read past the identifier / property path and whitespace
bool HSSCodeParser::isComparison()
{
    QSharedPointer<HSSToken> peekToken;
    peekToken = d->currentToken;
    switch (peekToken->getType())
    {
        case HSSNegator:
        {
            peekToken = d->tokenizer->peekNextToken();
            if (peekToken->isA(HSSEqualSign))
            {
                d->tokenizer->resetPeek();
                return true;
            }
            break;
        }
        case HSSEqualComparator:
        case HSSBiggerComparator:
        case HSSSmallerComparator:
        case HSSBiggerOrEqualComparator:
        case HSSSmallerOrEqualComparator:
        {
            d->tokenizer->resetPeek();
            return true;
        }
        default:
            break;
    }
    d->tokenizer->resetPeek();
    return false;
}

inline HSSComparisonType AXRHSSTokenTypeToComparisonType(HSSTokenType tokenType)
{
    switch (tokenType) {
        case HSSEqualComparator: return HSSComparisonTypeEqual;
        case HSSBiggerComparator: return HSSComparisonTypeBigger;
        case HSSSmallerComparator: return HSSComparisonTypeSmaller;
        case HSSBiggerOrEqualComparator: return HSSComparisonTypeBiggerOrEqual;
        case HSSSmallerOrEqualComparator: return HSSComparisonTypeSmallerOrEqual;
        case HSSNegator: return HSSComparisonTypeNotEqual;
        default:
            break;
    }
    return HSSComparisonTypeNone;
}

//this function assumes the comparison sign has already been checked
QSharedPointer<HSSComparison> HSSCodeParser::readComparison(HSSString propertyName, QSharedPointer<HSSParserNode> leftNode)
{
    QSharedPointer<HSSComparison> errorState;

    HSSTokenType tokenType = d->currentToken->getType();

    QSharedPointer<HSSComparison> theComp;

    if (tokenType == HSSNegator)
    {
        this->skip(tokenType);
        this->skip(HSSEqualSign);
    }
    else
    {
        this->skip(tokenType);
    }


    if (this->atEndOfSource())
        return errorState;
    this->skip(HSSWhitespace);
    if (this->atEndOfSource())
        return errorState;

    theComp = QSharedPointer<HSSComparison>(new HSSComparison(AXRHSSTokenTypeToComparisonType(tokenType), d->controller));
    theComp->setLeftNode(leftNode);

    bool rightNodeValid = true;
    QSharedPointer<HSSParserNode> rightNode = this->readSingleVal(propertyName, rightNodeValid);
    if(!rightNodeValid)
        return errorState;

    theComp->setRightNode(rightNode);
    return theComp;
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
    if (selectorChains.empty())
    {
        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventInvalid, HSSInvalidNode::createInvalidNode(d->currentToken->getString(), d->controller));
        }
        this->readNextToken();
        return errorState;
    }

    if (atEndOfSource())
        return errorState;

    if (selectorChains.empty())
    {
        return errorState;
    }

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
    d->currentContext.push_back(HSSParserContextRule);

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
                if (d->currentToken->getString() == "*")
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
                    AXRError("HSSParser", "Unexpected symbol " + d->currentToken->getString() + " while reading rule", d->file->sourceUrl(), d->line, d->column).raise();
                    d->receiver->receiveParserEvent(HSSParserEventInvalid, HSSInvalidNode::createInvalidNode(d->currentToken->getString(), d->controller));
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
    bool hasValue = false;

    //at root context, we don't want to prepend with @this
    if(d->currentContext.back() == HSSParserContextRoot){
        rootContext = true;
    }

    if(!rootContext){
        QSharedPointer<HSSCombinator> beginning_combinator = this->readCombinator();
        if (beginning_combinator)
        {
            QSharedPointer<HSSSimpleSelector> newSs = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(d->controller));
            newSs->setName(QSharedPointer<HSSThisSelector>(new HSSThisSelector(d->controller)));
            ret->add(newSs);
            ret->add(beginning_combinator);
            hasValue = true;
        }
    }

    while (!done)
    {
        QSharedPointer<HSSSimpleSelector> ss = this->readSimpleSelector();
        if (ss)
        {
            ret->add(ss);
            hasValue = true;
        } else {
            break;
        }
        if (this->atEndOfSource())
            break;

        QSharedPointer<HSSCombinator> cc = this->readChildrenCombinatorOrSkip();
        if (cc)
        {
            ret->add(cc);
            hasValue = true;
        }
        if (this->atEndOfSource())
            break;

        QSharedPointer<HSSCombinator> combinator = this->readCombinator();
        if (combinator)
        {
            ret->add(combinator);
            hasValue = true;
        }
        if (this->atEndOfSource())
            break;

        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            break;

        if (d->currentToken->isA(HSSComma))
        {
            retvect.push_back(ret);

            this->skip(HSSComma);
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

    if (ret && hasValue)
    {
        retvect.push_back(ret);
    }

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
            const char currentTokenValue = *(d->currentToken->getString()).data().c_str();
            switch (currentTokenValue)
            {
                case '*':
                {
                    ret = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(d->controller));
                    QSharedPointer<HSSUniversalSelector> uniSel = QSharedPointer<HSSUniversalSelector>(new HSSUniversalSelector(d->controller));
                    ret->setName(uniSel);

                    if (d->notifiesReceiver)
                    {
                        //notify the receiver
                        d->receiver->receiveParserEvent(HSSParserEventUniversalSelector, uniSel);
                    }

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

    if (this->atEndOfSource())
        return ret;

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
                AXRString objtype = d->currentToken->getString();
                if (objtype == "this")
                {
                    if (d->notifiesReceiver)
                    {
                        //notify the receiver
                        d->receiver->receiveParserEvent(HSSParserEventObjectType, HSSObjectNameConstant::createConstant(objtype, d->controller));
                    }

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
                    if (d->notifiesReceiver)
                    {
                        //notify the receiver
                        d->receiver->receiveParserEvent(HSSParserEventObjectType, HSSObjectNameConstant::createConstant(objtype, d->controller));
                    }

                    ret = QSharedPointer<HSSParentSelector>(new HSSParentSelector(d->controller));
                    this->readNextToken();
                    break;
                }
                else if (objtype == "root")
                {
                    if (d->notifiesReceiver)
                    {
                        //notify the receiver
                        d->receiver->receiveParserEvent(HSSParserEventObjectType, HSSObjectNameConstant::createConstant(objtype, d->controller));
                    }

                    ret = QSharedPointer<HSSRootSelector>(new HSSRootSelector(d->controller));
                    this->readNextToken();
                    break;
                }
                else if (objtype == "event")
                {
                    if (d->notifiesReceiver)
                    {
                        //notify the receiver
                        d->receiver->receiveParserEvent(HSSParserEventObjectType, HSSObjectNameConstant::createConstant(objtype, d->controller));
                    }

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
        {
            AXRError("HSSCodeParser", "Unknown token type while reading object selector", d->file->sourceUrl(), d->currentToken->line, d->currentToken->column).raise();
            break;
        }
    }
    return ret;
}

bool HSSCodeParser::isNegator()
{
    if (d->currentToken->isA(HSSNegator))
    {
        this->skip(HSSNegator);
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

            AXRString flagName = d->currentToken->getString();
            QSharedPointer<HSSFlag> theFlag = QSharedPointer<HSSFlag>(new HSSFlag(d->controller));
            theFlag->setName(flagName);
            d->receiver->receiveParserEvent(HSSParserEventFlagName, HSSObjectNameConstant::createConstant(flagName, d->controller));
            this->readNextToken();
            return theFlag;
        }
        else if (d->currentToken->isA(HSSIdentifier))
        {
            AXRString filterName = d->currentToken->getString();
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
            const char currentTokenValue = *(d->currentToken->getString()).data().c_str();
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
    //all combinators are symbols
    switch (token->getType())
    {
        case HSSSymbol:
        {
            const char currentTokenChar = *(token.data())->getString().data().c_str();
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
                        AXRString objtype = peekToken->getString();
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
                                    AXRString objtype = peekToken->getString();
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
    const char currentTokenChar = *(d->currentToken->getString()).data().c_str();
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
    AXRString theValue = d->currentToken->getString();
    QSharedPointer<HSSNameSelector> ret = QSharedPointer<HSSNameSelector>(new HSSNameSelector(theValue, d->controller));
    ret->setNegating(isNegating);


    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventNameSelector, ret);
    }

    this->readNextToken();

    return ret;
}

//this assumes currentToken is an object sign or an ampersand
///@todo propertyName here doesn't deal at all with dot notation
QSharedPointer<HSSObjectDefinition> HSSCodeParser::readObjectDefinition(AXRString propertyName)
{
    QSharedPointer<HSSObjectDefinition> errorState;
    AXRString objtype;
    QSharedPointer<HSSObjectDefinition> ret;

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
                objtype = "value";
            }
        }
        else if (d->currentToken->isA(HSSIdentifier))
        {
            //alright, we've got a type, look it up
            objtype = d->currentToken->getString();
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

        ret = QSharedPointer<HSSObjectDefinition>(new HSSObjectDefinition(objtype, d->controller));
        d->_lastObjectType = objtype;

    }
    else
    {
        this->skip(HSSAmpersand);
        if (this->atEndOfSource())
            return errorState;
        objtype = d->_lastObjectType;

        if (d->currentToken->isA(HSSObjectSign))
        {
            AXRWarning("HSSCodeParser", "Using &@ is obsolete, you shouldn't do that anymore", d->file->sourceUrl(), d->line, d->column).raise();
            this->readNextToken();
            if (this->atEndOfSource())
                return errorState;
        }
        ret = QSharedPointer<HSSObjectDefinition>(new HSSObjectDefinition(objtype, d->controller));
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
            AXRString name = d->currentToken->getString();
            ret->setName(name);

            if (d->notifiesReceiver)
            {
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParserEventObjectName, HSSObjectNameConstant::createConstant(name, d->controller));
            }
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
            return ret;
        }

        default:
        {
            //Unexpected token while reading object definition
            AXRError("HSSCodeParser", "Unexpected token while reading object definition", d->file->sourceUrl(), d->currentToken->line, d->currentToken->column).raise();
            return errorState;
        }
    }

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
    d->currentContext.push_back(HSSParserContextObjectDefinition);

    //read the inner part of the block
    QSharedPointer<HSSObject> theObj = ret->getObject();
    d->currentObjectContext.push(theObj);

    while (!d->currentToken->isA(HSSBlockClose))
    {
        switch (d->currentToken->getType())
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
                d->_lastObjectType = objtype;
                QSharedPointer<HSSObjectDefinition> childDef = this->readObjectDefinition(propertyName);
                if (childDef)
                {
                    childDef->setParentNode(ret);
                    ret->childrenAdd(childDef);
                }
                else
                {
                    return ret;
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
                        ret->propertiesAdd(propertyDefinition);
                    }
                    else
                    {
                        AXRWarning("HSSCodeParser", "Ignoring malformed property definition", d->file->sourceUrl(), d->currentToken->line, d->currentToken->column).raise();
                        //allow the object definition to be alive with malformed properties
                        if (this->atEndOfSource())
                            return errorState;

                        if (d->notifiesReceiver)
                        {
                            //notify the receiver
                            d->receiver->receiveParserEvent(HSSParserEventInvalid, HSSInvalidNode::createInvalidNode(d->currentToken->getString(), d->controller));
                        }
                        this->readNextToken();
                        continue;
                    }
                }
                else
                {
                    QSharedPointer<HSSRule> theRule = this->readRule();
                    if (theRule)
                    {
                        ret->rulesAdd(theRule);
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
                        ret->propertiesAdd(propertyDefinition);
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
                        ret->rulesAdd(childRule);
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
                        ret->propertiesAdd(propertyDefinition);
                    }
                    else
                    {
                        return errorState;
                    }
                }
                else
                {
                    //Unexpected token
                    AXRWarning("HSSCodeParser", "Ignoring unexpected token while reading object definition", d->file->sourceUrl(), d->currentToken->line, d->currentToken->column).raise();
                    this->skip(d->currentToken->getType());
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
            AXRWarning("HSSCodeParser", "Auto closing block of object definition with name " + ret->name() + " because of unexpected end of file", d->file->sourceUrl(), d->line, d->column).raise();
            //leave the block context
            d->currentContext.pop_back();
            return errorState;
        }
    }

    //reset the index of the shorthand properties
    theObj->shorthandReset();
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

    return ret;
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

QSharedPointer<HSSRefFunction> HSSCodeParser::readObjectPath()
{
    QSharedPointer<HSSRefFunction> errorState;
    QSharedPointer<HSSRefFunction> refFunction = QSharedPointer<HSSRefFunction>(new HSSRefFunction(d->controller));

    //only names allowed are this, parent, root, event
    QSharedPointer<HSSNameSelector> nameSel = this->readObjectSelector();
    if (!nameSel)
        return errorState;

    QSharedPointer<HSSSelectorChain> selectorChain;
    selectorChain = QSharedPointer<HSSSelectorChain>(new HSSSelectorChain(d->controller));
    QSharedPointer<HSSSimpleSelector> newSs = QSharedPointer<HSSSimpleSelector>(new HSSSimpleSelector(d->controller));
    newSs->setName(nameSel);
    selectorChain->add(newSs);
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
    selectorChains.push_back(selectorChain);
    refFunction->setSelectorChains(selectorChains);

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    this->skip(HSSDot);
    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    refFunction->setPropertyPath(this->readPropertyPath());
    return refFunction;
}

bool HSSCodeParser::isObjectPath()
{
    QSharedPointer<HSSToken> peekToken;
    if (d->currentToken->isA(HSSObjectSign))
    {
        peekToken = d->tokenizer->peekNextToken();
        if (peekToken->isA(HSSIdentifier))
        {
            AXRString objtype = peekToken->getString();
            if (
                objtype == "this"
                || objtype == "super"
                || objtype == "parent"
                || objtype == "root"
                || objtype == "event"
                )
            {
                d->tokenizer->resetPeek();
                return true;
            }
        }
        d->tokenizer->resetPeek();
        return false;
    }
    return false;
}

QSharedPointer<HSSPropertyDefinition> HSSCodeParser::readPropertyDefinition()
{
    return this->readPropertyDefinition(false, false);
}

QSharedPointer<HSSPropertyDefinition> HSSCodeParser::readPropertyDefinition(bool shorthandChecked, bool isShorthand)
{

    std::vector<QSharedPointer<HSSPropertyPath> > propertyPaths;

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
                        AXRString propertyName = d->currentToken->getString();
                        QSharedPointer<HSSPropertyPath> path(new HSSPropertyPath(d->controller));
                        path->add(propertyName);

                        if (d->notifiesReceiver)
                        {
                            //notify the receiver
                            d->receiver->receiveParserEvent(HSSParserEventPropertyPath, QSharedPointer<HSSPropertyPathNode>(new HSSPropertyPathNode(HSSPropertyNameConstant::createConstant(propertyName, d->controller), d->controller)));
                        }

                        this->readNextToken();
                        if (this->atEndOfSource())
                            return errorState;

                        //allow whitespace before colon
                        this->skip(HSSWhitespace);
                        if (this->atEndOfSource())
                            return errorState;

                        if (d->currentToken->isA(HSSDot))
                        {
                            if (d->notifiesReceiver)
                            {
                                //notify the receiver
                                d->receiver->receiveParserEvent(HSSParserEventPropertyPath, HSSSymbolNode::createSymbol(".", d->controller));
                            }
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
                                AXRString prop = d->currentToken->getString();
                                path->add(prop);

                                if (d->notifiesReceiver)
                                {
                                    //notify the receiver
                                    d->receiver->receiveParserEvent(HSSParserEventPropertyPath, QSharedPointer<HSSPropertyPathNode>(new HSSPropertyPathNode(HSSPropertyNameConstant::createConstant(prop, d->controller), d->controller)));
                                }
                                this->readNextToken();
                                if (this->atEndOfSource())
                                    return errorState;

                                if (d->currentToken->isA(HSSDot))
                                {
                                    done2 = false;
                                    if (d->notifiesReceiver)
                                    {
                                        //notify the receiver
                                        d->receiver->receiveParserEvent(HSSParserEventPropertyPath, HSSSymbolNode::createSymbol(".", d->controller));
                                    }
                                    this->readNextToken();
                                    if (this->atEndOfSource())
                                        return errorState;
                                }
                            }
                        }
                        propertyPaths.push_back(path);
                        if (d->currentToken->isA(HSSComma))
                        {
                            if (d->notifiesReceiver)
                            {
                                //notify the receiver
                                d->receiver->receiveParserEvent(HSSParserEventOther, HSSSymbolNode::createSymbol(",", d->controller));
                            }

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
            bool hasNext = d->currentObjectContext.top()->shorthandNext();
            //if it doesn't have next shorthand, it is invalid
            if (!hasNext)
                return errorState;
            break;
        }
    }

    QSharedPointer<HSSPropertyDefinition> newPropeDef = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(propertyPaths, d->controller));

    //do not allow empty property definitions
    if (d->currentToken->isA(HSSEndOfStatement) || d->currentToken->isA(HSSBlockClose))
        return errorState;

    bool valuesValid = this->readVals(newPropeDef, propertyPaths.front()->front()->evaluate());

    if (!valuesValid) valid = false;

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

bool HSSCodeParser::readVals(const QSharedPointer<AXR::HSSParserNode> &target, AXRString propertyName)
{
    return this->readVals(target, propertyName, true, false);
}

bool HSSCodeParser::readVals(const QSharedPointer<AXR::HSSParserNode> &target, AXRString propertyName, bool useComma, bool strict)
{
    bool done = false;
    bool valid = true;

    QSharedPointer<HSSParserNode> localNode;

    localNode = this->readSingleVal(propertyName, valid);

    if (strict && !localNode)
        valid = false;

    while (!done && localNode && valid)
    {
        done = true;

        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            return false;

        if (useComma && d->currentToken->isA(HSSComma))
        {
            done = false;
            this->skip(HSSComma);

            this->skip(HSSWhitespace);
            if (this->atEndOfSource())
                return false;

            target->addValue(localNode);
            localNode = this->readVal(propertyName, valid);
            continue;
        }
        else if (!useComma && d->currentToken->isA(HSSEndOfStatement))
        {
            this->skip(HSSEndOfStatement);

            this->skip(HSSWhitespace);
            if (this->atEndOfSource())
                return false;

            target->addValue(localNode);
            localNode = this->readSingleVal(propertyName, valid);
            done = false;
            continue;
        }
        if (localNode)
            target->addValue(localNode);
    }
    return valid;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readSingleVal(AXRString propertyName, bool &valid)
{
    QSharedPointer<HSSParserNode> errorState;

    bool done = false;

    QSharedPointer<HSSParserNode> localNode;

    bool isNegation = false;
    if (d->currentToken->isA(HSSNegator))
    {
        isNegation = true;
        this->skip(HSSNegator);
        if (this->atEndOfSource())
            return errorState;
        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            return errorState;
    }
    if (d->currentToken->isA(HSSParenthesisOpen))
    {
        this->skip(HSSParenthesisOpen);
        if (this->atEndOfSource())
            return errorState;
        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            return errorState;
        QSharedPointer<HSSParserNode> leftNode = this->readSingleVal(propertyName, valid);
        if (this->atEndOfSource())
            return errorState;
        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            return errorState;
        this->skip(HSSParenthesisClose);
        if (this->atEndOfSource())
            return errorState;
        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            return errorState;
    }
    else
    {
        localNode = this->readVal(propertyName, valid);
    }

    if (!localNode || !d->currentToken)
        valid = false;

    while (!done && localNode && valid)
    {
        done = true;

        if (this->isExpressionSign())
        {
            //the following types are incompatible with expressions
            if (
                localNode->isA(HSSStatementTypeObjectDefinition)
                || localNode->isA(HSSFunctionTypeFunction)
                )
            {
                AXRError("HSSParser", HSSString::format("Missing semicolon on line %d and column %d", d->currentToken->line, d->currentToken->column)).raise();
                return errorState;
            }
            localNode = this->readExpression(propertyName, localNode);
        }
        else if (this->isComparison())
        {
            //the following types are incompatible with expressions
            if (
                localNode->isA(HSSStatementTypeObjectDefinition)
                || (localNode->isA(HSSParserNodeTypeFunction) && localNode->isA(HSSFunctionTypeFunction))
                )
            {
                AXRError("HSSParser", HSSString::format("Missing semicolon on line %d and column %d", d->currentToken->line, d->currentToken->column)).raise();
                return errorState;
            }
            localNode = this->readComparison(propertyName, localNode);
        }
    }
    if (isNegation)
    {
        QSharedPointer<HSSNegatedVal> theNeg = QSharedPointer<HSSNegatedVal>(new HSSNegatedVal(d->controller));
        theNeg->setValue(localNode);
        return theNeg;
    }
    return localNode;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readVal(AXRString propertyName, bool &valid)
{
    QSharedPointer<HSSParserNode> errorState;

    switch (d->currentToken->getType())
    {
        case HSSNumber:
        {
            QSharedPointer<HSSNumberConstant> numberConstant(new HSSNumberConstant(d->currentToken->getNumber(), d->controller));
            numberConstant->setOriginalStringRep(d->currentToken->getString());
            QSharedPointer<HSSParserNode> ret = numberConstant;

            if (d->notifiesReceiver)
            {
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParserEventNumberConstant, ret);
            }

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
            QSharedPointer<HSSPercentageConstant> percentageConstant(new HSSPercentageConstant(d->currentToken->getNumber(), d->controller));
            percentageConstant->setOriginalStringRep(d->currentToken->getString());
            QSharedPointer<HSSParserNode> ret = percentageConstant;

            if (d->notifiesReceiver)
            {
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParserEventPercentageConstant, ret);
            }

            this->readNextToken();
            if (atEndOfSource())
                return errorState;

            this->skip(HSSWhitespace);
            if (atEndOfSource())
                return errorState;

            return ret;
        }

        case HSSIdentifier:
        {
            //this is either a function, a keyword or an object name
            AXRString valuestr = d->currentToken->getString();
            //check if it is a function
            QSharedPointer<HSSObject> objectContext = d->currentObjectContext.top();

            if(this->isBoolean()){
                return this->readBoolean();
            }
            else if (objectContext->isFunction(valuestr, propertyName)) ///@todo: what to do if the property names give different functions?
            {
                return this->readFunction();
            }
            else if (objectContext->isKeyword(valuestr, propertyName)) ///@todo: what to do if the property names give different keywords?
            {
                QSharedPointer<HSSKeywordConstant> kwConstant = QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(valuestr, d->controller));
                if (d->notifiesReceiver)
                {
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventKeywordConstant, kwConstant);
                }

                this->readNextToken();
                if (this->atEndOfSource())
                    return errorState;

                return kwConstant;
            }
            else
            {
                return this->readPropertyPath(true);
            }
        }
        case HSSSingleQuoteString:
        case HSSDoubleQuoteString:
        {
            QSharedPointer<HSSStringToken> strToken = qSharedPointerCast<HSSStringToken>(d->currentToken);
            AXRString theString;
            QSharedPointer<HSSStringConstant> ret;

            if (strToken->hasArguments())
            {
                if (d->notifiesReceiver)
                {
                    QSharedPointer<HSSStringConstant> firstChunk = HSSStringConstant::stringToConstant(strToken->getString(), d->controller);
                    firstChunk->setHasEndQuote(false);
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventStringConstant, firstChunk);
                }
                this->readNextToken();
                if (this->atEndOfSource())
                    return errorState;

                QSharedPointer<HSSParserNode> arg = this->readStringArgument();
                strToken->addArgument(arg);

                if (d->currentToken->isA(HSSBlockClose) && d->notifiesReceiver)
                {
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventOther, HSSSymbolNode::createSymbol("}", d->controller));
                    if (this->atEndOfSource())
                        return errorState;
                }

                bool stringDone = false;
                QSharedPointer<HSSStringToken> strChunk;
                while(!stringDone){
                    //readString() will set stringDone to false, and re-set it to true if needed
                    strChunk = d->tokenizer->readString(strToken, stringDone);

                    this->readNextToken();
                    if (this->atEndOfSource())
                        return errorState;

                    if (!stringDone)
                    {
                        //notify the mid chunk of the string
                        if (d->notifiesReceiver)
                        {
                            QSharedPointer<HSSStringConstant> midChunk = HSSStringConstant::stringToConstant(strChunk->getString(), d->controller);
                            midChunk->setHasStartQuote(false);
                            midChunk->setHasEndQuote(false);
                            //notify the receiver
                            d->receiver->receiveParserEvent(HSSParserEventStringConstant, midChunk);
                        }

                        QSharedPointer<HSSParserNode> arg = this->readStringArgument();
                        strToken->addArgument(arg);

                        if (d->currentToken->isA(HSSBlockClose) && d->notifiesReceiver)
                        {
                            //notify the receiver
                            d->receiver->receiveParserEvent(HSSParserEventOther, HSSSymbolNode::createSymbol("}", d->controller));
                        }
                        if (this->atEndOfSource())
                            return errorState;
                    }
                }
                //notify the end chunk of the string
                if (d->notifiesReceiver)
                {
                    QSharedPointer<HSSStringConstant> endChunk = HSSStringConstant::stringToConstant(strChunk->getString(), d->controller);
                    endChunk->setHasStartQuote(false);
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventStringConstant, endChunk);
                }

                //strToken now contains the full string
                ret = QSharedPointer<HSSStringConstant>(new HSSStringConstant(strToken->getString(), d->controller));
                ret->setArguments(strToken->getArguments());
                ret->setIndexes(strToken->getIndexes());

            }
            else
            {
                theString = strToken->getString();
                ret = QSharedPointer<HSSStringConstant>(new HSSStringConstant(theString, d->controller));
                if (d->notifiesReceiver)
                {
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventStringConstant, ret);
                }
                this->readNextToken();
                if (this->atEndOfSource())
                    return errorState;
            }

            this->skip(HSSWhitespace);
            if (this->atEndOfSource())
                return errorState;

            return ret;
        }
        case HSSSymbol:
        {
            if (this->isUnaryExpression())
            {
                return this->readUnaryExpression();
            }
            else
            {
                HSSString symbStr = d->currentToken->getString();
                if (symbStr != "+" && symbStr != "-")
                {
                    AXRError("HSSParser", "Unexpected symbol", d->file->sourceUrl(), d->currentToken->line, d->currentToken->column).raise();
                    return errorState;
                }
                this->readNextToken();
                if (atEndOfSource())
                    return errorState;

                if (!d->currentToken->isA(HSSNumber) && !d->currentToken->isA(HSSPercentageNumber))
                {
                    AXRError("HSSParser", "Unexpected token", d->file->sourceUrl(), d->currentToken->line, d->currentToken->column).raise();
                    return errorState;
                }
                HSSUnit numValue = d->currentToken->getNumber();
                if (symbStr == "-")
                {
                    numValue *= -1;
                }
                if (d->currentToken->isA(HSSNumber))
                {
                    QSharedPointer<HSSNumberConstant> numberConstant(new HSSNumberConstant(numValue, d->controller));
                    numberConstant->setOriginalStringRep(symbStr + d->currentToken->getString());
                    QSharedPointer<HSSParserNode> ret = numberConstant;

                    if (d->notifiesReceiver)
                    {
                        //notify the receiver
                        d->receiver->receiveParserEvent(HSSParserEventNumberConstant, ret);
                    }

                    this->readNextToken();
                    if (atEndOfSource())
                        return errorState;

                    this->skip(HSSWhitespace);
                    if (atEndOfSource())
                        return errorState;

                    return ret;
                }
                else
                {
                    QSharedPointer<HSSPercentageConstant> percentageConstant(new HSSPercentageConstant(numValue, d->controller));
                    percentageConstant->setOriginalStringRep(symbStr + d->currentToken->getString());
                    QSharedPointer<HSSParserNode> ret = percentageConstant;

                    if (d->notifiesReceiver)
                    {
                        //notify the receiver
                        d->receiver->receiveParserEvent(HSSParserEventPercentageConstant, ret);
                    }

                    this->readNextToken();
                    if (atEndOfSource())
                        return errorState;

                    this->skip(HSSWhitespace);
                    if (atEndOfSource())
                        return errorState;

                    return ret;
                }
            }

        }
        case HSSInstructionSign:
        {
            QSharedPointer<HSSInstruction> theInstruction = this->readInstruction();
            if (!theInstruction)
                return errorState;
            return this->getObjectFromInstruction(theInstruction);
        }
        case HSSObjectSign:
        {
            if (this->isObjectPath())
            {
                QSharedPointer<HSSRefFunction> refFunc = this->readObjectPath();
                if (refFunc)
                {
                    if (d->currentToken->isA(HSSParenthesisOpen))
                    {
                        QSharedPointer<HSSFunctionCall> funcCall = this->readFunctionCall();
                        funcCall->setSubject(refFunc);
                        return funcCall;
                    }

                    return refFunc;
                }
                else
                {
                    valid = false;
                }
            }
            else
            {
                return this->readObjectDefinition(propertyName);
            }
            break;
        }
        default:
            break;
    }
    return errorState;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readIdentifierValue()
{
    QSharedPointer<HSSParserNode> errorState;
    AXRString valuestr = d->currentToken->getString();

    QSharedPointer<HSSPropertyPath> ppath;
    if (this->isPropertyPath())
    {
        ppath = this->readPropertyPath(true);
    }
    if (ppath)
    {
        return ppath;
    }
    else
    {
        //we assume it is an object name at this point
        QSharedPointer<HSSObjectNameConstant> objName = QSharedPointer<HSSObjectNameConstant>(new HSSObjectNameConstant(valuestr, d->controller));
        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventObjectName, objName);
        }
        this->readNextToken();
        if (this->atEndOfSource())
            return errorState;
        return objName;
    }
    return errorState;
}

const bool HSSCodeParser::isExpressionSign() const
{
    if (d->currentToken->isA(HSSSymbol))
    {
        const char currentTokenChar = *(d->currentToken->getString().chardata());
        switch (currentTokenChar)
        {
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            {
                return true;
            }
            default:
                return false;
        }
    }
    return false;
}

QSharedPointer<HSSExpression> HSSCodeParser::readExpression(AXRString propertyName, QSharedPointer<AXR::HSSParserNode> leftNode)
{
    QSharedPointer<HSSExpression> errorState;

    if (d->currentToken->isA(HSSSymbol))
    {
        const char sign = *(d->currentToken->getString()).data().c_str();
        this->skip(HSSSymbol);

        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;

        bool rightNodeValid = true;
        QSharedPointer<HSSParserNode> rightNode = this->readVal(propertyName, rightNodeValid);
        if(!rightNodeValid)
            return errorState;

        switch (sign)
        {
            case '+':
            {
                return QSharedPointer<HSSSum>(new HSSSum(leftNode, rightNode, d->controller));
            }
            case '-':
            {
                return QSharedPointer<HSSSubtraction>(new HSSSubtraction(leftNode, rightNode, d->controller));
            }
            case '*':
            {
                return QSharedPointer<HSSMultiplication>(new HSSMultiplication(leftNode, rightNode, d->controller));
            }
            case '/':
            {
                return QSharedPointer<HSSDivision>(new HSSDivision(leftNode, rightNode, d->controller));
            }
            case '%':
            {
                //todo
            }
            default:
                break;
        }
    }
    else
    {
        return errorState;
    }
    return errorState;
}

QSharedPointer<HSSPropertyPath> HSSCodeParser::readPropertyPath()
{
    return this->readPropertyPath(false);
}

QSharedPointer<HSSPropertyPath> HSSCodeParser::readPropertyPath(bool firstNodeIsVariableName)
{
    QSharedPointer<HSSPropertyPath> errorState;

    QSharedPointer<HSSPropertyPath> ppath(new HSSPropertyPath(d->controller));

    bool isFirst = true;

    bool done = false;
    while (!done)
    {
        done = true;
        if (d->currentToken->isA(HSSIdentifier))
        {
            QSharedPointer<HSSPropertyPathNode> ppn(new HSSPropertyPathNode(HSSPropertyNameConstant::createConstant(d->currentToken->getString(), d->controller), d->controller));
            ppath->add(ppn);
            if (d->notifiesReceiver)
            {
                if (firstNodeIsVariableName && isFirst)
                {
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventVariableName, HSSObjectNameConstant::createConstant(d->currentToken->getString(), d->controller));
                }
                else
                {
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventPropertyPath, ppn);
                }
            }

            this->readNextToken();
            if (this->atEndOfSource())
                return errorState;
        }
        if (this->isFunctionCall())
        {
            QSharedPointer<HSSFunctionCall> fCall = this->readFunctionCall();
            QSharedPointer<HSSPropertyPathNode> ppn(new HSSPropertyPathNode(fCall, d->controller));
            ppn->setHasName(false);
            ppath->add(ppn);
        }
        if (d->currentToken->isA(HSSDot))
        {
            done = false;
            this->skip(HSSDot);
            if (this->atEndOfSource())
                return errorState;
        }

        isFirst = false;
    }

    if (ppath->size() == 0)
        return errorState;

    if (atEndOfSource())
        return ppath;

    this->skip(HSSWhitespace);

    return ppath;
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
        currentval = d->currentToken->getString();
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

        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventColor, ret);
        }

        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            return errorState;
        return ret;
    }
    else if (d->currentToken->isA(HSSIdentifier))
    {
        QSharedPointer<HSSInstruction> ret;
        currentval = d->currentToken->getString();
        if (currentval == "new")
        {
            ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSNewInstruction, currentval, d->controller));

            if (d->notifiesReceiver)
            {
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParserEventInstruction, ret);
            }

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
                    HSSUnit number = d->currentToken->getNumber();
                    QSharedPointer<HSSNumberConstant> numberConstant(new HSSNumberConstant(number, d->controller));
                    numberConstant->setOriginalStringRep(d->currentToken->getString());
                    ret->setArgument(numberConstant);
                    if (d->notifiesReceiver)
                    {
                        //notify the receiver
                        d->receiver->receiveParserEvent(HSSParserEventOther, numberConstant);
                    }
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
                AXRString theString = d->currentToken->getString();
                ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSImportInstruction, theString, d->controller));
            }
            else if (d->currentToken->isA(HSSIdentifier))
            {
                AXRString instructionKw = d->currentToken->getString();
                if (instructionKw == "UIFramework")
                {
                    AXRString url(HSSFRAMEWORK_PROTOCOL);
                    url += "://framework/UIFramework.hss";
                    ret = QSharedPointer<HSSInstruction>(new HSSInstruction(HSSImportInstruction, url, d->controller));
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
            unsigned int hexValue;
            AXRString tempstr = instruction->getValue();
            if (instructionType == HSSGrayscale1Instruction)
            {
                tempstr = tempstr + tempstr;
            }

            hexValue = tempstr.toHex();

            ret = QSharedPointer<HSSObjectDefinition>(new HSSObjectDefinition("rgb", d->controller));

            QSharedPointer<HSSPropertyDefinition> newRed = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(d->controller));
            QSharedPointer<HSSPropertyPath> redPath(new HSSPropertyPath(d->controller));
            redPath->add("red");
            std::vector<QSharedPointer<HSSPropertyPath> > redPaths;
            redPaths.push_back(redPath);
            newRed->setPaths(redPaths);
            newRed->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(hexValue, d->controller)));
            ret->propertiesAdd(newRed);

            QSharedPointer<HSSPropertyDefinition> newGreen = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(d->controller));
            QSharedPointer<HSSPropertyPath> greenPath(new HSSPropertyPath(d->controller));
            greenPath->add("green");
            std::vector<QSharedPointer<HSSPropertyPath> > greenPaths;
            greenPaths.push_back(greenPath);
            newGreen->setPaths(greenPaths);
            newGreen->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(hexValue, d->controller)));
            ret->propertiesAdd(newGreen);

            QSharedPointer<HSSPropertyDefinition> newBlue = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(d->controller));
            QSharedPointer<HSSPropertyPath> bluePath(new HSSPropertyPath(d->controller));
            bluePath->add("blue");
            std::vector<QSharedPointer<HSSPropertyPath> > bluePaths;
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
                    red = instruction->getValue().substr(0, 1);
                    red += red;
                    green = instruction->getValue().substr(1, 1);
                    green += green;
                    blue = instruction->getValue().substr(2, 1);
                    blue += blue;
                    alpha = "FF";
                    break;
                case HSSRGBAInstruction:
                    red = instruction->getValue().substr(0, 1);
                    red += red;
                    green = instruction->getValue().substr(1, 1);
                    green += green;
                    blue = instruction->getValue().substr(2, 1);
                    blue += blue;
                    alpha = instruction->getValue().substr(3, 1);
                    alpha += alpha;
                    break;
                case HSSRGBAAInstruction:
                    red = instruction->getValue().substr(0, 1);
                    red += red;
                    green = instruction->getValue().substr(1, 1);
                    green += green;
                    blue = instruction->getValue().substr(2, 1);
                    blue += blue;
                    alpha = instruction->getValue().substr(3, 2);
                    break;
                case HSSRRGGBBInstruction:
                    red = instruction->getValue().substr(0, 2);
                    green = instruction->getValue().substr(2, 2);
                    blue = instruction->getValue().substr(4, 2);
                    alpha = "FF";
                    break;
                case HSSRRGGBBAInstruction:
                    red = instruction->getValue().substr(0, 2);
                    green = instruction->getValue().substr(2, 2);
                    blue = instruction->getValue().substr(4, 2);
                    alpha = instruction->getValue().substr(6, 1);
                    alpha += alpha;
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

            redHex = red.toHex();
            greenHex = green.toHex();
            blueHex = blue.toHex();
            alphaHex = alpha.toHex();

            ret = QSharedPointer<HSSObjectDefinition>(new HSSObjectDefinition("rgb", d->controller));

            QSharedPointer<HSSPropertyDefinition> newRed = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(d->controller));
            QSharedPointer<HSSPropertyPath> redPath(new HSSPropertyPath(d->controller));
            redPath->add("red");
            std::vector<QSharedPointer<HSSPropertyPath> > redPaths;
            redPaths.push_back(redPath);
            newRed->setPaths(redPaths);
            newRed->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(redHex, d->controller)));
            ret->propertiesAdd(newRed);

            QSharedPointer<HSSPropertyDefinition> newGreen = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(d->controller));
            QSharedPointer<HSSPropertyPath> greenPath(new HSSPropertyPath(d->controller));
            greenPath->add("green");
            std::vector<QSharedPointer<HSSPropertyPath> > greenPaths;
            greenPaths.push_back(greenPath);
            newGreen->setPaths(greenPaths);
            newGreen->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(greenHex, d->controller)));
            ret->propertiesAdd(newGreen);

            QSharedPointer<HSSPropertyDefinition> newBlue = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(d->controller));
            QSharedPointer<HSSPropertyPath> bluePath(new HSSPropertyPath(d->controller));
            bluePath->add("blue");
            std::vector<QSharedPointer<HSSPropertyPath> > bluePaths;
            bluePaths.push_back(bluePath);
            newBlue->setPaths(bluePaths);
            newBlue->setValue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(blueHex, d->controller)));
            ret->propertiesAdd(newBlue);

            QSharedPointer<HSSPropertyDefinition> newAlpha = QSharedPointer<HSSPropertyDefinition>(new HSSPropertyDefinition(d->controller));
            QSharedPointer<HSSPropertyPath> alphaPath(new HSSPropertyPath(d->controller));
            alphaPath->add("alpha");
            std::vector<QSharedPointer<HSSPropertyPath> > alphaPaths;
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
        const char currentTokenChar = *(d->currentToken->getString()).data().c_str();
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

        const char currentTokenChar = *(d->currentToken->getString()).data().c_str();
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
            QSharedPointer<HSSNumberConstant> numberConstant(new HSSNumberConstant(d->currentToken->getNumber(), d->controller));
            numberConstant->setOriginalStringRep(d->currentToken->getString());
            QSharedPointer<HSSParserNode> ret = numberConstant;

            if (d->notifiesReceiver)
            {
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParserEventNumberConstant, ret);
            }

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
            QSharedPointer<HSSPercentageConstant> percentageConstant(new HSSPercentageConstant(d->currentToken->getNumber(), d->controller));
            percentageConstant->setOriginalStringRep(d->currentToken->getString());
            QSharedPointer<HSSParserNode> ret = percentageConstant;

            if (d->notifiesReceiver)
            {
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParserEventPercentageConstant, ret);
            }

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
            if (d->notifiesReceiver)
            {
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParserEventOther, HSSSymbolNode::createSymbol("(", d->controller));
            }

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
        case HSSSymbol:
        {
            return this->readUnaryExpression();
        }

        default:
            return errorState;
    }
    return errorState;
}

bool HSSCodeParser::isUnaryExpression()
{
    QSharedPointer<HSSToken> peekToken = d->tokenizer->peekNextToken();
    if (peekToken && peekToken->isA(HSSWhitespace))
    {
        peekToken = d->tokenizer->peekNextToken();
    }
    if (peekToken && peekToken->isA(HSSParenthesisOpen))
    {
        d->tokenizer->resetPeek();
        return true;
    }
    d->tokenizer->resetPeek();
    return false;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readUnaryExpression()
{
    QSharedPointer<HSSParserNode> errorState;

    QSharedPointer<HSSUnaryExpression> uExp;
    HSSString sign = d->currentToken->getString();
    if (sign == "+")
    {
        uExp = QSharedPointer<HSSUnarySum>(new HSSUnarySum(d->controller));
    }
    else if (sign == "-")
    {
        uExp = QSharedPointer<HSSUnarySubtraction>(new HSSUnarySubtraction(d->controller));
    }
    else if (sign == "*")
    {
        AXRError("HSSCodeParser", "Unary multiplication has not been implemented yet.").raise();
//        uExp = QSharedPointer<HSSUnaryMultiplication>(new HSSUnaryMultiplication(d->controller));
    }
    else if (sign == "/")
    {
        AXRError("HSSCodeParser", "Unary division has not been implemented yet.").raise();
//        uExp = QSharedPointer<HSSUnaryDivision>(new HSSUnaryDivision(d->controller));
    }
    this->skip(HSSSymbol);

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

    bool valueValid = this->readVals(uExp, "value");
    if (!valueValid)
        return errorState;

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    this->skip(HSSParenthesisClose);
    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    return uExp;
}

//this method expects the currentToken to be an identifier
QSharedPointer<HSSParserNode> HSSCodeParser::readFlag()
{
    QSharedPointer<HSSFlag> ret;

    AXRString flagName = d->currentToken->getString();
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
        AXRString name = d->currentToken->getString();
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
        else if (name == "if")
        {
            return this->readIfFunction();
        }
        else if (name == "else")
        {
            return this->readElseFunction();
        }
        else if (name == "function")
        {
            return this->readUserDefinedFunction();
        }
        else if (name == "return")
        {
            return this->readReturnFunction();
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

    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant("ref", d->controller)));
    }

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
        AXRString firstValue = d->currentToken->getString();
        if (firstValue == "min"
            || firstValue == "max"
            || firstValue == "avg")
        {
            refFunction->setModifier(firstValue);

            if (d->notifiesReceiver)
            {
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParserEventFunctionModifier, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(firstValue, d->controller)));
            }

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
        if (!d->currentToken->isA(HSSIdentifier) || d->currentToken->getString() != "of")
        {
            return errorState;
        }

        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventFunctionModifier, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant("of", d->controller)));
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
        if (atEndOfSource())
            return errorState;
    }

    refFunction->setSelectorChains(selectorChains);
    return refFunction;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readSelFunction()
{
    QSharedPointer<HSSParserNode> errorState;

    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant("sel", d->controller)));
    }

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
    AXRString name = d->currentToken->getString();
    QSharedPointer<HSSFlagFunction> flagFunction = QSharedPointer<HSSFlagFunction>(new HSSFlagFunction(HSSFlagFunction::flagFunctionTypeFromString(name), d->controller));

    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));
    }

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
        AXRString flagName = d->currentToken->getString();
        flagFunction->setName(flagName);

        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventFlagName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(flagName, d->controller)));
        }
    }
    else if (d->currentToken->isA(HSSSymbol) && d->currentToken->getString() == "*")
    {
        flagFunction->setName("*");

        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventOther, HSSSymbolNode::createSymbol("*", d->controller));
        }
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
        if (d->currentToken->getString() != "with")
        {
            AXRError("HSSCodeParser", "Unexpected token while reading flagging function: " + HSSToken::tokenStringRepresentation(d->currentToken->getType())).raise();
            return errorState;
        }

        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventFunctionModifier, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant("with", d->controller)));
        }

        this->readNextToken();
        if (atEndOfSource())
            return errorState;

        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;

        if (d->currentToken->isA(HSSIdentifier))
        {
            AXRString flagName = d->currentToken->getString();
            flagFunction->setName2(flagName);
            if (d->notifiesReceiver)
            {
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParserEventFlagName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(flagName, d->controller)));
            }
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
        if (!d->currentToken->isA(HSSIdentifier) || d->currentToken->getString() != "of")
        {
            AXRError("HSSCodeParser", "Unexpected token while reading flagging function: " + HSSToken::tokenStringRepresentation(d->currentToken->getType())).raise();
            return errorState;
        }

        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventFunctionModifier, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant("of", d->controller)));
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

    flagFunction->setSelectorChains(selectorChains);
    return flagFunction;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readAttrFunction()
{
    QSharedPointer<HSSParserNode> errorState;
    AXRString name = d->currentToken->getString();
    QSharedPointer<HSSAttrFunction> attrFunction = QSharedPointer<HSSAttrFunction>(new HSSAttrFunction(d->controller));

    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));
    }

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
        attrFunction->setAttributeName(d->currentToken->getString());
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
        if (!d->currentToken->isA(HSSIdentifier) || d->currentToken->getString() != "of")
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
    AXRString name = d->currentToken->getString();
    QSharedPointer<HSSLogFunction> logFunction = QSharedPointer<HSSLogFunction>(new HSSLogFunction(d->controller));

    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));
    }

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
        bool valueValid = true;
        QSharedPointer<HSSParserNode> value = this->readSingleVal("value", valueValid);
        if(value && valueValid){
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
    AXRString name = d->currentToken->getString();

    QSharedPointer<HSSOverrideFunction> overrideFunction = QSharedPointer<HSSOverrideFunction>(new HSSOverrideFunction(d->controller));

    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));
    }

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

    //if shorthand notation -- assumes '@this'
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
        selectorChains = this->readSelectorChains(HSSParenthesisClose);

    }
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
    AXRString name = d->currentToken->getString();

    QSharedPointer<HSSStartTimerFunction> startTimerFunction = QSharedPointer<HSSStartTimerFunction>(new HSSStartTimerFunction(d->controller));

    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));
    }

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
        AXRString id = d->currentToken->getString();

        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventObjectName, QSharedPointer<HSSObjectNameConstant>(new HSSObjectNameConstant(id, d->controller)));
        }

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
    bool msValid = true;
    QSharedPointer<HSSParserNode> ms = this->readSingleVal("ms", msValid);
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
    AXRString name = d->currentToken->getString();

    QSharedPointer<HSSStopTimerFunction> stopTimerFunction = QSharedPointer<HSSStopTimerFunction>(new HSSStopTimerFunction(d->controller));

    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));
    }

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
        AXRString id = d->currentToken->getString();
        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventObjectName, QSharedPointer<HSSObjectNameConstant>(new HSSObjectNameConstant(id, d->controller)));
        }

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
    AXRString name = d->currentToken->getString();

    QSharedPointer<HSSToggleTimerFunction> toggleTimerFunction = QSharedPointer<HSSToggleTimerFunction>(new HSSToggleTimerFunction(d->controller));

    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));
    }

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
        AXRString id = d->currentToken->getString();
        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventObjectName, QSharedPointer<HSSObjectNameConstant>(new HSSObjectNameConstant(id, d->controller)));
        }

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
    AXRString name = d->currentToken->getString();

    QSharedPointer<HSSInsertFunction> insertFunction = QSharedPointer<HSSInsertFunction>(new HSSInsertFunction(d->controller));

    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));
    }

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

    bool valueValid = this->readVals(insertFunction, "value");
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
        if (!d->currentToken->isA(HSSIdentifier) || d->currentToken->getString() != "of")
        {
            AXRError("HSSCodeParser", "Unexpected token while reading flagging function: " + HSSToken::tokenStringRepresentation(d->currentToken->getType())).raise();
            return errorState;
        }

        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventFunctionModifier, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant("of", d->controller)));
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

    insertFunction->setSelectorChains(selectorChains);

    this->skip(HSSParenthesisClose);
    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    return insertFunction;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readIfFunction()
{
    QSharedPointer<HSSParserNode> errorState;

    AXRString name = d->currentToken->getString();

    QSharedPointer<HSSIfFunction> ifFunction = QSharedPointer<HSSIfFunction>(new HSSIfFunction(d->controller));

    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));
    }

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

    bool conditionValid = true;
    QSharedPointer<HSSParserNode> conditionNode = this->readSingleVal("condition", conditionValid);
    if (conditionNode && conditionValid)
    {
        ifFunction->addValue(conditionNode);
    }
    else
    {
        return errorState;
    }

    //we expect the closing parenthesis here
    this->skip(HSSParenthesisClose);
    if (atEndOfSource())
        return errorState;

    //skip any whitespace
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

    //now we're inside the block
    d->currentObjectContext.push(d->_containerContextObj);
    d->currentContext.push_back(HSSParserContextEvaluables);

    //read the inner part of the block
    ifFunction->setReadEvaluables(true);
    bool evaluablesValid = this->readEvaluables(ifFunction);
    if (!evaluablesValid)
        return errorState;
    ifFunction->setReadEvaluables(false);

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
    return ifFunction;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readElseFunction()
{
    QSharedPointer<HSSParserNode> errorState;
    HSSString tokenStr = d->currentToken->getString();
    if (tokenStr == "else")
    {
        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(tokenStr, d->controller)));
        }

        QSharedPointer<HSSIfFunction> elseFunction = QSharedPointer<HSSIfFunction>(new HSSIfFunction(d->controller));

        this->readNextToken();
        if (this->atEndOfSource())
            return errorState;
        //skip any whitespace
        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;

        if (d->currentToken->isA(HSSBlockOpen))
        {
            elseFunction->setFunctionType(HSSFunctionTypeElse);

            this->skip(HSSBlockOpen);
            if (atEndOfSource())
                return errorState;

            //skip any whitespace
            this->skip(HSSWhitespace);
            if (atEndOfSource())
                return errorState;

            //now we're inside the block
            d->currentObjectContext.push(d->_containerContextObj);
            d->currentContext.push_back(HSSParserContextEvaluables);

            //read the inner part of the block
            elseFunction->setReadEvaluables(true);
            bool evaluablesValid = this->readEvaluables(elseFunction);
            if (!evaluablesValid)
                return errorState;
            elseFunction->setReadEvaluables(false);

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
            return elseFunction;
        }
        else if (d->currentToken->isA(HSSIdentifier) && d->currentToken->getString() == "if")
        {
            QSharedPointer<HSSParserNode> elseFunc = this->readIfFunction();
            if (elseFunc && elseFunc->isA(HSSFunctionTypeIf))
            {
                qSharedPointerCast<HSSIfFunction>(elseFunc)->setFunctionType(HSSFunctionTypeElseIf);
            }
            if (elseFunc)
                return elseFunc;
        }
        else
            return errorState;
    }
    return errorState;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readUserDefinedFunction()
{
    QSharedPointer<HSSParserNode> errorState;

    AXRString name = d->currentToken->getString();

    QSharedPointer<HSSFunctionFunction> fFunction = QSharedPointer<HSSFunctionFunction>(new HSSFunctionFunction(d->controller));

    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));
    }

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

    bool valueValid = true;
    bool argumentsDone = false;
    while (!argumentsDone)
    {
        argumentsDone = true;
        if (d->currentToken->isA(HSSIdentifier))
        {
            bool isValid = true;
            if (d->currentToken->isA(HSSIdentifier))
            {
                HSSString tokenStr = d->currentToken->getString();
                AXRString name;
                if (tokenStr == "var")
                {
                    if (d->notifiesReceiver)
                    {
                        //notify the receiver
                        d->receiver->receiveParserEvent(HSSParserEventKeywordConstant, HSSKeywordConstant::keyword(tokenStr, d->controller));
                    }

                    this->readNextToken();

                    if (atEndOfSource())
                        return errorState;

                    this->skip(HSSWhitespace);
                    if (atEndOfSource())
                        return errorState;

                    //read the variable name
                    if (!d->currentToken->isA(HSSIdentifier))
                        return errorState;

                    name = d->currentToken->getString();
                }
                else
                {
                    AXRWarning("HSSCodeParser", HSSString::format("Warning: missing var in argument declaration on line %d and column %d.", d->currentToken->line, d->currentToken->column)).raise();
                    name = tokenStr;
                }
                QSharedPointer<HSSObjectNameConstant> argName = QSharedPointer<HSSObjectNameConstant>(new HSSObjectNameConstant(name, d->controller));
                fFunction->addArgument(argName);
                if (d->notifiesReceiver)
                {
                    //notify the receiver
                    d->receiver->receiveParserEvent(HSSParserEventObjectName, argName);
                }
                //
                this->readNextToken();
                if (atEndOfSource())
                    return errorState;
            }
            this->skip(HSSWhitespace);
            if (atEndOfSource())
                return errorState;

            if (d->currentToken->isA(HSSComma))
            {
                argumentsDone = false;
                this->skip(HSSComma);
                this->skip(HSSWhitespace);
                if (atEndOfSource())
                    return errorState;
            }
            if(!isValid) valueValid = false;
        }
    }
    if (!valueValid)
        return errorState;

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    if (d->currentToken->isA(HSSParenthesisClose))
    {
        this->skip(HSSParenthesisClose);
        if (atEndOfSource())
            return errorState;

        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
    }

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
    d->currentContext.push_back(HSSParserContextEvaluables);

    //read the inner part of the block
    bool evaluablesValid = this->readEvaluables(fFunction);
    if (!evaluablesValid)
        return errorState;

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
    return fFunction;
}

bool HSSCodeParser::isFunctionCall()
{
    return d->currentToken->isA(HSSParenthesisOpen);
}

QSharedPointer<HSSFunctionCall> HSSCodeParser::readFunctionCall()
{
    QSharedPointer<HSSFunctionCall> errorState;
    if (d->currentToken->isA(HSSParenthesisOpen))
    {
        QSharedPointer<HSSFunctionCall> fCall = QSharedPointer<HSSFunctionCall>(new HSSFunctionCall(d->controller));

        this->skip(HSSParenthesisOpen);
        if (atEndOfSource())
            return errorState;

        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;

        bool valueValid = true;
        bool argumentsDone = false;
        while (!argumentsDone && valueValid)
        {
            argumentsDone = true;
            bool isValid = true;
            QSharedPointer<HSSArgument> arg = this->readArgument(isValid);
            if (isValid && arg)
            {
                fCall->addArgument(arg);
            }
            if (!isValid) valueValid = false;

            this->skip(HSSWhitespace);
            if (atEndOfSource())
                return errorState;

            if (d->currentToken->isA(HSSComma))
            {
                argumentsDone = false;
                this->skip(HSSComma);
                this->skip(HSSWhitespace);
                if (atEndOfSource())
                    return errorState;
            }
        }
        if (!valueValid)
            return errorState;

        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;

        if (d->currentToken->isA(HSSParenthesisClose))
        {
            this->skip(HSSParenthesisClose);
            if (atEndOfSource())
                return errorState;

            this->skip(HSSWhitespace);
            if (atEndOfSource())
                return errorState;
        }
        return fCall;
    }
    else
    {
        return errorState;
    }
    return errorState;
}

QSharedPointer<HSSArgument> HSSCodeParser::readArgument(bool & isValid)
{
    QSharedPointer<HSSArgument> errorState;

    QSharedPointer<HSSArgument> argument(new HSSArgument(d->controller));
    QSharedPointer<HSSParserNode> node = this->readVal("argument", isValid);
    if (node) {
        argument->addValue(node);
        return argument;
    }

    return errorState;
}

bool HSSCodeParser::readEvaluables(QSharedPointer<HSSParserNode> target)
{
    bool ret = true;
    bool evalsDone = false;
    QSharedPointer<HSSParserNode> currentNode;
    while (!evalsDone && ret == true)
    {
        evalsDone = true;
        currentNode.clear();

        if (d->currentToken->isA(HSSIdentifier))
        {
            //this is either a function, a keyword or an object name
            AXRString valuestr = d->currentToken->getString();

            if (valuestr == "var")
            {
                d->currentObjectContext.push(d->_containerContextObj);
                QSharedPointer<HSSVarDeclaration> varDecl = this->readVarDecl();
                d->currentObjectContext.pop();

                if (varDecl)
                {
                    target->addValue(varDecl);
                }
                else
                {
                    AXRError("HSSParser", HSSString::format("Error while reading var declaration on line %d and column %d", d->currentToken->line, d->currentToken->column)).raise();
                }
                if (this->atEndOfSource())
                    return ret;
                this->skip(HSSWhitespace);
                if (this->atEndOfSource())
                    return ret;
            }
            else
            {
                //check if it is a function
                QSharedPointer<HSSObject> objectContext = d->currentObjectContext.top();

                if (objectContext->isFunction(valuestr, ""))
                {
                    QSharedPointer<HSSParserNode> theFunc = this->readFunction();
                    if (theFunc)
                    {
                        target->addValue(theFunc);
                        if ((theFunc->isA(HSSFunctionTypeIf) || theFunc->isA(HSSFunctionTypeElseIf)) && d->currentToken->isA(HSSIdentifier) && d->currentToken->getString() == "else")
                        {
                            evalsDone = false;
                            continue;
                        }
                    }
                    else
                    {
                        AXRError("HSSParser", HSSString::format("Error while reading function on line %d and column %d", d->currentToken->line, d->currentToken->column)).raise();
                    }
                }
                else if (this->isPropertyPath())
                {
                    QSharedPointer<HSSPropertyPath> thePath = this->readPropertyPath(true);
                    if (this->isAssignment())
                    {
                        QSharedPointer<HSSAssignment> asgmt = this->readAssignment(thePath);
                        if (asgmt)
                        {
                            currentNode = asgmt;
                        }
                        else
                        {
                            AXRError("HSSParser", HSSString::format("Error while reading function on line %d and column %d", d->currentToken->line, d->currentToken->column)).raise();
                        }
                    }
                    else if (this->isComparison())
                    {
                        QSharedPointer<HSSComparison> comparison = this->readComparison(thePath->front()->evaluate(), thePath);
                        if (comparison)
                        {
                            currentNode = comparison;
                            evalsDone = false;
                        }
                        else
                        {
                            AXRError("HSSParser", HSSString::format("Error while reading comparison on line %d and column %d", d->currentToken->line, d->currentToken->column)).raise();
                        }
                    }
                    else
                    {
                        currentNode = thePath;
                    }
                }
                else if (this->isAssignment())
                {
                    QSharedPointer<HSSToken> identifierToken = d->currentToken;
                    if (d->notifiesReceiver)
                    {
                        //notify the receiver
                        d->receiver->receiveParserEvent(HSSParserEventVariableName, HSSObjectNameConstant::createConstant(identifierToken->getString(), d->controller));
                    }

                    this->readNextToken();
                    if (atEndOfSource())
                        return ret;

                    QSharedPointer<HSSAssignment> asgmt = this->readAssignment(identifierToken);
                    if (asgmt)
                    {
                        currentNode = asgmt;
                    }
                    else
                    {
                        AXRError("HSSParser", HSSString::format("Error while reading assignment on line %d and column %d", d->currentToken->line, d->currentToken->column)).raise();
                    }
                }
                else if (this->isComparison())
                {
                    QSharedPointer<HSSToken> identifierToken = d->currentToken;
                    QSharedPointer<HSSObjectNameConstant> objName = HSSObjectNameConstant::createConstant(identifierToken->getString(), d->controller);
                    if (d->notifiesReceiver)
                    {
                        //notify the receiver
                        d->receiver->receiveParserEvent(HSSParserEventVariableName, objName);
                    }
                    this->readNextToken();
                    if (atEndOfSource())
                        return ret;

                    QSharedPointer<HSSComparison> comparison = this->readComparison("", objName);
                    if (comparison)
                    {
                        currentNode = comparison;
                        evalsDone = false;
                    }
                    else
                    {
                        AXRError("HSSParser", HSSString::format("Error while reading comparison on line %d and column %d", d->currentToken->line, d->currentToken->column)).raise();
                    }
                }
                else if (d->currentToken->isA(HSSIdentifier))
                {
                    currentNode = this->readPropertyPath(true);
                }

                if (currentNode)
                {
                    target->addValue(currentNode);
                }
            }
        }
        if (this->atEndOfSource())
            return ret;
        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            return ret;
        if (d->currentToken->isA(HSSEndOfStatement))
        {
            evalsDone = false;
            this->skip(HSSEndOfStatement);
            if (this->atEndOfSource())
                return ret;
            this->skip(HSSWhitespace);
            if (this->atEndOfSource())
                return ret;
        }
        else if (!d->currentToken->isA(HSSBlockClose))
        {
            AXRError("HSSParser", HSSString::format("Unexpected token while reading evaluables on line %d and column %d", d->currentToken->line, d->currentToken->column)).raise();
            this->skipInvalidToken();
            evalsDone = false;
        }
    }
    return ret;
}

/**
 *  Reads the return function, which tells an user defined function to
 *  use a value as the result of evaluating the function.
 *  @return A shared pointer to the function node.
 */
QSharedPointer<HSSParserNode> HSSCodeParser::readReturnFunction()
{
    QSharedPointer<HSSFunctionCall> errorState;
    AXRString name = d->currentToken->getString();
    QSharedPointer<HSSReturnFunction> returnFunction = QSharedPointer<HSSReturnFunction>(new HSSReturnFunction(d->controller));

    if (d->notifiesReceiver)
    {
        //notify the receiver
        d->receiver->receiveParserEvent(HSSParserEventFunctionName, QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(name, d->controller)));
    }

    this->readNextToken();
    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    //parentheses are optional
    if (d->currentToken->isA(HSSParenthesisOpen))
    {
        this->skip(HSSParenthesisOpen);
        if (atEndOfSource())
            return errorState;

        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
    }
    else if (d->currentToken->isA(HSSEndOfStatement))
    {
        return returnFunction;
    }

    //read the return value
    this->readVals(returnFunction, "return");

    this->skip(HSSWhitespace);
    if (atEndOfSource())
        return errorState;

    if (d->currentToken->isA(HSSParenthesisClose))
    {
        this->skip(HSSParenthesisClose);
        if (atEndOfSource())
            return errorState;

        this->skip(HSSWhitespace);
        if (atEndOfSource())
            return errorState;
    }

    return returnFunction;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readCustomFunction()
{
    QSharedPointer<HSSParserNode> errorState;
    AXRString name = d->currentToken->getString();
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

    std::vector<QSharedPointer<HSSArgument> > arguments;
    while (!d->currentToken->isA(HSSParenthesisClose) && !this->atEndOfSource())
    {
        bool valid;
        arguments.push_back(this->readArgument(valid));
        if (d->currentToken->isA(HSSComma))
        {
            this->skip(HSSComma);
            if (atEndOfSource())
                return errorState;
        }
    }
    QSharedPointer<HSSCustomFunction> theFunction = QSharedPointer<HSSCustomFunction>(new HSSCustomFunction(d->controller));
    theFunction->setArguments(arguments);
    theFunction->setName(name);
    this->readNextToken();
    if (atEndOfSource())
        return errorState;

    this->skip(HSSWhitespace);
    return theFunction;
}

void HSSCodeParser::setIgnoreTokenReadCalls(bool value)
{
    d->ignoreTokenReadCalls = value;
}

void HSSCodeParser::readNextToken()
{
    //read next one
    if (!d->ignoreTokenReadCalls)
    {
        QSharedPointer<HSSToken> theToken = d->tokenizer->readNextToken();
        this->updateCurrentToken(theToken);
    }
}

void HSSCodeParser::updateCurrentToken(QSharedPointer<HSSToken> theToken)
{
    if (theToken && (theToken->isA(HSSBlockComment) || theToken->isA(HSSLineComment)))
    {
        d->receiver->receiveParserEvent(HSSParserEventComment, HSSCommentNode::createComment(theToken->getString(), d->controller));
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

void HSSCodeParser::skipInvalidToken()
{
    if (d->currentToken->isA(HSSSingleQuoteString) || d->currentToken->isA(HSSDoubleQuoteString))
    {
        QSharedPointer<HSSStringToken> strToken = qSharedPointerCast<HSSStringToken>(d->currentToken);
        AXRString theString;
        if (strToken->hasArguments())
        {
            if (d->notifiesReceiver)
            {
                QSharedPointer<HSSInvalidNode> firstChunk = HSSInvalidNode::createInvalidNode(strToken->getString(), d->controller);
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParserEventInvalid, firstChunk);
            }
            this->readNextToken();
            if (this->atEndOfSource())
                return;

            this->skipStringArgument();
            if (d->currentToken->isA(HSSBlockClose) && d->notifiesReceiver)
            {
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParserEventInvalid, HSSInvalidNode::createInvalidNode("}", d->controller));
                if (this->atEndOfSource())
                    return;
            }

            bool stringDone = false;
            QSharedPointer<HSSStringToken> strChunk;
            while(!stringDone){
                //readString() will set stringDone to false, and re-set it to true if needed
                strChunk = d->tokenizer->readString(strToken, stringDone);

                this->readNextToken();
                if (this->atEndOfSource())
                    return;

                if (!stringDone)
                {
                    //notify the mid chunk of the string
                    if (d->notifiesReceiver)
                    {
                        QSharedPointer<HSSInvalidNode> midChunk = HSSInvalidNode::createInvalidNode(strChunk->getString(), d->controller);
                        //notify the receiver
                        d->receiver->receiveParserEvent(HSSParserEventStringConstant, midChunk);
                    }

                    this->skipStringArgument();

                    if (d->currentToken->isA(HSSBlockClose) && d->notifiesReceiver)
                    {
                        //notify the receiver
                        d->receiver->receiveParserEvent(HSSParserEventInvalid, HSSInvalidNode::createInvalidNode("}", d->controller));
                    }
                    if (this->atEndOfSource())
                        return;
                }
            }
            //notify the end chunk of the string
            if (d->notifiesReceiver)
            {
                QSharedPointer<HSSInvalidNode> endChunk = HSSInvalidNode::createInvalidNode(strChunk->getString(), d->controller);
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParserEventInvalid, endChunk);
            }
        }
        else
        {
            if (d->notifiesReceiver)
            {
                //notify the receiver
                d->receiver->receiveParserEvent(HSSParserEventInvalid, HSSInvalidNode::createInvalidNode(d->currentToken->getString(), d->controller));
            }
            this->readNextToken();
        }
    }
    else
    {
        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventInvalid, HSSInvalidNode::createInvalidNode(d->currentToken->getString(), d->controller));
        }
        this->readNextToken();
    }
}

void HSSCodeParser::skipStringArgument()
{
    //skip the %
    if (d->currentToken->isA(HSSSymbol) && d->currentToken->getString() == "%") {
        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventInvalid, HSSInvalidNode::createInvalidNode(d->currentToken->getString(), d->controller));
        }
        this->readNextToken();
        if (this->atEndOfSource())
            return;
    }

    bool hasBlock = false;
    if (d->currentToken->isA(HSSBlockOpen))
    {
        hasBlock = true;
        if (d->notifiesReceiver)
        {
            //notify the receiver
            d->receiver->receiveParserEvent(HSSParserEventInvalid, HSSInvalidNode::createInvalidNode(d->currentToken->getString(), d->controller));
        }
        this->readNextToken();
        if (this->atEndOfSource())
            return;

        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            return;
    }

    bool valueValid = true;
    bool currentIgnoresTokenReadCallsValue = d->ignoreTokenReadCalls;
    if (!hasBlock)
    {
        //read the value, but du not advance the current token yet
        this->setIgnoreTokenReadCalls(true);
    }
    this->readVal("argument", valueValid);
    if (!hasBlock)
    {
        this->setIgnoreTokenReadCalls(currentIgnoresTokenReadCallsValue);
    }
    return;
}

QSharedPointer<HSSParserNode> HSSCodeParser::readStringArgument()
{
    QSharedPointer<HSSParserNode> errorState;
    QSharedPointer<HSSParserNode> ret;

    //skip the %
    if (d->currentToken->isA(HSSSymbol) && d->currentToken->getString() == "%") {
        this->skip(HSSSymbol);
        if (this->atEndOfSource())
            return errorState;
    }

    bool hasBlock = false;
    if (d->currentToken->isA(HSSBlockOpen))
    {
        hasBlock = true;
        this->skip(HSSBlockOpen);
        if (this->atEndOfSource())
            return errorState;

        this->skip(HSSWhitespace);
        if (this->atEndOfSource())
            return errorState;
    }

    bool valueValid = true;
    bool currentIgnoresTokenReadCallsValue = d->ignoreTokenReadCalls;
    if (!hasBlock)
    {
        //read the value, but du not advance the current token yet
        this->setIgnoreTokenReadCalls(true);
    }
    ret = this->readVal("argument", valueValid);
    if (!hasBlock)
    {
        this->setIgnoreTokenReadCalls(currentIgnoresTokenReadCallsValue);
    }
    if (!valueValid || this->atEndOfSource())
        return errorState;
    return ret;
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
    if (d->currentToken && d->currentToken->isA(type))
    {
        if (type == HSSWhitespace)
        {
            QSharedPointer<HSSWhitespaceNode> theNode = HSSWhitespaceNode::createWhitespace(d->currentToken->getString(), d->controller);
            d->receiver->receiveParserEvent(HSSParserEventWhitespace, theNode);
            this->readNextToken();
            if (!this->atEndOfSource() && d->currentToken->isA(type))
            {
                this->skip(type);
            }
        }
        else if (type == HSSNumber)
        {
            d->receiver->receiveParserEvent(HSSParserEventOther, HSSSymbolNode::createSymbol(HSSString::number(d->currentToken->getNumber()), d->controller));
            this->readNextToken();
        }
        else
        {
            d->receiver->receiveParserEvent(HSSParserEventOther, HSSSymbolNode::createSymbol(d->currentToken->getString(), d->controller));
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
