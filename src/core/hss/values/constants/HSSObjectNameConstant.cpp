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
#include "HSSObjectNameConstant.h"

using namespace AXR;

QSharedPointer<HSSObjectNameConstant> HSSObjectNameConstant::createConstant(AXRString value, AXRController * controller)
{
    return QSharedPointer<HSSObjectNameConstant>(new HSSObjectNameConstant(value, controller));
}

HSSObjectNameConstant::HSSObjectNameConstant(AXRString value, AXRController * controller)
: HSSParserNode(HSSParserNodeTypeObjectNameConstant, controller)
{
    this->value = value;
}

QSharedPointer<HSSObjectNameConstant> HSSObjectNameConstant::clone() const
{
    return qSharedPointerCast<HSSObjectNameConstant> (this->cloneImpl());
}

HSSObjectNameConstant::~HSSObjectNameConstant()
{

}

void HSSObjectNameConstant::setValue(AXRString newValue)
{
    this->value = newValue;
}

AXRString HSSObjectNameConstant::getValue()
{
    return this->value;
}

AXRString HSSObjectNameConstant::toString()
{
    return "HSSObjectNameConstant with value " + this->value;
}

AXRString HSSObjectNameConstant::stringRep()
{
    return this->value;
}

bool HSSObjectNameConstant::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSObjectNameConstant> castedNode = qSharedPointerCast<HSSObjectNameConstant>(otherNode);
    if ( this->value != castedNode->value ) return false;
    return true;
}

QSharedPointer<HSSClonable> HSSObjectNameConstant::cloneImpl() const
{
    return QSharedPointer<HSSObjectNameConstant>(new HSSObjectNameConstant(*this));
}
