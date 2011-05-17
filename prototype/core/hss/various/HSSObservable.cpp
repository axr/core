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
 *      Last changed: 2011/05/18
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 4
 *
 ********************************************************************/

#include "HSSObservable.h"
#include <iostream>
#include "../../axr/AXRDebugging.h"

using namespace AXR;

std::string HSSObservable::observablePropertyStringRepresentation(HSSObservableProperty property){
    std::string types[20];
	types[HSSObservablePropertyValue] = "HSSObservablePropertyValue";
	types[HSSObservablePropertyWidth] = "HSSObservablePropertyWidth";
	types[HSSObservablePropertyHeight] = "HSSObservablePropertyHeight";
	types[HSSObservablePropertyAnchorX] = "HSSObservablePropertyAnchorX";
	types[HSSObservablePropertyAnchorY] = "HSSObservablePropertyAnchorY";
	types[HSSObservablePropertyFlow] = "HSSObservablePropertyFlow";
	types[HSSObservablePropertyAlignX] = "HSSObservablePropertyAlignX";
	types[HSSObservablePropertyAlignY] = "HSSObservablePropertyAlignY";
	types[HSSObservablePropertyContentAlignX] = "HSSObservablePropertyContentAlignX";
	types[HSSObservablePropertyContentAlignY] = "HSSObservablePropertyContentAlignY";
    return types[property];
}

HSSObservable::HSSObservable()
{
    
}

HSSObservable::~HSSObservable()
{
    this->_propertyObservers.clear();
}

void HSSObservable::observe(HSSObservableProperty target, HSSObservableProperty source, HSSObservable * object, HSSCallback *callback)
{
    //std_log1("added observer: "+object->name);
    std::size_t hash = 0;
    boost::hash_combine(hash, object);
    boost::hash_combine(hash, source);
    
    if(this->_propertyObservers.count(target) != 0){
        HSSObservable::observed &theObserved = this->_propertyObservers[target];
        theObserved[hash] = callback;
        std_log3("added observer for "+HSSObservable::observablePropertyStringRepresentation(target));
    } else {
        HSSObservable::observed theObserved;
        theObserved[hash] = callback;
        this->_propertyObservers[target] = theObserved;
        std_log3("added observer for new "+HSSObservable::observablePropertyStringRepresentation(target));
    }
}

void HSSObservable::removeObserver(HSSObservableProperty target, HSSObservableProperty source, HSSObservable * object)
{
    if(this->_propertyObservers.count(target) != 0){
        HSSObservable::observed &theObserved = this->_propertyObservers[target];
        std::size_t hash = 0;
        boost::hash_combine(hash, object);
        boost::hash_combine(hash, source);
        if(theObserved.count(hash) != 0){
            theObserved.erase(hash);
            std_log3("removing observer for "+HSSObservable::observablePropertyStringRepresentation(target));
            return;
        }
    }
    std_log3("tried to remove non existent observer for "+HSSObservable::observablePropertyStringRepresentation(target));
}

void HSSObservable::propertyChanged(HSSObservableProperty property, void *data)
{
    std_log1("property changed");
}

void HSSObservable::notifyObservers(HSSObservableProperty property, void *data)
{
    HSSObservable::observed::iterator it;
    if(this->_propertyObservers.count(property) != 0){
        HSSObservable::observed &theObserved = this->_propertyObservers[property];
        for (it=theObserved.begin(); it != theObserved.end() ; it++) {
            //std_log1("notifying observer: "+(*(*it).second).ptr->name);
            HSSCallback * callback = (*it).second;
            if((long double*)data == NULL){
                std_log1("data is null");
            }
            callback->call(property, data);
        }
    }
}
