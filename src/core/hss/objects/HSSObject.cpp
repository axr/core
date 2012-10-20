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
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#include <QMap>
#include "AXRController.h"
#include "AXRDebugging.h"
#include "AXRInitializer.h"
#include "AXRWarning.h"
#include "HSSCircle.h"
#include "HSSColorStop.h"
#include "HSSLineBorder.h"
#include "HSSLinearGradient.h"
#include "HSSLog.h"
#include "HSSMultipleValue.h"
#include "HSSMultipleValueDefinition.h"
#include "HSSObject.h"
#include "HSSObjectNameConstant.h"
#include "HSSPolygon.h"
#include "HSSRectangle.h"
#include "HSSRequest.h"
#include "HSSRoundedRect.h"
#include "HSSTypeEnums.h"

using namespace AXR;

HSSObject::p HSSObject::newObjectWithType(AXRString type)
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
        return HSSContainer::p(new HSSContainer());
    }

    case HSSObjectTypeBorder:
    {
        /**
         *  @todo border tyes?
         */
        return HSSLineBorder::p(new HSSLineBorder());
    }

    case HSSObjectTypeGradient:
    {
        /**
         *  @todo gradient tyes?
         */
        return HSSLinearGradient::p(new HSSLinearGradient());
    }

    case HSSObjectTypeColorStop:
    {
        return HSSColorStop::p(new HSSColorStop());
    }

    case HSSObjectTypeValue:
    {
        return HSSValue::p(new HSSValue());
    }

    case HSSObjectTypeMargin:
    {
        return HSSMargin::p(new HSSMargin());
    }

    case HSSObjectTypeRgb:
    {
        return HSSRgb::p(new HSSRgb());
    }

    case HSSObjectTypeFont:
    {
        return HSSFont::p(new HSSFont());
    }

    case HSSObjectTypeShape:
    {
        if (type == "rectangle")
        {
            return HSSRectangle::p(new HSSRectangle());
        }
        else if (type == "roundedRect")
        {
            return HSSRoundedRect::p(new HSSRoundedRect());
        }
        else if (type == "circle")
        {
            return HSSCircle::p(new HSSCircle());
        }
        else if (type == "polygon")
        {
            return HSSPolygon::p(new HSSPolygon());
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
            return HSSEvent::p(new HSSEvent(eventTypes[type]));
        }

        //fall through
    }

    case HSSObjectTypeAction:
    {
        if (type == "request")
        {
            return HSSRequest::p(new HSSRequest());
        }
        else if (type == "log")
        {
            return HSSLog::p(new HSSLog());
        }
    }

    default:
        throw AXRError("HSSObject", type);
    }

    return HSSObject::p();
}

HSSObject::HSSObject(HSSObjectType type)
{
    this->_isNamed = false;
    this->name = "";
    this->type = type;
    this->shorthandIndex = 0;
}

HSSObject::HSSObject(const HSSObject & orig)
{
    this->name = orig.name;
    this->type = orig.type;
    this->_isNamed = orig._isNamed;
    this->scope = orig.scope;
    this->thisObj = orig.thisObj;
    this->axrController = orig.axrController;
    this->shorthandIndex = orig.shorthandIndex;

}

HSSObject::p HSSObject::clone() const
{
    return qSharedPointerCast<HSSObject, HSSClonable > (this->cloneImpl());
}

HSSClonable::p HSSObject::cloneImpl() const
{
    return HSSClonable::p(new HSSObject(*this));
}

HSSObject::~HSSObject()
{

}

