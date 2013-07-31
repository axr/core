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

#ifndef HSSCALLBACK_H
#define HSSCALLBACK_H

#include <QSharedPointer>
#include "AXRGlobal.h"
#include "HSSObservableProperties.h"

namespace AXR
{
    class HSSObject;
    class HSSObservable;
    class HSSParserNode;

    /* ============================== STACK ================================= */
    /**
     *  @brief This class provides the interface for the compute callback,
     *  regardless of template parameter
     */
    class AXR_API HSSAbstractStackCallback
    {
    public:
        HSSAbstractStackCallback();
        virtual ~HSSAbstractStackCallback();

        /**
         *  Execute the callback, calling the stored method on the stored object.
         *  @param parserNode   The new parser node to be stacked into an HSSObject
         */
        virtual void call(QSharedPointer<HSSParserNode> parserNode) = 0;
    };

    /**
     *  @brief Helper class that allows you to create callbacks for computing properties.
     */
    template <class T> class AXR_API HSSStackCallback : public HSSAbstractStackCallback
    {
        void (T::*fptr)(QSharedPointer<HSSParserNode> parserNode);
        T* ptr;

    public:
        /**
         *  Creates a new instance of a callback, that wraps a function to be called at a later time.
         *  @param _ptr     A regular pointer to the object that will receive the callback.
         *  @param _fptr    The function pointer to the method that will be called.
         */
        HSSStackCallback(T* _ptr, void(T::*_fptr)(QSharedPointer<HSSParserNode> parserNode))
        {
            ptr = _ptr;
            fptr = _fptr;
        }

        virtual ~HSSStackCallback() {}

        void call(QSharedPointer<HSSParserNode> parserNode)
        {
            return (*ptr.*fptr)(parserNode);
        }
    };

    /* ============================== COMPUTE ================================= */
    /**
     *  @brief This class provides the interface for the compute callback,
     *  regardless of template parameter
     */
    class AXR_API HSSAbstractComputeCallback
    {
    public:
        HSSAbstractComputeCallback();
        virtual ~HSSAbstractComputeCallback();

        /**
         *  Execute the callback, calling the stored method on the stored object.
         *  @param parserNode   The new parser node to be computed into an HSSObject
         */
        virtual QSharedPointer<HSSObject> call(QSharedPointer<HSSParserNode> parserNode) = 0;
    };

    /**
     *  @brief Helper class that allows you to create callbacks for computing properties.
     */
    template <class T> class AXR_API HSSComputeCallback : public HSSAbstractComputeCallback
    {
        QSharedPointer<HSSObject> (T::*fptr)(QSharedPointer<HSSParserNode> parserNode);
        T* ptr;

    public:
        /**
         *  Creates a new instance of a callback, that wraps a function to be called at a later time.
         *  @param _ptr     A regular pointer to the object that will receive the callback.
         *  @param _fptr    The function pointer to the method that will be called.
         */
        HSSComputeCallback(T* _ptr, QSharedPointer<HSSObject>(T::*_fptr)(QSharedPointer<HSSParserNode> parserNode))
        {
            ptr = _ptr;
            fptr = _fptr;
        }

        virtual ~HSSComputeCallback() {}

        QSharedPointer<HSSObject> call(QSharedPointer<HSSParserNode> parserNode)
        {
            return (*ptr.*fptr)(parserNode);
        }
    };

    /* ============================== OBSERVE ================================= */
    /**
     *  @brief This class provides the interface for the compute callback,
     *  regardless of template parameter
     */
    class AXR_API HSSAbstractObserveCallback
    {
    public:
        HSSAbstractObserveCallback();
        virtual ~HSSAbstractObserveCallback();

        /**
         *  Execute the callback, calling the stored method on the stored object.
         *  @param theObj   The object to be listened to or notified
         */
        virtual void call(QSharedPointer<HSSObject> theObj) = 0;
    };

    /**
     *  @brief Helper class that allows you to create callbacks for computing properties.
     */
    template <class T> class AXR_API HSSObserveCallback : public HSSAbstractObserveCallback
    {
        void (T::*fptr)(QSharedPointer<HSSObject> theObj);
        T* ptr;

    public:
        /**
         *  Creates a new instance of a callback, that wraps a function to be called at a later time.
         *  @param _ptr     A regular pointer to the object that will receive the callback.
         *  @param _fptr    The function pointer to the method that will be called.
         */
        HSSObserveCallback(T* _ptr, void(T::*_fptr)(QSharedPointer<HSSObject> theObj))
        {
            ptr = _ptr;
            fptr = _fptr;
        }

        virtual ~HSSObserveCallback() {}

        void call(QSharedPointer<HSSObject> theObj)
        {
            return (*ptr.*fptr)(theObj);
        }
    };

    /* ============================== VALUE CHANGED ================================= */
    /**
     *  @brief This class provides the interface for the value changed callback,
     *  regardless of template parameter
     */
    class AXR_API HSSAbstractValueChangedCallback
    {
    public:
        HSSAbstractValueChangedCallback();
        virtual ~HSSAbstractValueChangedCallback();

        /**
         *  Execute the callback, calling the stored method on the stored object.
         *  @param property The property where the callback originated.
         *  @param data     The new value or the data that goes along with the callback.
         */
        virtual void call(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj) = 0;
    };

    /**
     *  @brief Helper class to call value changed callbacks.
     */
    template <class T> class HSSValueChangedCallback : public HSSAbstractValueChangedCallback
    {
        void (T::*fptr)(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj);
        T* ptr;

    public:
        friend class HSSObject;

        /**
         *  Creates a new instance of a callback, that wraps a function to be called at a later time.
         *  @param _ptr     A regular pointer to the object that will receive the callback.
         *  @param _fptr    The function pointer to the method that will be called.
         */
        HSSValueChangedCallback(T* _ptr, void(T::*_fptr)(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj))
        {
            ptr = _ptr;
            fptr = _fptr;
        }

        virtual ~HSSValueChangedCallback() {}

        void call(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
        {
            (*ptr.*fptr)(target, source, theObj);
        }
    };
}

#endif
