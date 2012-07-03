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
 *      Revision: 4
 *
 ********************************************************************/

#include "HSSFilter.h"
#include <boost/unordered_map.hpp>
#include "HSSFilters.h"

using namespace AXR;

std::string HSSFilter::filterTypeStringRepresentation(HSSFilterType filterType){
    static std::string types[30];
    static bool HSSFilterHasInitializedTypes = false;
    if (!HSSFilterHasInitializedTypes) {
        HSSFilterHasInitializedTypes = true;
        
        //position
        types[HSSFilterTypeFirst] = "HSSFilterTypeFirst";
        types[HSSFilterTypeLast] = "HSSFilterTypeLast";
        types[HSSFilterTypeEven] = "HSSFilterTypeEven";
        types[HSSFilterTypeOdd] = "HSSFilterTypeOdd";
        types[HSSFilterTypeIndex] = "HSSFilterTypeIndex";
        types[HSSFiltertypeRange] = "HSSFiltertypeRange";
        types[HSSFilterTypeNth] = "HSSFilterTypeNth";
        //hierarchy
        types[HSSFilterTypeParent] = "HSSFilterTypeParent";
        types[HSSFilterTypeParentOf] = "HSSFilterTypeParentOf";
        types[HSSFilterTypeAncestorOf] = "HSSFilterTypeAncestorOf";
        types[HSSFilterTypeHas] = "HSSFilterTypeHas";
        //interaction
        types[HSSFilterTypePressed] = "HSSFilterTypePressed";
        types[HSSFilterTypeDragged] = "HSSFilterTypeDragged";
        types[HSSFilterTypeDestination] = "HSSFilterTypeDestination";
        types[HSSFilterTypeTarget] = "HSSFilterTypeTarget";
        types[HSSFilterTypeFocused] = "HSSFilterTypeFocused";
        types[HSSFilterTypeBlurred] = "HSSFilterTypeBlurred";
        types[HSSFilterTypeAnimated] = "HSSFilterTypeAnimated";
        //text matching
        types[HSSFilterTypeContains] = "HSSFilterTypeContains";
        types[HSSFilterTypeEquals] = "HSSFilterTypeEquals";
        types[HSSFilterTypeStartsWith] = "HSSFilterTypeStartsWith";
        types[HSSFilterTypeEndsWith] = "HSSFilterTypeEndsWith";
        types[HSSFilterTypeMatch] = "HSSFilterTypeMatch";
        types[HSSFilterTypeEmpty] = "HSSFilterTypeEmpty";
        types[HSSFilterTypeLength] = "HSSFilterTypeLength";
        //special
        types[HSSFilterTypeEach] = "HSSFilterTypeEach";
        types[HSSFilterTypeCustom] = "HSSFilterTypeCustom";
    }
    
    return types[filterType];
}

HSSFilterType HSSFilter::filterTypeFromString(std::string name)
{
    static boost::unordered_map<std::string, HSSFilterType> filterTypes;
    
    if (filterTypes.size() == 0) {
        //position
        filterTypes["first"] = HSSFilterTypeFirst;
        filterTypes["last"] = HSSFilterTypeLast;
        filterTypes["even"] = HSSFilterTypeEven;
        filterTypes["odd"] = HSSFilterTypeOdd;
        filterTypes["index"] = HSSFilterTypeIndex;
        filterTypes["range"] = HSSFiltertypeRange;
        filterTypes["nth"] = HSSFilterTypeNth;
        //hierarchy
        filterTypes["parent"] = HSSFilterTypeParent;
        filterTypes["parentOf"] = HSSFilterTypeParentOf;
        filterTypes["ancestorOf"] = HSSFilterTypeAncestorOf;
        filterTypes["has"] = HSSFilterTypeHas;
        //interaction
        filterTypes["pressed"] = HSSFilterTypePressed;
        filterTypes["dragged"] = HSSFilterTypeDragged;
        filterTypes["destination"] = HSSFilterTypeDestination;
        filterTypes["target"] = HSSFilterTypeTarget;
        filterTypes["focused"] = HSSFilterTypeFocused;
        filterTypes["blurred"] = HSSFilterTypeBlurred;
        filterTypes["animated"] = HSSFilterTypeAnimated;
        //text matching
        filterTypes["contains"] = HSSFilterTypeContains;  
        filterTypes["equals"] = HSSFilterTypeEquals;
        filterTypes["startsWith"] = HSSFilterTypeStartsWith;
        filterTypes["endsWith"] = HSSFilterTypeEndsWith;
        filterTypes["match"] = HSSFilterTypeMatch;
        filterTypes["length"] = HSSFilterTypeLength;
        //special
        filterTypes["each"] = HSSFilterTypeEach;
        filterTypes["custom"] = HSSFilterTypeCustom;
    }
    
    
    return filterTypes[name];
}

HSSFilter::p HSSFilter::newFilterWithStringType(std::string stringType)
{
    return HSSFilter::newFilterWithType(HSSFilter::filterTypeFromString(stringType));
}

HSSFilter::p HSSFilter::newFilterWithType(HSSFilterType filterType)
{
    HSSFilter::p ret;
    switch (filterType) {
        case HSSFilterTypeParent:
        {
            ret = HSSParentFilter::p(new HSSParentFilter());
            break;
        }
        
        case HSSFilterTypeFirst:
        {
            ret = HSSFirstFilter::p(new HSSFirstFilter());
            break;
        }
            
        case HSSFilterTypeLast:
        {
            ret = HSSLastFilter::p(new HSSLastFilter());
            break;
        }
            
            
        case HSSFilterTypeEach:
        {
            ret = HSSFilter::p(new HSSFilter());
            ret->filterType = filterType;
            break;
        }
            
        default:
            throw AXRError::p(new AXRError("HSSFilter", "Unknown filter type."));
    }
    
    return ret;
}


HSSFilter::HSSFilter()
: HSSParserNode()
{
    this->nodeType = HSSParserNodeTypeFilter;
}

HSSFilter::p HSSFilter::clone() const{
    return boost::static_pointer_cast<HSSFilter, HSSClonable>(this->cloneImpl());
}

HSSFilter::~HSSFilter()
{
    
}

std::string HSSFilter::toString()
{
    return "Generic filter";
}

bool HSSFilter::isA(HSSFilterType type)
{
    return this->filterType == type;
}

HSSFilterType HSSFilter::getFilterType()
{
    return this->filterType;
}

const std::vector<HSSDisplayObject::p> HSSFilter::apply(const std::vector<HSSDisplayObject::p> &scope, bool negating)
{
    return scope;
}

HSSClonable::p HSSFilter::cloneImpl() const{
    return HSSClonable::p(new HSSFilter(*this));
}
