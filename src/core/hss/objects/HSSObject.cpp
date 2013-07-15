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

#include <QMap>
#include <QVariant>
#include "AXRController.h"
#include "AXRDocument.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSCallback.h"
#include "HSSCircle.h"
#include "HSSColorStop.h"
#include "HSSContainer.h"
#include "HSSEvent.h"
#include "HSSFont.h"
#include "HSSKeywordConstant.h"
#include "HSSLineBorder.h"
#include "HSSLinearGradient.h"
#include "HSSRadialGradient.h"
#include "HSSLog.h"
#include "HSSMargin.h"
#include "HSSMultipleValue.h"
#include "HSSMultipleValueDefinition.h"
#include "HSSNumberConstant.h"
#include "HSSObject.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSParentSelector.h"
#include "HSSPercentageConstant.h"
#include "HSSPolygon.h"
#include "HSSPropertyDefinition.h"
#include "HSSRectangle.h"
#include "HSSRefFunction.h"
#include "HSSRequest.h"
#include "HSSRgb.h"
#include "HSSRoundedRect.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelection.h"
#include "HSSSimpleSelector.h"
#include "HSSStringConstant.h"
#include "HSSTypeEnums.h"
#include "HSSValue.h"

using namespace AXR;

Q_DECLARE_METATYPE(AXR::HSSTextAlignType *)

QSharedPointer<HSSObject> HSSObject::newObjectWithType(AXRString type, AXRController * controller)
{
    static QMap<AXRString, HSSObjectType> types;
    if (types.empty())
    {
        types["container"] = HSSObjectTypeContainer;
        types["lineBorder"] = HSSObjectTypeBorder;
        types["object"] = HSSObjectTypeGeneric;
        types["value"] = HSSObjectTypeValue;
        types["margin"] = HSSObjectTypeMargin;
        types["rgb"] = HSSObjectTypeRgb;
        types["linearGradient"] = HSSObjectTypeGradient;
        types["radialGradient"] = HSSObjectTypeGradient;
        types["colorStop"] = HSSObjectTypeColorStop;
        types["font"] = HSSObjectTypeFont;
        types["rectangle"] = HSSObjectTypeShape;
        types["roundedRect"] = HSSObjectTypeShape;
        types["circle"] = HSSObjectTypeShape;
        types["polygon"] = HSSObjectTypeShape;
        types["load"] = HSSObjectTypeEvent;
        types["click"] = HSSObjectTypeEvent;
        types["doubleClick"] = HSSObjectTypeEvent;
        types["tripleClick"] = HSSObjectTypeEvent;
        types["mouseDown"] = HSSObjectTypeEvent;
        types["mouseUp"] = HSSObjectTypeEvent;
        types["mouseOver"] = HSSObjectTypeEvent;
        types["mouseOut"] = HSSObjectTypeEvent;
        types["mouseHold"] = HSSObjectTypeEvent;
        types["mouseMove"] = HSSObjectTypeEvent;
        types["clickSecondary"] = HSSObjectTypeEvent;
        types["clickTertiary"] = HSSObjectTypeEvent;
        types["scroll"] = HSSObjectTypeEvent;
        types["request"] = HSSObjectTypeAction;
        types["log"] = HSSObjectTypeAction;
    }

    HSSObjectType objectType = HSSObjectTypeNone;
    if (types.contains(type))
    {
        objectType = types[type];
    }

    switch (objectType)
    {
    case HSSObjectTypeContainer:
    {
        return QSharedPointer<HSSContainer>(new HSSContainer(controller));
    }

    case HSSObjectTypeBorder:
    {
        /**
         *  @todo border tyes?
         */
        return QSharedPointer<HSSLineBorder>(new HSSLineBorder(controller));
    }

    case HSSObjectTypeGradient:
    {
        if (type == "linearGradient")
        {
            return QSharedPointer<HSSLinearGradient>(new HSSLinearGradient(controller));
        }
        else if (type == "radialGradient")
        {
            return QSharedPointer<HSSRadialGradient>(new HSSRadialGradient(controller));
        }
    }

    case HSSObjectTypeColorStop:
    {
        return QSharedPointer<HSSColorStop>(new HSSColorStop(controller));
    }

    case HSSObjectTypeValue:
    {
        return QSharedPointer<HSSValue>(new HSSValue(controller));
    }

    case HSSObjectTypeMargin:
    {
        return QSharedPointer<HSSMargin>(new HSSMargin(controller));
    }

    case HSSObjectTypeRgb:
    {
        return QSharedPointer<HSSRgb>(new HSSRgb(controller));
    }

    case HSSObjectTypeFont:
    {
        return QSharedPointer<HSSFont>(new HSSFont(controller));
    }

    case HSSObjectTypeShape:
    {
        if (type == "rectangle")
        {
            return QSharedPointer<HSSRectangle>(new HSSRectangle(controller));
        }
        else if (type == "roundedRect")
        {
            return QSharedPointer<HSSRoundedRect>(new HSSRoundedRect(controller));
        }
        else if (type == "circle")
        {
            return QSharedPointer<HSSCircle>(new HSSCircle(controller));
        }
        else if (type == "polygon")
        {
            return QSharedPointer<HSSPolygon>(new HSSPolygon(controller));
        }
    }

    case HSSObjectTypeEvent:
    {
        static QMap<AXRString, HSSEventType> eventTypes;
        if (eventTypes.empty())
        {
            eventTypes["load"] = HSSEventTypeLoad;
            eventTypes["click"] = HSSEventTypeClick;
            eventTypes["doubleClick"] = HSSEventTypeDoubleClick;
            eventTypes["tripleClick"] = HSSEventTypeTripleClick;
            eventTypes["mouseDown"] = HSSEventTypeMouseDown;
            eventTypes["mouseUp"] = HSSEventTypeMouseUp;
            eventTypes["mouseOver"] = HSSEventTypeMouseOver;
            eventTypes["mouseOut"] = HSSEventTypeMouseOut;
            eventTypes["mouseHold"] = HSSEventTypeMouseHold;
            eventTypes["mouseMove"] = HSSEventTypeMouseMove;
            eventTypes["clickSecondary"] = HSSEventTypeClickSecondary;
            eventTypes["clickTertiary"] = HSSEventTypeClickTertiary;
            eventTypes["scroll"] = HSSEventTypeScroll;
        }

        if (eventTypes.contains(type))
        {
            return QSharedPointer<HSSEvent>(new HSSEvent(eventTypes[type], controller));
        }

        //fall through
    }

    case HSSObjectTypeAction:
    {
        if (type == "request")
        {
            return QSharedPointer<HSSRequest>(new HSSRequest(controller));
        }
        else if (type == "log")
        {
            return QSharedPointer<HSSLog>(new HSSLog(controller));
        }
    }

    default:
        throw AXRError("HSSObject", type);
    }

    return QSharedPointer<HSSObject>();
}

