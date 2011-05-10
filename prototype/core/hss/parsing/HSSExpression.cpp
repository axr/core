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
 *      AUTHORS: Miro Keller
 *      
 *      COPYRIGHT: ©2011 - All Rights Reserved
 *
 *      LICENSE: see License.txt file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2011/04/27
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 7
 *
 ********************************************************************/

#include "HSSExpression.h"
#include <iostream>
#include "../../axr/AXRDebugging.h"
#include "HSSNumberConstant.h"
#include "HSSPercentageConstant.h"

using namespace AXR;

HSSExpression::HSSExpression(HSSParserNode::p _left, HSSParserNode::p _right)
: HSSParserNode()
{
    this->nodeType = HSSParserNodeTypeExpression;
    this->expressionType = HSSExpressionTypeGeneric;
    this->setLeft(_left);
    this->setRight(_right);
    
    this->percentageObserved = NULL;
}

HSSExpression::~HSSExpression()
{
    
}


bool HSSExpression::isA(HSSExpressionType otherType)
{
	return otherType == this->expressionType;
}

HSSExpressionType HSSExpression::getExpressionType()
{
    return this->expressionType;
}

std::string HSSExpression::expressionTypeStringRepresentation(HSSExpressionType type){
    std::string types[20];
	types[HSSExpressionTypeGeneric] = "HSSExpression";
	types[HSSExpressionTypeSum] = "HSSSum";
	types[HSSExpressionTypeSubtraction] = "HSSSubtraction";
	types[HSSExpressionTypeMultiplication] = "HSSMultiplication";
	types[HSSExpressionTypeDivision] = "HSSDivision";
    
    return types[type];
}

long double HSSExpression::evaluate()
{
    if (this->isDirty()) {
        this->setDirty(false);
        
        long double leftval;
        long double rightval;
        
        //left
        if(this->getLeft()->isA(HSSParserNodeTypeExpression)){
            HSSExpression::p leftExpression = boost::static_pointer_cast<HSSExpression>(this->getLeft());
            leftval = leftExpression->evaluate();
            
        } else if(this->getLeft()->isA(HSSParserNodeTypeNumberConstant)){
            HSSNumberConstant::p leftNumber = boost::static_pointer_cast<HSSNumberConstant>(this->getLeft());
            leftval = leftNumber->getValue();
            
        } else if (this->getLeft()->isA(HSSParserNodeTypePercentageConstant)){
            HSSPercentageConstant::p leftPercentage = boost::static_pointer_cast<HSSPercentageConstant>(this->getLeft());
            leftval = leftPercentage->getValue(this->percentageBase);
        }
        
        //right
        if(this->getRight()->isA(HSSParserNodeTypeExpression)){
            HSSExpression::p rightExpression = boost::static_pointer_cast<HSSExpression>(this->getRight());
            rightval = rightExpression->evaluate();
        } else if(this->getRight()->isA(HSSParserNodeTypeNumberConstant)){
            HSSNumberConstant::p rightNumber = boost::static_pointer_cast<HSSNumberConstant>(this->getRight());
            rightval = rightNumber->getValue();
            
        } else if (this->getRight()->isA(HSSParserNodeTypePercentageConstant)){
            HSSPercentageConstant::p rightPercentage = boost::static_pointer_cast<HSSPercentageConstant>(this->getRight());
            rightval = rightPercentage->getValue(this->percentageBase);
        }
        
        this->setValue(this->calculate(leftval, rightval));
    }
    
    return this->getValue();
}

void HSSExpression::setLeft(HSSParserNode::p newLeft)
{
    this->left = newLeft;
}

HSSParserNode::p HSSExpression::getLeft()
{
    return this->left;
}

void HSSExpression::setRight(HSSParserNode::p newRight)
{
    this->right = newRight;
}

HSSParserNode::p HSSExpression::getRight()
{
    return this->right;
}

void HSSExpression::propertyChanged(HSSObservableProperty property, void* data)
{
    if(property == this->percentageObservedProperty){
        long double newPercentageBase = *(long double *)data;
        this->setPercentageBase(newPercentageBase);
        std_log1("property of expression changed");
        this->notifyObservers(HSSObservablePropertyValue, NULL);
    } else {
        std_log1("############### unknown observed property");
    }
}

void HSSExpression::setDirty(bool value)
{
    this->_isDirty = value;
}

bool HSSExpression::isDirty()
{
    return this->_isDirty;
}

void HSSExpression::setValue(long double newValue){
    this->_value = newValue;
}

long double HSSExpression::getValue()
{
    return this->_value;
}

void HSSExpression::setPercentageBase(long double value)
{
    this->percentageBase = value;
    this->setDirty(true);
    
    //propagate values
    if (this->left && this->left->isA(HSSParserNodeTypeExpression)) {
        HSSExpression::p leftExpression = boost::static_pointer_cast<HSSExpression>(this->left);
        leftExpression->setPercentageBase(value);
    }
    if (this->right && this->right->isA(HSSParserNodeTypeExpression)) {
        HSSExpression::p rightExpression = boost::static_pointer_cast<HSSExpression>(this->right);
        rightExpression->setPercentageBase(value);
    }
}

void HSSExpression::setPercentageObserved(HSSObservableProperty property, HSSObservable *observed)
{
    if(this->percentageObserved != NULL)
    {
        this->percentageObserved->removeObserver(this->percentageObservedProperty, HSSObservablePropertyValue, this);
    }
    this->percentageObservedProperty = property;
    this->percentageObserved = observed;
    observed->observe(property, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSExpression>(this, &HSSExpression::propertyChanged));
    
    //propagate values
    if (this->left && this->left->isA(HSSParserNodeTypeExpression)) {
        HSSExpression::p leftExpression = boost::static_pointer_cast<HSSExpression>(this->left);
        leftExpression->setPercentageObserved(property, observed);
    }
    if (this->right && this->right->isA(HSSParserNodeTypeExpression)) {
        HSSExpression::p rightExpression = boost::static_pointer_cast<HSSExpression>(this->right);
        rightExpression->setPercentageObserved(property, observed);
    }
}

void HSSExpression::setScope(const std::vector<HSSDisplayObject::p> * newScope)
{
    this->scope = newScope;
    //propagate values
    if (this->left && this->left->isA(HSSParserNodeTypeExpression)) {
        HSSExpression::p leftExpression = boost::static_pointer_cast<HSSExpression>(this->left);
        leftExpression->setScope(newScope);
    }
    if (this->right && this->right->isA(HSSParserNodeTypeExpression)) {
        HSSExpression::p rightExpression = boost::static_pointer_cast<HSSExpression>(this->right);
        rightExpression->setScope(newScope);
    }
}
