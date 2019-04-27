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
#include "HSSUnaryExpression.h"

using namespace AXR;

HSSUnaryExpression::HSSUnaryExpression(HSSUnaryExpressionType type, AXRController * controller)
: HSSParserNode(HSSParserNodeTypeUnaryExpression, controller)
{
    this->expressionType = type;
    
    this->leftval = 0;
    this->rightval = 0;
    this->_value = 0;
    this->_isDirty = true;
}

HSSUnaryExpression::HSSUnaryExpression(const HSSUnaryExpression &orig)
: HSSParserNode(orig)
{
    this->expressionType = orig.expressionType;
    if (orig._leftNode)
    {
        this->_leftNode = orig._leftNode->clone();
    }
    if (orig._rightNode)
    {
        this->_rightNode = orig._rightNode->clone();
    }
    this->leftval = orig.leftval;
    this->rightval = orig.rightval;
    this->_value = 0;
    this->_isDirty = true;
}

HSSUnaryExpression::~HSSUnaryExpression()
{
    this->cleanTrackedObservers();
}

AXRString HSSUnaryExpression::toString()
{
    AXRString tempstr;
    this->_nodesToValuesIfNeeded();
    QSharedPointer<HSSObject> left = this->getLeft();
    QSharedPointer<HSSObject> right = this->getRight();
    if (left && right)
    {
        tempstr = AXRString("HSSUnaryExpression with following components:\n");
        tempstr.append(this->getLeft()->toString() + "\n" + this->getRight()->toString());
    }
    else
    {
        tempstr = AXRString("HSSUnaryExpression with unknown components");
    }
    return tempstr;
}

bool HSSUnaryExpression::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSUnaryExpression> castedNode = qSharedPointerCast<HSSUnaryExpression>(otherNode);
    if ( this->_leftNode && !this->_leftNode->equalTo(castedNode->_leftNode)) return false;
    if ( ! this->_rightNode->equalTo(castedNode->_rightNode)) return false;
    return true;
}

bool HSSUnaryExpression::isA(HSSUnaryExpressionType otherType) const
{
    return otherType == this->expressionType;
}

HSSUnaryExpressionType HSSUnaryExpression::getUnaryExpressionType() const
{
    return this->expressionType;
}

AXRString HSSUnaryExpression::expressionTypeStringRepresentation(HSSUnaryExpressionType type)
{
    AXRString types[20];
    types[HSSUnaryExpressionTypeSum] = "HSSUnarySum";
    types[HSSUnaryExpressionTypeSubtraction] = "HSSUnarySubtraction";
    types[HSSUnaryExpressionTypeMultiplication] = "HSSUnaryMultiplication";
    types[HSSUnaryExpressionTypeDivision] = "HSSUnaryDivision";
    return types[type];
}

HSSUnit HSSUnaryExpression::evaluate()
{
    if (this->isDirty())
    {
        this->setDirty(false);
        this->_nodesToValuesIfNeeded();
        //left
        QSharedPointer<HSSValue> left = this->getLeft();
        if (!left)
            return 0;

        this->leftval = left->getNumber();
        if (this->left->getValue()->isA(HSSParserNodeTypeExpression) || this->left->getValue()->isA(HSSParserNodeTypeFunction))
        {
            left->observe("__impl_private__valueChanged", "left", this, new HSSValueChangedCallback<HSSUnaryExpression>(this, &HSSUnaryExpression::leftChanged));
        }
        //right
        QSharedPointer<HSSValue> right = this->getRight();
        this->rightval = right->getNumber();
        if (this->right->getValue()->isA(HSSParserNodeTypeExpression) || this->right->getValue()->isA(HSSParserNodeTypeFunction))
        {
            right->observe("__impl_private__valueChanged", "right", this, new HSSValueChangedCallback<HSSUnaryExpression>(this, &HSSUnaryExpression::rightChanged));
        }
        //calculate
        this->setValue(this->calculate(this->leftval, this->rightval));
    }
    
    return this->getValue();
}

void HSSUnaryExpression::setLeft(QSharedPointer<HSSValue> newLeft)
{
    this->left = newLeft;
}

void HSSUnaryExpression::setLeftNode(QSharedPointer<HSSParserNode> newLeftNode)
{
    this->_leftNode = newLeftNode;
    this->left.clear();
}

QSharedPointer<HSSValue> HSSUnaryExpression::getLeft()
{
    return this->left;
}

void HSSUnaryExpression::setRight(QSharedPointer<HSSValue> newRight)
{
    this->right = newRight;
}

