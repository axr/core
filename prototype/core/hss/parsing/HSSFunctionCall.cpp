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
 *      Last changed: 2011/09/29
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 ********************************************************************/

#include "HSSFunctionCall.h"
#include "../objects/HSSFunctions.h"
#include "../../axr/AXRDebugging.h"

using namespace AXR;

HSSFunctionCall::HSSFunctionCall()
: HSSParserNode()
{
    this->nodeType = HSSParserNodeTypeFunctionCall;
    this->_isDirty = true;
    
    this->percentageObserved = NULL;
    this->scope = NULL;
}

HSSFunctionCall::~HSSFunctionCall()
{
    
}

std::string HSSFunctionCall::toString()
{    
    std::string tempstr = std::string("HSSFunctionCall with the following function:\n");
    tempstr.append(this->function->toString());
    
    const int argcount = this->argumentsSize();
    if(argcount > 0){
        tempstr.append(" and the following properties: \n");
        unsigned i;
        for (i=0; i<argcount; i++) {
            tempstr.append("   ").append(this->arguments[i]->toString()).append("\n");
        }
    }
    
    return tempstr;
}

const HSSFunction::p & HSSFunctionCall::getFunction() const
{
    return this->function;
}

void HSSFunctionCall::setFunction(HSSFunction::p newFunction)
{
    this->function = newFunction;
}

const std::deque<HSSParserNode::p> & HSSFunctionCall::getArguments() const
{
    return this->arguments;
}

void HSSFunctionCall::setArguments(std::deque<HSSParserNode::p> newArguments)
{
    this->arguments = newArguments;
}

void HSSFunctionCall::argumentsAdd(HSSParserNode::p newArgument)
{
    this->arguments.push_back(newArgument);
}

HSSParserNode::p HSSFunctionCall::argumentsFirst()
{
    return this->arguments.front();
}

const HSSParserNode::p HSSFunctionCall::argumentsFirst() const
{
    return this->arguments.front();
}

HSSParserNode::p HSSFunctionCall::argumentsLast()
{
    return this->arguments.back();
}

const HSSParserNode::p HSSFunctionCall::argumentsLast() const
{
    return this->arguments.back();
}

unsigned int HSSFunctionCall::argumentsSize()
{
    return this->arguments.size();
}

void HSSFunctionCall::argumentsRemoveFirst()
{
    this->arguments.pop_front();
}

void HSSFunctionCall::argumentsRemoveLast()
{
    this->arguments.pop_back();
}

void HSSFunctionCall::setPercentageBase(long double value)
{
    this->setDirty(true);
    
    //propagate values
    if(this->function){
        this->function->setPercentageBase(value);
    }
}
void HSSFunctionCall::setPercentageObserved(HSSObservableProperty property, HSSObservable *observed)
{
    //propagate values
    if(this->function){
        this->function->setPercentageObserved(property, observed);
    }
}

void HSSFunctionCall::setScope(const std::vector<HSSDisplayObject::p> * newScope)
{
    //propagate values
    if(this->function){
        this->function->setScope(newScope);
    }
}


long double HSSFunctionCall::evaluate()
{
    if (this->isDirty()) {
        this->setDirty(false);
        
        if(this->function){
            this->setValue(this->function->evaluate(this->arguments));
        }
    }
    
    void * theValue = this->getValue();
    
    if(theValue != NULL){
        return *(long double*) theValue;
    } else {
        return 0;
    }
}


void HSSFunctionCall::setDirty(bool value)
{
    this->_isDirty = value;
}

bool HSSFunctionCall::isDirty()
{
    return this->_isDirty;
}

void HSSFunctionCall::setValue(void * newValue){
    this->_value = newValue;
}

void * HSSFunctionCall::getValue()
{
    return this->_value;
}


