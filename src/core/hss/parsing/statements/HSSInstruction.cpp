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

#include "HSSInstruction.h"

using namespace AXR;

HSSInstruction::HSSInstruction(HSSInstructionType type)
: HSSStatement(HSSStatementTypeInstruction)
{
    this->instructionType = type;
}

HSSInstruction::HSSInstruction(HSSInstructionType type, AXRString value)
: HSSStatement(HSSStatementTypeInstruction)
{
    this->instructionType = type;
    this->value = value;
}

HSSInstruction::HSSInstruction(const HSSInstruction &orig)
: HSSStatement(orig)
{
    this->instructionType = orig.instructionType;
    this->value = orig.value;
    this->argument = orig.argument;
}

HSSInstruction::p HSSInstruction::clone() const
{
    return qSharedPointerCast<HSSInstruction, HSSClonable > (this->cloneImpl());
}

HSSInstruction::~HSSInstruction()
{

}

HSSInstructionType HSSInstruction::getInstructionType()
{
    return this->instructionType;
}

AXRString HSSInstruction::getValue()
{
    return this->value;
}

bool HSSInstruction::isA(HSSInstructionType type)
{
    return this->instructionType == type;
}

AXRString HSSInstruction::toString()
{
    AXRString tempstr = "HSSInstruction of type: " + this->instructionStringRepresentation(this->instructionType);
    if (
            this->instructionType != HSSNewInstruction
            && this->instructionType != HSSEnsureInstruction
            && this->instructionType != HSSImportInstruction
            && this->instructionType != HSSMoveInstruction
            && this->instructionType != HSSDeleteInstruction
            )
    {
        tempstr.append(" and value: " + this->value);
    }
    return tempstr;
}

AXRString HSSInstruction::instructionStringRepresentation(HSSInstructionType type)
{
    AXRString types[20];
    types[HSSNewInstruction] = "HSSNewInstruction";
    types[HSSEnsureInstruction] = "HSSEnsureInstruction";
    types[HSSImportInstruction] = "HSSImportInstruction";
    types[HSSMoveInstruction] = "HSSMoveInstruction";
    types[HSSDeleteInstruction] = "HSSDeleteInstruction";
    types[HSSGrayscale1Instruction] = "HSSGrayscale1Instruction";
    types[HSSGrayscale2Instruction] = "HSSGrayscale2Instruction";
    types[HSSRGBInstruction] = "HSSRGBInstruction";
    types[HSSRGBAInstruction] = "HSSRGBAInstruction";
    types[HSSRGBAAInstruction] = "HSSRGBAAInstruction";
    types[HSSRRGGBBInstruction] = "HSSRRGGBBInstruction";
    types[HSSRRGGBBAInstruction] = "HSSRRGGBBAInstruction";
    types[HSSRRGGBBAAInstruction] = "HSSRRGGBBAAInstruction";
    return types[type];
}

HSSClonable::p HSSInstruction::cloneImpl() const
{
    return HSSClonable::p(new HSSInstruction(*this));
}

HSSParserNode::p HSSInstruction::getArgument()
{
    return this->argument;
}

void HSSInstruction::setArgument(HSSParserNode::p newValue)
{
    this->argument = newValue;
}
