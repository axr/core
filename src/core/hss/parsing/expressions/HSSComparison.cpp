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
#include "HSSComparison.h"

using namespace AXR;

HSSComparison::HSSComparison(HSSComparisonType type, QSharedPointer<HSSParserNode> _left, QSharedPointer<HSSParserNode> _right, AXRController * controller)
: HSSParserNode(HSSParserNodeTypeComparison, controller)
{
    this->comparisonType = type;
    this->setLeftNode(_left);
    this->setRightNode(_right);

    this->_value = false;
    this->_isDirty = true;
}

HSSComparison::HSSComparison(HSSComparisonType type, AXRController * controller)
: HSSParserNode(HSSParserNodeTypeComparison, controller)
{
    this->comparisonType = type;

    this->_value = false;
    this->_isDirty = true;
}

HSSComparison::HSSComparison(const HSSComparison &orig)
: HSSParserNode(orig)
{
    this->comparisonType = orig.comparisonType;
    this->_value = false;
    this->_isDirty = true;
}

HSSComparison::~HSSComparison()
{
    this->cleanTrackedObservers();
}

QSharedPointer<HSSComparison> HSSComparison::clone() const
{
    return qSharedPointerCast<HSSComparison> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSComparison::cloneImpl() const
{
    QSharedPointer<HSSComparison> clone(new HSSComparison(*this));
    clone->setLeftNode(this->_leftNode->clone());
    clone->setRightNode(this->_rightNode->clone());
    return clone;
}

AXRString HSSComparison::toString()
{
    AXRString tempstr;
    this->_nodesToValuesIfNeeded();
    QSharedPointer<HSSObject> left = this->getLeft();
    QSharedPointer<HSSObject> right = this->getRight();
    if (left && right)
    {
        tempstr = AXRString("HSSComparison with following components:\n");
        tempstr.append(this->getLeft()->toString() + "\n" + this->getRight()->toString());
    }
    else
    {
        tempstr = AXRString("HSSComparison with unknown components");
    }
    return tempstr;
}

bool HSSComparison::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSComparison> castedNode = qSharedPointerCast<HSSComparison>(otherNode);
    if ( ! this->_leftNode->equalTo(castedNode->_leftNode)) return false;
    if ( ! this->_rightNode->equalTo(castedNode->_rightNode)) return false;
    return true;
}

bool HSSComparison::isA(HSSComparisonType otherType) const
{
    return otherType == this->comparisonType;
}

HSSComparisonType HSSComparison::getComparisonType() const
{
    return this->comparisonType;
}

AXRString HSSComparison::comparisonTypeStringRepresentation(HSSComparisonType type)
{
    AXRString types[20];
    types[HSSComparisonTypeNone] = "HSSComparison";
    types[HSSComparisonTypeAnd] = "HSSComparisonTypeAnd";
    types[HSSComparisonTypeOr] = "HSSComparisonTypeOr";
    
    return types[type];
}

bool HSSComparison::evaluate()
{
    if (this->isDirty())
    {
//        this->setDirty(false);
        this->_nodesToValuesIfNeeded();
        //left
        QSharedPointer<HSSObject> left = this->getLeft();
        if (left->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSValue> leftValue = qSharedPointerCast<HSSValue>(left);
            if (leftValue->getValue()->isA(HSSParserNodeTypeComparison) || leftValue->getValue()->isA(HSSParserNodeTypeFunction))
            {
                left->observe("__impl_private__valueChanged", "left", this, new HSSValueChangedCallback<HSSComparison>(this, &HSSComparison::leftChanged));
            }
        }
        //right
        QSharedPointer<HSSObject> right = this->getRight();
        if (right->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSValue> rightValue = qSharedPointerCast<HSSValue>(right);
            if (rightValue->getValue()->isA(HSSParserNodeTypeComparison) || rightValue->getValue()->isA(HSSParserNodeTypeFunction))
            {
                right->observe("__impl_private__valueChanged", "right", this, new HSSValueChangedCallback<HSSComparison>(this, &HSSComparison::rightChanged));
            }
        }
        //calculate
        this->setValue(this->compare(left, right));
    }
    
    return this->getValue();
}

void HSSComparison::setLeft(QSharedPointer<HSSObject> newLeft)
{
    this->left = newLeft;
}

QSharedPointer<HSSObject> HSSComparison::getLeft()
{
    return this->left;
}

void HSSComparison::setRight(QSharedPointer<HSSObject> newRight)
{
    this->right = newRight;
}

QSharedPointer<HSSObject> HSSComparison::getRight()
{
    return this->right;
}

void HSSComparison::setLeftNode(QSharedPointer<AXR::HSSParserNode> newLeftNode)
{
    newLeftNode->setParentNode(this->shared_from_this());
    this->_leftNode = newLeftNode;
}

void HSSComparison::setRightNode(QSharedPointer<AXR::HSSParserNode> newRightNode)
{
    newRightNode->setParentNode(this->shared_from_this());
    this->_rightNode = newRightNode;
}

