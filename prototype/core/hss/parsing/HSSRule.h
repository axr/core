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
 *      Last changed: 2012/03/21
 *      HSS version: 1.0
 *      Core version: 0.46
 *      Revision: 7
 *
 ********************************************************************/

#ifndef HSSRULE_H
#define HSSRULE_H

#import "HSSStatement.h"
#import "HSSPropertyDefinition.h"
#import "HSSSelectorChain.h"
#import "HSSInstruction.h"
#import <string>
#import <vector>
#import <boost/shared_ptr.hpp>
#import <boost/unordered_map.hpp>
#import "HSSFilterType.h"

namespace AXR {
    
    class HSSDisplayObject;
    class HSSFilter;
    
    enum HSSRuleState
    {
        HSSRuleStateOff = 0,
        HSSRuleStateOn = 1,
        HSSRuleStateActivate,
        HSSRuleStatePurge
    };
    
    class HSSRuleStatus
    {
    public:
        typedef boost::shared_ptr<HSSRuleStatus> p;
        HSSRuleState state;
        boost::shared_ptr<HSSRule> rule;
    };
    
    class HSSRule : public HSSStatement
    {    
    public:
        typedef boost::shared_ptr<HSSRule> p;
        typedef std::deque<p>::iterator it;
        typedef std::deque<p>::const_iterator const_it;
        
        HSSRule();
        HSSRule(const HSSRule &orig);
        p clone() const;
        ~HSSRule();
        
        std::string toString();
        
        void setSelectorChain(HSSSelectorChain::p newChain);
        HSSSelectorChain::p getSelectorChain();
        
        const std::vector<HSSPropertyDefinition::p> & getProperties() const;
        void propertiesAdd(HSSPropertyDefinition::p & newProperty);
        HSSPropertyDefinition::p &propertiesGet(unsigned index);
        void propertiesRemove(unsigned index);
        void propertiesRemoveLast();
        HSSPropertyDefinition::p &propertiesLast();
        const int propertiesSize();
        
        void childrenAdd(HSSRule::p newRule);
        HSSRule::p childrenGet(unsigned index);
        void childrenRemove(unsigned index);
        void childrenRemoveLast();
        const int childrenSize();
        
        void setInstruction(HSSInstruction::p newInstruction);
        HSSInstruction::p getInstruction();
        
        virtual void setThisObj(boost::shared_ptr<HSSDisplayObject> value);
        void treeChanged(HSSObservableProperty source, void*data);
        
        bool getActiveByDefault();
        void setActiveByDefault(bool newValue);
        
        const std::vector<boost::weak_ptr<HSSDisplayObject> > getAppliedTo() const;
        void setAppliedTo(std::vector<boost::weak_ptr<HSSDisplayObject> > newObjects);
        void appliedToAdd(boost::shared_ptr<HSSDisplayObject> displayObject);
        
        const std::vector<boost::shared_ptr<HSSDisplayObject> > getOriginalScope() const;
        void setOriginalScope(const std::vector<boost::shared_ptr<HSSDisplayObject> > & scope);
        
    protected:
        HSSRule::p shared_from_this();
        
        HSSSelectorChain::p selectorChain;
        std::vector<HSSPropertyDefinition::p> properties;
        std::vector<HSSRule::p>children;
        HSSInstruction::p instruction;
        
        bool _activeByDefault;
        
        std::vector<boost::weak_ptr<HSSDisplayObject> > appliedTo;
        
        std::vector<boost::shared_ptr<HSSDisplayObject> > _originalScope;
        
    private:
        virtual HSSClonable::p cloneImpl() const;
        
        boost::unordered_map<HSSFilterType, std::vector<boost::shared_ptr<HSSDisplayObject> > > _interactors;
        
    };
}

#endif
