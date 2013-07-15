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

#ifndef HSSSTATEMENT_H
#define HSSSTATEMENT_H

#include "HSSParserNode.h"

namespace AXR
{
    /**
     *  @brief Abstract base class that provides the basic functionality for statements.
     *
     *  Statements are the basic execution units for the rendering engine. They are high level abstractions
     *  of groups of parser nodes that have a particular meaning (e.g. a rule is for applying properties to an element,
     *  and is made up of a selector chain, and property definitions.
     */
    class AXR_API HSSStatement : public HSSParserNode
    {
    public:
        virtual AXRString toString() = 0;

        /**
         *  Call this to know if a statement is of the given type.
         *  @param otherType    The statement type to compare against.
         *  @return Wether the instance is a statement of given type.
         */
        virtual const bool isA(HSSStatementType otherType) const;

        /**
         *  @return The statement type of the current instance.
         */
        virtual const HSSStatementType getStatementType() const;

        /**
         *  When logging, you often need a string representation of the statement type.
         *  @param type     The statement type to represent as a string.
         *  @return A string representation of the given type.
         */
        AXRString statementStringRepresentation(HSSStatementType type);

        const bool isA(HSSInstructionType type) const;

    protected:
        /**
         *  Creates a new instance of a statement node. This class shouldn't be called directly,
         *  but by the subclasses.
         *  @param type The type of this statement subclass for identification purposes.
         */
        HSSStatement(HSSStatementType type, AXRController * controller);

        /**
         *  Copy constructor for HSSStatement nodes. Do not call directly, use clone() on one of the
         *  subclasses instead.
         */
        HSSStatement(const HSSStatement & orig);

        QSharedPointer<HSSStatement> shared_from_this();

    private:
        HSSStatementType type;
    };
}

#endif
