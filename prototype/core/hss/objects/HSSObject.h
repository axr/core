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
 *      Revision: 5
 *
 ********************************************************************/

#ifndef HSSOBJECT_H
#define HSSOBJECT_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "../various/HSSCallback.h"
#include <map>
#include "../various/HSSObservableProperties.h"

namespace AXR {
    
    enum HSSObjectType
    {
        HSSObjectTypeGeneric,
        HSSObjectTypeDisplayObject,
        HSSObjectTypeContainer,
        HSSObjectTypeBorderGeneric,
        HSSObjectTypeLineBorder,
        HSSObjectTypeMarginGeneric,
        HSSObjectTypeStraightMargin,
        HSSObjectTypeProjectedMargin,
        HSSObjectTypeValue,
        HSSObjectTypeMultipleValue
    };
    
    class HSSObject
    {
    public:
        typedef boost::shared_ptr<HSSObject> p;
        typedef std::map<HSSObject *, HSSValueChangedCallback* > observed;
        
        static HSSObject::p newObjectWithType(std::string type);
        
        std::string name;
        
        HSSObject();
        HSSObject(std::string name);
        ~HSSObject();
        
        virtual std::string toString();
        bool isNamed();
        void setName(std::string newName);
        void dropName();
        
        bool isA(HSSObjectType otherType);
        HSSObjectType getType();
        
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);
        
        void observe(HSSObservableProperty property, HSSObject * object, HSSValueChangedCallback *callback);
        void removeObserver(HSSObservableProperty property, HSSObject * object);
        virtual void propertyChanged(HSSObservableProperty property, void * data);
        void notifyObservers(HSSObservableProperty property, void * data);
        
    protected:
        HSSObjectType type;
        
    private:
        bool _isNamed;
        std::map<HSSObservableProperty, HSSObject::observed>_propertyObservers;
    };

}

#endif