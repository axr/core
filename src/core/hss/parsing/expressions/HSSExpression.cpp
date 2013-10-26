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

#include "AXRLoggerManager.h"
#include "HSSCallback.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSNumberConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSRefFunction.h"
#include "HSSSimpleSelection.h"
#include "HSSValue.h"

using namespace AXR;

HSSExpression::HSSExpression(HSSExpressionType type, QSharedPointer<HSSParserNode> _left, QSharedPointer<HSSParserNode> _right, AXRController * controller)
: HSSParserNode(HSSParserNodeTypeExpression, controller)
{
    this->expressionType = type;
    this->_leftNode = _left;
    this->_rightNode = _right;

    this->leftval = 0;
    this->rightval = 0;
    this->_value = 0;
    this->_isDirty = true;
}

HSSExpression::HSSExpression(const HSSExpression &orig)
: HSSParserNode(orig)
{
    this->expressionType = orig.expressionType;
    this->_leftNode = orig._leftNode->clone();
    this->_rightNode = orig._rightNode->clone();
    this->leftval = orig.leftval;
    this->rightval = orig.rightval;
    this->_value = 0;
    this->_isDirty = true;
}

HSSExpression::~HSSExpression()
{
    this->cleanTrackedObservers();
}

AXRString HSSExpression::toString()
{
    AXRString tempstr;
    this->_nodesToValuesIfNeeded();
    QSharedPointer<HSSObject> left = this->getLeft();
    QSharedPointer<HSSObject> right = this->getRight();
    if (left && right)
    {
        tempstr = AXRString("HSSExpression with following components:\n");
        tempstr.append(this->getLeft()->toString() + "\n" + this->getRight()->toString());
    }
    else
    {
        tempstr = AXRString("HSSExpression with unknown components");
    }
    return tempstr;
}

bool HSSExpression::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSExpression> castedNode = qSharedPointerCast<HSSExpression>(otherNode);
    if ( ! this->_leftNode->equalTo(castedNode->_leftNode)) return false;
    if ( ! this->_rightNode->equalTo(castedNode->_rightNode)) return false;
    return true;
}

bool HSSExpression::isA(HSSExpressionType otherType) const
{
    return otherType == this->expressionType;
}

HSSExpressionType HSSExpression::getExpressionType() const
{
    return this->expressionType;
}

AXRString HSSExpression::expressionTypeStringRepresentation(HSSExpressionType type)
{
    AXRString types[20];
    types[HSSExpressionTypeNone] = "HSSExpression";
    types[HSSExpressionTypeSum] = "HSSSum";
    types[HSSExpressionTypeSubtraction] = "HSSSubtraction";
    types[HSSExpressionTypeMultiplication] = "HSSMultiplication";
    types[HSSExpressionTypeDivision] = "HSSDivision";

    return types[type];
}

HSSUnit HSSExpression::evaluate()
{
    if (this->isDirty())
    {
        this->setDirty(false);
        this->_nodesToValuesIfNeeded();
        //left
        QSharedPointer<HSSValue> left = this->getLeft();
        this->leftval = left->getNumber();
        if (this->left->getValue()->isA(HSSParserNodeTypeExpression) || this->left->getValue()->isA(HSSParserNodeTypeFunctionCall))
        {
            left->observe("__impl_private__valueChanged", "left", this, new HSSValueChangedCallback<HSSExpression>(this, &HSSExpression::leftChanged));
        }
        //right
        QSharedPointer<HSSValue> right = this->getRight();
        this->rightval = right->getNumber();
        if (this->right->getValue()->isA(HSSParserNodeTypeExpression) || this->right->getValue()->isA(HSSParserNodeTypeFunctionCall))
        {
            right->observe("__impl_private__valueChanged", "right", this, new HSSValueChangedCallback<HSSExpression>(this, &HSSExpression::rightChanged));
        }
        //calculate
        this->setValue(this->calculate(this->leftval, this->rightval));
    }

    return this->getValue();
}

void HSSExpression::setLeft(QSharedPointer<HSSValue> newLeft)
{
    this->left = newLeft;
}

QSharedPointer<HSSValue> HSSExpression::getLeft()
{
    return this->left;
}

void HSSExpression::setRight(QSharedPointer<HSSValue> newRight)
{
    this->right = newRight;
}

QSharedPointer<HSSValue> HSSExpression::getRight()
{
    return this->right;
}

void HSSExpression::leftChanged(const AXRString target, const AXRString source, QSharedPointer<HSSObject> theObj)
{
    this->leftval = this->getLeft()->getNumber();
    this->setValue(this->calculate(this->leftval, this->rightval));
    QSharedPointer<HSSObject> newValue = HSSValue::valueFromParserNode(this->getController(), HSSNumberConstant::number(this->getValue(), this->getController()), theObj->getSpecificity(), this->getThisObj(), this->scope);
    this->notifyObservers("__impl_private__expressionResult", newValue);
}

void HSSExpression::rightChanged(const AXRString target, const AXRString source, QSharedPointer<HSSObject> theObj)
{
    this->rightval = this->getRight()->getNumber();
    this->setValue(this->calculate(this->leftval, this->rightval));
    QSharedPointer<HSSObject> newValue = HSSValue::valueFromParserNode(this->getController(), HSSNumberConstant::number(this->getValue(), this->getController()), theObj->getSpecificity(), this->getThisObj(), this->scope);
    this->notifyObservers("__impl_private__expressionResult", newValue);
}

void HSSExpression::setDirty(bool value)
{
    this->_isDirty = value;
}

bool HSSExpression::isDirty()
{
    return this->_isDirty;
}

void HSSExpression::setValue(HSSUnit newValue)
{
    this->_value = newValue;
}

HSSUnit HSSExpression::getValue()
{
    return this->_value;
}

void HSSExpression::setScope(QSharedPointer<HSSSimpleSelection> newScope)
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

void HSSExpression::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    this->thisObj = value;
    //propagate values
    this->_nodesToValuesIfNeeded();
    if (this->left) this->left->setThisObj(value);
    if (this->right) this->right->setThisObj(value);

    HSSParserNode::setThisObj(value);
}

void HSSExpression::_nodesToValuesIfNeeded()
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

void HSSExpression::setHostProperty(AXRString newValue)
{
    HSSParserNode::setHostProperty(newValue);
    if (this->_leftNode) this->_leftNode->setHostProperty(newValue);
    if (this->_rightNode) this->_rightNode->setHostProperty(newValue);
}

void HSSExpression::replace(QSharedPointer<HSSObject> theObj)
{
    if (this->_leftNode)
    {
        switch (this->_leftNode->getType())
        {
            case HSSParserNodeTypeFunctionCall:
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
                QSharedPointer<HSSExpression> expObj = qSharedPointerCast<HSSExpression>(this->_leftNode);
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
            case HSSParserNodeTypeFunctionCall:
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
                QSharedPointer<HSSExpression> expObj = qSharedPointerCast<HSSExpression>(this->_rightNode);
                expObj->replace(theObj);
                break;
            }

            default:
                break;
        }
    }
}
