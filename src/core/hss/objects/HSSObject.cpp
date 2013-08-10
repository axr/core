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
#include "HSSLog.h"
#include "HSSMargin.h"
#include "HSSMultipleValue.h"
#include "HSSMultipleValueDefinition.h"
#include "HSSMultiplication.h"
#include "HSSNumberConstant.h"
#include "HSSObject.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSParentSelector.h"
#include "HSSPercentageConstant.h"
#include "HSSPolygon.h"
#include "HSSPropertyDefinition.h"
#include "HSSRadialGradient.h"
#include "HSSRectangle.h"
#include "HSSRefFunction.h"
#include "HSSRequest.h"
#include "HSSRgb.h"
#include "HSSRoundedRect.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelection.h"
#include "HSSSimpleSelector.h"
#include "HSSStringConstant.h"
#include "HSSThisSelector.h"
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
    this->_hostProperty = "";
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
    this->_hostProperty = orig._hostProperty;

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
    //copy the stack values
    QMapIterator<AXRString, QSharedPointer<HSSObject> > stackIt(orig._stackValues);
    while (stackIt.hasNext())
    {
        stackIt.next();
        this->_stackValues.insert(stackIt.key(), stackIt.value()->clone());
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
    this->addCallback("isA", new HSSStackCallback<HSSObject>(this, &HSSObject::stackIsA));
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

void HSSObject::stackIsA(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeObjectNameConstant:
        {
            try
            {
                QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (parserNode);
                QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeNodeNamed(objname->getValue());
                std::deque<QSharedPointer<HSSPropertyDefinition> > properties = objdef->getProperties();

                for (size_t i = 0; i < properties.size(); ++i)
                {
                    QVector<QVector<AXRString> > paths = properties[i]->getPaths();
                    Q_FOREACH(QVector<AXRString> path, paths)
                    {
                        this->setStackNode(path.front(), properties[i]->getValue()->clone());
                    }
                }
            }
            catch (const AXRError &e)
            {
                e.raise();
            }

            break;
        }

        default:
            break;
    }
}

QSharedPointer<HSSSimpleSelection> HSSObject::getScope() const
{
    return this->scope;
}

void HSSObject::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    this->scope = newScope;
    //propagate values
    QMapIterator<AXRString, QSharedPointer<HSSObject> > computedIt(this->_computedValues);
    while (computedIt.hasNext())
    {
        computedIt.next();
        computedIt.value()->setScope(newScope);
    }
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
    //propagate values
    QMapIterator<AXRString, QSharedPointer<HSSObject> > computedIt(this->_computedValues);
    while (computedIt.hasNext())
    {
        computedIt.next();
        computedIt.value()->setThisObj(value);
    }
}

QSharedPointer<HSSDisplayObject> HSSObject::getThisObj() const
{
    return thisObj;
}

const bool HSSObject::isA(HSSObjectType otherType) const
{
    return otherType == this->type;
}

const HSSObjectType HSSObject::getObjectType() const
{
    return this->type;
}

const bool HSSObject::isA(HSSShapeType otherType) const
{
    return false;
}

const HSSShapeType HSSObject::getShapeType() const
{
    return HSSShapeTypeNone;
}

const bool HSSObject::isA(HSSRequestModeType otherType) const
{
    return false;
}

const HSSRequestModeType HSSObject::getRequestModeType() const
{
    return HSSRequestModeTypeNone;
}

const bool HSSObject::isA(HSSEventType otherType) const
{
    return false;
}

const HSSEventType HSSObject::getEventType() const
{
    return HSSEventTypeNone;
}

const bool HSSObject::isA(HSSActionType otherType) const
{
    return false;
}

const HSSActionType HSSObject::getActionType() const
{
    return HSSActionTypeNone;
}

const bool HSSObject::isA(HSSGradientType otherType) const
{
    return false;
}

const HSSGradientType HSSObject::getGradientType() const
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

bool HSSObject::hasStackValue(AXRString property) const
{
    return this->_stackValues.contains(property);
}

const QSharedPointer<HSSObject> HSSObject::getStackValue(AXRString property) const
{
    if (this->hasStackValue(property))
    {
        return this->_stackValues.value(property);
    }
    return QSharedPointer<HSSObject>();
}

