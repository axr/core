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
 *      Last changed: 2011/11/20
 *      HSS version: 1.0
 *      Core version: 0.42
 *      Revision: 1
 *
 ********************************************************************/

#include "HSSLog.h"
#include "HSSRefFunction.h"
#include <boost/pointer_cast.hpp>
#include "../parsing/HSSFunctionCall.h"
#include "../parsing/HSSStringConstant.h"

using namespace AXR;

HSSLog::HSSLog()
: HSSAction(HSSActionTypeLog)
{
    this->observedValue = NULL;
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("value");
    this->setShorthandProperties(shorthandProperties);
}

HSSLog::~HSSLog()
{
    
}

std::string HSSLog::toString()
{
    return "HSSLog";
}

std::string HSSLog::defaultObjectType()
{
    return "log";
}

void HSSLog::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name) {
        case HSSObservablePropertyValue:
            this->setDValue(value);
            break;
            
        default:
            HSSAction::setProperty(name, value);
            break;
    }
}


void HSSLog::fire()
{
    switch (this->dValue->getType()) {
        case HSSParserNodeTypeFunctionCall:
        {
            HSSFunctionCall::p fcall = boost::static_pointer_cast<HSSFunctionCall>(this->dValue);
            HSSFunction::p func = fcall->getFunction();
            if(func->isA(HSSFunctionTypeRef)){
                func->setScope(this->scope);
                std_log(func->evaluate());
                break;
            }
        }
            
        case HSSParserNodeTypeStringConstant:
        {
            HSSStringConstant::p str = boost::static_pointer_cast<HSSStringConstant>(this->dValue);
            std_log(str->getValue());
            break;
        }
            
        default:
        {
            std_log(this->dValue->toString());
            break;
        }
    }
}


HSSParserNode::p HSSLog::getDValue() { return this->dValue; }
void HSSLog::setDValue(HSSParserNode::p value)
{
    this->dValue = value;
    if(this->observedValue != NULL)
    {
        this->observedValue->removeObserver(this->observedValueProperty, HSSObservablePropertyValue, this);
    }
    
    this->notifyObservers(HSSObservablePropertyValue, &this->dValue);
}

void HSSLog::valueChanged(AXR::HSSObservableProperty source, void *data)
{
    this->dValue = *(HSSParserNode::p *)data;
    
    this->notifyObservers(HSSObservablePropertyValue, &this->dValue);
}

