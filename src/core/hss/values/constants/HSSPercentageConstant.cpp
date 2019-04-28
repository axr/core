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

#include "axr.h"
#include "HSSPercentageConstant.h"

using namespace AXR;

HSSPercentageConstant::HSSPercentageConstant(HSSUnit number, AXRController * controller)
: HSSParserNode(HSSParserNodeTypePercentageConstant, controller)
{
    this->_number = number / 100.;
}

QSharedPointer<HSSPercentageConstant> HSSPercentageConstant::clone() const
{
    return qSharedPointerCast<HSSPercentageConstant> (this->cloneImpl());
}

HSSPercentageConstant::~HSSPercentageConstant()
{

}

AXRString HSSPercentageConstant::toString()
{
    return HSSString::format("HSSPercentageConstant with value %f%", this->_number);
}

AXRString HSSPercentageConstant::stringRep()
{
    if (!this->_originalRep.isEmpty())
    {
        return this->_originalRep + "%";
    }
    return HSSString::format("%f%", this->_number*100);
}

bool HSSPercentageConstant::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSPercentageConstant> castedNode = qSharedPointerCast<HSSPercentageConstant>(otherNode);
    if ( this->_number != castedNode->_number ) return false;
    return true;
}

void HSSPercentageConstant::setNumber(HSSUnit newValue)
{
    this->_number = newValue;
}

HSSUnit HSSPercentageConstant::getNumber()
{
    return this->_number;
}

void HSSPercentageConstant::setOriginalStringRep(HSSString string)
{
    this->_originalRep = string;
}

HSSString HSSPercentageConstant::originalStringRep()
{
    return this->_originalRep;
}

QSharedPointer<HSSClonable> HSSPercentageConstant::cloneImpl() const
{
    return QSharedPointer<HSSPercentageConstant>(new HSSPercentageConstant(*this));
}
