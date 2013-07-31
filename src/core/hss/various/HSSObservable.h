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

#ifndef HSSOBSERVABLE_H
#define HSSOBSERVABLE_H

#include <QMap>
#include "HSSObservableProperties.h"

template <class T> class QSharedPointer;

namespace AXR
{
    class HSSAbstractValueChangedCallback;
    class HSSObservableMapping;
    class HSSObject;

    /**
     *  @brief This class provides base functionality for property observing.
     *
     *  Observers are stored as per object and per source property, which means that
     *  the same object can be observing the same property (on the instance that inherits from this class),
     *  from within different properties in the observer.
     */
    class AXR_API HSSObservable
    {
    public:
        /**
         *  When logging, for example, it is useful to be able to show a string representation of a
         *  HSSObservableProperty representation.
         *
         *  @param property     The property in enum representation.
         *  @return A string containing a textual representation of the HSSObservableProperty.
         */
        static AXRString observablePropertyStringRepresentation(HSSObservableProperty property);

        /**
         *  Returns a HSSObservableProperty from a string representation of a property name. In situations like
         *  when the parser has a property name encapsulated in a parser node and needs to give an enum
         *  property representation, this method is called.
         *
         *  @param name     A string containing the property name.
         *  @return The HSSObservableProperty that corresponds to that name, or HSSObservablePropertyNone if
         *  not found.
         */
        static HSSObservableProperty observablePropertyFromString(AXRString name);

        /**
         *  The typedef for the inner map, which will be stored as the value for each HSSObservableProperty as
         *  key inside the private _propertyObservers map.
         *  The key for this map is a hash of HSSObservable * observed and HSObservableProperty source
         */
        typedef QList<QSharedPointer<HSSObservableMapping> > observed;

        /**
         *  Creates a new instance of a observable object. Don't call directly, this is for the use of subclasses.
         */
        HSSObservable();

        /**
         *  Destructs this instance of an observable object, clearing the list of observers it has stored.
         */
        virtual ~HSSObservable();

        /**
         *  Call this method when you want to receive notifications of this instance about changes in the values
         *  of the target property. When such a change happens, the given callback will be called on the
         *  given object, passing the source property and a void pointer to the data that goes along with the
         *  notification.
         *
         *  @param target   The property that we want to observe.
         *  @param source   The property from which the observer is calling this, it will be passed in the callback.
         *  @param object   A regular pointer to the object that we are observing. It must have HSSObservable as
         *                  one of its base classes.
         *  @param callback A regular pointer to a HSSCallback object, which encapsulates the function pointer that
         *                  will be called to notify the observer about the change.
         *
         *  @todo I think the requirement for the object to be a HSSObservable subclass is not really needed. Maybe
         *  a "void*" would do equally well, since we are actually only interested in the pointer itself.
         */
        void observe(const AXRString target, const AXRString source, HSSObservable * object, HSSAbstractValueChangedCallback *callback);

        /**
         *  Removes an observer from the list of observers for the given combination between target and source properties
         *  and the pointer to the object.
         *
         *  @param target   The property that we are observing and for which we want to remove the observer.
         *  @param source   The property from which the observer was receiving the notifications.
         *  @param object   A regular pointer to the observer to be removed. It must be a subclass of HSSObservable.
         *
         *  @todo I think the requirement for the object to be a HSSObservable subclass is not really needed. Maybe
         *  a "void*" would do equally well, since we are actually only interested in the pointer itself.
         */
        void removeObserver(const AXRString target, const AXRString source, HSSObservable * object);

        /**
         *  This is the form that the functions that will receive the notifications must have.
         *  The implementation in this class is to just out the name of the property when we are debugging,
         *  otherwise does nothing.
         *
         *  @param property The property which we are observing.
         *  @param data     A shared pointer to the object that is sent along the notification.
         */
        virtual void propertyChanged(const AXRString source, const AXRString target, const QSharedPointer<HSSObject> theObj);

        /**
         *  Subclasses should call this method on themselves to trigger the delivery of the notification.
         *  These are syncronous, which means that they won't return until the function receiving the
         *  notification have completed.
         *  @param property The property which we are observing.
         *  @param data     A shared pointer to the object that is sent along the notification (e.g. the new value).
         */
        void notifyObservers(const AXRString property, const QSharedPointer<HSSObject> theObj);

        bool tracksObserver(const AXRString source);
        HSSObservable * getTrackedObserver(const AXRString source);
        AXRString getTrackedProperty(const AXRString source);
        void trackObserver(const AXRString target, const AXRString source, HSSObservable* observable);
        void untrackObserver(const AXRString source);
        void cleanTrackedObservers();

    private:
        QMap<AXRString, HSSObservable::observed> _propertyObservers;
        QMap<AXRString, HSSObservable* > _trackedObservers;
        QMap<AXRString, AXRString> _trackedProperties;
    };
}

#endif
