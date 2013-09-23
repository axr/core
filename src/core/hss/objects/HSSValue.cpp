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

#include "AXRController.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSCallback.h"
#include "HSSDisplayObject.h"
#include "HSSFunction.h"
#include "HSSExpression.h"
#include "HSSKeywordConstant.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSSimpleSelection.h"
#include "HSSStringConstant.h"
#include "HSSValue.h"

using namespace AXR;

QSharedPointer<HSSValue> HSSValue::valueFromParserNode(AXRController * controller, QSharedPointer<HSSParserNode> parserNode, QSharedPointer<HSSDisplayObject> thisObj, QSharedPointer<HSSSimpleSelection> scope)
{
    QSharedPointer<HSSValue> valueObj = QSharedPointer<HSSValue>(new HSSValue(controller));
    valueObj->setValue(parserNode);
    valueObj->setThisObj(thisObj);
    valueObj->setScope(scope);
    return valueObj;
}

HSSValue::HSSValue(AXRController * controller)
: HSSObject(HSSObjectTypeValue, controller)
{
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("value");
    this->setShorthandProperties(shorthandProperties);

    this->_initialize();
}

HSSValue::HSSValue(const HSSValue & orig)
: HSSObject(orig)
{
    this->_initialize();
    this->value = orig.value->clone();
}

void HSSValue::_initialize()
{

}

QSharedPointer<HSSValue> HSSValue::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSValue: cloning value object");
    return qSharedPointerCast<HSSValue>(this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSValue::cloneImpl() const
{
    return QSharedPointer<HSSValue>(new HSSValue(*this));
}

HSSValue::~HSSValue()
{
    axr_log(LoggerChannelGeneralSpecific, AXRString("HSSValue (").append(this->getHostProperty()).append("): destructing value object"));
    this->cleanTrackedObservers();
}

AXRString HSSValue::toString()
{
    return "HSSValue: " + (value ? this->value->toString() : "(null)");
}

bool HSSValue::equalTo(QSharedPointer<HSSObject> otherObj)
{
    //check wether pointers are the same
    if (this == otherObj.data()) return true;
    //other checks
    if ( ! HSSObject::equalTo(otherObj)) return false;
    QSharedPointer<HSSValue> castedObj = qSharedPointerCast<HSSValue>(otherObj);
    if ( ! this->value->equalTo(castedObj->value)) return false;
    return true;
}

AXRString HSSValue::defaultObjectType()
{
    return "value";
}

AXRString HSSValue::defaultObjectType(AXRString property)
{
    if (property == "value")
    {
        return "request";
    }
    else
    {
        return HSSObject::defaultObjectType(property);
    }
}

const QSharedPointer<HSSParserNode> HSSValue::getValue() const
{
    return this->value;
}

void HSSValue::setValue(QSharedPointer<HSSParserNode> parserNode)
{
    this->value = parserNode;
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeFunctionCall:
            parserNode->observe("__impl_private__remoteValue", "__impl_private__valueObj", this, new HSSValueChangedCallback<HSSValue>(this, &HSSValue::valueChanged));
            break;
        case HSSParserNodeTypeExpression:
            parserNode->observe("__impl_private__expressionResult", "__impl_private__valueObj", this, new HSSValueChangedCallback<HSSValue>(this, &HSSValue::valueChanged));
            break;

        default:
            break;
    }
}

void HSSValue::valueChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->notifyObservers("__impl_private__valueChanged", theObj);
}

const HSSUnit HSSValue::getNumber() const
{
    switch (this->value->getType())
    {
        case HSSParserNodeTypeFunctionCall:
        {
            QSharedPointer<HSSObject> remoteObj = qSharedPointerCast<HSSFunction>(this->value)->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                return this->_getNumber(qSharedPointerCast<HSSValue>(remoteObj)->getValue());
            }
            break;
        }
        default:
            return this->_getNumber(this->value);
    }
    return 0.;
}

