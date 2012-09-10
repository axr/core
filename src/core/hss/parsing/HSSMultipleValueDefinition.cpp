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

#include "HSSMultipleValueDefinition.h"

using namespace AXR;

HSSMultipleValueDefinition::HSSMultipleValueDefinition()
: HSSParserNode(HSSParserNodeTypeMultipleValueDefinition)
{

}

HSSMultipleValueDefinition::p HSSMultipleValueDefinition::clone() const
{
    return boost::static_pointer_cast<HSSMultipleValueDefinition, HSSClonable > (this->cloneImpl());
}

HSSMultipleValueDefinition::~HSSMultipleValueDefinition()
{

}

std::string HSSMultipleValueDefinition::toString()
{
    std::string tempstr = std::string("HSSMultipleValueDefinition with the following values:\n");

    for (std::vector<HSSParserNode::p>::iterator it = this->values.begin(); it != this->values.end(); ++it)
    {
        HSSParserNode::p value = *it;
        tempstr.append(value->toString().append("\n"));
    }

    return tempstr;
}

std::vector<HSSParserNode::p> HSSMultipleValueDefinition::getValues()
{
    return this->values;
}

void HSSMultipleValueDefinition::setValues(std::vector<HSSParserNode::p> newValues)
{
    this->values = newValues;
}

void HSSMultipleValueDefinition::add(HSSParserNode::p newValue)
{
    this->values.push_back(newValue);
}

HSSClonable::p HSSMultipleValueDefinition::cloneImpl() const
{
    return HSSClonable::p(new HSSMultipleValueDefinition(*this));
}
