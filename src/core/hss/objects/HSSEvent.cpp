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
#include "HSSDisplayObject.h"
#include "HSSFunction.h"
#include "HSSEvent.h"
#include "HSSMultipleValue.h"
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

void HSSEvent::fire(QSharedPointer<HSSObject> theObj)
{
    if (theObj)
    {
        if (theObj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
            HSSEvent::_fire(parserNode);
        }
        else if (theObj->isA(HSSObjectTypeMultipleValue))
        {
            Q_FOREACH(const QSharedPointer<HSSObject> & aObj, qSharedPointerCast<HSSMultipleValue>(theObj)->getValues())
            {
                if (aObj->isA(HSSObjectTypeValue))
                {
                    QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(aObj)->getValue();
                    HSSEvent::_fire(parserNode);
                }
            }
        }
    }
}

void HSSEvent::_fire(QSharedPointer<HSSParserNode> parserNode)
{
    if (parserNode->isA(HSSParserNodeTypeFunctionCall))
    {
        QSharedPointer<HSSFunction> theFunction = qSharedPointerCast<HSSFunction>(parserNode);
        theFunction->evaluate();
    }
}
