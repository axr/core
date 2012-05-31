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
 *      Last changed: 2012/05/30
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 3
 *
 ********************************************************************/

#ifndef HSSFLAG_H
#define HSSFLAG_H

#include "HSSParserNode.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "HSSRule.h"

namespace AXR {
    
    class HSSSelectorChain;
    class HSSDisplayObject;
    
    /**
     *  @brief Parser Node type representing flags.
     *  Flags (e.g. ::hover) are a way to conditionally target elements in HSS, inside
     *  a selector chain.
     */
    class HSSFlag : public HSSParserNode
    {
    public:
        typedef boost::shared_ptr<HSSFlag> p;
        
        /**
         *  Creates a new instance of a flag node.
         */
        HSSFlag();
        
        /**
         *  Clones an instance of HSSFlag and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSFlag
         */
        p clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSFlag();
        
        virtual std::string toString();
        
        /**
         *  Getter for the name.
         *  @return The name of the flag. This is what comes after the double colon
         *  in HSS (e.g. "hover").
         */
        std::string getName();
        
        /**
         *  Setter for the name.
         *  @param  newValue    A string containing the new name of the flag.
         */
        void setName(std::string newValue);
        
        /**
         *  Connects the flag to the display object, so that when it changes state
         *  style resolution happens.
         *  @param displayObject    A shared pointer to the display object to bind to.
         *  @param negating         Wether the flag is being negated or not.
         */
        void apply(boost::shared_ptr<HSSDisplayObject> displayObject, bool negating);
        
        /**
         *  When the status of a flag changes, this method should be called. It will create a
         *  new selection from the original scope of the rule, and then set the new purging 
         *  status on the selected elements.
         *  @param newStatus    The new status to which it should be set
         */
        void flagChanged(HSSRuleState newStatus);
        
        /**
         *  @return A shared pointer to the instance on which it is called. A shared pointer to this
         *  instance must already exist before calling this.
         */
        HSSFlag::p shared_from_this();
        
        /**
         *  When selecting (not connecting, which is different) elements based on the flag,
         *  the flag acts like a filter, reducing the size of the selection.
         *  @param scope        The original selection: A vector of shared pointers to display objects.
         *  @param negating     Wether we are negating the flag or not.
         *  @return A vector of shared pointers to the display objects in the resulting selection.
         */
        const std::vector<boost::shared_ptr<HSSDisplayObject> > filter(const std::vector<boost::shared_ptr<HSSDisplayObject> > &scope, bool negating);
        
        /**
         *  Getter for the purging state.
         *  @return The HSSRuleState that represents the current purging status.
         */
        HSSRuleState getPurging();
        
        /**
         *  Setter for the purging state.
         *  @param newValue     The new purging status for this flag.
         */
        void setPurging(HSSRuleState newValue);

    protected:
        std::string _name;
        
    private:
        virtual HSSClonable::p cloneImpl() const;
        HSSRuleState _purging;
    };
}

#endif
