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
#include "HSSPropertyPathNode.h"

using namespace AXR;

HSSPropertyPathNode::HSSPropertyPathNode(AXRString propertyName, AXRController * controller)
: HSSParserNode(HSSParserNodeTypePropertyPathNode, controller)
{
    this->_propertyName = propertyName;
}

HSSPropertyPathNode::HSSPropertyPathNode(const HSSPropertyPathNode & orig)
: HSSParserNode(orig)
{
    this->_propertyName = orig._propertyName;
}

QSharedPointer<HSSPropertyPathNode> HSSPropertyPathNode::clone() const
{
    return qSharedPointerCast<HSSPropertyPathNode> (this->cloneImpl());
}

HSSPropertyPathNode::~HSSPropertyPathNode()
{

}

AXRString HSSPropertyPathNode::toString()
{
    return this->_propertyName;
}

AXRString HSSPropertyPathNode::stringRep()
{
    return this->_propertyName;
}

bool HSSPropertyPathNode::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSPropertyPathNode> castedNode = qSharedPointerCast<HSSPropertyPathNode>(otherNode);
    if (castedNode->_propertyName != this->_propertyName) return false;
    return true;
}

const AXRString HSSPropertyPathNode::getPropertyName() const
{
    return this->_propertyName;
}

QSharedPointer<HSSClonable> HSSPropertyPathNode::cloneImpl() const
{
    return QSharedPointer<HSSPropertyPathNode>(new HSSPropertyPathNode(*this));
}
