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
#include "HSSEvent.h"

using namespace AXR;

AXRString HSSEvent::eventTypeStringRepresentation(HSSEventType eventType)
{
    static std::map<HSSEventType, AXRString> types;
    if (types.empty())
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
        types[HSSEventTypeKeyDown] = "HSSEventTypeKeyDown";
        types[HSSEventTypeKeyUp] = "HSSEventTypeKeyUp";
    }

    return types[eventType];
}

AXRString HSSEvent::eventTypeToName(HSSEventType eventType)
{
    static std::map<HSSEventType, AXRString> types;
    if (types.empty())
    {
        types[HSSEventTypeNone] = "";
        types[HSSEventTypeLoad] = "load";
        types[HSSEventTypeFlag] = "flag";
        types[HSSEventTypeUnflag] = "unflag";
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
        types[HSSEventTypeKeyDown] = "keyDown";
        types[HSSEventTypeKeyUp] = "keyUp";
    }

    return types[eventType];
}

HSSEventType HSSEvent::nameToEventType(AXRString name)
{
    static std::map<AXRString, HSSEventType> types;
    if (types.empty())
    {
        types["load"] = HSSEventTypeLoad;
        types["flag"] = HSSEventTypeFlag;
        types["unflag"] = HSSEventTypeUnflag;
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

QSharedPointer<HSSEvent> HSSEvent::createEvent(AXRController * cntl, HSSInputEvent *event)
{
    QSharedPointer<HSSEvent> ret;
    switch (event->type())
    {
        case HSSEventTypeKeyUp:
        case HSSEventTypeKeyDown:
        {
            ret = QSharedPointer<HSSEvent>(new HSSEvent(cntl));
            HSSKeyboardEvent * kbEvent = static_cast<HSSKeyboardEvent *>(event);
            ret->setComputedValue("key", AXRString("\"")+kbEvent->keyCode()+AXRString("\""), 1);
            break;
        }
        default:
            break;
    }
    return ret;
}

HSSEvent::HSSEvent(AXRController * controller)
: HSSDisplayObject(HSSObjectTypeEvent, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSEvent: creating event object");
    
}

HSSEvent::HSSEvent(const HSSEvent & orig)
: HSSDisplayObject(orig)
{
    
}

QSharedPointer<HSSEvent> HSSEvent::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSEvent: cloning event object");
    return qSharedPointerCast<HSSEvent> (this->cloneImpl());
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
    HSSDisplayObject::setDefaults();
}

AXRString HSSEvent::defaultObjectType()
{
    return "event";
}

AXRString HSSEvent::toString()
{
    return "@event";
}
