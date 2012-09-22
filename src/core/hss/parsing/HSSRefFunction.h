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

#ifndef HSSREFFUNCTION_H
#define HSSREFFUNCTION_H

#include "AXRGlobal.h"
#include "HSSFunction.h"
#include "HSSSelectorChain.h"

namespace AXR
{
    /**
     *  @brief Implementation of the ref([<modfier>] <name> [of <selector chain>]) function in HSS.
     *
     *  References allow the author of HSS documents to bind together values of either
     *  the same object (using \@this) or to another object. This class stores the arguments
     *  and implements the _evaluate() method for this type of function.
     *
     *  The modifier determines what to do in case the selector chain returns multiple elements.
     */
    class AXR_API HSSRefFunction : public HSSFunction
    {
    public:
        typedef boost::shared_ptr<HSSRefFunction> p;

        /**
         *  Creates a new instance of a ref function.
         */
        HSSRefFunction();

        /**
         *  Copy constructor for HSSRefFunction objects. Do not call directly, use clone() instead.
         */
        HSSRefFunction(const HSSRefFunction & orig);

        /**
         *  Clones an instance of HSSRefFunction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSRefFunction
         */
        HSSFunction::p clone() const;

        /**
         *  Destructor for this class. Removes itself from the observed object.
         */
        virtual ~HSSRefFunction();

        /**
         *  Getter for the modifier.
         *  @return A string containing the modifier that was passed.
         *
         *  @todo Make an enum based on the string and use that instead.
         */
        const AXRString & getModifier() const;

        /**
         *  Setter for the modifier.
         *  @param newValue     A string containing the modifier for the function.
         *
         *  @todo Make an enum based on the string and use that instead.
         */
        void setModifier(AXRString newValue);

        /**
         *  Getter for the property name.
         *  @return The observable property that we are referencing.
         */
        const HSSObservableProperty & getPropertyName() const;

        /**
         *  Setter for the property name.
         *  @param  newValue    The observable property that we will be observing.
         */
        void setPropertyName(HSSObservableProperty newValue);

        /**
         *  Getter for the selector chain.
         *  @return A vector of shared pointers to the selector chains.
         */
        const std::vector<HSSSelectorChain::p> & getSelectorChains() const;

        /**
         *  Setter for the selector chains.
         *  @param newValues    A vector of shared pointers to the new selector chains to be used.
         */
        void setSelectorChains(std::vector<HSSSelectorChain::p> newValues);

        /**
         *  Add a selector chain to the selector chains vector.
         *  @param newSelectorChain A shared pointer to the selector chain to be added.
         */
        void selectorChainsAdd(HSSSelectorChain::p & newSelectorChain);

        /**
         *  Get a selector chain by index.
         *  @param index    An unsigned integer with the index of the selector chain.
         *  @return A shared pointer to the element at that index.
         */
        HSSSelectorChain::p &selectorChainsGet(unsigned index);

        /**
         *  Removes a selector chain by index.
         *  @param index    An unsigned integer with the index of the selector chain to be deleted.
         */
        void selectorChainsRemove(unsigned index);

        /**
         *  Removes the last element in the selector chains vector.
         */
        void selectorChainsRemoveLast();

        /**
         *  @return the last element of the selector chains vector.
         */
        HSSSelectorChain::p &selectorChainsLast();

        /**
         *  @return the size of the selector chains vector
         */
        int selectorChainsSize();

        /**
         *  This is the actual implementation of what the function does. It selects from the elements
         *  in the scope and then observes the property, storing  its value.
         */
        virtual boost::any _evaluate();

        /**
         *  Callback that will update the value when the observed property changes.
         *  @param source   The property which we are observing (ignored).
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void valueChanged(HSSObservableProperty source, void*data);

    private:
        AXRString modifier;
        HSSObservableProperty propertyName;
        std::vector<HSSSelectorChain::p> selectorChains;

        HSSObservable * observed;

        HSSClonable::p cloneImpl() const;
    };
}

#endif
