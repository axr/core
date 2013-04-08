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
#include "AXRController.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSAction.h"
#include "HSSCallback.h"
#include "HSSDisplayObject.h"
#include "HSSEvent.h"
#include "HSSFlagAction.h"
#include "HSSFlagFunction.h"
#include "HSSFunction.h"
#include "HSSFunctionAction.h"
#include "HSSMultipleValueDefinition.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

AXRString HSSEvent::eventTypeStringRepresentation(HSSEventType eventType)
{
    static QMap<HSSEventType, AXRString> types;
    if (types.isEmpty())
    {
        types[HSSEventTypeNone] = "HSSEventTypeNone";
        types[HSSEventTypeLoad] = "HSSEventTypeLoad";
        types[HSSEventTypeClick] = "HSSEventTypeClick";
        types[HSSEventTypeDoubleClick] = "HSSEventTypeDoubleClick";
        types[HSSEventTypeTripleClick] = "HSSEventTypeTripleClick";
        types[HSSEventTypeMouseDown] = "HSSEventTypeMouseDown";
        types[HSSEventTypeMouseUp] = "HSSEventTypeMouseUp";
        types[HSSEventTypeMouseOver] = "HSSEventTypeMouseOver";
        types[HSSEventTypeMouseOut] = "HSSEventTypeMouseOut";
        types[HSSEventTypeMouseHold] = "HSSEventTypeMouseHold";
        types[HSSEventTypeMouseMove] = "HSSEventTypeMouseMove";
        types[HSSEventTypeClickSecondary] = "HSSEventTypeClickSecondary";
        types[HSSEventTypeClickTertiary] = "HSSEventTypeClickTertiary";
        types[HSSEventTypeScroll] = "HSSEventTypeScroll";
        types[HSSEventTypeExitedWindow] = "HSSEventTypeExitedWindow";
    }

    return types[eventType];
}

HSSEvent::HSSEvent(HSSEventType type, AXRController * controller)
: HSSObject(HSSObjectTypeEvent, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSEvent: creating event object");
    this->eventType = type;

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("action");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertyAction, QVariant::fromValue(&this->action));
}

HSSEvent::HSSEvent(const HSSEvent & orig)
: HSSObject(orig)
{
    this->eventType = orig.eventType;

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("action");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertyAction, QVariant::fromValue(&this->action));
}

QSharedPointer<HSSEvent> HSSEvent::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSEvent: cloning event object");
    return qSharedPointerCast<HSSEvent>(this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSEvent::cloneImpl() const
{
    return QSharedPointer<HSSEvent>(new HSSEvent(*this));
}

HSSEvent::~HSSEvent()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSEvent: destructing event object");
}

AXRString HSSEvent::toString()
{
    return "Generic HSSEvent of type" + HSSEvent::eventTypeStringRepresentation(this->eventType);
}

AXRString HSSEvent::defaultObjectType()
{
    return "event";
}

AXRString HSSEvent::defaultObjectType(AXRString property)
{
    if (property == "action")
    {
        return "request";
    }
    else
    {
        return HSSObject::defaultObjectType(property);
    }
}

void HSSEvent::setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value)
{
    switch (name)
    {
    case HSSObservablePropertyAction:
        this->setDAction(value);
        break;

    default:
        HSSObject::setProperty(name, value);
        break;
    }
}

bool HSSEvent::isA(HSSEventType type)
{
    return type == this->eventType;
}

HSSEventType HSSEvent::getEventType()
{
    return this->eventType;
}

std::vector<QSharedPointer<HSSAction> > HSSEvent::getAction()
{
    return this->action;
}

const QSharedPointer<HSSParserNode> HSSEvent::getDAction() const
{
    return this->dAction;
}

void HSSEvent::setDAction(QSharedPointer<HSSParserNode> value)
{
    this->action.clear();
    this->dAction = value;
    this->addDAction(value);
}

void HSSEvent::addDAction(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        QSharedPointer<HSSMultipleValueDefinition> multiDef = qSharedPointerCast<HSSMultipleValueDefinition>(value);
        std::vector<QSharedPointer<HSSParserNode> > values = multiDef->getValues();
        for (HSSParserNode::it iterator = values.begin(); iterator != values.end(); ++iterator)
        {
            this->addDAction(*iterator);
        }

        valid = true;
        break;
    }

    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant>(value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeNodeNamed(objname->getValue())->clone();
            objdef->setScope(this->scope);
            objdef->setThisObj(this->getThisObj());
            objdef->apply();

            QSharedPointer<HSSObject> obj = objdef->getObject();
            switch (obj->getObjectType())
            {
            case HSSObjectTypeAction:
                this->action.push_back(qSharedPointerCast<HSSAction>(obj));
                valid = true;
                break;

            default:
                break;
            }

        }
        catch (const AXRError &e)
        {
            e.raise();
        }

        break;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction>(value)->clone();
        switch (fnct->getFunctionType())
        {
        case HSSFunctionTypeRef:
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<QSharedPointer<HSSParserNode> >())
            {
                try
                {
                    QSharedPointer<HSSParserNode> theVal = remoteValue.value<QSharedPointer<HSSParserNode> >();
                    this->addDAction(theVal);
                    valid = true;
                }
                catch (const AXRError &e)
                {
                    e.raise();
                }
            }

            break;
        }

        case HSSFunctionTypeFlag:
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            QSharedPointer<HSSFlagFunction> flagFnct = qSharedPointerCast<HSSFlagFunction>(fnct);
            QSharedPointer<HSSFlagAction> flagAction = QSharedPointer<HSSFlagAction>(new HSSFlagAction(this->getController()));
            flagAction->setFlagFunction(flagFnct);
            flagAction->setScope(this->scope);
            flagAction->setThisObj(this->getThisObj());
            this->action.push_back(flagAction);
            valid = true;
            break;
        }

        case HSSFunctionTypeCustom:
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            QSharedPointer<HSSFunction> theFnct = qSharedPointerCast<HSSFunction>(fnct);
            QSharedPointer<HSSFunctionAction> fnctAction = QSharedPointer<HSSFunctionAction>(new HSSFunctionAction(this->getController()));
            fnctAction->setFunction(theFnct);
            fnctAction->setScope(this->scope);
            fnctAction->setThisObj(this->getThisObj());
            this->action.push_back(fnctAction);
            valid = true;
            break;
        }

        default:
            break;
        }

        break;
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition>(value)->clone();
        if (objdef->getObject()->isA(HSSObjectTypeAction))
        {
            objdef->setScope(this->scope);
            objdef->setThisObj(this->getThisObj());
            objdef->apply();
            QSharedPointer<HSSObject> theObj = objdef->getObject();
            theObj->observe(HSSObservablePropertyValue, HSSObservablePropertyAction, this, new HSSValueChangedCallback<HSSEvent > (this, &HSSEvent::actionChanged));
            this->action.push_back(qSharedPointerCast<HSSAction>(theObj));
            valid = true;
        }
        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDisplayObject", "Invalid value for action of @event " + this->name);

    this->notifyObservers(HSSObservablePropertyAction, &this->action);
}

void HSSEvent::actionChanged(HSSObservableProperty source, void*data)
{
    axr_log(LoggerChannelObsolete0, "HSSEvent::actionChanged unimplemented");
}

void HSSEvent::fire()
{
    std::vector<QSharedPointer<HSSAction> > actions = this->getAction();

    for (size_t i = 0; i < actions.size(); ++i)
    {
        actions[i]->fire();
    }
}
