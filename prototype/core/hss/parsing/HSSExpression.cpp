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
 ********************************************************************/

#include <iostream>
#include "AXRDebugging.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSNumberConstant.h"
#include "HSSPercentageConstant.h"

using namespace AXR;

HSSExpression::HSSExpression(HSSExpressionType type, HSSParserNode::p _left, HSSParserNode::p _right)
: HSSParserNode(HSSParserNodeTypeExpression)
{
    this->expressionType = type;
    this->setLeft(_left);
    this->setRight(_right);
    
    this->percentageObserved = NULL;
    this->leftObserved = NULL;
    this->rightObserved = NULL;
}

HSSExpression::HSSExpression(const HSSExpression &orig)
:HSSParserNode(orig)
{
    this->expressionType = orig.expressionType;
    this->setLeft(orig.left->clone());
    this->setRight(orig.right->clone());
    this->percentageObserved = NULL;
    this->leftObserved = NULL;
    this->rightObserved = NULL;
}

HSSExpression::~HSSExpression()
{
    if(this->leftObserved != NULL)
    {
        this->leftObserved->removeObserver(this->leftObservedProperty, HSSObservablePropertyValue, this);
    }
    if(this->rightObserved != NULL)
    {
        this->rightObserved->removeObserver(this->rightObservedProperty, HSSObservablePropertyValue, this);
    }
}

std::string HSSExpression::toString()
{    
    std::string tempstr = std::string("HSSExpression with following components:\n");
    tempstr.append(this->getLeft()->toString()+"\n"+this->getRight()->toString());
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

std::string HSSExpression::expressionTypeStringRepresentation(HSSExpressionType type){
    std::string types[20];
	types[HSSExpressionTypeNone] = "HSSExpression";
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
        
        //left
        switch (this->getLeft()->getType()) {
            case HSSParserNodeTypeExpression:
            {
                HSSExpression::p leftExpression = boost::static_pointer_cast<HSSExpression>(this->getLeft());
                this->leftval = leftExpression->evaluate();
                if(this->leftObserved != NULL)
                {
                    this->leftObserved->removeObserver(this->leftObservedProperty, HSSObservablePropertyValue, this);
                }
                leftExpression->observe(HSSObservablePropertyValue, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSExpression>(this, &HSSExpression::leftChanged));
                this->leftObserved = leftExpression.get();
                this->leftObservedProperty = HSSObservablePropertyValue;
                break;
            }
                
            case HSSParserNodeTypeNumberConstant:
            {
                HSSNumberConstant::p leftNumber = boost::static_pointer_cast<HSSNumberConstant>(this->getLeft());
                this->leftval = leftNumber->getValue();
                break;
            }
                
            case HSSParserNodeTypePercentageConstant:
            {
                HSSPercentageConstant::p leftPercentage = boost::static_pointer_cast<HSSPercentageConstant>(this->getLeft());
                this->leftval = leftPercentage->getValue(this->percentageBase);
                if(this->leftObserved != NULL)
                {
                    this->leftObserved->removeObserver(this->leftObservedProperty, HSSObservablePropertyValue, this);
                }
                this->percentageObserved->observe(this->percentageObservedProperty, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSExpression>(this, &HSSExpression::leftChanged));
                this->leftObserved = this->percentageObserved;
                this->leftObservedProperty = this->percentageObservedProperty;
                break;
            }
                
            case HSSParserNodeTypeFunctionCall:
            {
                HSSFunction::p leftFunction = boost::static_pointer_cast<HSSFunction>(this->getLeft());
                this->leftval = *(long double*)leftFunction->evaluate();
                
                if(this->leftObserved != NULL)
                {
                    this->leftObserved->removeObserver(this->leftObservedProperty, HSSObservablePropertyValue, this);
                }
                leftFunction->observe(HSSObservablePropertyValue, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSExpression>(this, &HSSExpression::leftChanged));
                this->leftObserved = leftFunction.get();
                this->leftObservedProperty = HSSObservablePropertyValue;
                break;
            }
                
            default:
                std_log1("############ HSSExpression::evaluate() unknwown node type in left member");
                break;
        }
        
        
        //right
        switch (this->getRight()->getType()) {
            case HSSParserNodeTypeExpression:
            {
                HSSExpression::p rightExpression = boost::static_pointer_cast<HSSExpression>(this->getRight());
                this->rightval = rightExpression->evaluate();
                if(this->rightObserved != NULL)
                {
                    this->rightObserved->removeObserver(this->rightObservedProperty, HSSObservablePropertyValue, this);
                }
                rightExpression->observe(HSSObservablePropertyValue, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSExpression>(this, &HSSExpression::rightChanged));
                this->rightObserved = rightExpression.get();
                this->rightObservedProperty = HSSObservablePropertyValue;
                break;
            }
                
            case HSSParserNodeTypeNumberConstant:
            {
                HSSNumberConstant::p rightNumber = boost::static_pointer_cast<HSSNumberConstant>(this->getRight());
                this->rightval = rightNumber->getValue();
                break;
            }
                
            case HSSParserNodeTypePercentageConstant:
            {
                HSSPercentageConstant::p rightPercentage = boost::static_pointer_cast<HSSPercentageConstant>(this->getRight());
                this->rightval = rightPercentage->getValue(this->percentageBase);
                if(this->rightObserved != NULL)
                {
                    this->rightObserved->removeObserver(this->rightObservedProperty, HSSObservablePropertyValue, this);
                }
                this->percentageObserved->observe(this->percentageObservedProperty, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSExpression>(this, &HSSExpression::rightChanged));
                this->rightObserved = this->percentageObserved;
                this->rightObservedProperty = this->percentageObservedProperty;
                break;
            }
                
            case HSSParserNodeTypeFunctionCall:
            {
                HSSFunction::p rightFunction = boost::static_pointer_cast<HSSFunction>(this->getRight());
                this->rightval = *(long double*)rightFunction->evaluate();
                
                if(this->rightObserved != NULL)
                {
                    this->rightObserved->removeObserver(this->rightObservedProperty, HSSObservablePropertyValue, this);
                }
                rightFunction->observe(HSSObservablePropertyValue, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSExpression>(this, &HSSExpression::rightChanged));
                this->rightObserved = rightFunction.get();
                this->rightObservedProperty = HSSObservablePropertyValue;
                break;
            }
                
            default:
                std_log1("############ HSSExpression::evaluate() unknwown node type in right member");
                break;
        }
        
        this->setValue(this->calculate(this->leftval, this->rightval));
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

void HSSExpression::leftChanged(HSSObservableProperty property, void* data)
{
    switch (this->getLeft()->getType()) {
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypeFunctionCall:
        {
            this->leftval = *(long double*) data;
            break;
        }
        
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p leftPercentage = boost::static_pointer_cast<HSSPercentageConstant>(this->getLeft());
            this->leftval = leftPercentage->getValue(*(long double*) data);
            break;
        }
            
        default:
            std_log1("############ HSSExpression::evaluate() unknwown node type in left member");
            break;
    }
    
    this->_value = this->calculate(this->leftval, this->rightval);
    this->notifyObservers(HSSObservablePropertyValue, &this->_value);
}

void HSSExpression::rightChanged(HSSObservableProperty property, void* data)
{
    switch (this->getRight()->getType()) {
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypeFunctionCall:
        {
            this->rightval = *(long double*) data;
            break;
        }
            
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p rightPercentage = boost::static_pointer_cast<HSSPercentageConstant>(this->getRight());
            this->rightval = rightPercentage->getValue(*(long double*) data);
            break;
        }
            
        default:
            std_log1("############ HSSExpression::evaluate() unknwown node type in right member");
            break;
    }
    this->_value = this->calculate(this->leftval, this->rightval);
    this->notifyObservers(HSSObservablePropertyValue, &this->_value);
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
    
    //propagate values
    if (this->left && this->left->isA(HSSParserNodeTypeExpression)) {
        HSSExpression::p leftExpression = boost::static_pointer_cast<HSSExpression>(this->left);
        leftExpression->setPercentageBase(value);
    } else if (this->left && this->left->isA(HSSParserNodeTypeFunctionCall)){
        HSSFunction::p leftFunction = boost::static_pointer_cast<HSSFunction>(this->left);
        leftFunction->setPercentageBase(value);
    }
    if (this->right && this->right->isA(HSSParserNodeTypeExpression)) {
        HSSExpression::p rightExpression = boost::static_pointer_cast<HSSExpression>(this->right);
        rightExpression->setPercentageBase(value);
    } else if (this->right && this->right->isA(HSSParserNodeTypeFunctionCall)){
        HSSFunction::p rightFunction = boost::static_pointer_cast<HSSFunction>(this->right);
        rightFunction->setPercentageBase(value);
    }
    
    this->setDirty(true);
}

