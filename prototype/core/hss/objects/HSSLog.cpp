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
 *      Last changed: 2012/03/25
 *      HSS version: 1.0
 *      Core version: 0.46
 *      Revision: 3
 *
 ********************************************************************/

#include "HSSLog.h"
#include <boost/pointer_cast.hpp>
#include "../parsing/HSSFunction.h"
#include "../parsing/HSSRefFunction.h"
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

HSSLog::HSSLog(const HSSLog & orig)
: HSSAction(orig)
{
    this->observedValue = NULL;
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("value");
    this->setShorthandProperties(shorthandProperties);
}

HSSLog::p HSSLog::clone() const{
    return boost::static_pointer_cast<HSSLog, HSSClonable>(this->cloneImpl());
}

HSSClonable::p HSSLog::cloneImpl() const{
    return HSSClonable::p(new HSSLog(*this));
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
            HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction>(this->dValue);
            if(fnct->isA(HSSFunctionTypeRef)){
                HSSRefFunction::p refFnct = boost::static_pointer_cast<HSSRefFunction>(fnct);
                refFnct->setScope(this->scope);
                void * data = refFnct->evaluate();
                switch (refFnct->getPropertyName()) {
                    case HSSObservablePropertyWidth:
                    case HSSObservablePropertyHeight:
                    case HSSObservablePropertyAnchorX:
                    case HSSObservablePropertyAnchorY:
                    case HSSObservablePropertyAlignX:
                    case HSSObservablePropertyAlignY:
                    case HSSObservablePropertyContentAlignX:
                    case HSSObservablePropertyContentAlignY:
                    case HSSObservablePropertyStartX:
                    case HSSObservablePropertyStartY:
                    case HSSObservablePropertyEndX:
                    case HSSObservablePropertyEndY:
                    case HSSObservablePropertyPosition:
                    case HSSObservablePropertyRed:
                    case HSSObservablePropertyGreen:
                    case HSSObservablePropertyBlue:
                    case HSSObservablePropertyAlpha:
                    case HSSObservablePropertySize:
                    case HSSObservablePropertyCorners:
                    {
                        std_log(*(long double*)data);
                        break;
                    }
                        
                    case HSSObservablePropertyMargin:
                    case HSSObservablePropertyPadding:
                    case HSSObservablePropertyShape:
                    case HSSObservablePropertyStartColor:
                    case HSSObservablePropertyEndColor:
                    case HSSObservablePropertyColorStops:
                    case HSSObservablePropertyBalance:
                    case HSSObservablePropertyFace:
                    case HSSObservablePropertyColor:
                    case HSSObservablePropertyAction:
                    {
                        std_log((*(HSSObject::p *)data)->toString());
                        break;
                    }
                        
                    case HSSObservablePropertyBackground:
                    case HSSObservablePropertyBorder:
                    case HSSObservablePropertyFont:
                    {
                        std::vector<HSSObject::p> v_data = *(std::vector<HSSObject::p>*)data;
                        std::vector<HSSObject::p>::iterator it;
                        if (v_data.size() == 0) {
                            std_log("empty");
                        } else {
                            for (it=v_data.begin(); it!=v_data.end(); it++) {
                                std_log((*it)->toString());
                            }
                        }
                        break;
                    }
                        
                    case HSSObservablePropertyOn:
                    {
                        boost::unordered_map<HSSEventType, std::vector<HSSObject::p> > m_data = *(boost::unordered_map<HSSEventType, std::vector<HSSObject::p> >*)data;
                        boost::unordered_map<HSSEventType, std::vector<HSSObject::p> >::iterator it;
                        if (m_data.size() == 0) {
                            std_log("empty");
                        } else {
                            for (it=m_data.begin(); it!=m_data.end(); it++) {
                                std::vector<HSSObject::p> v_data = (*it).second;
                                std::vector<HSSObject::p>::iterator it2;
                                for (it2=v_data.begin(); it2!=v_data.end(); it2++) {
                                    std_log((*it2)->toString());
                                }
                            }
                        }
                        break;
                    }
                        
                    case HSSObservablePropertyDirectionPrimary:
                    case HSSObservablePropertyDirectionSecondary:
                    case HSSObservablePropertyTransform:
                    case HSSObservablePropertyTextAlign:
                    case HSSObservablePropertyWeight:
                    case HSSObservablePropertySrc:
                    case HSSObservablePropertyMode:
                    case HSSObservablePropertyContentTarget:
                    {
                        std_log(*(std::string*)data);
                        break;
                    }
                        
                    case HSSObservablePropertyHover:
                    case HSSObservablePropertyText:
                    case HSSObservablePropertyTarget:
                    default:
                    {
                        std_log("logging this property is not supported");
                        break;
                    }
                }
            }
            break;
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
    this->dValue->setThisObj(this->getThisObj());
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

