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
 *      COPYRIGHT: ©2019 - All Rights Reserved
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

#include "HSSInvalidNode.h"
#include "HSSDisplayObject.h"

using namespace AXR;

QSharedPointer<HSSInvalidNode> HSSInvalidNode::createInvalidNode(AXRString value, AXRController * controller)
{
    return QSharedPointer<HSSInvalidNode>(new HSSInvalidNode(value, controller));
}

HSSInvalidNode::HSSInvalidNode(AXRString value, AXRController * controller)
: HSSParserNode(HSSParserNodeTypeInvalidNode, controller)
{
    this->value = value;
}

QSharedPointer<HSSInvalidNode> HSSInvalidNode::clone() const
{
    return qSharedPointerCast<HSSInvalidNode> (this->cloneImpl());
}

HSSInvalidNode::~HSSInvalidNode()
{
    
}

void HSSInvalidNode::setValue(AXRString newValue)
{
    this->value = newValue;
}

AXRString HSSInvalidNode::getValue()
{
    return this->value;
}

AXRString HSSInvalidNode::toString()
{
    return this->value;
}

AXRString HSSInvalidNode::stringRep()
{
    return this->value;
}

bool HSSInvalidNode::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSInvalidNode> castedNode = qSharedPointerCast<HSSInvalidNode>(otherNode);
    if ( this->value != castedNode->value ) return false;
    return true;
}

QSharedPointer<HSSClonable> HSSInvalidNode::cloneImpl() const
{
    return QSharedPointer<HSSInvalidNode>(new HSSInvalidNode(*this));
}
