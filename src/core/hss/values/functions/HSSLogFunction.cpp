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
#include "HSSLogFunction.h"

using namespace AXR;

HSSLogFunction::HSSLogFunction(AXRController * controller)
: HSSFunction(HSSFunctionTypeLog, controller)
{

}

HSSLogFunction::~HSSLogFunction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLogFunction: destructing log function");
    this->cleanTrackedObservers();
}

HSSLogFunction::HSSLogFunction(const HSSLogFunction & orig)
: HSSFunction(orig)
{
    
}

QSharedPointer<HSSFunction> HSSLogFunction::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

AXRString HSSLogFunction::toString()
{
    AXRString tempstr = AXRString("HSSLogFunction\n");
    return tempstr;
}

std::vector<QSharedPointer<HSSParserNode> > HSSLogFunction::getValues()
{
    return this->values;
}

void HSSLogFunction::setValues(std::vector<QSharedPointer<HSSParserNode> > newValues)
{
    std::vector<QSharedPointer<HSSParserNode> >::const_iterator it;
    QSharedPointer<HSSParserNode> parentNode = this->shared_from_this();
    for (it = newValues.begin(); it != newValues.end(); ++it)
    {
        (*it)->setParentNode(parentNode);
    }
    this->values = newValues;
}

void HSSLogFunction::add(QSharedPointer<HSSParserNode> newValue)
{
    newValue->setParentNode(this->shared_from_this());
    this->values.push_back(newValue);
}

bool HSSLogFunction::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSFunction::equalTo(otherNode)) return false;
    return true;
}

QSharedPointer<HSSObject> HSSLogFunction::_evaluate()
{
    std::vector<QSharedPointer<HSSParserNode> >::const_iterator it;
    for (it = this->values.begin(); it != this->values.end(); ++it)
    {
        const QSharedPointer<HSSParserNode> & parserNode = *it;
        this->_logParserNode(parserNode);
    }
    this->setDirty(true);
    return this->_value;
}

