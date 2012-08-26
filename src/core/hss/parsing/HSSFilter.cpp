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
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
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

#warning __FILE__ " is using deprecated enum members. Remove this message once the project compiles cleanly with AXR_NO_DEPRECATED_ENUMS defined."

#include <boost/unordered_map.hpp>
#include "HSSDisplayObject.h"
#include "HSSFilter.h"
#include "HSSFilters.h"

using namespace AXR;

std::string HSSFilter::filterTypeStringRepresentation(HSSFilterType filterType)
{
    static std::string types[30];
    static bool HSSFilterHasInitializedTypes = false;
    if (!HSSFilterHasInitializedTypes)
    {
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
        //flags
        types[HSSFilterTypeFlag] = "HSSFilterTypeFlag";
    }

    return types[filterType];
}

HSSFilterType HSSFilter::filterTypeFromString(std::string name)
{
    static boost::unordered_map<std::string, HSSFilterType> filterTypes;

    if (filterTypes.size() == 0)
    {
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
        //index
        filterTypes["firstChild"] = HSSFilterTypeFirstChild;
        filterTypes["lastChild"] = HSSFilterTypeLastChild;
        filterTypes["evenChild"] = HSSFilterTypeEvenChild;
        filterTypes["oddChild"] = HSSFilterTypeOddChild;
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
    switch (filterType)
    {
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

    case HSSFilterTypeFirstChild:
    {
        ret = HSSFirstChildFilter::p(new HSSFirstChildFilter());
        break;
    }

    case HSSFilterTypeLastChild:
    {
        ret = HSSLastChildFilter::p(new HSSLastChildFilter());
        break;
    }

    case HSSFilterTypeLast:
    {
        ret = HSSLastFilter::p(new HSSLastFilter());
        break;
    }

    case HSSFilterTypeEven:
    {
        ret = HSSEvenFilter::p(new HSSEvenFilter());
        break;
    }

    case HSSFilterTypeEvenChild:
    {
        ret = HSSEvenChildFilter::p(new HSSEvenChildFilter());
        break;
    }

    case HSSFilterTypeOdd:
    {
        ret = HSSOddFilter::p(new HSSOddFilter());
        break;
    }

    case HSSFilterTypeOddChild:
    {
        ret = HSSOddChildFilter::p(new HSSOddChildFilter());
        break;
    }

        //        case HSSFilterTypeEach:
        //        {
        //            ret = HSSFilter::p(new HSSEachFilter());
        //            ret->filterType = filterType;
        //            break;
        //        }

    default:
        throw AXRError::p(new AXRError("HSSFilter", "Unknown filter type."));
    }

    return ret;
}

HSSFilter::HSSFilter(HSSFilterType type)
: HSSParserNode(HSSParserNodeTypeFilter)
{
    this->filterType = type;
    this->_negating = false;
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

const bool HSSFilter::getNegating() const
{
    return this->_negating;
}

void HSSFilter::setNegating(bool value)
{
    this->_negating = value;
}