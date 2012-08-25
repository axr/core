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
 *      COPYRIGHT: ©2012 - All Rights Reserved
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
 ********************************************************************/

#include <boost/unordered_map.hpp>
#include "AXR.h"
#include "AXRController.h"
#include "HSSMultipleValue.h"
#include "HSSMultipleValueDefinition.h"
#include "HSSObject.h"
#include "HSSObjects.h"
#include "HSSObjectNameConstant.h"
#include "HSSTypeEnums.h"

using namespace AXR;

HSSObject::p HSSObject::newObjectWithType(std::string type)
{

    static boost::unordered_map<std::string, HSSObjectType>types;
    if (types.size() == 0)
    {
        types["container"] = HSSObjectTypeContainer;
        types["displayObject"] = HSSObjectTypeDisplayObject;
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
    if (types.find(type) != types.end())
    {
        objectType = types[type];
    }

    switch (objectType)
    {
    case HSSObjectTypeContainer:
    {
        return HSSContainer::p(new HSSContainer());
    }

    case HSSObjectTypeDisplayObject:
    {
        return HSSDisplayObject::p(new HSSDisplayObject());
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
        static boost::unordered_map<std::string, HSSEventType>eventTypes;
        if (eventTypes.size() == 0)
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

        if (eventTypes.find(type) != eventTypes.end())
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
        throw AXRError::p(new AXRError("HSSObject", type));
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
    return boost::static_pointer_cast<HSSObject, HSSClonable > (this->cloneImpl());
}

HSSClonable::p HSSObject::cloneImpl() const
{
    return HSSClonable::p(new HSSObject(*this));
}

HSSObject::~HSSObject()
{

}

bool HSSObject::isKeyword(std::string value, std::string property)
{
    if (value == "default"
            || value == "inherit"
            || value == "undefined"
            || value == "none")
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool HSSObject::isFunction(std::string value, std::string property)
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
        return AXRCore::getInstance()->isCustomFunction(value);
    }
}

std::string HSSObject::toString()
{
    if (this->isNamed())
    {
        return std::string("HSSObject: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSObject";
    }
}

bool HSSObject::isNamed()
{
    return this->_isNamed;
}

void HSSObject::setName(std::string newName)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSObject: setting name to " + newName);
    this->name = newName;
    this->_isNamed = true;
}

std::string HSSObject::getName()
{
    return this->name;
}

void HSSObject::dropName()
{
    this->name = "";
    this->_isNamed = false;
}

std::string HSSObject::defaultObjectType()
{
    return "value";
}

std::string HSSObject::defaultObjectType(std::string property)
{
    return "value";
}

std::string HSSObject::getPropertyForCurrentValue()
{
    bool done = false;
    std::string ret;

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

void HSSObject::setShorthandProperties(std::vector<std::string> newValues)
{
    this->shorthandProperties = newValues;
    this->shorthandReset();
}

void HSSObject::shorthandSkip(std::string propertyName)
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
        HSSParserNode::it iterator;
        HSSMultipleValueDefinition::p multiDef = boost::static_pointer_cast<HSSMultipleValueDefinition > (value);
        std::vector<HSSParserNode::p> values = multiDef->getValues();
        for (iterator = values.begin(); iterator != values.end(); iterator++)
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
            HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
            //objdef->apply();
            std::deque<HSSPropertyDefinition::p> properties = objdef->getProperties();
            unsigned i, size;
            for (i = 0, size = properties.size(); i < size; i++)
            {
                HSSObservableProperty propertyName = HSSObservable::observablePropertyFromString(properties[i]->getName());
                if (propertyName != HSSObservablePropertyNone)
                {
                    try
                    {
                        this->setProperty(propertyName, properties[i]->getValue()->clone());
                    }
                    catch (AXRError::p e)
                    {
                        e->raise();
                    }
                    catch (AXRWarning::p e)
                    {
                        e->raise();
                    }
                }

                //else store as value
            }
            valid = true;

        }
        catch (AXRError::p e)
        {
            e->raise();
        }
        catch (AXRWarning::p e)
        {
            e->raise();
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        AXRError::p(new AXRError("HSSObject", "HSSParserNodeTypeKeywordConstant Unimplemented"))->raise();
        break;
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        AXRError::p(new AXRError("HSSObject", "HSSStatementTypeObjectDefinition Unimplemented in isA"))->raise();
        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning::p(new AXRWarning("HSSObject", "Invalid value for isA of " + this->name));

    this->notifyObservers(HSSObservablePropertyIsA, &this->dIsA);
}

void HSSObject::isAChanged(AXR::HSSObservableProperty source, void *data)
{

}

void HSSObject::setPropertyWithName(std::string name, HSSParserNode::p value)
{
    HSSObservableProperty property = HSSObservable::observablePropertyFromString(name);
    if (property != HSSObservablePropertyNone)
    {
        this->setProperty(property, value);
    }
    else
    {
        AXRWarning::p(new AXRWarning("HSSDisplayObject", "Unknown property " + name + ", ignoring value"))->raise();
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
        AXRWarning::p(new AXRWarning("HSSDisplayObject", "Unknown property " + HSSObservable::observablePropertyStringRepresentation(name) + ", ignoring value"))->raise();
    }
}

void HSSObject::setProperty(HSSObservableProperty name, void * value)
{
    AXRWarning::p(new AXRWarning("HSSDisplayObject", "Unknown property " + HSSObservable::observablePropertyStringRepresentation(name) + ", ignoring value"))->raise();
}

void * HSSObject::getProperty(HSSObservableProperty name)
{
    if (this->properties.find(name) == this->properties.end())
    {
        AXRError::p(new AXRError("HSSObject", "Unknown property " + HSSObservable::observablePropertyStringRepresentation(name)))->raise();
        return NULL;
    }
    return this->properties[name];
}

void HSSObject::registerProperty(HSSObservableProperty name, void * property)
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
