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
 *      Last changed: 2011/05/06
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 3
 *
 ********************************************************************/

#ifndef HSSOBSERVABLE_H
#define HSSOBSERVABLE_H

#include "HSSObservableProperties.h"
#include "HSSCallback.h"
#include <map>
#include <string>
#include <boost/unordered_map.hpp>

namespace AXR {
    
    class HSSObservable
    {
    public:
        static std::string observablePropertyStringRepresentation(HSSObservableProperty property);
        
        //the key is a hash of HSSObservable * observed and HSObservableProperty source
        typedef boost::unordered_map<std::size_t, HSSCallback* > observed;
        
        HSSObservable();
        virtual ~HSSObservable();
        
        void observe(HSSObservableProperty target, HSSObservableProperty source, HSSObservable * object, HSSCallback *callback);
        void removeObserver(HSSObservableProperty target, HSSObservableProperty source, HSSObservable * object);
        virtual void propertyChanged(HSSObservableProperty property, void * data);
        void notifyObservers(HSSObservableProperty property, void * data);
        
    private:
        std::map<HSSObservableProperty, HSSObservable::observed>_propertyObservers;
        
    };
}

#endif