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
 *      Last changed: 2011/12/15
 *      HSS version: 1.0
 *      Core version: 0.42
 *      Revision: 3
 *
 ********************************************************************/

#include "HSSRefFunction.h"
#include "../../axr/AXRDebugging.h"
#include "../../axr/errors/errors.h"
#include "../../axr/AXRController.h"

using namespace AXR;

HSSRefFunction::HSSRefFunction()
: HSSFunction()
{
    this->observed = NULL;
    this->functionType = HSSFunctionTypeRef;
}

HSSRefFunction::~HSSRefFunction()
{
    
}

const std::string & HSSRefFunction::getModifier() const
{
    return this->modifier;
}

void HSSRefFunction::setModifier(std::string newValue)
{
    this->modifier = newValue;
    this->setDirty(true);
}

const HSSObservableProperty & HSSRefFunction::getPropertyName() const
{
    return this->propertyName;
}

void HSSRefFunction::setPropertyName(HSSObservableProperty newValue)
{
    this->propertyName = newValue;
    this->setDirty(true);
}

const HSSSelectorChain::p & HSSRefFunction::getSelectorChain() const
{
    return this->selectorChain;
}

void HSSRefFunction::setSelectorChain(HSSSelectorChain::p newValue)
{
    this->selectorChain = newValue;
    this->setDirty(true);
}

void * HSSRefFunction::_evaluate()
{
    //FIXME: this works only on numeric values, with other kind of data I don't know what will happen
    //we need to figure out how to deal with non-numeric values here
    
    this->axrController->setSelectorChain(this->selectorChain);
    std::vector< std::vector<HSSDisplayObject::p> > selection = this->axrController->selectHierarchical(*this->scope);
    if (selection.size() == 0){
        // ignore
    } else if (selection.size() == 1 && selection[0].size() == 1){
        HSSDisplayObject::p container = selection[0][0];
        this->_value = container->getProperty(this->propertyName);
        
        container->observe(this->propertyName, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSRefFunction>(this, &HSSRefFunction::valueChanged));
        
        this->observed = container.get();
    } else {
        /*
        if (this->modifier == "min"){
            unsigned i, size;
            long double tempval;
            HSSDisplayObject::p container;
            for (i=0, size = selection.size(); i<size; i++) {
                tempval = *(long double*)selection[i]->getProperty(this->propertyName);
                if (tempval < *(long double*)this->_value){
                    this->_value = &tempval;
                    container = selection[i];
                }
            }
            
            this->_value = container->getProperty(this->propertyName);
            
            container->observe(this->propertyName, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSRefFunction>(this, &HSSRefFunction::valueChanged));
            
            
        } else if (this->modifier == "max"){
            
        } else if (this->modifier == "avg"){
            
        }
         */
        throw AXRError::p(new AXRError("HSSRefFunciton", "Using modifiers in ref functions is not implemented yet"));
    }
    return this->_value;
}

void * HSSRefFunction::_evaluate(std::deque<HSSParserNode::p> arguments)
{
    return this->_evaluate();
}

void HSSRefFunction::valueChanged(HSSObservableProperty source, void*data)
{
    std_log1("######################### valueChanged");
    this->setDirty(true);
    this->_value = data;
    this->notifyObservers(HSSObservablePropertyValue, this->_value);
}
