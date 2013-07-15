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
}

HSSExpression::HSSExpression(const HSSExpression &orig)
: HSSParserNode(orig)
{
    this->expressionType = orig.expressionType;
    this->_leftNode = orig._leftNode->clone();
    this->_rightNode = orig._rightNode->clone();
    this->leftval = orig.leftval;
    this->rightval = orig.rightval;
}

HSSExpression::~HSSExpression()
{
}

AXRString HSSExpression::toString()
{
    AXRString tempstr = AXRString("HSSExpression with following components:\n");
    this->_nodesToValuesIfNeeded();
    tempstr.append(this->getLeft()->toString() + "\n" + this->getRight()->toString());
    return tempstr;
}

bool HSSExpression::isA(HSSExpressionType otherType)
{
    return otherType == this->expressionType;
}

HSSExpressionType HSSExpression::getExpressionType()
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
        left->observe("value", "left", this, new HSSValueChangedCallback<HSSExpression>(this, &HSSExpression::leftChanged));
        //right
        QSharedPointer<HSSValue> right = this->getRight();
        this->rightval = right->getNumber();
        right->observe("value", "right", this, new HSSValueChangedCallback<HSSExpression>(this, &HSSExpression::rightChanged));
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

void HSSExpression::leftChanged(AXRString property, QSharedPointer<HSSObject> theObj)
{
    this->leftval = this->getLeft()->getNumber();
    this->setValue(this->calculate(this->leftval, this->rightval));
    this->notifyObservers("value", HSSValue::valueFromParserNode(this->getController(), HSSNumberConstant::number(this->getValue(), this->getController()), this->getThisObj(), this->scope));
}

void HSSExpression::rightChanged(AXRString property, QSharedPointer<HSSObject> theObj)
{
    this->rightval = this->getRight()->getNumber();
    this->setValue(this->calculate(this->leftval, this->rightval));
    this->notifyObservers("value", HSSValue::valueFromParserNode(this->getController(), HSSNumberConstant::number(this->getValue(), this->getController()), this->getThisObj(), this->scope));
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

void HSSExpression::setPercentageBase(HSSUnit value)
{
    this->percentageBase = value;

    //propagate values
    this->_nodesToValuesIfNeeded();
    if (this->left)
    {
        this->left->setPercentageBase(value);
    }
    if (this->right)
    {
        this->right->setPercentageBase(value);
    }

    this->setDirty(true);
}

void HSSExpression::setPercentageObserved(const AXRString property, QSharedPointer<HSSObservable> observed)
{
    this->percentageObserved = observed;
    this->percentageObservedProperty = property;

    //propagate values
    this->_nodesToValuesIfNeeded();
    if (this->left)
    {
        this->left->setPercentageObserved(observed, property);
    }
    if (this->right)
    {
        this->right->setPercentageObserved(observed, property);
    }

    this->setDirty(true);
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
        this->left = HSSValue::valueFromParserNode(this->getController(), this->_leftNode, this->getThisObj(), this->scope);
    }
    if (!this->right && this->_rightNode)
    {
        this->right = HSSValue::valueFromParserNode(this->getController(), this->_rightNode, this->getThisObj(), this->scope);
    }
}
