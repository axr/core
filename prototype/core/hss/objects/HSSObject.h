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
 *      Last changed: 2011/09/26
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 9
 *
 ********************************************************************/

#ifndef HSSOBJECT_H
#define HSSOBJECT_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "../various/HSSObservable.h"
#include "../parsing/HSSParserNode.h"
#include <map>
//#include "../../axr/AXRController.h"

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
        HSSObjectTypeRgba,
        HSSObjectTypeFont,
        HSSObjectTypeValue,
        HSSObjectTypeMultipleValue
    };
    
    class AXRController;
    
    class HSSObject : public HSSObservable
    {
    public:
        typedef boost::shared_ptr<HSSObject> p;
        
        static HSSObject::p newObjectWithType(std::string type);
        static bool isKeyword(std::string value, std::string property);
        
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
        virtual void setProperty(std::string name, HSSParserNode::p value);
        
        AXRController * axrController;
        
    protected:
        HSSObjectType type;
        
    private:
        bool _isNamed;
    };

}

#endif