HSSObject::HSSObject(HSSObjectType type, AXRController * controller)
{
    this->_initialize();
    this->_isNamed = false;
    this->name = "";
    this->type = type;
    this->shorthandIndex = 0;
    this->axrController = controller;
}

HSSObject::HSSObject(const HSSObject & orig)
{
    this->_initialize();
    this->name = orig.name;
    this->type = orig.type;
    this->_isNamed = orig._isNamed;
    this->scope = orig.scope;
    this->thisObj = orig.thisObj;
    this->axrController = orig.axrController;
    this->shorthandIndex = orig.shorthandIndex;

    //copy the shorthand list
    Q_FOREACH(AXRString value, orig.shorthandProperties)
    {
        this->shorthandProperties.push_back(value);
    }
    //copy the default values
    QMapIterator<AXRString, QSharedPointer<HSSObject> > defaultIt(orig._defaultValues);
    while (defaultIt.hasNext())
    {
        defaultIt.next();
        this->_defaultValues.insert(defaultIt.key(), defaultIt.value()->clone());
    }
    //copy the computed values
    QMapIterator<AXRString, QSharedPointer<HSSObject> > computedIt(orig._computedValues);
    while (computedIt.hasNext())
    {
        computedIt.next();
        this->_computedValues.insert(computedIt.key(), computedIt.value()->clone());
    }
}

void HSSObject::_initialize()
{
}

