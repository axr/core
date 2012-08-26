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

#include "HSSMultipleValueDefinition.h"
#include "HSSPropertyDefinition.h"

using namespace AXR;

HSSPropertyDefinition::HSSPropertyDefinition()
: HSSStatement(HSSStatementTypePropertyDefinition)
{
    this->name = "";
}

HSSPropertyDefinition::HSSPropertyDefinition(std::string name)
: HSSStatement(HSSStatementTypePropertyDefinition)
{
    this->name = name;
}

HSSPropertyDefinition::HSSPropertyDefinition(std::string name, HSSParserNode::p value)
: HSSStatement(HSSStatementTypePropertyDefinition)
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
    return boost::static_pointer_cast<HSSPropertyDefinition, HSSClonable > (this->cloneImpl());
}

HSSPropertyDefinition::~HSSPropertyDefinition()
{

}

std::string HSSPropertyDefinition::toString()
{
    std::string ret = "HSSPropertyDefinition: " + this->name;
    if (this->value != NULL)
    {
        ret.append(" with value: " + this->value->toString());
    }
    return ret;
}

void HSSPropertyDefinition::setName(std::string name)
{
    this->name = name;
}

std::string HSSPropertyDefinition::getName()
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
            HSSMultipleValueDefinition::p mvDef = boost::static_pointer_cast<HSSMultipleValueDefinition > (this->value);
            value->setParentNode(this->shared_from_this());
            mvDef->add(value);
        }
        else
        {
            HSSMultipleValueDefinition::p mvDef = HSSMultipleValueDefinition::p(new HSSMultipleValueDefinition());
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

void HSSPropertyDefinition::setThisObj(boost::shared_ptr<HSSDisplayObject> value)
{
    this->value->setThisObj(value);
    HSSStatement::setThisObj(value);
}

HSSPropertyDefinition::p HSSPropertyDefinition::shared_from_this()
{
    return boost::static_pointer_cast<HSSPropertyDefinition > (HSSStatement::shared_from_this());
}

HSSClonable::p HSSPropertyDefinition::cloneImpl() const
{
    HSSPropertyDefinition::p clone = HSSPropertyDefinition::p(new HSSPropertyDefinition(*this));
    clone->setValue(this->value->clone());
    return clone;
}
