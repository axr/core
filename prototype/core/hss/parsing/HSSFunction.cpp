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
 *      Last changed: 2012/06/11
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 6
 *
 ********************************************************************/

#include "HSSFunction.h"
#include "../../axr/AXRDebugging.h"
#include "../../AXR.h"
#include <string>
//#include "../parsing/HSSExpression.h"
//#include "../parsing/HSSConstants.h"
//#include <sstream>
//#include "../parsing/HSSObjectDefinition.h"

using namespace AXR;

HSSFunction::HSSFunction()
:HSSParserNode()
{
    this->nodeType = HSSParserNodeTypeFunctionCall;
    this->functionType = HSSFunctionTypeNone;
    this->scope = NULL;
    this->percentageObserved = NULL;
    this->_isDirty = true;
    this->_value = NULL;
}

HSSFunction::HSSFunction(HSSFunctionType type)
:HSSParserNode()
{
    this->nodeType = HSSParserNodeTypeFunctionCall;
    this->functionType = type;
    this->scope = NULL;
    this->percentageObserved = NULL;
    this->_isDirty = true;
    this->_value = NULL;
}

HSSFunction::HSSFunction(const HSSFunction & orig)
: HSSParserNode(orig)
{
    this->functionType = orig.functionType;
    this->scope = NULL;
    this->percentageObserved = NULL;
    this->_isDirty = orig._isDirty;
    this->_value = orig._value;
    this->axrController = orig.axrController;
}

HSSFunction::~HSSFunction()
{
    if (this->percentageObserved != NULL) {
        this->percentageObserved->removeObserver(this->percentageObservedProperty, HSSObservablePropertyValue, this);
        this->percentageObserved = NULL;
    }
}

std::string HSSFunction::toString()
{    
    std::string tempstr = std::string("HSSFunction\n");
    return tempstr;
}

void * HSSFunction::evaluate()
{
    if(this->_isDirty){
        this->_isDirty = false;
        
        this->_value = this->_evaluate();
    }
    
    return this->_value;
}

void * HSSFunction::evaluate(std::deque<HSSParserNode::p> arguments)
{
    if(this->_isDirty){
        this->_isDirty = false;
        
        this->_value = this->_evaluate(arguments);
    }
    
    return this->_value;
}

void * HSSFunction::_evaluate()
{
    return this->_evaluate(std::deque<HSSParserNode::p>());
}

void * HSSFunction::_evaluate(std::deque<HSSParserNode::p> arguments)
{
    AXRCore::tp & core = AXRCore::getInstance();
    core->evaluateCustomFunction(this->getName(), (void*) &arguments);
    return NULL;
}

void HSSFunction::propertyChanged(HSSObservableProperty property, void* data)
{
    this->notifyObservers(HSSObservablePropertyValue, data);
}

void HSSFunction::setPercentageBase(long double value)
{
    this->percentageBase = value;
}

void HSSFunction::setPercentageObserved(HSSObservableProperty property, HSSObservable *observed)
{
//    if(this->percentageObserved != NULL)
//    {
//        this->percentageObserved->removeObserver(this->percentageObservedProperty, HSSObservablePropertyValue, this);
//        this->percentageObserved = NULL;
//    }
    this->percentageObservedProperty = property;
    this->percentageObserved = observed;
    //observed->observe(property, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSFunction>(this, &HSSFunction::propertyChanged));
}

void HSSFunction::setScope(const std::vector<HSSDisplayObject::p> * newScope)
{
    this->scope = newScope;
}

void HSSFunction::setDirty(bool value)
{
    this->_isDirty = value;
}

bool HSSFunction::isDirty()
{
    return this->_isDirty;
}

void * HSSFunction::getValue()
{
    return this->_value;
}

bool HSSFunction::isA(HSSFunctionType type)
{
    return this->functionType == type;
}

HSSFunctionType HSSFunction::getFunctionType()
{
    return this->functionType;
}

void HSSFunction::setController(AXRController * controller)
{
    this->axrController = controller;
}

AXRController * HSSFunction::getController()
{
    return this->axrController;
}

void HSSFunction::setArguments(std::deque<HSSParserNode::p> arguments)
{
    this->_arguments = arguments;
}

std::deque<HSSParserNode::p> HSSFunction::getArguments()
{
    return this->_arguments;
}

void HSSFunction::setName(std::string newName)
{
    this->_name = newName;
}

std::string HSSFunction::getName()
{
    return this->_name;
}

