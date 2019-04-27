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

HSSPropertyPathNode::HSSPropertyPathNode(QSharedPointer<HSSParserNode> value, AXRController * controller)
: HSSParserNode(HSSParserNodeTypePropertyPathNode, controller)
{
    this->_value = value;
    this->_hasName = true;
}

HSSPropertyPathNode::HSSPropertyPathNode(const HSSPropertyPathNode & orig)
: HSSParserNode(orig)
{
    this->_hasName = orig._hasName;
}

QSharedPointer<HSSPropertyPathNode> HSSPropertyPathNode::clone() const
{
    return qSharedPointerCast<HSSPropertyPathNode> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSPropertyPathNode::cloneImpl() const
{
    QSharedPointer<HSSPropertyPathNode> clone(new HSSPropertyPathNode(*this));
    clone->_value = this->_value->clone();
    return clone;
}

HSSPropertyPathNode::~HSSPropertyPathNode()
{

}

AXRString HSSPropertyPathNode::toString()
{
    return this->_value->toString();
}

AXRString HSSPropertyPathNode::stringRep()
{
    return this->_value->stringRep();
}

bool HSSPropertyPathNode::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSPropertyPathNode> castedNode = qSharedPointerCast<HSSPropertyPathNode>(otherNode);
    if (castedNode->_value != this->_value) return false;
    if (castedNode->_hasName != this->_hasName) return false;
    return true;
}

void HSSPropertyPathNode::setHasName(bool newValue)
{
    this->_hasName = newValue;
}

const bool HSSPropertyPathNode::hasName() const
{
    return this->_hasName;
}

const QSharedPointer<HSSParserNode> & HSSPropertyPathNode::getValue() const
{
    return this->_value;
}

AXRString HSSPropertyPathNode::evaluate()
{
    if (this->_hasName)
        return this->_evaluate(this->_value);
    return "";
}

AXRString HSSPropertyPathNode::_evaluate(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeStringConstant:
        {
            return qSharedPointerCast<HSSStringConstant>(parserNode)->getValue();
        }
        case HSSParserNodeTypePropertyNameConstant:
        {
            return qSharedPointerCast<HSSPropertyNameConstant>(parserNode)->getValue();
        }
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> theFunc = qSharedPointerCast<HSSFunction>(parserNode);
            QSharedPointer<HSSObject> remoteObj = theFunc->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                if (valueNode)
                {
                    return this->_evaluate(valueNode);
                }
            }
            break;
        }
        default:
            break;
    }
    return "";
}

void HSSPropertyPathNode::setThisObj(QSharedPointer<AXR::HSSDisplayObject> value)
{
    HSSParserNode::setThisObj(value);
    this->_value->setThisObj(value);
}

void HSSPropertyPathNode::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    switch (this->_value->getType()) {
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> theFunc = qSharedPointerCast<HSSFunction>(this->_value);
            theFunc->setScope(newScope);
            break;
        }
        case HSSParserNodeTypeStringConstant:
        {
            QSharedPointer<HSSStringConstant> theStr = qSharedPointerCast<HSSStringConstant>(this->_value);
            theStr->setScope(newScope);
            break;
        }
        default:
            break;
    }
}