const HSSUnit HSSValue::_getNumber(const QSharedPointer<HSSParserNode> & parserNode) const
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeNumberConstant:
            return qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue();
        case HSSParserNodeTypeExpression:
            return qSharedPointerCast<HSSExpression>(parserNode)->evaluate();
        case HSSParserNodeTypeStringConstant:
        {
            AXRString string = qSharedPointerCast<HSSStringConstant>(parserNode)->getValue();
            return string.toDouble();
        }
        case HSSParserNodeTypeKeywordConstant:
        {
            AXRString kw = qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue();
            if (kw == "yes")
            {
                return 1.;
            }
        }
        default:
            break;
    }
    return 0.;
}

const bool HSSValue::getBool() const
{
    switch (this->value->getType())
    {
        case HSSParserNodeTypeFunctionCall:
        {
            QSharedPointer<HSSObject> remoteObj = qSharedPointerCast<HSSFunction>(this->value)->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                return this->_getBool(qSharedPointerCast<HSSValue>(remoteObj)->getValue());
            }
            break;
        }
        default:
            return this->_getBool(this->value);
    }
    return false;
}

const bool HSSValue::_getBool(const QSharedPointer<HSSParserNode> & parserNode) const
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeNumberConstant:
            return qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue() > 0.;
        case HSSParserNodeTypeExpression:
            return qSharedPointerCast<HSSExpression>(parserNode)->evaluate() > 0;
        case HSSParserNodeTypeKeywordConstant:
        {
            AXRString kwValue = qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue();
            if (kwValue == "yes")
            {
                return true;
            }
            break;
        }
        default:
            break;
    }
    return false;
}

const AXRString HSSValue::getString() const
{
    switch (this->value->getType())
    {
        case HSSParserNodeTypeFunctionCall:
        {
            QSharedPointer<HSSObject> remoteObj = qSharedPointerCast<HSSFunction>(this->value)->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                return this->_getString(qSharedPointerCast<HSSValue>(remoteObj)->getValue());
            }
            break;
        }
        default:
            return this->_getString(this->value);
    }
    return "";
}

const AXRString HSSValue::_getString(const QSharedPointer<HSSParserNode> & parserNode) const
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeStringConstant:
        {
            AXRString ret = qSharedPointerCast<HSSStringConstant>(parserNode)->getValue();
            return ret;
        }
        case HSSParserNodeTypeNumberConstant:
            return QString::number(qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue());
        case HSSParserNodeTypeExpression:
            return QString::number(qSharedPointerCast<HSSExpression>(parserNode)->evaluate());
        case HSSParserNodeTypeKeywordConstant:
        {
            return qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue();
        }
        default:
            break;
    }
    return "";
}

const QSharedPointer<HSSObject> HSSValue::getObject() const
{
    switch (this->value->getType())
    {
        case HSSParserNodeTypeFunctionCall:
        {
            return qSharedPointerCast<HSSFunction>(this->value)->evaluate();
        }
        default:
            break;
    }
    return QSharedPointer<HSSObject>();
}

void HSSValue::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    HSSObject::setScope(newScope);
    //propagate values
    if (this->value)
    {
        switch (this->value->getType())
        {
            case HSSParserNodeTypeExpression:
                qSharedPointerCast<HSSExpression>(this->value)->setScope(newScope);
                break;
            case HSSParserNodeTypeFunctionCall:
                qSharedPointerCast<HSSFunction>(this->value)->setScope(newScope);
                break;
            default:
                break;
        }
    }
}

void HSSValue::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    HSSObject::setThisObj(value);
    //propagate values
    if (this->value)
    {
        this->value->setThisObj(value);
    }
}

void HSSValue::setHostProperty(AXRString newValue)
{
    HSSObject::setHostProperty(newValue);
    if (this->value)
    {
        this->value->setHostProperty(newValue);
    }
}
