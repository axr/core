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
 *      Last changed: 2012/06/14
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 2
 *
 ********************************************************************/

#import "HSSFlagAction.h"
#import "../../axr/AXRController.h"

using namespace AXR;

HSSFlagAction::HSSFlagAction()
: HSSAction(HSSActionTypeFlag)
{
    
}

HSSFlagAction::HSSFlagAction(const HSSFlagAction & orig)
: HSSAction(orig.actionType)
{
    this->_flagFunction = orig._flagFunction->clone();
}

HSSFlagAction::p HSSFlagAction::clone() const
{
    return boost::static_pointer_cast<HSSFlagAction, HSSClonable>(this->cloneImpl());
}

HSSClonable::p HSSFlagAction::cloneImpl() const
{
    HSSFlagAction::p clone = HSSFlagAction::p(new HSSFlagAction(*this));
    return clone;
}

HSSFlagAction::~HSSFlagAction()
{
    
}


std::string HSSFlagAction::toString()
{
    return "HSSFlag";
}

std::string HSSFlagAction::defaultObjectType()
{
    return "Flag";
}


void HSSFlagAction::fire()
{
    HSSFlagFunction::p flagFunction = this->getFlagFunction();
    this->axrController->setSelectorChains(flagFunction->getSelectorChains());
    std::vector< std::vector<HSSDisplayObject::p> > selection = this->axrController->selectHierarchical(*this->scope, this->getThisObj(), false, false);
    if (selection.size() == 0){
        // ignore
    } else {
        
        std::vector< std::vector<HSSDisplayObject::p> >::iterator outerIt;
        std::vector<HSSDisplayObject::p>::iterator innerIt;
        
        for (outerIt=selection.begin(); outerIt!=selection.end(); outerIt++) {
            std::vector<HSSDisplayObject::p> & inner = *outerIt;
            for (innerIt=inner.begin(); innerIt!=inner.end(); innerIt++) {
                HSSDisplayObject::p container = *innerIt;
                switch (flagFunction->getFlagFunctionType()) {
                    case HSSFlagFunctionTypeFlag:
                        container->flagsActivate(flagFunction->getName());
                        break;
                    case HSSFlagFunctionTypeUnflag:
                        container->flagsDeactivate(flagFunction->getName());
                        break;
                    case HSSFlagFunctionTypeToggleFlag:
                        container->flagsToggle(flagFunction->getName());
                        break;
                        
                    default:
                        throw AXRWarning::p(new AXRWarning("HSSFlagAction", "Invalid flag function type for flag action"));
                        break;
                }
            }
        }
        
    }
}

HSSFlagFunction::p HSSFlagAction::getFlagFunction()
{
    return this->_flagFunction;
}

void HSSFlagAction::setFlagFunction(HSSFlagFunction::p newValue)
{
    this->_flagFunction = newValue;
}



