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

#include "AXRLoggerManager.h"
#include "HSSDisplayObject.h"
#include "HSSMultipleValueDefinition.h"

using namespace AXR;

HSSMultipleValueDefinition::HSSMultipleValueDefinition(AXRController * controller)
: HSSParserNode(HSSParserNodeTypeMultipleValueDefinition, controller)
{

}

QSharedPointer<HSSMultipleValueDefinition> HSSMultipleValueDefinition::clone() const
{
    return qSharedPointerCast<HSSMultipleValueDefinition> (this->cloneImpl());
}

HSSMultipleValueDefinition::~HSSMultipleValueDefinition()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSMultipleValueDefinition: destructing multiple value definition");
    this->cleanTrackedObservers();
}

AXRString HSSMultipleValueDefinition::toString()
{
    AXRString tempstr = AXRString("HSSMultipleValueDefinition with the following values:\n");

    for (std::vector<QSharedPointer<HSSParserNode> >::iterator it = this->values.begin(); it != this->values.end(); ++it)
    {
        QSharedPointer<HSSParserNode> value = *it;
        tempstr.append(value->toString().append("\n"));
    }

    return tempstr;
}

std::vector<QSharedPointer<HSSParserNode> > HSSMultipleValueDefinition::getValues()
{
    return this->values;
}

void HSSMultipleValueDefinition::setValues(std::vector<QSharedPointer<HSSParserNode> > newValues)
{
    this->values = newValues;
}

void HSSMultipleValueDefinition::add(QSharedPointer<HSSParserNode> newValue)
{
    this->values.push_back(newValue);
}

QSharedPointer<HSSClonable> HSSMultipleValueDefinition::cloneImpl() const
{
    return QSharedPointer<HSSMultipleValueDefinition>(new HSSMultipleValueDefinition(*this));
}