void HSSComparison::leftChanged(const AXRString target, const AXRString source, QSharedPointer<HSSObject> theObj)
{
    this->setValue(this->compare(this->left, this->right));
    QSharedPointer<HSSObject> newValue = HSSValue::valueFromParserNode(this->getController(), HSSNumberConstant::number(this->getValue(), this->getController()), theObj->getSpecificity(), this->getThisObj(), this->scope);
    this->notifyObservers("__impl_private__expressionResult", newValue);
}

void HSSComparison::rightChanged(const AXRString target, const AXRString source, QSharedPointer<HSSObject> theObj)
{
    this->setValue(this->compare(this->left, this->right));
    QSharedPointer<HSSObject> newValue = HSSValue::valueFromParserNode(this->getController(), HSSNumberConstant::number(this->getValue(), this->getController()), theObj->getSpecificity(), this->getThisObj(), this->scope);
    this->notifyObservers("__impl_private__expressionResult", newValue);
}

void HSSComparison::setDirty(bool value)
{
    this->_isDirty = value;
}

bool HSSComparison::isDirty()
{
    return this->_isDirty;
}

void HSSComparison::setValue(bool newValue)
{
    this->_value = newValue;
}

bool HSSComparison::getValue()
{
    return this->_value;
}

void HSSComparison::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    this->scope = newScope;
    this->setDirty(true);
}

void HSSComparison::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    this->thisObj = value;
    this->_leftNode->setThisObj(value);
    this->_rightNode->setThisObj(value);
    HSSParserNode::setThisObj(value);
}

void HSSComparison::_nodesToValuesIfNeeded()
{
    if (!this->left && this->_leftNode)
    {
        QSharedPointer<HSSDisplayObject> thisObj = this->getThisObj();
        if(thisObj)
        {
            this->left = qSharedPointerCast<HSSObject>(thisObj->computeObject(this->_leftNode, this->getHostProperty()));
            this->left->setThisObj(this->getThisObj());
            this->left->setScope(this->scope);
        }
    }
    if (!this->right && this->_rightNode)
    {
        QSharedPointer<HSSDisplayObject> thisObj = this->getThisObj();
        if(thisObj)
        {
            this->right = qSharedPointerCast<HSSObject>(thisObj->computeObject(this->_rightNode, this->getHostProperty()));
            this->right->setThisObj(this->getThisObj());
            this->right->setScope(this->scope);
        }
    }
}

void HSSComparison::setHostProperty(AXRString newValue)
{
    HSSParserNode::setHostProperty(newValue);
    if (this->_leftNode) this->_leftNode->setHostProperty(newValue);
    if (this->_rightNode) this->_rightNode->setHostProperty(newValue);
}

void HSSComparison::replace(QSharedPointer<HSSObject> theObj)
{
    if (this->_leftNode)
    {
        switch (this->_leftNode->getType())
        {
            case HSSParserNodeTypeFunction:
            {
                if (this->_leftNode->isA(HSSFunctionTypeRef))
                {
                    QSharedPointer<HSSRefFunction> refFn = qSharedPointerCast<HSSRefFunction>(this->_leftNode);
                    refFn->replace(theObj);
                }
                break;
            }
                
            case HSSParserNodeTypeComparison:
            {
                QSharedPointer<HSSComparison> expObj = qSharedPointerCast<HSSComparison>(this->_leftNode);
                expObj->replace(theObj);
                break;
            }
                
            default:
                break;
        }
    }
    if (this->_rightNode)
    {
        switch (this->_rightNode->getType())
        {
            case HSSParserNodeTypeFunction:
            {
                if (this->_rightNode->isA(HSSFunctionTypeRef))
                {
                    QSharedPointer<HSSRefFunction> refFn = qSharedPointerCast<HSSRefFunction>(this->_rightNode);
                    refFn->replace(theObj);
                }
                break;
            }
                
            case HSSParserNodeTypeComparison:
            {
                QSharedPointer<HSSComparison> expObj = qSharedPointerCast<HSSComparison>(this->_rightNode);
                expObj->replace(theObj);
                break;
            }
                
            default:
                break;
        }
    }
}

bool HSSComparison::compare(QSharedPointer<HSSObject> left, QSharedPointer<HSSObject> right)
{
    if (left->getObjectType() != right->getObjectType())
        return false;
    if (left->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSValue> leftVal = qSharedPointerCast<HSSValue>(left);
        QSharedPointer<HSSParserNode> leftNode = leftVal->getValue();
        //if there is no parser node, it means it is a regular @value object with properties
        if (!leftVal)
        {
//                    return this->_objectCompare(left, right);
        }
        
        QSharedPointer<HSSValue> rightVal = qSharedPointerCast<HSSValue>(right);
        QSharedPointer<HSSParserNode> rightNode = rightVal->getValue();
        if (leftNode && rightNode)
        {
            return this->_compare(this->_evaluate(leftNode), this->_evaluate(rightNode));
        }
    }
    else
    {
//      return this->_objectCompare(left, right);
    }
    return false;
}