QSharedPointer<HSSObject> HSSObject::clone() const
{
    return qSharedPointerCast<HSSObject> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSObject::cloneImpl() const
{
    return QSharedPointer<HSSObject>(new HSSObject(*this));
}

HSSObject::~HSSObject()
{

}

bool HSSObject::isKeyword(AXRString value, AXRString property)
{
    return (value == "default" || value == "inherit" || value == "undefined");
}

bool HSSObject::isFunction(AXRString value, AXRString property)
{
    if (value == "min"
            || value == "max"
            || value == "floor"
            || value == "ceil"
            || value == "round"
            || value == "ref"
            || value == "sel"
            || value == "flag"
            || value == "unflag"
            || value == "toggleFlag"
            || value == "takeFlag"
            || value == "attr"
            )
    {
        return true;
    }
    else
    {
        return this->getController()->document()->isCustomFunction(value);
    }
}

AXRString HSSObject::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSObject: ").append(this->name);
    }
    else
    {
        return "Anonymous HSSObject";
    }
}

bool HSSObject::isNamed() const
{
    return this->_isNamed;
}

void HSSObject::setName(AXRString newName)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSObject: setting name to " + newName);
    this->name = newName;
    this->_isNamed = true;
}

AXRString HSSObject::getName()
{
    return this->name;
}

void HSSObject::dropName()
{
    this->name = "";
    this->_isNamed = false;
}

AXRString HSSObject::defaultObjectType()
{
    return "value";
}

AXRString HSSObject::defaultObjectType(AXRString property)
{
    return "value";
}

AXRString HSSObject::getPropertyForCurrentValue()
{
    bool done = false;
    AXRString ret;

    security_brake_init();
    while (!done)
    {
        if (this->shorthandProperties.size() > this->shorthandIndex)
        {
            ret = this->shorthandProperties[this->shorthandIndex];
            if (this->skipShorthand.find(ret) == this->skipShorthand.end())
            {
                return ret;
            }
        }

        done = !this->shorthandNext();

        security_brake();
    }
    security_brake_reset();
    return "";
}

void HSSObject::setShorthandProperties(std::vector<AXRString> newValues)
{
    this->shorthandProperties = newValues;
    this->shorthandReset();
}

void HSSObject::shorthandSkip(AXRString propertyName)
{
    this->skipShorthand[propertyName] = true;
}

bool HSSObject::shorthandNext()
{
    if (this->shorthandIndex < this->shorthandProperties.size() + 1)
    {
        this->shorthandIndex++;
        return true;
    }
    return false;
}

void HSSObject::shorthandReset()
{
    this->shorthandIndex = 0;
}

size_t HSSObject::getShorthandIndex() const
{
    return this->shorthandIndex;
}

void HSSObject::setShorthandIndex(size_t newValue)
{
    if (newValue < this->shorthandProperties.size())
    {
        this->shorthandIndex = newValue;
    }
    else
    {
        this->shorthandIndex = this->shorthandProperties.size();
    }
}

QSharedPointer<HSSSimpleSelection> HSSObject::getScope() const
{
    return this->scope;
}

void HSSObject::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    this->scope = newScope;
}

void HSSObject::setController(AXRController * controller)
{
    this->axrController = controller;
}

AXRController * HSSObject::getController() const
{
    return this->axrController;
}

void HSSObject::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    this->thisObj = value;
}

QSharedPointer<HSSDisplayObject> HSSObject::getThisObj()
{
    return thisObj;
}

bool HSSObject::isA(HSSObjectType otherType)
{
    return otherType == this->type;
}

HSSObjectType HSSObject::getObjectType()
{
    return this->type;
}

bool HSSObject::isA(HSSShapeType otherType)
{
    return false;
}

HSSShapeType HSSObject::getShapeType()
{
    return HSSShapeTypeNone;
}

bool HSSObject::isA(HSSRequestModeType otherType)
{
    return false;
}

HSSRequestModeType HSSObject::getRequestModeType()
{
    return HSSRequestModeTypeNone;
}

bool HSSObject::isA(HSSEventType otherType)
{
    return false;
}

HSSEventType HSSObject::getEventType()
{
    return HSSEventTypeNone;
}

bool HSSObject::isA(HSSActionType otherType)
{
    return false;
}

HSSActionType HSSObject::getActionType()
{
    return HSSActionTypeNone;
}

bool HSSObject::isA(HSSGradientType otherType)
{
    return false;
}

