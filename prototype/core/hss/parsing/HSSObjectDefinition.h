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
 *      Revision: 9
 *
 ********************************************************************/

#ifndef HSSOBJECTDEFINITION_H
#define HSSOBJECTDEFINITION_H

#include "HSSStatement.h"
#include <string>
#include <vector>
#include <deque>
#include "HSSParserNodes.h"
#include "../objects/HSSObject.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "../objects/HSSDisplayObject.h"
#include "HSSRule.h"

namespace AXR {
    class HSSObjectDefinition : public HSSStatement
    {
    public:
        typedef boost::shared_ptr<HSSObjectDefinition> p;
        
        HSSObjectDefinition(HSSObject::p prototype);
        HSSObjectDefinition(const HSSObjectDefinition & orig);
        p clone() const;
        ~HSSObjectDefinition();
        virtual std::string toString();
        
        void apply();
        
        //adds a new property definition to the properties list
        void propertiesAdd(HSSPropertyDefinition::p &newProperty);
        void propertiesAdd(const HSSPropertyDefinition::p &newProperty);
        void propertiesPrepend(HSSPropertyDefinition::p &newProperty);
        void propertiesPrepend(const HSSPropertyDefinition::p &newProperty);
        //removes last statement from the list and then deletes it
        void propertiesRemoveLast();
        //returns a pointer to the last statement in the list
        HSSPropertyDefinition::p &propertiesLast();
        //returns how many statements there are in the properties list
        const int propertiesSize();
        //returns all the properties
        std::deque<HSSPropertyDefinition::p> getProperties();
        
        
        //adds a new object definition to the children
        void childrenAdd(HSSObjectDefinition::p &child);
        void childrenAdd(const HSSObjectDefinition::p &child);
        //removes last statement from the list and then deletes it
        void childrenRemoveLast();
        //returns a pointer to the last statement in the list
        HSSObjectDefinition::p &childrenLast();
        //returns how many statements there are in the children list
        const int childrenSize();
        //returns all the children
        const std::vector<HSSObjectDefinition::p> getChildren() const;
        
        HSSObject::p getObject();
        
        void setScope(const std::vector<HSSDisplayObject::p> * newScope);
        
        void setThisObj(HSSDisplayObject::p value);
        HSSDisplayObject::p getThisObj();
        
        void setRules(std::deque<HSSRule::p> newRules);
        const std::deque<HSSRule::p> getRules() const;
        void rulesAdd(HSSRule::p rule);
        void rulesPrepend(HSSRule::p rule);
        void rulesRemove(HSSRule::p rule);
        
    protected:
        HSSObjectDefinition::p shared_from_this();
        
        std::deque<HSSPropertyDefinition::p> properties;
        std::vector<HSSObjectDefinition::p> children;

        
        HSSDisplayObject::p thisObj;
        const std::vector<HSSDisplayObject::p> * scope;
        
    private:
        virtual HSSClonable::p cloneImpl() const;
        HSSObject::p prototype;
        std::deque<HSSRule::p> _rules;
    };
}


#endif
