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

#include "HSSDisplayObject.h"
#include "HSSParserNode.h"

using namespace AXR;

AXRString HSSParserNode::parserNodeStringRepresentation(HSSParserNodeType type)
{
    static QMap<HSSParserNodeType, AXRString> types;
    if (types.isEmpty())
    {
        types[HSSParserNodeTypeGeneric] = "HSSParserNode";
        types[HSSParserNodeTypeSelector] = "HSSSelector";
        types[HSSParserNodeTypeCombinator] = "HSSCombinator";
        types[HSSParserNodeTypeFilter] = "HSSFilter";
        types[HSSParserNodeTypeStatement] = "HSSStatement";
        types[HSSParserNodeTypeExpression] = "HSSExpression";
        types[HSSParserNodeTypeNumberConstant] = "HSSNumberConstant";
        types[HSSParserNodeTypePercentageConstant] = "HSSPercentageConstant";
        types[HSSParserNodeTypeStringConstant] = "HSSStringConstant";
        types[HSSParserNodeTypeKeywordConstant] = "HSSKeywordConstant";
        types[HSSParserNodeTypeObjectNameConstant] = "HSSObjectNameConstant";
        types[HSSParserNodeTypeFunctionCall] = "HSSFunctionCall";
        types[HSSParserNodeTypeMultipleValueDefinition] = "HSSParserNodeTypeMultipleValueDefinition";
        types[HSSParserNodeTypeNegation] = "HSSParserNodeTypeNegation";
        types[HSSParserNodeTypeFlag] = "HSSParserNodeTypeFlag";
    }

    AXRString ret = "";
    if (types.contains(type))
    {
        ret = types[type];
    }

    return ret;
}

HSSParserNode::HSSParserNode(HSSParserNodeType type, AXRController * controller)
{
    this->nodeType = type;
    this->controller = controller;
}

//doesn't clone any part of the node tree, nor the observers array

HSSParserNode::HSSParserNode(const HSSParserNode &orig)
{
    this->nodeType = orig.nodeType;
    this->thisObj = orig.thisObj;
    this->controller = orig.controller;
}

QSharedPointer<HSSParserNode> HSSParserNode::clone() const
{
    return qSharedPointerCast<HSSParserNode> (this->cloneImpl());
}

AXRString HSSParserNode::toString()
{
    return "Generic parser node - you forgot to override toString in your subclass or somehow using HSSParserNode directly";
}

std::string HSSParserNode::toStdString()
{
    AXRString tempstr = this->toString();
    return tempstr.toStdString();
}

AXRString HSSParserNode::stringRep()
{
    return "Generic parser node - you forgot to override toString in your subclass or somehow using HSSParserNode directly";
}

bool HSSParserNode::isA(HSSParserNodeType otherType)
{
    return otherType == this->nodeType;
}

HSSParserNodeType HSSParserNode::getType()
{
    return this->nodeType;
}

QSharedPointer<HSSParserNode> HSSParserNode::getParentNode()
{
    if (!this->_parentNode.isNull())
    {
        QSharedPointer<HSSParserNode> parent = this->_parentNode.toStrongRef();
        return parent;
    }
    else
    {
        return QSharedPointer<HSSParserNode>();
    }
}

void HSSParserNode::setParentNode(QSharedPointer<HSSParserNode> newParent)
{
    this->_parentNode = QWeakPointer<HSSParserNode>(newParent);
}

void HSSParserNode::removeFromParentNode()
{
    QSharedPointer<HSSParserNode> parentNode = this->getParentNode();
    if (parentNode) parentNode->removeNode(this->shared_from_this());
}

void HSSParserNode::addNode(QSharedPointer<HSSParserNode> child)
{
    this->_childNodes.push_back(child);
}

void HSSParserNode::removeNode(QSharedPointer<HSSParserNode> child)
{
    HSSParserNode::it it = find(this->_childNodes.begin(), this->_childNodes.end(), child);
    if (it != this->_childNodes.end())
    {
        this->_childNodes.erase(it);
    }
}

const std::vector<QSharedPointer<HSSParserNode> > HSSParserNode::getChildNodes() const
{
    return this->_childNodes;
}

QSharedPointer<HSSClonable> HSSParserNode::cloneImpl() const
{
    return QSharedPointer<HSSParserNode>(new HSSParserNode(*this));
}

void HSSParserNode::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    this->thisObj = value;
}

QSharedPointer<HSSDisplayObject> HSSParserNode::getThisObj()
{
    return thisObj;
}

bool HSSParserNode::isA(HSSExpressionType otherType)
{
    return false;
}

HSSExpressionType HSSParserNode::getExpressionType()
{
    return HSSExpressionTypeNone;
}

bool HSSParserNode::isA(HSSStatementType otherType)
{
    return false;
}

HSSStatementType HSSParserNode::getStatementType()
{
    return HSSStatementTypeNone;
}

bool HSSParserNode::isA(HSSInstructionType otherType)
{
    return false;
}

HSSInstructionType HSSParserNode::getInstructionType()
{
    return HSSInstructionTypeNone;
}

bool HSSParserNode::isA(HSSSelectorType otherType)
{
    return false;
}

HSSSelectorType HSSParserNode::getSelectorType()
{
    return HSSSelectorTypeNone;
}

bool HSSParserNode::isA(HSSCombinatorType otherType)
{
    return false;
}

HSSCombinatorType HSSParserNode::getCombinatorType()
{
    return HSSCombinatorTypeNone;
}

bool HSSParserNode::isA(HSSFilterType otherType)
{
    return false;
}

HSSFilterType HSSParserNode::getFilterType()
{
    return HSSFilterTypeNone;
}

bool HSSParserNode::isA(HSSFunctionType otherType)
{
    return false;
}

HSSFunctionType HSSParserNode::getFunctionType()
{
    return HSSFunctionTypeNone;
}

bool HSSParserNode::isA(HSSFlagFunctionType otherType)
{
    return false;
}

HSSFlagFunctionType HSSParserNode::getFlagFunctionType()
{
    return HSSFlagFunctionTypeNone;
}

QSharedPointer<HSSParserNode> HSSParserNode::shared_from_this()
{
    if (!ptr)
        ptr = QWeakPointer<HSSParserNode>(this);

    return ptr.toStrongRef();
}

AXRController* HSSParserNode::getController()
{
    return controller;
}

void HSSParserNode::setController(AXRController *controller)
{
    this->controller = controller;
}