void HSSObject::clearStackValues()
{
    this->_stackValues.clear();
}

void HSSObject::setStackNode(AXRString propertyName, QSharedPointer<AXR::HSSParserNode> parserNode)
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
    if (parserNode->isA(HSSParserNodeTypeObjectNameConstant))
    {
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (parserNode);
            parserNode = this->getController()->objectTreeNodeNamed(objname->getValue())->clone();
        }
        catch (const AXRError &e)
        {
            e.raise();
        }
    }
    if (parserNode->isA(HSSStatementTypeObjectDefinition))
    {
        qSharedPointerCast<HSSObjectDefinition>(parserNode)->applyStack();
    }
    this->setStackValue(propertyName, this->computeValue(propertyName, parserNode));
}

void HSSObject::setStackValue(AXRString propertyName, QSharedPointer<HSSObject> theObject)
{
    //default handling
    this->_setStackValue(propertyName, theObject);
}

void HSSObject::appendStackValue(AXRString propertyName, QSharedPointer<HSSObject> theObject)
{
    if (!this->_stackValues.contains(propertyName))
    {
        this->_setStackValue(propertyName, theObject);
    }
    else
    {
        QSharedPointer<HSSObject> stackValue = this->_stackValues[propertyName];
        if (stackValue)
        {
            if (stackValue->isA(HSSObjectTypeMultipleValue))
            {
                QSharedPointer<HSSMultipleValue> multiVal = qSharedPointerCast<HSSMultipleValue> (stackValue);
                multiVal->add(theObject);
            }
            else
            {
                QSharedPointer<HSSMultipleValue> multiVal(new HSSMultipleValue(this->getController()));
                multiVal->add(stackValue);
                multiVal->add(theObject);
                this->_setStackValue(propertyName, multiVal);
            }
        }
    }
}

void HSSObject::_setStackValue(AXRString propertyName, QSharedPointer<HSSObject> theObject)
{
    switch (theObject->getObjectType())
    {
        case HSSObjectTypeMultipleValue:
        {
            if (this->_stackValues.contains(propertyName))
            {
                this->_stackValues.remove(propertyName);
            }
            QList<QSharedPointer<HSSObject> >::iterator iterator;
            QSharedPointer<HSSMultipleValue> multiVal = qSharedPointerCast<HSSMultipleValue> (theObject);
            QList<QSharedPointer<HSSObject> > values = multiVal->getValues();
            for (iterator = values.begin(); iterator != values.end(); ++iterator)
            {
                this->appendStackValue(propertyName, *iterator);
            }
            return;
        }

        default:
            break;
    }
    this->_stackValues.insert(propertyName, theObject);
}

void HSSObject::commitStackValues()
{
    QMap<AXRString, QSharedPointer<HSSObject> >::iterator it;
    for (it = this->_stackValues.begin(); it != this->_stackValues.end(); ++it)
    {
        this->setComputed(it.key(), it.value());
    }
}

QSharedPointer<HSSObject> HSSObject::computeValue(AXRString propertyName, QSharedPointer<HSSParserNode> parserNode)
{
    //set the hosting property
    parserNode->setHostProperty(propertyName);

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
    if (!parserNode)
    {
        AXRWarning("HSSObject", "Creating value object from empty parser node.");
        return QSharedPointer<HSSValue>();
    }
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            if (qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue() == "inherit")
            {
                parserNode = this->_inheritProperty(propertyName);
            }
            break;
        }

        case HSSParserNodeTypePercentageConstant:
        {
            parserNode = this->getPercentageExpression(parserNode, propertyName);
            break;
        }

        default:
            break;
    }

    QSharedPointer<HSSValue> ret = qSharedPointerCast<HSSValue>(this->computeValueObject(parserNode));
    ret->setHostProperty(propertyName);
    return ret;
}

QSharedPointer<HSSParserNode> HSSObject::getPercentageExpression(QSharedPointer<HSSParserNode> parserNode, AXRString propertyName)
{
    if (!parserNode || !parserNode->isA(HSSParserNodeTypePercentageConstant))
    {
        AXRError("HSSObject", "You can only create a percentage expression from a percentage constant.");
        return QSharedPointer<HSSParserNode>();
    }

    //by default, get the same property from the parent
    HSSUnit number = qSharedPointerCast<HSSPercentageConstant>(parserNode)->getNumber();
    return this->getPercentageExpressionFromParent(number, propertyName);
}

