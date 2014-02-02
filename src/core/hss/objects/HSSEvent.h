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

#ifndef HSSEVENT_H
#define HSSEVENT_H

#include "HSSObject.h"

namespace AXR
{
    class HSSAction;
    class HSSParserNode;

    /**
     *  @brief Represents all event objects in HSS.
     *
     *  All event objects, such as \@click, \@keyDown, etc, have the same behavior, they
     *  fire their assigned action, so only one class is needed.
     */
    class AXR_API HSSEvent
    {
    public:
        /**
         *  Print the type as a string, useful for logging.
         *  @param  eventType   The event type that will be printed as a string.
         */
        static AXRString eventTypeStringRepresentation(HSSEventType eventType);

        static AXRString eventTypeToName(HSSEventType eventType);
        static HSSEventType nameToEventType(AXRString name);

        /**
         *  Executes the action which is stored in this event object.
         */
        static void fire(QSharedPointer<HSSObject> theObj);

    private:
        static void _fire(QSharedPointer<HSSParserNode> parserNode);
    };
}

#endif
