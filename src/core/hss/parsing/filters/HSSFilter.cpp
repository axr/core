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
 *      AUTHORS: see AUTHORS file
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

#include <QMap>
#include "AXRError.h"
#include "HSSDisplayObject.h"
#include "HSSEmptyFilter.h"
#include "HSSEvenChildFilter.h"
#include "HSSEvenFilter.h"
#include "HSSFilter.h"
#include "HSSFirstChildFilter.h"
#include "HSSFirstFilter.h"
#include "HSSLastChildFilter.h"
#include "HSSLastFilter.h"
#include "HSSOddChildFilter.h"
#include "HSSOddFilter.h"

using namespace AXR;

AXRString HSSFilter::filterTypeStringRepresentation(HSSFilterType filterType)
{
    static QMap<HSSFilterType, AXRString> types;
    if (types.isEmpty())
    {
        //position
        types[HSSFilterTypeFirst] = "HSSFilterTypeFirst";
        types[HSSFilterTypeLast] = "HSSFilterTypeLast";
        types[HSSFilterTypeEven] = "HSSFilterTypeEven";
        types[HSSFilterTypeOdd] = "HSSFilterTypeOdd";
        types[HSSFilterTypeNth] = "HSSFilterTypeNth";
        //hierarchy
        types[HSSFilterTypeHas] = "HSSFilterTypeHas";
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

HSSFilterType HSSFilter::filterTypeFromString(AXRString name)
{
    static QMap<AXRString, HSSFilterType> filterTypes;
    if (filterTypes.isEmpty())
    {
        //position
        filterTypes["first"] = HSSFilterTypeFirst;
        filterTypes["last"] = HSSFilterTypeLast;
        filterTypes["even"] = HSSFilterTypeEven;
        filterTypes["odd"] = HSSFilterTypeOdd;
        filterTypes["nth"] = HSSFilterTypeNth;
        //hierarchy
        filterTypes["has"] = HSSFilterTypeHas;
        //text matching
        filterTypes["contains"] = HSSFilterTypeContains;
        filterTypes["equals"] = HSSFilterTypeEquals;
        filterTypes["startsWith"] = HSSFilterTypeStartsWith;
        filterTypes["endsWith"] = HSSFilterTypeEndsWith;
        filterTypes["match"] = HSSFilterTypeMatch;
        filterTypes["empty"] = HSSFilterTypeEmpty;
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

HSSFilter::p HSSFilter::newFilterWithStringType(AXRString stringType, AXRController * controller)
{
    return HSSFilter::newFilterWithType(HSSFilter::filterTypeFromString(stringType), controller);
}

HSSFilter::p HSSFilter::newFilterWithType(HSSFilterType filterType, AXRController * controller)
{
    HSSFilter::p ret;
    switch (filterType)
    {
    case HSSFilterTypeFirst:
    {
        ret = HSSFirstFilter::p(new HSSFirstFilter(controller));
        break;
    }

    case HSSFilterTypeFirstChild:
    {
        ret = HSSFirstChildFilter::p(new HSSFirstChildFilter(controller));
        break;
    }

    case HSSFilterTypeLastChild:
    {
        ret = HSSLastChildFilter::p(new HSSLastChildFilter(controller));
        break;
    }

    case HSSFilterTypeLast:
    {
        ret = HSSLastFilter::p(new HSSLastFilter(controller));
        break;
    }

    case HSSFilterTypeEven:
    {
        ret = HSSEvenFilter::p(new HSSEvenFilter(controller));
        break;
    }

    case HSSFilterTypeEvenChild:
    {
        ret = HSSEvenChildFilter::p(new HSSEvenChildFilter(controller));
        break;
    }

    case HSSFilterTypeOdd:
    {
        ret = HSSOddFilter::p(new HSSOddFilter(controller));
        break;
    }

    case HSSFilterTypeOddChild:
    {
        ret = HSSOddChildFilter::p(new HSSOddChildFilter(controller));
        break;
    }

    case HSSFilterTypeEmpty:
    {
        ret = HSSEmptyFilter::p(new HSSEmptyFilter(controller));
        break;
    }


        //        case HSSFilterTypeEach:
        //        {
        //            ret = HSSFilter::p(new HSSEachFilter(controller));
        //            ret->filterType = filterType;
        //            break;
        //        }

    default:
        throw AXRError("HSSFilter", "Unknown filter type.");
    }

    return ret;
}

HSSFilter::HSSFilter(HSSFilterType type, AXRController * controller)
: HSSParserNode(HSSParserNodeTypeFilter, controller)
{
    this->filterType = type;
    this->_negating = false;
}

HSSFilter::~HSSFilter()
{

}

AXRString HSSFilter::toString()
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

bool HSSFilter::getNegating() const
{
    return this->_negating;
}

void HSSFilter::setNegating(bool value)
{
    this->_negating = value;
}
