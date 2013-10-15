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

#ifndef HSSFLAGFUNCTION_H
#define HSSFLAGFUNCTION_H

#include "HSSFunction.h"

namespace AXR
{
    /**
     *  @brief Function type to manipulate the activation status of a flag.
     *  All of the flagging functions [flag(), unflag() and toggleFlag()] are handled by this class.
     */
    class AXR_API HSSFlagFunction : public HSSFunction
    {
    public:
        /**
         *  When logging, you often need a string representation of the flag type.
         *  @param name   The action type to represent as a string.
         *  @return A string representation of the given type.
         */
        static HSSFlagFunctionType flagFunctionTypeFromString(AXRString name);

        /**
         *  Creates a new instance of a flagging function, of the given type.
         *  @param flagFunctionType     The HSSFlagFunctionType that corresponds with the desired
         *                              behavior of the function.
         */
        HSSFlagFunction(HSSFlagFunctionType flagFunctionType, AXRController * controller);

        /**
         *  Copy constructor for HSSAction objects. Do not call directly, use clone() instead.
         */
        HSSFlagFunction(const HSSFlagFunction & orig);

        /**
         *  Clones an instance of HSSFlagFunction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSFlagFunction
         */
        QSharedPointer<HSSFunction> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSFlagFunction();

        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        bool equalTo(QSharedPointer<HSSParserNode> otherNode);

        /**
         *  Getter for the name parameter.
         *  @return A string containing the name of the HSS property the function should access.
         */
        const AXRString & getName();

        /**
         *  Setter for the name parameter.
         *  @param newValue The new HSS property name which the function should use.
         */
        void setName(AXRString newValue);

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

        /**
         *  @todo make private
         */
        virtual QSharedPointer<HSSObject> _evaluate();

        /**
         *  @todo is this one even used?
         */
        virtual QSharedPointer<HSSObject> _evaluate(std::deque<QSharedPointer<HSSParserNode> > arguments);

        /**
         *  Method to be passed as callback when observing changes.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void valueChanged(const AXRString source, const QSharedPointer<HSSObject> theObj);

        /**
         *  @return The flag type of this instance.
         */
        HSSFlagFunctionType getFlagFunctionType() const;

    protected:
        AXRString _name;
        std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
        HSSFlagFunctionType _flagFunctionType;

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif
