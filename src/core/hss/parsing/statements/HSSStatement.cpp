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

#include "HSSStatement.h"

using namespace AXR;

HSSStatement::HSSStatement(HSSStatementType type)
: HSSParserNode(HSSParserNodeTypeStatement)
{
    this->type = type;
}

HSSStatement::HSSStatement(const HSSStatement &orig)
: HSSParserNode(orig)
{
    this->type = orig.type;
}

bool HSSStatement::isA(HSSStatementType otherType)
{
    return otherType == this->type;
}

HSSStatementType HSSStatement::getStatementType()
{
    return this->type;
}

AXRString HSSStatement::statementStringRepresentation(HSSStatementType type)
{
    static QMap<HSSStatementType, AXRString> types;
    if (types.isEmpty())
    {
        types[HSSStatementTypeNone] = "HSSGenericStatement";
        types[HSSStatementTypeRule] = "HSSRule";
        types[HSSStatementTypePropertyDefinition] = "HSSPropertyDefinition";
        types[HSSStatementTypeObjectDefinition] = "HSSObjectDefinition";
        types[HSSStatementTypeComment] = "HSSComment";
        types[HSSStatementTypeInstruction] = "HSSInstruction";
    }

    return types[type];
}

HSSStatement::p HSSStatement::shared_from_this()
{
    return qSharedPointerCast<HSSStatement>(HSSParserNode::shared_from_this());
}

bool HSSStatement::isA(HSSInstructionType type)
{
    return HSSParserNode::isA(type);
}