HSSGradientType HSSObject::getGradientType()
{
    return HSSGradientTypeNone;
}

QSharedPointer<HSSObject> HSSObject::shared_from_this()
{
    if (!ptr)
        ptr = QWeakPointer<HSSObject>(this);

    return ptr.toStrongRef();
}

bool HSSObject::validate(AXRString propertyName, QSharedPointer<HSSParserNode> & value) const
{
    return true;
}

void HSSObject::clearProperties()
{
    this->clearDefaultValues();
    this->clearComputedValues();
    this->clearStackValues();
}

void HSSObject::addCallback(AXRString name, HSSAbstractStackCallback *callback)
{
    this->_stackCallbacks.insert(name, callback);
}

void HSSObject::addCallback(AXRString name, HSSAbstractComputeCallback *callback)
{
    this->_computeCallbacks.insert(name, callback);
}

void HSSObject::addCallback(AXRString name, HSSAbstractObserveCallback *setCallback)
{
    this->_setCallbacks.insert(name, setCallback);
}

void HSSObject::addCallback(AXRString name, HSSAbstractObserveCallback *listenCallback, HSSAbstractObserveCallback *notifyCallback)
{
    this->_listenCallbacks.insert(name, listenCallback);
    this->_notifyCallbacks.insert(name, notifyCallback);
}

void HSSObject::addCallback(AXRString name, HSSAbstractComputeCallback *computeCallback, HSSAbstractObserveCallback *listenCallback, HSSAbstractObserveCallback *notifyCallback)
{
    this->_computeCallbacks.insert(name, computeCallback);
    this->_listenCallbacks.insert(name, listenCallback);
    this->_notifyCallbacks.insert(name, notifyCallback);
}

void HSSObject::addListenCallback(AXRString name, HSSAbstractObserveCallback *listenCallback)
{
    this->_listenCallbacks.insert(name, listenCallback);
}

void HSSObject::addNotifyCallback(AXRString name, HSSAbstractObserveCallback *notifyCallback)
{
    this->_notifyCallbacks.insert(name, notifyCallback);
}

void HSSObject::clearDefaultValues()
{
    this->_defaultValues.clear();
}

void HSSObject::setDefaults()
{
    //do nothing
}

void HSSObject::setDefaultValue(AXRString propertyName, QSharedPointer<HSSObject> theObj)
{
    this->_defaultValues.insert(propertyName, theObj);
}

const QSharedPointer<HSSObject> HSSObject::getDefaultValue(AXRString propertyName) const
{
    return this->_defaultValues[propertyName];
}

void HSSObject::setDefault(AXRString propertyName, QSharedPointer<HSSParserNode> parserNode)
{
    QSharedPointer<HSSObject> theObj = this->computeValue(propertyName, parserNode);
    if (theObj)
    {
        this->setDefaultValue(propertyName, theObj);
    }
}

void HSSObject::setDefault(AXRString propertyName, HSSUnit value)
{
    this->setDefault(propertyName, this->numberToConstant(value));
}

void HSSObject::setDefaultPercentage(AXRString propertyName, HSSUnit value)
{
    this->setDefault(propertyName, this->percentageToConstant(value));
}

void HSSObject::setDefault(AXRString propertyName, AXRString value)
{
    this->setDefault(propertyName, this->stringToConstant(value));
}

void HSSObject::setDefaultKw(AXRString propertyName, AXRString value)
{
    this->setDefault(propertyName, this->stringToKeyword(value));
}

void HSSObject::fillWithDefaults()
{
    QMapIterator<AXRString, QSharedPointer<HSSObject> > it(this->_defaultValues);
    while (it.hasNext())
    {
        it.next();
        AXRString propertyName = it.key();
        QSharedPointer<HSSObject> defaultValue = it.value();
        if (!this->_computedValues.contains(propertyName))
        {
            this->setComputed(propertyName, defaultValue);
        }
    }
}

void HSSObject::clearStackValues()
{
    this->_stackValues.clear();
}

