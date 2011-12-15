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
 *      Last changed: 2011/11/06
 *      HSS version: 1.0
 *      Core version: 0.42
 *      Revision: 1
 *
 ********************************************************************/

#import "HSSSelFunction.h"
#import "../../axr/AXRController.h"

using namespace AXR;

HSSSelFunction::HSSSelFunction()
: HSSFunction()
{
    this->functionType = HSSFunctionTypeSel;
}

HSSSelFunction::~HSSSelFunction()
{
    
}

std::string HSSSelFunction::toString()
{    
    std::string tempstr = std::string("HSSSelFunction\n");
    return tempstr;
}

const HSSSelectorChain::p & HSSSelFunction::getSelectorChain() const
{
    return this->selectorChain;
}

void HSSSelFunction::setSelectorChain(HSSSelectorChain::p newValue)
{
    this->selectorChain = newValue;
    this->setDirty(true);
}

void * HSSSelFunction::_evaluate()
{
    this->axrController->setSelectorChain(this->selectorChain);
    this->selection = this->axrController->selectHierarchical(*this->scope);
    this->_value = (void*) &this->selection;
    std_log(selection.size());
    return this->_value;
}

void * HSSSelFunction::_evaluate(std::deque<HSSParserNode::p> arguments)
{
    return this->_evaluate();
}

//void HSSSelFunction::valueChanged(HSSObservableProperty source, void*data)
//{
//    std_log1("######################### valueChanged");
//    this->setDirty(true);
//    this->_value = data;
//    this->notifyObservers(HSSObservablePropertyValue, this->_value);
//}



