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

#ifndef HSSINSTRUCTION_H
#define HSSINSTRUCTION_H

#include "HSSStatement.h"

namespace AXR
{
    /**
     *  @brief This class encapsulates all the different types of instructions in HSS.
     *
     *  Instructions are used to instruct the system to alter its behavior. What actually happens
     *  when processing an instruction is not implemented in this class, this only holds information
     *  about what kind of information whas passed by the author in the HSS code.
     */
    class AXR_API HSSInstruction : public HSSStatement
    {
    public:
        /**
         *  Creates a new instance of an instruction node with the given type. To be used when
         *  a value is not applicable, such as \#delete.
         *
         *  @param type     The HSSInstructionType that corresponds to the wanted type of instruction.
         */
        HSSInstruction(HSSInstructionType type, AXRController * controller);

        /**
         *  Creates a new instance of an instruction node with the given type and value.
         *
         *  @param type     The HSSInstructionType that corresponds to the wanted type of instruction.
         *  @param value    A string containing the value of the instruction.
         */
        HSSInstruction(HSSInstructionType type, AXRString value, AXRController * controller);

        /**
         *  Copy constructor for HSSInstructions objects. Do not call directly, use clone() instead.
         */
        HSSInstruction(const HSSInstruction &orig);

        /**
         *  Clones an instance of this class and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new instance
         */
        QSharedPointer<HSSInstruction> clone() const;

        /**
         *  Destructor for this class.
         */
        ~HSSInstruction();

        /**
         *  @return The instruction type of this instance.
         */
        HSSInstructionType getInstructionType() const;

        /**
         *  Getter for the value of the instruction.
         *  @return A string containing a value of the instruction.
         */
        AXRString getValue();

        /**
         *  Allows you to check if this instruction is of the given type.
         *  @param  type    The instruction type to which to check against.
         *  @return Wether it is of the given type or not.
         */
        bool isA(HSSInstructionType type) const;

        /**
         *  When logging, you often need a string representation of the instruction type.
         *  @param type     The instruction type to represent as a string.
         *  @return A string representation of the given type.
         */
        static AXRString instructionStringRepresentation(HSSInstructionType type);

        /**
         *  Prints itself as a textual representation, useful for loggin or introspection.
         *  @return A string containing a textual representation of the keyword constant.
         */
        virtual AXRString toString();

        virtual AXRString stringRep();

        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        bool equalTo(QSharedPointer<HSSParserNode> otherNode);

        /**
         *  Setter for argument.
         *  Some instructions accept an argument, when written in functional notation (e.g. \#new(3) { ... }).
         *  When using an argument, this will hold the parser node that represents that value.
         *
         *  @param newValue     A shared pointer to the parser node that holds the argument.
         */
        void setArgument(QSharedPointer<HSSParserNode> newValue);

        /**
         *  Getter for argument.
         *  Some instructions accept an argument, when written in functional notation (e.g. \#new(3) { ... }).
         *  When using an argument, this will hold the parser node that represents that value.
         *
         *  @return A shared pointer to the parser node that holds the argument.
         */
        QSharedPointer<HSSParserNode> getArgument();

    protected:
        HSSInstructionType instructionType;
        AXRString value;
        QSharedPointer<HSSParserNode> argument;

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif
