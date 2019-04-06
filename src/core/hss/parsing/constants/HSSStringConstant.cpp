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

#include "axr.h"
#include "HSSStringConstant.h"

using namespace AXR;

QSharedPointer<HSSStringConstant> HSSStringConstant::stringToConstant(HSSString value, AXRController * controller)
{
    return QSharedPointer<HSSStringConstant>(new HSSStringConstant(value, controller));
}

HSSStringConstant::HSSStringConstant(HSSString value, AXRController * controller)
: HSSScopedParserNode(HSSParserNodeTypeStringConstant, controller)
, _hasArguments(false)
, _hasStartQuote(true)
, _hasEndQuote(true)
{
    this->value = value;
}

HSSStringConstant::HSSStringConstant(const HSSStringConstant & orig)
: HSSScopedParserNode(orig)
{
    this->value = orig.value;
    this->_hasArguments = orig._hasArguments;
    this->_hasStartQuote = orig._hasStartQuote;
    this->_hasEndQuote = orig._hasEndQuote;
    this->_indexes = orig._indexes;
}

QSharedPointer<HSSStringConstant> HSSStringConstant::clone() const
{
    return qSharedPointerCast<HSSStringConstant> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSStringConstant::cloneImpl() const
{
    QSharedPointer<HSSStringConstant> clone(new HSSStringConstant(*this));
    std::vector<QSharedPointer<HSSParserNode> >::const_iterator it;
    for (it = this->_arguments.begin(); it != this->_arguments.end(); ++it)
    {
        clone->addArgument((*it)->clone());
    }
    return clone;
}

HSSStringConstant::~HSSStringConstant()
{
    
}

void HSSStringConstant::setValue(HSSString newValue)
{
    this->value = newValue;
}

HSSString HSSStringConstant::getValue()
{
    if (this->_hasArguments)
    {
        return this->evaluate();
    }
    return this->value;
}

HSSString HSSStringConstant::toString()
{
    return "HSSStringConstant with value " + this->value;
}

HSSString HSSStringConstant::stringRep()
{
    return this->value;
}

bool HSSStringConstant::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSStringConstant> castedNode = qSharedPointerCast<HSSStringConstant>(otherNode);
    if ( this->value != castedNode->value ) return false;
    if ( this->_hasStartQuote != castedNode->_hasStartQuote ) return false;
    if ( this->_hasEndQuote != castedNode->_hasEndQuote ) return false;
    return true;
}

void HSSStringConstant::setHasArguments(bool newValue)
{
    this->_hasArguments = newValue;
}

bool HSSStringConstant::hasArguments() const
{
    return this->_hasArguments;
}

void HSSStringConstant::setArguments(const std::vector<QSharedPointer<HSSParserNode> > &args)
{
    if (args.size() > 0)
    {
        this->setHasArguments(true);
        this->_arguments.clear();
        for (std::vector<QSharedPointer<HSSParserNode> >::const_iterator it = args.begin(); it != args.end(); ++it)
        {
            this->addArgument((*it)->clone());
        }
    }
}

void HSSStringConstant::addArgument(QSharedPointer<HSSParserNode> parserNode)
{
    parserNode->setParentNode(this->shared_from_this());
    this->_arguments.push_back(parserNode);
}

void HSSStringConstant::valueChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    HSSString newsString = this->evaluate();
    QSharedPointer<HSSObject> newValue = HSSValue::valueFromParserNode(this->getController(), HSSStringConstant::stringToConstant(this->getValue(), this->getController()), theObj->getSpecificity(), this->getThisObj(), this->scope());
    this->notifyObservers("__impl_private__remoteValue", newValue);
}

void HSSStringConstant::setIndexes(const std::vector<size_t> &indexes)
{
    this->_indexes = indexes;
}

HSSString HSSStringConstant::evaluate()
{
    size_t j = this->_indexes.size();
    if (j==0) {
        return this->value;
    }
    
    this->_bindToArguments();

    std::string beginStr = this->value.substr(0, this->_indexes[0]).data();

    std::string midStr = "";
    for (size_t i = 0; i<j; ++i)
    {
        size_t index = this->_indexes[i];
        QSharedPointer<HSSParserNode> argument = this->_arguments[i];
        midStr.append(this->_evaluateNode(argument).stripQuotes().data());
        if (i<j-1)
        {
            midStr.append(this->value.substr(index, (this->_indexes[i+1]-index)).data());
        }
    }
    std::string endStr = this->value.substr(this->_indexes.back()).data();
    return beginStr + midStr + endStr;
}