void HSSObject::setStackValue(AXRString propertyName, QSharedPointer<HSSParserNode> parserNode)
{
    //ignore invalid values
    if (!this->validate(propertyName, parserNode))
    {
        return;
    }
    //handle with callback
    if (this->_stackCallbacks.contains(propertyName))
    {
        HSSAbstractStackCallback * callback = this->_stackCallbacks[propertyName];
        callback->call(parserNode);
        return;
    }
    //default handling
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeMultipleValueDefinition:
        {
            HSSParserNode::it iterator;
            QSharedPointer<HSSMultipleValueDefinition> multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (parserNode);
            std::vector<QSharedPointer<HSSParserNode> > values = multiDef->getValues();
            for (iterator = values.begin(); iterator != values.end(); ++iterator)
            {
                this->appendStackValue(propertyName, *iterator);
            }
            return;
        }

        default:
            break;
    }
    this->_setStackValue(propertyName, parserNode);
}

void HSSObject::appendStackValue(AXRString propertyName, QSharedPointer<HSSParserNode> parserNode)
{
    if (!this->_stackValues.contains(propertyName))
    {
        this->_setStackValue(propertyName, parserNode);
    }
    else
    {
        QSharedPointer<HSSParserNode> stackValue = this->_stackValues[propertyName];
        if (stackValue)
        {
            if (stackValue->isA(HSSParserNodeTypeMultipleValueDefinition))
            {
                QSharedPointer<HSSMultipleValueDefinition> multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (stackValue);
                multiDef->add(parserNode);
            }
            else
            {
                QSharedPointer<HSSMultipleValueDefinition> multiDef(new HSSMultipleValueDefinition(this->getController()));
                multiDef->add(stackValue);
                multiDef->add(parserNode);
                this->_setStackValue(propertyName, multiDef);
            }
        }
    }
}

void HSSObject::_setStackValue(AXRString propertyName, QSharedPointer<HSSParserNode> parserNode)
{
    this->_stackValues.insert(propertyName, parserNode);
}

void HSSObject::commitStackValues()
{
    QMap<AXRString, QSharedPointer<HSSParserNode> >::iterator it;
    for (it = this->_stackValues.begin(); it != this->_stackValues.end(); ++it)
    {
        QSharedPointer<HSSObject> theObj = this->computeValue(it.key(), it.value());
        if (theObj)
        {
            this->setComputed(it.key(), theObj);
        }
    }
}

QSharedPointer<HSSObject> HSSObject::computeValue(AXRString propertyName, QSharedPointer<HSSParserNode> parserNode)
{
    //handle multiple values
    if (parserNode->isA(HSSParserNodeTypeMultipleValueDefinition))
    {
        HSSParserNode::it iterator;
        QSharedPointer<HSSMultipleValue> valueList(new HSSMultipleValue(this->getController()));
        QSharedPointer<HSSMultipleValueDefinition> multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (parserNode);
        std::vector<QSharedPointer<HSSParserNode> > values = multiDef->getValues();
        for (iterator = values.begin(); iterator != values.end(); ++iterator)
        {
            valueList->add(this->computeValue(propertyName, *iterator));
        }
        return valueList;
    }

    //is it an object definition?
    switch (parserNode->getStatementType())
    {
        case HSSStatementTypeObjectDefinition:
        {
            QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (parserNode);
            if (this->isA(HSSObjectTypeContainer) || this->isA(HSSObjectTypeTextBlock))
            {
                QSharedPointer<HSSDisplayObject> thisDO = qSharedPointerCast<HSSDisplayObject>(this->shared_from_this());
                objdef->setThisObj(thisDO);
                QSharedPointer<HSSContainer> parent = thisDO->getParent();
                if (parent)
                {
                    objdef->setScope(parent->getChildren());
                }
                else
                {
                    QSharedPointer<HSSSimpleSelection> rootScope(new HSSSimpleSelection());
                    rootScope->add(thisDO);
                    objdef->setScope(rootScope);
                }
            }
            else
            {
                objdef->setThisObj(this->getThisObj());
            }
            objdef->apply();
            QSharedPointer<HSSObject> theObj = objdef->getObject();
            if (theObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSValue> valueObj = qSharedPointerCast<HSSValue>(theObj);
                QSharedPointer<HSSObject> valueObjProp = valueObj->getComputedValue("value");
                if (valueObjProp)
                {
                    if (valueObjProp->isA(HSSObjectTypeValue))
                    {
                        return this->computeValue(propertyName, qSharedPointerCast<HSSValue>(valueObjProp)->getValue());
                    }
                    else
                    {
                        return valueObjProp;
                    }
                }
                return this->computeValue(propertyName, valueObj->getValue());
            }
            return theObj;
        }
        default:
            break;
    }
    //object name constant
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeObjectNameConstant:
        {
            try
            {
                QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (parserNode);
                return this->computeValue(propertyName, this->getController()->objectTreeNodeNamed(objname->getValue()));
            }
            catch (const AXRError &e)
            {
                e.raise();
                return QSharedPointer<HSSValue>();
            }
            break;
        }

        default:
            break;
    }

    //let the specific subclass handle the property
    if (this->_computeCallbacks.contains(propertyName))
    {
        HSSAbstractComputeCallback * callback = this->_computeCallbacks[propertyName];
        return callback->call(parserNode);
    }
    //else no special handling, just wrap in a @value
    return this->computeValueObject(parserNode, propertyName);
}

