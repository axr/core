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
    class HSSFlagFunction;

    /**
     *  @brief Represents all event objects in HSS.
     *
     *  All event objects, such as \@click, \@keyDown, etc, have the same behavior, they
     *  fire their assigned action, so only one class is needed.
     */
    class AXR_API HSSEvent : public HSSObject
    {
    public:
        /**
         *  Print the type as a string, useful for logging.
         *  @param  eventType   The event type that will be printed as a string.
         */
        static AXRString eventTypeStringRepresentation(HSSEventType eventType);

        /**
         *  Constructor for HSSEvent objects.
         *  @param  type    The event type that this event will respond to.
         */
        HSSEvent(HSSEventType type, AXRController * controller);
        /**
         *  Copy constructor for HSSEvent objects. Do not use directly, use clone() instead.
         */
        HSSEvent(const HSSEvent & orig);
        /**
         *  Clones an instance of HSSEvent and gives a shared pointer of the newly instanciated
         *  object.
         *  @return A shared pointer to the new HSSEvent.
         */
        QSharedPointer<HSSEvent> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSEvent();

        virtual void setDefaults();
        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);

        /**
         *  Allows you to check if this event is of the given type.
         *  @param  type    The event type to which to check against.
         *  @return Wether it is of the given type or not.
         */
        const bool isA(HSSEventType type) const;

        /**
         *  @return The event type of this instance.
         */
        const HSSEventType getEventType() const;

        /**
         *  Getter for the computed value of action.
         *  @return A shared pointer to the object.
         */
        const QSharedPointer<HSSObject> getAction() const;

        /**
         *  Executes the action which is stored in this event object.
         */
        virtual void fire();

    protected:
        HSSEventType eventType;

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        void _fireFlagFunction(QSharedPointer<HSSFlagFunction> flagFunction);
    };
}

#endif
