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

#include "HSSDisplayObject.h"
#include "HSSInstruction.h"

using namespace AXR;

HSSInstruction::HSSInstruction(HSSInstructionType type, AXRController * controller)
: HSSStatement(HSSStatementTypeInstruction, controller)
{
    this->instructionType = type;
}

HSSInstruction::HSSInstruction(HSSInstructionType type, AXRString value, AXRController * controller)
: HSSStatement(HSSStatementTypeInstruction, controller)
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

QSharedPointer<HSSInstruction> HSSInstruction::clone() const
{
    return qSharedPointerCast<HSSInstruction>(this->cloneImpl());
}

HSSInstruction::~HSSInstruction()
{

}

HSSInstructionType HSSInstruction::getInstructionType() const
{
    return this->instructionType;
}

AXRString HSSInstruction::getValue()
{
    return this->value;
}

bool HSSInstruction::isA(HSSInstructionType type) const
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

AXRString HSSInstruction::stringRep()
{
    return this->value;
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

bool HSSInstruction::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSStatement::equalTo(otherNode)) return false;
    QSharedPointer<HSSInstruction> castedNode = qSharedPointerCast<HSSInstruction>(otherNode);
    if ( this->instructionType != castedNode->instructionType ) return false;
    if ( this->value != castedNode->value ) return false;
    if ( (this->argument && !castedNode->argument) || (!this->argument && castedNode->argument)) return false;
    if ( this->argument && castedNode->argument && ! this->argument->equalTo(castedNode->argument)) return false;
    return true;
}

QSharedPointer<HSSClonable> HSSInstruction::cloneImpl() const
{
    return QSharedPointer<HSSInstruction>(new HSSInstruction(*this));
}

QSharedPointer<HSSParserNode> HSSInstruction::getArgument()
{
    return this->argument;
}

void HSSInstruction::setArgument(QSharedPointer<HSSParserNode> newValue)
{
    this->argument = newValue;
}
