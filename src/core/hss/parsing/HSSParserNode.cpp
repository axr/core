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
#include "HSSKeywordConstant.h"
#include "HSSNumberConstant.h"
#include "HSSParserNode.h"
#include "HSSPercentageConstant.h"
#include "HSSStringConstant.h"

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

QSharedPointer<HSSNumberConstant> HSSParserNode::number(HSSUnit value, AXRController * controller)
{
    return QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(value, controller));
}

QSharedPointer<HSSPercentageConstant> HSSParserNode::percentage(HSSUnit value, AXRController * controller)
{
    return QSharedPointer<HSSPercentageConstant>(new HSSPercentageConstant(value, controller));
}

QSharedPointer<HSSStringConstant> HSSParserNode::string(AXRString value, AXRController * controller)
{
    return QSharedPointer<HSSStringConstant>(new HSSStringConstant(value, controller));
}

QSharedPointer<HSSKeywordConstant> HSSParserNode::keyword(AXRString value, AXRController * controller)
{
    return QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(value, controller));
}

HSSParserNode::HSSParserNode(HSSParserNodeType type, AXRController * controller)
{
    this->nodeType = type;
    this->controller = controller;
    this->_hostProperty = "";
    this->_specificity = 0;
}

//doesn't clone any part of the node tree, nor the observers array

HSSParserNode::HSSParserNode(const HSSParserNode &orig)
{
    this->nodeType = orig.nodeType;
    this->thisObj = orig.thisObj;
    this->controller = orig.controller;
    this->_hostProperty = orig._hostProperty;
    this->_specificity = orig._specificity;
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

bool HSSParserNode::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //check wether of same type
    if (otherNode->nodeType != this->nodeType) return false;
    //check wether the same amount of child nodes
    if (this->_childNodes.size() != otherNode->_childNodes.size()) return false;
    return true;
}

bool HSSParserNode::isA(HSSParserNodeType otherType) const
{
    return otherType == this->nodeType;
}

HSSParserNodeType HSSParserNode::getType() const
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

std::vector<QSharedPointer<HSSParserNode> > HSSParserNode::getChildNodes() const
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

bool HSSParserNode::isA(HSSExpressionType otherType) const
{
    return false;
}

HSSExpressionType HSSParserNode::getExpressionType() const
{
    return HSSExpressionTypeNone;
}

bool HSSParserNode::isA(HSSStatementType otherType) const
{
    return false;
}

HSSStatementType HSSParserNode::getStatementType() const
{
    return HSSStatementTypeNone;
}

bool HSSParserNode::isA(HSSInstructionType otherType) const
{
    return false;
}

HSSInstructionType HSSParserNode::getInstructionType() const
{
    return HSSInstructionTypeNone;
}

bool HSSParserNode::isA(HSSSelectorType otherType) const
{
    return false;
}

HSSSelectorType HSSParserNode::getSelectorType() const
{
    return HSSSelectorTypeNone;
}

bool HSSParserNode::isA(HSSCombinatorType otherType) const
{
    return false;
}

HSSCombinatorType HSSParserNode::getCombinatorType() const
{
    return HSSCombinatorTypeNone;
}

bool HSSParserNode::isA(HSSFilterType otherType) const
{
    return false;
}

HSSFilterType HSSParserNode::getFilterType() const
{
    return HSSFilterTypeNone;
}

bool HSSParserNode::isA(HSSFunctionType otherType) const
{
    return false;
}

HSSFunctionType HSSParserNode::getFunctionType() const
{
    return HSSFunctionTypeNone;
}

bool HSSParserNode::isA(HSSFlagFunctionType otherType) const
{
    return false;
}

HSSFlagFunctionType HSSParserNode::getFlagFunctionType() const
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

AXRString HSSParserNode::getHostProperty() const
{
    return this->_hostProperty;
}

void HSSParserNode::setHostProperty(AXRString newValue)
{
    this->_hostProperty = newValue;
}

unsigned HSSParserNode::getSpecificity() const
{
    return this->_specificity;
}

void HSSParserNode::setSpecificity(unsigned newValue)
{
    this->_specificity = newValue;
}