QSharedPointer<HSSObject> HSSObject::computeValueObject(QSharedPointer<HSSParserNode> parserNode)
{
    QSharedPointer<HSSDisplayObject> thisObj;
    QSharedPointer<HSSSimpleSelection> scope;

    if (this->isA(HSSObjectTypeContainer) || this->isA(HSSObjectTypeTextBlock))
    {
        thisObj = qSharedPointerCast<HSSDisplayObject>(this->shared_from_this());
        QSharedPointer<HSSContainer> parent = thisObj->getParent();
        if (parent)
        {
            scope = parent->getChildren();
        }
        else
        {
            QSharedPointer<HSSSimpleSelection> rootScope(new HSSSimpleSelection());
            rootScope->add(thisObj);
            scope = rootScope;
        }
    }
    else
    {
        thisObj = this->getThisObj();
        scope = this->getScope();
    }
    QSharedPointer<HSSObject> valueObj = HSSValue::valueFromParserNode(this->getController(), parserNode, thisObj, scope);
    return valueObj;
}

QSharedPointer<HSSObject> HSSObject::computeValueObject(QSharedPointer<HSSParserNode> parserNode, AXRString propertyName)
{
    if (parserNode->isA(HSSParserNodeTypeKeywordConstant) && qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue() == "inherit")
    {
        QSharedPointer<HSSRefFunction> refFunc(new HSSRefFunction(this->getController()));
        refFunc->setPropertyName(propertyName);
        std::vector< QSharedPointer<HSSSelectorChain> > selectorChains;
        QSharedPointer<HSSSelectorChain> selectorChain(new HSSSelectorChain(this->getController()));
        QSharedPointer<HSSSimpleSelector> simpleSelector(new HSSSimpleSelector(this->getController()));
        simpleSelector->setName(QSharedPointer<HSSParentSelector>(new HSSParentSelector(this->getController())));
        selectorChain->add(simpleSelector);
        selectorChains.push_back(selectorChain);
        refFunc->setSelectorChains(selectorChains);
        parserNode = refFunc;
    }
    return qSharedPointerCast<HSSValue>(this->computeValueObject(parserNode));
}

void HSSObject::setComputedValue(AXRString propertyName, QSharedPointer<HSSParserNode> parserNode)
{
    this->setComputed(propertyName, this->computeValue(propertyName, parserNode));
}

void HSSObject::setComputedValue(AXRString propertyName, HSSUnit value)
{
    this->setComputedValue(propertyName, this->numberToConstant(value));
}

void HSSObject::setComputedBool(AXRString propertyName, bool value)
{
    this->setComputedValue(propertyName, this->stringToKeyword(value ? "yes" : "no"));
}

void HSSObject::setComputedValue(AXRString propertyName, AXRString value)
{
    this->setComputedValue(propertyName, this->stringToConstant(value));
}

QSharedPointer<HSSNumberConstant> HSSObject::numberToConstant(HSSUnit value)
{
    return QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(value, this->getController()));
}

QSharedPointer<HSSPercentageConstant> HSSObject::percentageToConstant(HSSUnit value)
{
    return QSharedPointer<HSSPercentageConstant>(new HSSPercentageConstant(value, this->getController()));
}

