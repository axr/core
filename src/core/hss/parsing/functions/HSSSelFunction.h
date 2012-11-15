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

#ifndef HSSSELFUNCTION_H
#define HSSSELFUNCTION_H

#include "AXRGlobal.h"
#include "HSSFunction.h"

namespace AXR
{
    /**
     *  @brief Implementation of the sel(<selector chain>) function in HSS.
     *
     *  Sel functions allow the author of HSS documents to create selections that can
     *  be passed as a value to a property.
     */
    class AXR_API HSSSelFunction : public HSSFunction
    {
    public:
        typedef QSharedPointer<HSSSelFunction> p;

        /**
         *  Creates a new instance of a sel function.
         */
        HSSSelFunction(AXRController * controller);

        /**
         *  Copy constructor for HSSSelFunction objects. Do not call directly, use clone() instead.
         */
        HSSSelFunction(const HSSSelFunction & orig);

        /**
         *  Clones an instance of HSSSelFunction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSSelFunction
         */
        HSSFunction::p clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSSelFunction();

        virtual AXRString toString();

        /**
         *  Getter for the selector chains.
         *  @return A vector of shared pointers to the selector chains.
         */
        const std::vector<HSSSelectorChain::p> & getSelectorChains() const;

        /**
         *  Setter for the selector chains.
         *  @param newValues     A vector of shared pointers to the new selector chains.
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
         *  in the scope and then returns a selection.
         *  @return A pointer to a selection casted to void*.
         */
        virtual QVariant _evaluate();

        //        void valueChanged(HSSObservableProperty source, void*data);

    protected:
        std::vector<HSSSelectorChain::p> selectorChains;
        HSSSelection::p selection;

    private:
        virtual HSSClonable::p cloneImpl() const;
    };
}

#endif
