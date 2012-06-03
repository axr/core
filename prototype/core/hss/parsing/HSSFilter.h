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
 *      Revision: 4
 *
 ********************************************************************/

#ifndef HSSFILTER_H
#define HSSFILTER_H

#include "HSSParserNode.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "../objects/HSSDisplayObject.h"
#include "HSSFilterType.h"

namespace AXR {
    
    class HSSSelectorChain;
    
    /**
     *  @brief Abstract base class for parser node types representing filters.
     *  Do not use directly, use one of the specific subclasses instead.
     *
     *  Filters are used in the selector chain to reduce the size of the selection.
     */
    class HSSFilter : public HSSParserNode
    {
    public:
        typedef boost::shared_ptr<HSSFilter> p;
        
        /**
         *  When logging, you often need a string representation of the filter type.
         *  @param actionType   The action type to represent as a string.
         *  @return A string representation of the given type.
         */
        static std::string filterTypeStringRepresentation(HSSFilterType filterType);
        
        /**
         *  Gives a HSSFilterType representation from a string representation.
         *  @param name     The name of the filter (e.g. "first").
         *  @return The filter type that corresponds to that name.
         *
         *  @todo what if not found? there should be an error state in the enum
         */
        static HSSFilterType filterTypeFromString(std::string name);
        
        /**
         *  Instantiates a specific subclass for the given textual representation
         *  of a string (e.g. "first").
         *  @param stringType   The textual representation of the filter.
         *  @return A shared pointer to a new instance of a filter of the given type.
         */
        static HSSFilter::p newFilterWithStringType(std::string stringType);
        
        /**
         *  Instantiates a specific subclass for the given HSSFilterType.
         *  @param filterType   The filter type for the new instance.
         *  @return A shared pointer to a new instance of a filter of the given type.
         */
        static HSSFilter::p newFilterWithType(HSSFilterType filterType);
        
        /**
         *  Creates a new instance of a filter. Do not use directly.
         */
        HSSFilter();
        
        /**
         *  Clones an instance of HSSFilter and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSFilter.
         */
        p clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSFilter();
        
        virtual std::string toString();
        
        /**
         *  Each subclass implements its own way of processing the selection.
         *  This base implementation is (I think) what gets called when you use
         *  an unknown filter name in the source code, which currently returns the entire scope.
         *
         *  @param scope        The original selection: A vector of shared pointers to display objects.
         *  @param negating     Wether we are negating the filter or not.
         *  @return A vector of shared pointers to the display objects in the resulting selection.
         *
         *  @todo should this return an empty selection?
         */
        virtual const std::vector<HSSDisplayObject::p> apply(const std::vector<HSSDisplayObject::p> &scope, bool negating);
        
        /**
         *  Allows you to check if this filter is of the given type.
         *  @param  type    The filter type to which to check against.
         *  @return Wether it is of the given type or not.
         */
        bool isA(HSSFilterType type);
        
        /**
         *  @return The filter type of this instance.
         */
        HSSFilterType getFilterType();
        
        
    protected:
        HSSFilterType filterType;
    private:
        virtual HSSClonable::p cloneImpl() const;
    };
}

#endif
