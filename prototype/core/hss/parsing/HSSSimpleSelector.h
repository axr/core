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
 ********************************************************************/

#ifndef HSSSIMPLESELECTOR_H
#define HSSSIMPLESELECTOR_H

#include "HSSSelector.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include "HSSNameSelector.h"
#include "../objects/HSSDisplayObject.h"
#include "HSSFilter.h"
#include <list>

namespace AXR {
    
    /**
     *  @brief Simple selectors are basic units of a selector chain.
     */
    class HSSSimpleSelector : public HSSSelector {
    public:
        typedef boost::shared_ptr<HSSSimpleSelector> p;
        
        /**
         *  Creates a new instance of a simple selector.
         */
        HSSSimpleSelector();
        
        /**
         *  Copy constructor for HSSSimpleSelector objects. Do not call directly, use clone() instead.
         */
        HSSSimpleSelector(const HSSSimpleSelector &orig);
        
        /**
         *  Clones an instance of HSSSimpleSelector and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSSimpleSelector.
         */
        p clone() const;
        
        //see HSSObject.h for documentation of this method
        virtual std::string toString();
        
        void setName(HSSNameSelector::p name);
        const HSSNameSelector::p getName() const;
        
        const std::list<HSSFilter::p> getFilters() const;
        void filtersAdd(HSSFilter::p filter);
        
        /**
         * Reduces the selection according to the selector nodes it has stored.
         */
        std::vector<HSSDisplayObject::p> filterSelection(const std::vector<HSSDisplayObject::p> & scope, bool processing);
        
        //see HSSParserNode.h for documentation of this method
        virtual void setThisObj(HSSDisplayObject::p value);
        
    private:
        HSSNameSelector::p name;
        std::list<HSSFilter::p> filters;
        
        virtual HSSClonable::p cloneImpl() const;
        
        std::vector<HSSDisplayObject::p> _recursiveFilterSelection(const std::vector<HSSDisplayObject::p> & selection, std::list<HSSFilter::p>::const_iterator it, std::list<HSSFilter::p>::const_iterator end_it, bool processing);
    };
}



#endif