QSharedPointer<HSSStringConstant> HSSObject::stringToConstant(AXRString value)
{
    return HSSStringConstant::stringToConstant(value, this->getController());
}

QSharedPointer<HSSKeywordConstant> HSSObject::stringToKeyword(AXRString value)
{
    QSharedPointer<HSSKeywordConstant> ret = QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(value, this->getController()));
    return ret;
}

void HSSObject::setComputed(AXRString propertyName, QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSObject> currentValue = this->getComputedValue(propertyName);
    //FIXME: I don't know if this is comparing correctly
    if (!currentValue || currentValue != theObj)
    {
        //listen
        if (this->_listenCallbacks.contains(propertyName))
        {
            HSSAbstractObserveCallback * callback = this->_listenCallbacks[propertyName];
            callback->call(theObj);
        }
        else
        {
            if (theObj->isA(HSSObjectTypeValue))
            {
                theObj->observe("value", propertyName, this, new HSSValueChangedCallback<HSSObject>(this, &HSSObject::propertyChanged));
            }
        }
        //set
        if (this->_setCallbacks.contains(propertyName))
        {
            HSSAbstractObserveCallback * callback = this->_setCallbacks[propertyName];
            callback->call(theObj);
        }
        else
        {
            this->_computedValues.insert(propertyName, theObj);
        }
        //notify
        if (this->_notifyCallbacks.contains(propertyName))
        {
            HSSAbstractObserveCallback * callback = this->_notifyCallbacks[propertyName];
            callback->call(theObj);
        }
        else
        {
            this->notifyObservers(propertyName, theObj);
        }
    }
}

QSharedPointer<HSSObject> HSSObject::getComputedValue(AXRString property) const
{
    axr_log(LoggerChannelGeneralSpecific, AXRString("reading ").append(property).append(" of ").append(this->name));
    QSharedPointer<HSSObject> ret = this->_computedValues[property];
    if (ret && ret->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(ret)->getValue();
        if (parserNode->isA(HSSParserNodeTypeFunctionCall))
        {
            return qSharedPointerCast<HSSFunction>(parserNode)->evaluate();
        }
    }
    return ret;
}

const HSSUnit HSSObject::getComputedNumber(AXRString property) const
{
    QSharedPointer<HSSObject> value = this->getComputedValue(property);
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getNumber();
    }
    QSharedPointer<HSSObject> defaultValue = this->getDefaultValue(property);
    if (defaultValue && defaultValue->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(defaultValue)->getNumber();
    }
    return 0.;
}

const HSSUnit HSSObject::getComputedBool(AXRString property) const
{
    QSharedPointer<HSSObject> value = this->getComputedValue(property);
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getBool();
    }
    QSharedPointer<HSSObject> defaultValue = this->getDefaultValue(property);
    if (defaultValue && defaultValue->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(defaultValue)->getBool();
    }
    return FALSE;
}

const AXRString HSSObject::getComputedString(AXRString property) const
{
    QSharedPointer<HSSObject> value = this->getComputedValue(property);
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getString();
    }
    QSharedPointer<HSSObject> defaultValue = this->getDefaultValue(property);
    if (defaultValue && defaultValue->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(defaultValue)->getString();
    }
    return "";
}

void HSSObject::clearComputedValues()
{
    this->_computedValues.clear();
}

const QSharedPointer<HSSObject> HSSObject::inheritValue(AXRString propertyName) const
{
    QSharedPointer<HSSObject> ret;
    QSharedPointer<HSSDisplayObject> parent;
    if (this->isA(HSSObjectTypeContainer) || this->isA(HSSObjectTypeTextBlock))
    {
        parent = static_cast<const HSSDisplayObject*>(this)->getParent();
        if (parent)
        {
            ret = parent->getComputedValue(propertyName);
        }
        else
        {
            return QSharedPointer<HSSObject>();
        }
    }
    else
    {
        parent = this->thisObj->getParent();
        if (parent)
        {
            ret = parent->getComputedValue(propertyName);
        }
        else
        {
            return QSharedPointer<HSSObject>();
        }
    }
    return ret;
}

void HSSObject::inheritedChanged(const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    //source and target are the same name in this case
    this->setComputed(source, theObj);
}

void HSSObject::propertyChanged(const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    //this->notifyObservers(target, theObj);
}

