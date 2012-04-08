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
 *      Revision: 8
 *
 ********************************************************************/

#ifndef HSSSELECTORCHAIN_H
#define HSSSELECTORCHAIN_H

#include "HSSParserNode.h"
#include <string>
#include <deque>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "HSSSelector.h"

namespace AXR {
    
    class HSSRule;
    
    class HSSSelectorChain : public HSSParserNode
    {
    public:
        typedef boost::shared_ptr<HSSSelectorChain> p;
        typedef boost::weak_ptr<HSSRule> pp;
        
        HSSSelectorChain();
        HSSSelectorChain(const HSSSelectorChain &orig);
        p clone() const;
        ~HSSSelectorChain();
        std::string toString();
        
        const HSSParserNode::p & operator[] (const int i) const;
        const HSSParserNode::p & get(const int i) const;
        //adds a pointer to newNode to the selector chain
        //this function acquires ownership of the parser node
        void add(HSSParserNode::p newNode);
        //adds a pointer to newNode to the beginning of the selector chain
        //this function acquires ownership of the parser node
        void prepend(HSSParserNode::p newNode);
        //removes last newNode from the chain and then deletes it
        void removeLast();
        //returns a pointer to the last node in the chain
        HSSParserNode::p last();
        //returns how many nodes there are in the selector chain
        const int size();
        
        HSSSelector::p subject();
        
        virtual void setThisObj(boost::shared_ptr<HSSDisplayObject> value);
        
    protected:
        HSSSelectorChain::p shared_from_this();
        
        
        std::deque<HSSParserNode::p> nodeList;
        
    private:
        virtual HSSClonable::p cloneImpl() const;
        
        pp rule;
    };
}



#endif
