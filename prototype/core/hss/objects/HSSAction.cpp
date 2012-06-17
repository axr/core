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
 *      Revision: 3
 *
 ********************************************************************/

#import "HSSAction.h"
#import "../parsing/HSSObjectNameConstant.h"
#import "../parsing/HSSObjectDefinition.h"
#import <boost/unordered_map.hpp>
#import <boost/pointer_cast.hpp>
#import "../../axr/AXRController.h"

using namespace AXR;

std::string HSSAction::actionTypeStringRepresentation(AXR::HSSActionType actionType)
{
    boost::unordered_map<HSSActionType, std::string> types;
    types[HSSActionTypeRequest    ] = "HSSActionTypeRequest";
    types[HSSActionTypeSetProperty] = "HSSActionTypeSetProperty";
    types[HSSActionTypeJavascript ] = "HSSActionTypeJavascript";
    types[HSSActionTypeJsFunction ] = "HSSActionTypeJsFunction";
    types[HSSActionTypeAlert      ] = "HSSActionTypeAlert";
    types[HSSActionTypeLog        ] = "HSSActionTypeLog";
    types[HSSActionTypeFlag       ] = "HSSActionTypeFlag";
    types[HSSActionTypeFunction   ] = "HSSActionTypeFunction";
    
    return types[actionType];
}

HSSAction::HSSAction(HSSActionType type)
: HSSObject()
{
    this->type = HSSObjectTypeAction;
    this->actionType = type;
    
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("action");
    this->setShorthandProperties(shorthandProperties);
}

HSSAction::HSSAction(const HSSAction & orig)
: HSSObject(orig)
{
    this->actionType = orig.actionType;
    
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("action");
    this->setShorthandProperties(shorthandProperties);
}

HSSAction::p HSSAction::clone() const{
    return boost::static_pointer_cast<HSSAction, HSSClonable>(this->cloneImpl());
}

HSSClonable::p HSSAction::cloneImpl() const{
    return HSSClonable::p(new HSSAction(*this));
}

HSSAction::~HSSAction()
{
    
}

std::string HSSAction::toString()
{
    return "Generic HSSAction of type"+HSSAction::actionTypeStringRepresentation(this->actionType);
}

std::string HSSAction::defaultObjectType()
{
    return "action";
}

std::string HSSAction::defaultObjectType(std::string property)
{
    return HSSObject::defaultObjectType(property);
}

void HSSAction::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name) {
            
        default:
            HSSObject::setProperty(name, value);
            break;
    }
}

bool HSSAction::isA(HSSActionType type)
{
    return type == this->actionType;
}

HSSActionType HSSAction::getActionType()
{
    return this->actionType;
}

void HSSAction::fire()
{
    //do nothing
}

