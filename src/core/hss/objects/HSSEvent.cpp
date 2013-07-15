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
        else if (actionObj->isA(HSSObjectTypeMultipleValue))
        {
            Q_FOREACH(const QSharedPointer<HSSObject> & aObj, qSharedPointerCast<HSSMultipleValue>(actionObj)->getValues())
            {
                if (aObj->isA(HSSObjectTypeAction))
                {
                    qSharedPointerCast<HSSAction>(aObj)->fire();
                }
            }
        }
    }
}

const QSharedPointer<HSSObject> HSSEvent::getAction() const
{
    return this->getComputedValue("action");
}