void HSSLogFunction::_logParserNode(QSharedPointer<HSSParserNode> parserNode)
{
    if (!parserNode)
        return;

    switch (parserNode->getType())
    {
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (parserNode)->clone();
            if (fnct->isA(HSSFunctionTypeRef))
            {
                QSharedPointer<HSSRefFunction> refFnct = qSharedPointerCast<HSSRefFunction > (fnct);
                refFnct->setScope(this->scope);
                refFnct->setThisObj(this->getThisObj());
                QSharedPointer<HSSObject> remoteObj = refFnct->evaluate();
                if (remoteObj)
                {
                    if (remoteObj->isA(HSSObjectTypeValue))
                    {
                        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                        this->_logParserNode(parserNode);
                    }
                    else
                    {
                        this->_logObject(remoteObj);
                    }
                }
            }
            else if (fnct->isA(HSSFunctionTypeCall))
            {
                QSharedPointer<HSSFunctionCall> fnctCall = qSharedPointerCast<HSSFunctionCall > (fnct);
                QSharedPointer<HSSObject> remoteObj = fnctCall->HSSFunction::evaluate();
                if (remoteObj)
                {
                    if (remoteObj->isA(HSSObjectTypeValue))
                    {
                        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                        this->_logParserNode(parserNode);
                    }
                    else
                    {
                        this->_logObject(remoteObj);
                    }
                }
            }
            break;
        }

        case HSSParserNodeTypeStringConstant:
        {
            AXRString kwValue = qSharedPointerCast<HSSStringConstant>(parserNode)->getValue();
            axr_log(LoggerChannelLogFunction, kwValue.stripQuotes());
            break;
        }

        case HSSParserNodeTypeKeywordConstant:
        {
            AXRString kwValue("`");
            kwValue.append(qSharedPointerCast<HSSStringConstant>(parserNode)->getValue());
            kwValue.append("`");
            axr_log(LoggerChannelLogFunction, kwValue);
            break;
        }
        case HSSParserNodeTypeNumberConstant:
        {
            AXRString kwValue = HSSString::number(qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue());
            axr_log(LoggerChannelLogFunction, kwValue);
            break;
        }

        case HSSParserNodeTypeObjectNameConstant:
        {
            QSharedPointer<HSSObjectNameConstant> theObjName = qSharedPointerCast<HSSObjectNameConstant>(parserNode);
            QSharedPointer<HSSObject> remoteObj = this->getVar(theObjName->getValue());
            if(remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                if (valueNode)
                {
                    return this->_logParserNode(valueNode);
                }
            }
            break;
        }
            
        case HSSParserNodeTypePropertyPath:
        {
            QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath> (parserNode);
            QSharedPointer<HSSObject> searchResult = ppath->evaluate();
            if (searchResult && searchResult->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSValue> valueObj = qSharedPointerCast<HSSValue>(searchResult);
                this->_logParserNode(valueObj->getValue());
            }
            else
            {
                AXRWarning("HSSContainer", "Path " + ppath->stringRep() + " did not yield any results.").raise();
            }
            break;
        }
            
        case HSSParserNodeTypeExpression:
        {
            QSharedPointer<HSSExpression> theExp = qSharedPointerCast<HSSExpression>(parserNode);
            QSharedPointer<HSSParserNode> expResult = theExp->evaluateToConst();
            this->_logParserNode(expResult);
            break;
        }
            
        case HSSParserNodeTypeBooleanConstant:
        {
            QSharedPointer<HSSBooleanConstant> theBool = qSharedPointerCast<HSSBooleanConstant>(parserNode);
            if (theBool->getValue())
            {
                axr_log(LoggerChannelLogFunction, "true");
            }
            else
            {
                axr_log(LoggerChannelLogFunction, "false");
            }
            break;
        }
        case HSSParserNodeTypeNegatedVal:
        {
            QSharedPointer<HSSNegatedVal> theNeg = qSharedPointerCast<HSSNegatedVal>(parserNode);
            this->_logParserNode(theNeg->evaluate());
            break;
        }
        default:
        {
            AXRError("HSSLogFunction", "Unexpected parser node type in log function").raise();
            break;
        }
    }
}

void HSSLogFunction::_logObject(QSharedPointer<HSSObject> theObj) const
{
    AXRError("HSSLogFunction", "Logging objects is not implemented yet").raise();
}

QSharedPointer<HSSClonable> HSSLogFunction::cloneImpl() const
{
    QSharedPointer<HSSLogFunction> clone = QSharedPointer<HSSLogFunction>(new HSSLogFunction(*this));
    std::vector<QSharedPointer<HSSParserNode> >::const_iterator it;
    for (it = this->values.begin(); it != this->values.end(); ++it)
    {
        const QSharedPointer<HSSParserNode> & parserNode = *it;
        clone->add(parserNode->clone());
    }
    return clone;
}

void HSSLogFunction::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    HSSFunction::setScope(newScope);
    //propagate values
    std::vector<QSharedPointer<HSSParserNode> >::iterator it;
    for (it = this->values.begin(); it != this->values.end(); ++it)
    {
        QSharedPointer<HSSParserNode> value = *it;
        switch (value->getType())
        {
            case HSSParserNodeTypeExpression:
                qSharedPointerCast<HSSExpression>(value)->setScope(newScope);
                break;
            case HSSParserNodeTypeFunction:
                qSharedPointerCast<HSSFunction>(value)->setScope(newScope);
                break;
            case HSSParserNodeTypeStringConstant:
                qSharedPointerCast<HSSStringConstant>(value)->setScope(newScope);
                break;
            default:
                break;
        }
    }
}

void HSSLogFunction::setThisObj(QSharedPointer<HSSDisplayObject> newThis)
{
    HSSFunction::setThisObj(newThis);
    std::vector<QSharedPointer<HSSParserNode> >::iterator it;
    for (it = this->values.begin(); it != this->values.end(); ++it)
    {
        (*it)->setThisObj(newThis);
    }
}
