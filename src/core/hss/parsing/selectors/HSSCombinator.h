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

#ifndef HSSCOMBINATOR_H
#define HSSCOMBINATOR_H

#include "HSSSelector.h"

namespace AXR
{
    /**
     *  @brief Combinators are used inside of selector chains, to define the relations between the elements that
     *  are selected.
     *
     *  This class encapsulates all types of combinators. The actual behavior of the selection
     *  is defined in AXRController.
     */
    class AXR_API HSSCombinator : public HSSSelector
    {
    public:
        /**
         *  Creates a new instance of a combinator node, of the type you give. All types of combinators
         *  can be encapsulated with this class.
         */
        HSSCombinator(HSSCombinatorType type, AXRController * controller);

        /**
         *  Clones an instance of HSSCombinator and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSCombinator
         */
        QSharedPointer<HSSCombinator> clone() const;

        /**
         *  When logging, you often need a string representation of the combinator type.
         *  @param type     The combinator type to represent as a string.
         *  @return A string representation of the given type.
         */
        static AXRString combinatorStringRepresentation(HSSCombinatorType type);

        /**
         *  Allows you to check if this combinator is of the given type.
         *  @param  otherType   The combinator type to which to check against.
         *  @return Wether it is of the given type or not.
         */
        const bool isA(HSSCombinatorType otherType) const;

        /**
         *  @return The combinator type of this instance.
         */
        const HSSCombinatorType getCombinatorType() const;

        //see HSSParserNode.h for the documentation of this method
        virtual AXRString toString();

        //see HSSParserNode.h for the documentation of this method
        AXRString stringRep();

        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        bool equalTo(QSharedPointer<HSSParserNode> otherNode);

        QSharedPointer<HSSSelection> filterSelection(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing);

    protected:
        HSSCombinatorType combinatorType;
    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif
