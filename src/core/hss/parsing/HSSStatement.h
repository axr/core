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

#ifndef HSSSTATEMENT_H
#define HSSSTATEMENT_H

#include <string>
#include <boost/shared_ptr.hpp>
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
        /**
         *  The shared pointer to instances of this class.
         */
        typedef boost::shared_ptr<HSSStatement> p;

        virtual std::string toString() = 0;

        /**
         *  Call this to know if a statement is of the given type.
         *  @param otherType    The statement type to compare against.
         *  @return Wether the instance is a statement of given type.
         */
        bool isA(HSSStatementType otherType);

        /**
         *  @return The statement type of the current instance.
         */
        virtual HSSStatementType getStatementType();

        /**
         *  When logging, you often need a string representation of the statement type.
         *  @param type     The statement type to represent as a string.
         *  @return A string representation of the given type.
         */
        std::string statementStringRepresentation(HSSStatementType type);

        bool isA(HSSInstructionType type);

    protected:
        /**
         *  Creates a new instance of a statement node. This class shouldn't be called directly,
         *  but by the subclasses.
         *  @param type The type of this statement subclass for identification purposes.
         */
        HSSStatement(HSSStatementType type);

        /**
         *  Copy constructor for HSSStatement nodes. Do not call directly, use clone() on one of the
         *  subclasses instead.
         */
        HSSStatement(const HSSStatement & orig);

        HSSStatement::p shared_from_this();

    private:
        HSSStatementType type;
    };
}

#endif