void HSSUnaryExpression::setRightNode(QSharedPointer<HSSParserNode> newRightNode)
{
    this->_rightNode = newRightNode;
    this->right.clear();
}

QSharedPointer<HSSValue> HSSUnaryExpression::getRight()
{
    return this->right;
}

QSharedPointer<HSSParserNode> HSSUnaryExpression::getRightNode()
{
    return this->_rightNode;
}

void HSSUnaryExpression::leftChanged(const AXRString target, const AXRString source, QSharedPointer<HSSObject> theObj)
{
    this->leftval = this->getLeft()->getNumber();
    this->setValue(this->calculate(this->leftval, this->rightval));
    QSharedPointer<HSSObject> newValue = HSSValue::valueFromParserNode(this->getController(), HSSNumberConstant::number(this->getValue(), this->getController()), theObj->getSpecificity(), this->getThisObj(), this->scope);
    this->notifyObservers("__impl_private__expressionResult", newValue);
}

void HSSUnaryExpression::rightChanged(const AXRString target, const AXRString source, QSharedPointer<HSSObject> theObj)
{
    this->rightval = this->getRight()->getNumber();
    this->setValue(this->calculate(this->leftval, this->rightval));
    QSharedPointer<HSSObject> newValue = HSSValue::valueFromParserNode(this->getController(), HSSNumberConstant::number(this->getValue(), this->getController()), theObj->getSpecificity(), this->getThisObj(), this->scope);
    this->notifyObservers("__impl_private__expressionResult", newValue);
}

void HSSUnaryExpression::setDirty(bool value)
{
    this->_isDirty = value;
}

bool HSSUnaryExpression::isDirty()
{
    return this->_isDirty;
}

void HSSUnaryExpression::setValue(HSSUnit newValue)
{
    this->_value = newValue;
}

HSSUnit HSSUnaryExpression::getValue()
{
    return this->_value;
}

void HSSUnaryExpression::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    this->scope = newScope;
    //propagate values
    this->_nodesToValuesIfNeeded();
    if (this->left)
    {
        this->left->setScope(newScope);
    }
    if (this->right)
    {
        this->right->setScope(newScope);
    }
    
    this->setDirty(true);
}

void HSSUnaryExpression::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    this->thisObj = value;
    //propagate values
    this->_nodesToValuesIfNeeded();
    if (this->left) this->left->setThisObj(value);
    if (this->right) this->right->setThisObj(value);
    
    HSSParserNode::setThisObj(value);
}

void HSSUnaryExpression::_nodesToValuesIfNeeded()
{
    if (!this->left && this->_leftNode)
    {
        QSharedPointer<HSSDisplayObject> thisObj = this->getThisObj();
        if(thisObj)
        {
            this->left = qSharedPointerCast<HSSValue>(thisObj->computeValueObject(this->_leftNode, this->getHostProperty()));
        }
    }
    if (!this->right && this->_rightNode)
    {
        QSharedPointer<HSSDisplayObject> thisObj = this->getThisObj();
        if(thisObj)
        {
            this->right = qSharedPointerCast<HSSValue>(thisObj->computeValueObject(this->_rightNode, this->getHostProperty()));
        }
    }
}

void HSSUnaryExpression::setHostProperty(AXRString newValue)
{
    HSSParserNode::setHostProperty(newValue);
    if (this->_leftNode) this->_leftNode->setHostProperty(newValue);
    if (this->_rightNode) this->_rightNode->setHostProperty(newValue);
}

void HSSUnaryExpression::replace(QSharedPointer<HSSObject> theObj)
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
                
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSUnaryExpression> expObj = qSharedPointerCast<HSSUnaryExpression>(this->_leftNode);
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
                
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSUnaryExpression> expObj = qSharedPointerCast<HSSUnaryExpression>(this->_rightNode);
                expObj->replace(theObj);
                break;
            }
                
            default:
                break;
        }
    }
}

void HSSUnaryExpression::merge(QSharedPointer<HSSParserNode> other)
{
    switch (other->getType())
    {
        case HSSParserNodeTypeUnaryExpression:
        {
            QSharedPointer<HSSUnaryExpression> uExp = qSharedPointerCast<HSSUnaryExpression>(other);
            if (uExp->_rightNode->isA(HSSParserNodeTypeNumberConstant))
            {
                QSharedPointer<HSSNumberConstant> numConst = qSharedPointerCast<HSSNumberConstant>(uExp->_rightNode);
                this->mergeNumber(numConst, uExp);
            }
        }

        default:
            break;
    }
}