bool HSSObject::isKeyword(AXRString value, AXRString property)
{
    return (value == "default" || value == "inherit" || value == "undefined" || value == "yes" || value == "no");
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
            || value == "attr"
            )
    {
        return true;
    }
    else
    {
        return axrController->document()->isCustomFunction(value);
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

bool HSSObject::isNamed()
{
    return this->_isNamed;
}

void HSSObject::setName(AXRString newName)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSObject: setting name to " + newName);
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

unsigned HSSObject::getShorthandIndex()
{
    return this->shorthandIndex;
}

void HSSObject::setShorthandIndex(unsigned newValue)
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

HSSParserNode::p HSSObject::getDIsA()
{
    return this->dIsA;
}

void HSSObject::setDIsA(HSSParserNode::p value)
{
    this->dIsA = value;
    this->addDIsA(value);
}

void HSSObject::addDIsA(HSSParserNode::p value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSMultipleValueDefinition::p multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<HSSParserNode::p> values = multiDef->getValues();
        for (HSSParserNode::it iterator = values.begin(); iterator != values.end(); ++iterator)
        {
            this->addDIsA(*iterator);
        }

        valid = true;
        break;
    }

    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
            //objdef->apply();
            std::deque<HSSPropertyDefinition::p> properties = objdef->getProperties();

            for (unsigned i = 0, size = properties.size(); i < size; ++i)
            {
                HSSObservableProperty propertyName = HSSObservable::observablePropertyFromString(properties[i]->getName());
                if (propertyName != HSSObservablePropertyNone)
                {
                    try
                    {
                        this->setProperty(propertyName, properties[i]->getValue()->clone());
                    }
                    catch (const AXRError &e)
                    {
                        e.raise();
                    }
                    catch (const AXRWarning &e)
                    {
                        e.raise();
                    }
                }

                //else store as value
            }

            valid = true;
        }
        catch (const AXRError &e)
        {
            e.raise();
        }
        catch (const AXRWarning &e)
        {
            e.raise();
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        AXRError("HSSObject", "HSSParserNodeTypeKeywordConstant Unimplemented").raise();
        break;
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        AXRError("HSSObject", "HSSStatementTypeObjectDefinition Unimplemented in isA").raise();
        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSObject", "Invalid value for isA of " + this->name);

    this->notifyObservers(HSSObservablePropertyIsA, &this->dIsA);
}

void HSSObject::isAChanged(AXR::HSSObservableProperty source, void *data)
{

}

void HSSObject::setPropertyWithName(AXRString name, HSSParserNode::p value)
{
    HSSObservableProperty property = HSSObservable::observablePropertyFromString(name);
    if (property != HSSObservablePropertyNone)
    {
        this->setProperty(property, value);
    }
    else
    {
        AXRWarning("HSSDisplayObject", "Unknown property " + name + ", ignoring value").raise();
    }
}

void HSSObject::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    if (name == HSSObservablePropertyIsA)
    {
        this->setDIsA(value);

    }
    else
    {
        AXRWarning("HSSDisplayObject", "Unknown property " + HSSObservable::observablePropertyStringRepresentation(name) + ", ignoring value").raise();
    }
}

void HSSObject::setProperty(HSSObservableProperty name, QVariant value)
{
    AXRWarning("HSSDisplayObject", "Unknown property " + HSSObservable::observablePropertyStringRepresentation(name) + ", ignoring value").raise();
}

QVariant HSSObject::getProperty(HSSObservableProperty name)
{
    if (this->properties.find(name) == this->properties.end())
    {
        AXRError("HSSObject", "Unknown property " + HSSObservable::observablePropertyStringRepresentation(name)).raise();
        return QVariant();
    }
    return this->properties[name];
}

void HSSObject::registerProperty(HSSObservableProperty name, QVariant property)
{
    this->properties[name] = property;
}

const std::vector<HSSDisplayObject::p> * HSSObject::getScope() const
{
    return this->scope;
}

void HSSObject::setScope(const std::vector<HSSDisplayObject::p> * newScope)
{
    this->scope = newScope;
}

void HSSObject::setController(AXRController * controller)
{
    this->axrController = controller;
}

AXRController * HSSObject::getController()
{
    return this->axrController;
}

void HSSObject::setThisObj(HSSDisplayObject::p value)
{
    this->thisObj = value;
}

HSSDisplayObject::p HSSObject::getThisObj()
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

HSSObject::p HSSObject::shared_from_this()
{
    if (!ptr)
        ptr = QWeakPointer<HSSObject>(this);

    return HSSObject::p(ptr);
}
