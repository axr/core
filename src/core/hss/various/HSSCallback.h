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
 *      LICENSE: see LICENSE file
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

#ifndef HSSCALLBACK_H
#define HSSCALLBACK_H

#include <string>
#include "HSSObservableProperties.h"

namespace AXR
{

    /**
     *  @brief Helper class that allows you to create callbacks.
     */
    class HSSCallback
    {
    public:
        virtual ~HSSCallback();

        /**
         *  Execute the callback, calling the stored method on the stored object.
         *  @param property The property where the callback originated.
         *  @param data     The new value or the data that goes along with the callback.
         */
        virtual void call(HSSObservableProperty property, void* data) = 0;
    };

    class HSSObservable;

    /**
     *  @brief Specific helper class to call value changed callbacks.
     */
    template <class T> class HSSValueChangedCallback : public HSSCallback
    {
        void (T::*fptr)(HSSObservableProperty property, void* data);
        T* ptr;

    public:
        friend class HSSObject;

        /**
         *  Creates a new instance of a callback, that wraps a function to be called at a later time.
         *  @param _ptr     A regular pointer to the object that will receive the callback.
         *  @param _fptr    The function pointer to the method that will be called.
         */
        HSSValueChangedCallback(T* _ptr, void(T::*_fptr)(HSSObservableProperty property, void* data))
        {
            ptr = _ptr;
            fptr = _fptr;
        }

        void call(HSSObservableProperty property, void* data)
        {
            (*ptr.*fptr)(property, data);
        }
    };
}

#endif //HSSCALLBACK_H