void HSSUnaryExpression::merge(QSharedPointer<HSSUnaryExpression> other)
{
    if (other->left)
    {
        this->setLeft(other->getLeft());
    }
    else if (other->_leftNode)
    {
        this->_leftNode = other->_leftNode->clone();
    }
    
    QSharedPointer<HSSParserNode> newRight = this->__mergeVal(other->_rightNode, this->_rightNode);
    if (newRight)
    {
        this->_rightNode = newRight;
    }
    else
    {
        //this should never happen
        this->_rightNode = other->_rightNode->clone();
    }
    this->left.clear();
    this->right.clear();
    this->setDirty(true);
}

QSharedPointer<HSSParserNode> HSSUnaryExpression::__mergeVal(QSharedPointer<HSSParserNode> a, QSharedPointer<HSSParserNode> b)
{
    QSharedPointer<HSSParserNode> ret;
    switch (a->getType())
    {
        case HSSParserNodeTypeNumberConstant:
        {
            return this->__mergeNumConst(qSharedPointerCast<HSSNumberConstant>(a), b);
            break;
        }
        case HSSParserNodeTypePercentageConstant:
        {
            break;
        }
        case HSSParserNodeTypeExpression:
        {
            break;
        }
        case HSSParserNodeTypeFunction:
        {
            break;
        }
        default:
            break;
    }
    return ret;
}

QSharedPointer<HSSParserNode> HSSUnaryExpression::__mergeNumConst(QSharedPointer<HSSNumberConstant> a, QSharedPointer<HSSParserNode> b)
{
    QSharedPointer<HSSParserNode> ret;
    switch (b->getType()) {
        case HSSParserNodeTypeNumberConstant:
        {
            QSharedPointer<HSSNumberConstant> numB = qSharedPointerCast<HSSNumberConstant>(b);
            ret = QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(this->calculate(a->getValue(), numB->getValue()), this->getController()));
            break;
        }
        default:
            break;
    }
    return ret;
}

HSSUnit HSSUnaryExpression::calculate(HSSUnit leftval, HSSUnit rightval)
{
    //override this
    return 0;
}

void HSSUnaryExpression::mergeNumber(QSharedPointer<HSSNumberConstant> numConst, QSharedPointer<HSSUnaryExpression> other)
{
    //override this
}


QSharedPointer<HSSNumberConstant> HSSUnaryExpression::appendNumber(QSharedPointer<HSSObject> currentValue)
{
    QSharedPointer<HSSNumberConstant> ret;
    if (currentValue && currentValue->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> currentValueNode = qSharedPointerCast<HSSValue>(currentValue)->getValue();
        switch (currentValueNode->getType())
        {
            case HSSParserNodeTypeNumberConstant:
            {
                QSharedPointer<HSSNumberConstant> currentNum = qSharedPointerCast<HSSNumberConstant>(currentValueNode);
                QSharedPointer<HSSParserNode> rightNode = this->getRightNode();
                
                QSharedPointer<HSSNumberConstant> newNumConst(new HSSNumberConstant(this->calculate(currentNum->getValue(), this->_getRightNumber(rightNode)), this->getController()));
                return newNumConst;
            }
            default:
                break;
        }
    }
    return ret;
}

HSSUnit HSSUnaryExpression::_getRightNumber(QSharedPointer<HSSParserNode> rightNode)
{
    HSSUnit ret = 0;
    switch (rightNode->getType())
    {
        case HSSParserNodeTypeNumberConstant:
        {
            QSharedPointer<HSSNumberConstant> rightNum = qSharedPointerCast<HSSNumberConstant>(rightNode);
            ret = rightNum->getValue();
            break;
        }
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> rightFunction = qSharedPointerCast<HSSFunction>(rightNode);
            rightFunction->setThisObj(this->getThisObj());
            rightFunction->setScope(this->scope);
            QSharedPointer<HSSObject> remoteObj = rightFunction->evaluate();
            if(remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                if (valueNode)
                {
                    return this->_getRightNumber(valueNode);
                }
            }
            break;
        }
        case HSSParserNodeTypeObjectNameConstant:
        {
            QSharedPointer<HSSObjectNameConstant> theObjName = qSharedPointerCast<HSSObjectNameConstant>(rightNode);
            QSharedPointer<HSSObject> remoteObj = this->getController()->getGlobalVariable(theObjName->getValue());
            if(remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                if (valueNode)
                {
                    return this->_getRightNumber(valueNode);
                }
            }
        }
        default:
        {
            AXRWarning("HSSUnarySum", "Unknown node type while getting right number.").raise();
            break;
        }
    }
    return ret;
}
