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
 *      COPYRIGHT: ©2019 - All Rights Reserved
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

#ifndef HSSTARGETEDFUNCTION_H
#define HSSTARGETEDFUNCTION_H

#include "HSSFunction.h"

namespace AXR
{
    class HSSSelectorChain;
    
    /**
     *  @brief Abstract superclass for functions that use selector chains.
     */
    class AXR_API HSSTargetedFunction : public HSSFunction
    {
    public:
        /**
         *  Destructor for this class.
         */
        virtual ~HSSTargetedFunction();

        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        bool equalTo(QSharedPointer<HSSParserNode> otherNode);

        /**
         *  Getter for the selector chain parameter.
         *  @return A shared pointer to the selector chain that should be used in the function.
         */
        const std::vector<QSharedPointer<HSSSelectorChain> > & getSelectorChains() const;

        /**
         *  Setter for the selector chain parameter.
         *  @param newValues A shared pointer to the selector chain which the function should use.
         */
        void setSelectorChains(std::vector<QSharedPointer<HSSSelectorChain> > newValues);

        /**
         *  Add a selector chain to the selector chains vector.
         *  @param newSelectorChain A shared pointer to the selector chain to be added.
         */
        void selectorChainsAdd(QSharedPointer<HSSSelectorChain> & newSelectorChain);

        /**
         *  Get a selector chain by index.
         *  @param index    An unsigned integer with the index of the selector chain.
         *  @return A shared pointer to the element at that index.
         */
        QSharedPointer<HSSSelectorChain> &selectorChainsGet(unsigned index);

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
        QSharedPointer<HSSSelectorChain> &selectorChainsLast();

        /**
         *  @return the size of the selector chains vector
         */
        size_t selectorChainsSize() const;
        
        void cloneSelectorChains(const QSharedPointer<HSSTargetedFunction> & clone) const;

    protected:
        std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
        
        /**
         *  Creates a new instance of a targeted function.
         */
        HSSTargetedFunction(HSSFunctionType type, AXRController * controller);
        
        /**
         *  Copy constructor for the function. Do not call directly, use clone() instead.
         */
        HSSTargetedFunction(const HSSTargetedFunction & orig);
    };
}

#endif
