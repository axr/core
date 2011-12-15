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
 *      Last changed: 2011/10/29
 *      HSS version: 1.0
 *      Core version: 0.42
 *      Revision: 1
 *
 ********************************************************************/

#ifndef HSSFILTER_H
#define HSSFILTER_H

#include "HSSParserNode.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include "../objects/HSSDisplayObject.h"

namespace AXR {
    enum HSSFilterType {
        //position
        HSSFilterTypeFirst,
        HSSFilterTypeLast,
        HSSFilterTypeEven,
        HSSFilterTypeOdd,
        HSSFilterTypeIndex,
        HSSFiltertypeRange,
        HSSFilterTypeNth,
        //hierarchy
        HSSFilterTypeParent,
        HSSFilterTypeParentOf,
        HSSFilterTypeAncestorOf,
        HSSFilterTypeHas,
        //interaction
        HSSFilterTypeHover,
        HSSFilterTypePressed,
        HSSFilterTypeDragged,
        HSSFilterTypeDestination,
        HSSFilterTypeTarget,
        HSSFilterTypeFocused,
        HSSFilterTypeBlurred,
        HSSFilterTypeAnimated,
        //text matching
        HSSFilterTypeContains,
        HSSFilterTypeEquals,
        HSSFilterTypeStartsWith,
        HSSFilterTypeEndsWith,
        HSSFilterTypeMatch,
        HSSFilterTypeEmpty,
        HSSFilterTypeLength,
        //special
        HSSFilterTypeEach,
        HSSFilterTypeCustom
        
    };
    
    class HSSFilter : public HSSParserNode
    {
    public:
        typedef boost::shared_ptr<HSSFilter> p;
        
        static std::string filterTypeStringRepresentation(HSSFilterType filterType);
        static HSSFilterType filterTypeFromString(std::string name);
        static HSSFilter::p newFilterWithStringType(std::string stringType);
        static HSSFilter::p newFilterWithType(HSSFilterType filterType);
        
        HSSFilter();
        virtual ~HSSFilter();
        
        virtual std::string toString();
        
        virtual const std::vector<HSSDisplayObject::p> apply(const std::vector<HSSDisplayObject::p> &scope);
        
        bool isA(HSSFilterType type);
        HSSFilterType getFilterType();
        
    protected:
        HSSFilterType filterType;
    };
}

#endif