void HSSExpression::setPercentageObserved(HSSObservableProperty property, HSSObservable *observed)
{  
    this->percentageObserved = observed;
    this->percentageObservedProperty = property;
    
    //propagate values
    if (this->left && this->left->isA(HSSParserNodeTypeExpression)) {
        HSSExpression::p leftExpression = boost::static_pointer_cast<HSSExpression>(this->left);
        leftExpression->setPercentageObserved(property, observed);
    } else if (this->left && this->left->isA(HSSParserNodeTypeFunctionCall)){
        HSSFunction::p leftFunction = boost::static_pointer_cast<HSSFunction>(this->left);
        leftFunction->setPercentageObserved(property, observed);
    }
    if (this->right && this->right->isA(HSSParserNodeTypeExpression)) {
        HSSExpression::p rightExpression = boost::static_pointer_cast<HSSExpression>(this->right);
        rightExpression->setPercentageObserved(property, observed);
    } else if (this->right && this->right->isA(HSSParserNodeTypeFunctionCall)){
        HSSFunction::p rightFunction = boost::static_pointer_cast<HSSFunction>(this->right);
        rightFunction->setPercentageObserved(property, observed);
    }
    
    this->setDirty(true);
}

void HSSExpression::setScope(const std::vector<HSSDisplayObject::p> * newScope)
{
    this->scope = newScope;
    //propagate values
    if (this->left && this->left->isA(HSSParserNodeTypeExpression)) {
        HSSExpression::p leftExpression = boost::static_pointer_cast<HSSExpression>(this->left);
        leftExpression->setScope(newScope);
    } else if (this->left && this->left->isA(HSSParserNodeTypeFunctionCall)){
        HSSFunction::p leftFunction = boost::static_pointer_cast<HSSFunction>(this->left);
        leftFunction->setScope(newScope);
    }
    if (this->right && this->right->isA(HSSParserNodeTypeExpression)) {
        HSSExpression::p rightExpression = boost::static_pointer_cast<HSSExpression>(this->right);
        rightExpression->setScope(newScope);
    } else if (this->right && this->right->isA(HSSParserNodeTypeFunctionCall)){
        HSSFunction::p rightFunction = boost::static_pointer_cast<HSSFunction>(this->right);
        rightFunction->setScope(newScope);
    }
    
    this->setDirty(true);
}

void HSSExpression::setThisObj(HSSDisplayObject::p value)
{
    //propagate values
    if(this->left) this->left->setThisObj(value);
    if(this->right) this->right->setThisObj(value);
    
    HSSParserNode::setThisObj(value);
}
