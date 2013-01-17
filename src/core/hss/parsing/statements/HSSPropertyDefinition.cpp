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

#include "HSSDisplayObject.h"
#include "HSSMultipleValueDefinition.h"
#include "HSSPropertyDefinition.h"

using namespace AXR;

HSSPropertyDefinition::HSSPropertyDefinition(AXRController * controller)
: HSSStatement(HSSStatementTypePropertyDefinition, controller)
{
    this->name = "";
}

HSSPropertyDefinition::HSSPropertyDefinition(AXRString name, AXRController * controller)
: HSSStatement(HSSStatementTypePropertyDefinition, controller)
{
    this->name = name;
}

HSSPropertyDefinition::HSSPropertyDefinition(AXRString name, HSSParserNode::p value, AXRController * controller)
: HSSStatement(HSSStatementTypePropertyDefinition, controller)
{
    this->name = name;
    this->setValue(value);
}

HSSPropertyDefinition::HSSPropertyDefinition(const HSSPropertyDefinition &orig)
: HSSStatement(orig)
{
    this->name = orig.name;
}

HSSPropertyDefinition::p HSSPropertyDefinition::clone() const
{
    return qSharedPointerCast<HSSPropertyDefinition> (this->cloneImpl());
}

HSSPropertyDefinition::~HSSPropertyDefinition()
{

}

AXRString HSSPropertyDefinition::toString()
{
    AXRString ret = "HSSPropertyDefinition: " + this->name;
    if (this->value)
    {
        ret.append(" with value: " + this->value->toString());
    }
    return ret;
}

void HSSPropertyDefinition::setName(AXRString name)
{
    this->name = name;
}

AXRString HSSPropertyDefinition::getName()
{
    return this->name;
}

void HSSPropertyDefinition::setValue(HSSParserNode::p value)
{
    this->value = value;
    this->value->setParentNode(this->shared_from_this());
}

void HSSPropertyDefinition::addValue(HSSParserNode::p value)
{
    if (this->value)
    {
        if (this->value->isA(HSSParserNodeTypeMultipleValueDefinition))
        {
            HSSMultipleValueDefinition::p mvDef = qSharedPointerCast<HSSMultipleValueDefinition > (this->value);
            value->setParentNode(this->shared_from_this());
            mvDef->add(value);
        }
        else
        {
            HSSMultipleValueDefinition::p mvDef = HSSMultipleValueDefinition::p(new HSSMultipleValueDefinition(this->getController()));
            mvDef->setParentNode(this->shared_from_this());
            mvDef->add(this->value);
            value->setParentNode(this->shared_from_this());
            mvDef->add(value);
            this->value = mvDef;
        }
    }
    else
    {
        this->value = value;
    }
}

HSSParserNode::p HSSPropertyDefinition::getValue()
{
    return this->value;
}

void HSSPropertyDefinition::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    this->value->setThisObj(value);
    HSSStatement::setThisObj(value);
}

HSSPropertyDefinition::p HSSPropertyDefinition::shared_from_this()
{
    return qSharedPointerCast<HSSPropertyDefinition > (HSSStatement::shared_from_this());
}

HSSClonable::p HSSPropertyDefinition::cloneImpl() const
{
    HSSPropertyDefinition::p clone = HSSPropertyDefinition::p(new HSSPropertyDefinition(*this));
    clone->setValue(this->value->clone());
    return clone;
}