void HSSStringConstant::_bindToArguments()
{
    for (std::vector<QSharedPointer<HSSParserNode> >::const_iterator it = this->_arguments.begin(); it != this->_arguments.end(); ++it)
    {
        QSharedPointer<HSSParserNode> parserNode = *it;
        switch (parserNode->getType())
        {
            case HSSParserNodeTypeFunction:
                parserNode->observe("__impl_private__remoteValue", "__impl_private__stringConstant", this, new HSSValueChangedCallback<HSSStringConstant>(this, &HSSStringConstant::valueChanged));
                break;
            case HSSParserNodeTypeExpression:
                parserNode->observe("__impl_private__expressionResult", "__impl_private__stringConstant", this, new HSSValueChangedCallback<HSSStringConstant>(this, &HSSStringConstant::valueChanged));
                break;
            case HSSParserNodeTypeStringConstant:
                parserNode->observe("__impl_private__remoteValue", "__impl_private__stringConstant", this, new HSSValueChangedCallback<HSSStringConstant>(this, &HSSStringConstant::valueChanged));
                break;
                
            default:
                break;
        }
    }
}

AXRString HSSStringConstant::_evaluateNode(QSharedPointer<HSSParserNode> parserNode)
{
    AXRString ret = "";
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeStringConstant:
        {
            QSharedPointer<HSSStringConstant> theString = qSharedPointerCast<HSSStringConstant>(parserNode);
            return theString->getValue();
        }
        case HSSParserNodeTypeNumberConstant:
        {
            QSharedPointer<HSSNumberConstant> theNum = qSharedPointerCast<HSSNumberConstant>(parserNode);
            return HSSString::number(theNum->getValue());
        }
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> theFunc = qSharedPointerCast<HSSFunction>(parserNode);
            QSharedPointer<HSSObject> remoteObj = theFunc->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                return this->_evaluateNode(valueNode);
            }
            break;
        }
        case HSSParserNodeTypeObjectNameConstant:
        {
            AXRString objName = qSharedPointerCast<HSSObjectNameConstant>(parserNode)->getValue();
            QSharedPointer<HSSObject> globalVar = this->getController()->getGlobalVariable(objName);
            if (globalVar)
            {
                if (globalVar->isA(HSSObjectTypeValue))
                {
                    QSharedPointer<HSSValue> theValue = qSharedPointerCast<HSSValue>(globalVar);
                    QSharedPointer<HSSParserNode> valueNode = theValue->getValue();
                    return this->_evaluateNode(valueNode);
                }
            }
            break;
        }
        default:
            break;
    }
    return ret;
}

bool HSSStringConstant::hasStartQuote() const
{
    return this->_hasStartQuote;
}

void HSSStringConstant::setHasStartQuote(bool newValue)
{
    this->_hasStartQuote = newValue;
}

bool HSSStringConstant::hasEndQuote() const
{
    return this->_hasEndQuote;
}

void HSSStringConstant::setHasEndQuote(bool newValue)
{
    this->_hasStartQuote = newValue;
}

void HSSStringConstant::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    HSSParserNode::setThisObj(value);
    for (std::vector<QSharedPointer<HSSParserNode> >::iterator it = this->_arguments.begin(); it != this->_arguments.end(); ++it)
    {
        QSharedPointer<HSSParserNode> theArg = *it;
        theArg->setThisObj(value);
    }
}

void HSSStringConstant::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    HSSScopedParserNode::setScope(newScope);
    for (std::vector<QSharedPointer<HSSParserNode> >::iterator it = this->_arguments.begin(); it != this->_arguments.end(); ++it)
    {
        QSharedPointer<HSSParserNode> theArg = *it;
        switch (theArg->getType())
        {
            case HSSParserNodeTypeFunction:
            {
                QSharedPointer<HSSFunction> theFunc = qSharedPointerCast<HSSFunction>(theArg);
                theFunc->setScope(newScope);
                break;
            }
            case HSSParserNodeTypeUnaryExpression:
            {
                QSharedPointer<HSSUnaryExpression> theUExp = qSharedPointerCast<HSSUnaryExpression>(theArg);
                theUExp->setScope(newScope);
                break;
            }
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> theExp = qSharedPointerCast<HSSExpression>(theArg);
                theExp->setScope(newScope);
                break;
            }
            case HSSParserNodeTypeStringConstant:
            {
                QSharedPointer<HSSStringConstant> theString = qSharedPointerCast<HSSStringConstant>(theArg);
                theString->setScope(newScope);
                break;
            }
            default:
                break;
        }
    }
}