bool HSSComparison::_compare(QSharedPointer<HSSParserNode> leftNode, QSharedPointer<HSSParserNode> rightNode)
{
    if (!leftNode || !rightNode)
        return false;

    if (leftNode->getType() != rightNode->getType()) {
        if (this->getComparisonType() == HSSComparisonTypeNotEqual)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    switch (this->getComparisonType())
    {
        case HSSComparisonTypeEqual:
        {
            switch (leftNode->getType())
            {
                case HSSParserNodeTypeNumberConstant:
                {
                    return qSharedPointerCast<HSSNumberConstant>(leftNode)->getValue() == qSharedPointerCast<HSSNumberConstant>(rightNode)->getValue();
                }
                case HSSParserNodeTypeStringConstant:
                {
                    return qSharedPointerCast<HSSStringConstant>(leftNode)->getValue() == qSharedPointerCast<HSSStringConstant>(rightNode)->getValue();
                }
                case HSSParserNodeTypeBooleanConstant:
                {
                    return qSharedPointerCast<HSSBooleanConstant>(leftNode)->getValue() == qSharedPointerCast<HSSBooleanConstant>(rightNode)->getValue();
                }
                default:
                {
                    AXRError("HSSComparison", "Unexpected parser node type in comparison").raise();
                    break;
                }
            }
        }
        case HSSComparisonTypeNotEqual:
        {
            switch (leftNode->getType())
            {
                case HSSParserNodeTypeNumberConstant:
                {
                    return qSharedPointerCast<HSSNumberConstant>(leftNode)->getValue() != qSharedPointerCast<HSSNumberConstant>(rightNode)->getValue();
                }
                case HSSParserNodeTypeStringConstant:
                {
                    return qSharedPointerCast<HSSStringConstant>(leftNode)->getValue() != qSharedPointerCast<HSSStringConstant>(rightNode)->getValue();
                }
                case HSSParserNodeTypeBooleanConstant:
                {
                    return qSharedPointerCast<HSSBooleanConstant>(leftNode)->getValue() != qSharedPointerCast<HSSBooleanConstant>(rightNode)->getValue();
                }
                default:
                {
                    AXRError("HSSComparison", "Unexpected parser node type in comparison").raise();
                    break;
                }
            }
        }
        case HSSComparisonTypeBigger:
        {
            if (leftNode->isA(HSSParserNodeTypeNumberConstant))
            {
                return qSharedPointerCast<HSSNumberConstant>(leftNode)->getValue() > qSharedPointerCast<HSSNumberConstant>(rightNode)->getValue();
            }
            else
            {
                AXRError("HSSComparison", "Unexpected parser node type in comparison").raise();
            }
        }
        case HSSComparisonTypeSmaller:
        {
            if (leftNode->isA(HSSParserNodeTypeNumberConstant))
            {
                return qSharedPointerCast<HSSNumberConstant>(leftNode)->getValue() < qSharedPointerCast<HSSNumberConstant>(rightNode)->getValue();
            }
            else
            {
                AXRError("HSSComparison", "Unexpected parser node type in comparison").raise();
            }
        }
        case HSSComparisonTypeBiggerOrEqual:
        {
            if (leftNode->isA(HSSParserNodeTypeNumberConstant))
            {
                return qSharedPointerCast<HSSNumberConstant>(leftNode)->getValue() >= qSharedPointerCast<HSSNumberConstant>(rightNode)->getValue();
            }
            else
            {
                AXRError("HSSComparison", "Unexpected parser node type in comparison").raise();
            }
        }
        case HSSComparisonTypeSmallerOrEqual:
        {
            if (leftNode->isA(HSSParserNodeTypeNumberConstant))
            {
                return qSharedPointerCast<HSSNumberConstant>(leftNode)->getValue() <= qSharedPointerCast<HSSNumberConstant>(rightNode)->getValue();
            }
            else
            {
                AXRError("HSSComparison", "Unexpected parser node type in comparison").raise();
            }
        }
        default:
            break;
    }
    return false;
}

QSharedPointer<HSSParserNode> HSSComparison::_evaluate(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeStringConstant:
        {
            QSharedPointer<HSSStringConstant> theStr = qSharedPointerCast<HSSStringConstant>(parserNode);
            if (theStr->hasArguments())
            {
                QSharedPointer<HSSStringConstant> newStr(new HSSStringConstant(theStr->evaluate(), this->getController()));
                return newStr;
            }
            else
                return parserNode;
        }
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSObject> remoteObj = qSharedPointerCast<HSSFunction>(parserNode)->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                return this->_evaluate(valueNode);
            }
        }
        case HSSParserNodeTypePropertyPath:
        {
            QSharedPointer<HSSObject> remoteObj = qSharedPointerCast<HSSPropertyPath>(parserNode)->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                return this->_evaluate(valueNode);
            }
        }
        default:
            return parserNode;
    }
}

