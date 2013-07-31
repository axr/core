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
#include "HSSKeywordConstant.h"
#include "HSSMultipleValue.h"
#include "HSSMultipleValueDefinition.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSSimpleSelection.h"
#include "HSSValue.h"

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

AXRString HSSEvent::eventTypeToName(HSSEventType eventType)
{
    static QMap<HSSEventType, AXRString> types;
    if (types.isEmpty())
    {
        types[HSSEventTypeNone] = "";
        types[HSSEventTypeLoad] = "load";
        types[HSSEventTypeClick] = "click";
        types[HSSEventTypeDoubleClick] = "doubleClick";
        types[HSSEventTypeTripleClick] = "tripleClick";
        types[HSSEventTypeMouseDown] = "mouseDown";
        types[HSSEventTypeMouseUp] = "mouseUp";
        types[HSSEventTypeMouseOver] = "mouseOver";
        types[HSSEventTypeMouseOut] = "mouseOut";
        types[HSSEventTypeMouseHold] = "mouseHold";
        types[HSSEventTypeMouseMove] = "mouseMove";
        types[HSSEventTypeClickSecondary] = "clickSecondary";
        types[HSSEventTypeClickTertiary] = "clickTertiary";
        types[HSSEventTypeScroll] = "scroll";
        types[HSSEventTypeExitedWindow] = "exitedWindow";
    }

    return types[eventType];
}

HSSEventType HSSEvent::nameToEventType(AXRString name)
{
    static QMap<AXRString, HSSEventType> types;
    if (types.isEmpty())
    {
        types["load"] = HSSEventTypeLoad;
        types["click"] = HSSEventTypeClick;
        types["doubleClick"] = HSSEventTypeDoubleClick;
        types["tripleClick"] = HSSEventTypeTripleClick;
        types["mouseDown"] = HSSEventTypeMouseDown;
        types["mouseUp"] = HSSEventTypeMouseUp;
        types["mouseOver"] = HSSEventTypeMouseOver;
        types["mouseOut"] = HSSEventTypeMouseOut;
        types["mouseHold"] = HSSEventTypeMouseHold;
        types["mouseMove"] = HSSEventTypeMouseMove;
        types["clickSecondary"] = HSSEventTypeClickSecondary;
        types["clickTertiary"] = HSSEventTypeClickTertiary;
        types["scroll"] = HSSEventTypeScroll;
        types["exitedWindow"] = HSSEventTypeExitedWindow;
    }

    return types[name];
}

HSSEvent::HSSEvent(HSSEventType type, AXRController * controller)
: HSSObject(HSSObjectTypeEvent, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSEvent: creating event object");
    this->eventType = type;

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("action");
    this->setShorthandProperties(shorthandProperties);
}

HSSEvent::HSSEvent(const HSSEvent & orig)
: HSSObject(orig)
{
    this->eventType = orig.eventType;
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
    this->cleanTrackedObservers();
}

void HSSEvent::setDefaults()
{
    this->setDefaultKw("action", "no");
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

const bool HSSEvent::isA(HSSEventType type) const
{
    return type == this->eventType;
}

const HSSEventType HSSEvent::getEventType() const
{
    return this->eventType;
}

void HSSEvent::fire()
{
    QSharedPointer<HSSObject> actionObj = this->getAction();
    if (actionObj)
    {
        if (actionObj->isA(HSSObjectTypeAction))
        {
            qSharedPointerCast<HSSAction>(actionObj)->fire();
        }
        else if (actionObj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(actionObj)->getValue();
            if (parserNode->isA(HSSParserNodeTypeFunctionCall) && qSharedPointerCast<HSSFunction>(parserNode)->isA(HSSFunctionTypeFlag))
            {
                QSharedPointer<HSSFlagFunction> flagFunction = qSharedPointerCast<HSSFlagFunction>(parserNode);
                this->_fireFlagFunction(flagFunction);
            }
        }
        else if (actionObj->isA(HSSObjectTypeMultipleValue))
        {
            Q_FOREACH(const QSharedPointer<HSSObject> & aObj, qSharedPointerCast<HSSMultipleValue>(actionObj)->getValues())
            {
                if (aObj->isA(HSSObjectTypeAction))
                {
                    qSharedPointerCast<HSSAction>(aObj)->fire();
                }
                else if (aObj->isA(HSSObjectTypeValue))
                {
                    QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(aObj)->getValue();
                    if (parserNode->isA(HSSParserNodeTypeFunctionCall) && qSharedPointerCast<HSSFunction>(parserNode)->isA(HSSFunctionTypeFlag))
                    {
                        QSharedPointer<HSSFlagFunction> flagFunction = qSharedPointerCast<HSSFlagFunction>(parserNode);
                        this->_fireFlagFunction(flagFunction);
                    }
                }
            }
        }
    }
}

const QSharedPointer<HSSObject> HSSEvent::getAction() const
{
    return this->getComputedValue("action");
}

void HSSEvent::_fireFlagFunction(QSharedPointer<HSSFlagFunction> flagFunction)
{
    QSharedPointer<HSSDisplayObject> thisObj = this->getThisObj();
    QSharedPointer<HSSSelection> selection = this->getController()->select(flagFunction->getSelectorChains(), this->scope, thisObj);
    QSharedPointer<HSSSimpleSelection> inner = selection->joinAll();
    HSSFlagFunctionType type = flagFunction->getFlagFunctionType();
    const AXRString & flagName = flagFunction->getName();
    if(type == HSSFlagFunctionTypeTakeFlag){
        for (HSSSimpleSelection::iterator innerIt = inner->begin(); innerIt != inner->end(); ++innerIt)
        {
            QSharedPointer<HSSDisplayObject> container = *innerIt;
            if(container != thisObj)
            {
                container->flagsDeactivate(flagName);
            }
        }
        thisObj->flagsActivate(flagName);
    }
    else
    {
        for (HSSSimpleSelection::iterator innerIt = inner->begin(); innerIt != inner->end(); ++innerIt)
        {
            QSharedPointer<HSSDisplayObject> container = *innerIt;
            switch (flagFunction->getFlagFunctionType())
            {
                case HSSFlagFunctionTypeFlag:
                    container->flagsActivate(flagName);
                    break;
                case HSSFlagFunctionTypeUnflag:
                    container->flagsDeactivate(flagName);
                    break;
                case HSSFlagFunctionTypeToggleFlag:
                    container->flagsToggle(flagName);
                    break;

                default:
                    throw AXRWarning("HSSFlagAction", "Invalid flag function type for flag action");
                    break;
            }
        }
    }
}
