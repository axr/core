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

#include "axr.h"
#include "HSSSymbolNode.h"

using namespace AXR;

QSharedPointer<HSSSymbolNode> HSSSymbolNode::createSymbol(AXRString value, AXRController * controller)
{
    return QSharedPointer<HSSSymbolNode>(new HSSSymbolNode(value, controller));
}

HSSSymbolNode::HSSSymbolNode(AXRString value, AXRController * controller)
: HSSParserNode(HSSParserNodeTypeSymbolNode, controller)
{
    this->value = value;
}

QSharedPointer<HSSSymbolNode> HSSSymbolNode::clone() const
{
    return qSharedPointerCast<HSSSymbolNode> (this->cloneImpl());
}

HSSSymbolNode::~HSSSymbolNode()
{
    
}

void HSSSymbolNode::setValue(AXRString newValue)
{
    this->value = newValue;
}

AXRString HSSSymbolNode::getValue()
{
    return this->value;
}

AXRString HSSSymbolNode::toString()
{
    return this->value;
}

AXRString HSSSymbolNode::stringRep()
{
    return this->value;
}

bool HSSSymbolNode::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSSymbolNode> castedNode = qSharedPointerCast<HSSSymbolNode>(otherNode);
    if ( this->value != castedNode->value ) return false;
    return true;
}

QSharedPointer<HSSClonable> HSSSymbolNode::cloneImpl() const
{
    return QSharedPointer<HSSSymbolNode>(new HSSSymbolNode(*this));
}