QSharedPointer<HSSParserNode> HSSObject::getPercentageExpressionFromParent(HSSUnit number, AXRString propertyName)
{
    QSharedPointer<HSSParentSelector> parentSelector(new HSSParentSelector(this->getController()));
    QSharedPointer<HSSSimpleSelector> simpleSelector(new HSSSimpleSelector(this->getController()));
    simpleSelector->setName(parentSelector);
    return this->getPercentageExpression(number, simpleSelector, propertyName);
}

QSharedPointer<HSSParserNode> HSSObject::getPercentageExpressionFromThis(HSSUnit number, AXRString propertyName)
{
    QSharedPointer<HSSThisSelector> thisSelector(new HSSThisSelector(this->getController()));
    QSharedPointer<HSSSimpleSelector> simpleSelector(new HSSSimpleSelector(this->getController()));
    simpleSelector->setName(thisSelector);
    return this->getPercentageExpression(number, simpleSelector, propertyName);
}

QSharedPointer<HSSParserNode> HSSObject::getPercentageExpression(HSSUnit number, QSharedPointer<HSSSimpleSelector> target, AXRString propertyName)
{
    //left
    QSharedPointer<HSSRefFunction> refFunc(new HSSRefFunction(this->getController()));
    refFunc->setPropertyName(propertyName);
    std::vector< QSharedPointer<HSSSelectorChain> > selectorChains;
    QSharedPointer<HSSSelectorChain> selectorChain(new HSSSelectorChain(this->getController()));

    selectorChain->add(target);
    selectorChains.push_back(selectorChain);
    refFunc->setSelectorChains(selectorChains);

    //right
    QSharedPointer<HSSNumberConstant> right = this->numberToConstant(number);

    QSharedPointer<HSSMultiplication> ret(new HSSMultiplication(refFunc, right, this->getController()));
    return ret;
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
    theObj->setHostProperty(propertyName);
    //FIXME: I don't know if this is comparing correctly
    if (!currentValue || currentValue != theObj)
    {
        //prepare
        theObj->commitStackValues();
        theObj->fillWithDefaults();
        if (this->isA(HSSObjectTypeContainer) || this->isA(HSSObjectTypeTextBlock))
        {
            QSharedPointer<HSSDisplayObject> thisDO = qSharedPointerCast<HSSDisplayObject>(this->shared_from_this());
            theObj->setThisObj(thisDO);
            QSharedPointer<HSSContainer> parent = thisDO->getParent();
            if (parent)
            {
                theObj->setScope(parent->getChildren());
            }
            else
            {
                QSharedPointer<HSSSimpleSelection> rootScope(new HSSSimpleSelection());
                rootScope->add(thisDO);
                theObj->setScope(rootScope);
            }
        }
        else
        {
            theObj->setThisObj(this->getThisObj());
            theObj->setScope(this->getScope());
        }
        
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
                switch (qSharedPointerCast<HSSValue>(theObj)->getValue()->getType())
                {
                    case HSSParserNodeTypeExpression:
                    case HSSParserNodeTypeFunctionCall:
                    {
                        theObj->observe("valueChanged", propertyName, this, new HSSValueChangedCallback<HSSObject>(this, &HSSObject::propertyChanged));
                        break;
                    }

                    default:
                        break;
                }
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
    QSharedPointer<HSSObject> ret;
    if (this->_computedValues.contains(property))
    {
        ret = this->_computedValues[property];
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

const QSharedPointer<HSSObject> HSSObject::inheritValue(AXRString inheritProperty, AXRString hostProperty)
{
    return this->computeValueObject(this->_inheritProperty(inheritProperty), hostProperty);
}

const QSharedPointer<HSSParserNode> HSSObject::_inheritProperty(AXRString propertyName) const
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
    return refFunc;
}

void HSSObject::propertyChanged(const AXRString target, const AXRString source, QSharedPointer<HSSObject> theObj)
{
    this->notifyObservers(source, theObj);
}

const AXRString HSSObject::getHostProperty() const
{
    return this->_hostProperty;
}

void HSSObject::setHostProperty(AXRString newValue)
{
    this->_hostProperty = newValue;
}
