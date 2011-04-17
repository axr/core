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
 *      Last changed: 2011/04/16
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 4
 *
 ********************************************************************/

#ifndef HSSRULE_H
#define HSSRULE_H

#import "HSSStatement.h"
#import "HSSSelectorChain.h"
#import <string>
#import <vector>
#import <boost/shared_ptr.hpp>

namespace AXR {
    
    class HSSRule : public HSSStatement
    {    
    public:
        typedef boost::shared_ptr<HSSRule> p;
        
        HSSRule(HSSSelectorChain::p selectorChain);
        ~HSSRule();
        
        HSSSelectorChain::p selectorChain;
        
        std::string toString();
        
        //adds a pointer to newStatement to the properties list
        //this function acquires ownership of the statement
        void propertiesAdd(HSSStatement::p newStatement);
        //removes last statement from the list and then deletes it
        void propertiesRemoveLast();
        //returns a pointer to the last statement in the list
        HSSStatement::p propertiesLast();
        //returns how many statements there are in the properties list
        const int propertiesSize();
        
        void childrenAdd(HSSRule::p newRule);
        HSSRule::p childrenGet(unsigned index);
        void childrenRemove(unsigned index);
        void childrenRemoveLast();
        const int childrenSize();
        
    protected:
        std::vector<HSSStatement::p> properties;
        std::vector<HSSRule::p>children;
    };
}

#endif