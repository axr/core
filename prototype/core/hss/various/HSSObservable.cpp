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
 *      Last changed: 2011/04/25
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 ********************************************************************/

#include "HSSObservable.h"
#include <iostream>
#include "../../axr/AXRDebugging.h"

using namespace AXR;

HSSObservable::HSSObservable()
{
    
}

HSSObservable::~HSSObservable()
{
    this->_propertyObservers.clear();
}

void HSSObservable::observe(HSSObservableProperty property, HSSObservable * object, HSSValueChangedCallback *callback)
{
    //std_log1("added observer: "+object->name);
    if(this->_propertyObservers.count(property) != 0){
        HSSObservable::observed &theObserved = this->_propertyObservers[property];
        theObserved[object] = callback;
    } else {
        HSSObservable::observed theObserved;
        theObserved[object] = callback;
        this->_propertyObservers[property] = theObserved;
    }
}

void HSSObservable::removeObserver(HSSObservableProperty property, HSSObservable * object)
{
    //std_log1("removing observer");
    if(this->_propertyObservers.count(property) != 0){
        HSSObservable::observed &theObserved = this->_propertyObservers[property];
        theObserved.erase(object);
    } else {
        throw "removing non existent observer";
    }
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
            HSSValueChangedCallback callback = *(*it).second;
            callback(property, data);
        }
    }
}
