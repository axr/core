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
 *      COPYRIGHT: ©2013 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axrproject.org
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

QSharedPointer<HSSFilter> HSSFilter::newFilterWithStringType(AXRString stringType, AXRController * controller)
{
    return HSSFilter::newFilterWithType(HSSFilter::filterTypeFromString(stringType), controller);
}

QSharedPointer<HSSFilter> HSSFilter::newFilterWithType(HSSFilterType filterType, AXRController * controller)
{
    QSharedPointer<HSSFilter> ret;
    switch (filterType)
    {
    case HSSFilterTypeFirst:
    {
        ret = QSharedPointer<HSSFirstFilter>(new HSSFirstFilter(controller));
        break;
    }

    case HSSFilterTypeFirstChild:
    {
        ret = QSharedPointer<HSSFirstChildFilter>(new HSSFirstChildFilter(controller));
        break;
    }

    case HSSFilterTypeLastChild:
    {
        ret = QSharedPointer<HSSLastChildFilter>(new HSSLastChildFilter(controller));
        break;
    }

    case HSSFilterTypeLast:
    {
        ret = QSharedPointer<HSSLastFilter>(new HSSLastFilter(controller));
        break;
    }

    case HSSFilterTypeEven:
    {
        ret = QSharedPointer<HSSEvenFilter>(new HSSEvenFilter(controller));
        break;
    }

    case HSSFilterTypeEvenChild:
    {
        ret = QSharedPointer<HSSEvenChildFilter>(new HSSEvenChildFilter(controller));
        break;
    }

    case HSSFilterTypeOdd:
    {
        ret = QSharedPointer<HSSOddFilter>(new HSSOddFilter(controller));
        break;
    }

    case HSSFilterTypeOddChild:
    {
        ret = QSharedPointer<HSSOddChildFilter>(new HSSOddChildFilter(controller));
        break;
    }

    case HSSFilterTypeEmpty:
    {
        ret = QSharedPointer<HSSEmptyFilter>(new HSSEmptyFilter(controller));
        break;
    }


        //        case HSSFilterTypeEach:
        //        {
        //            ret = QSharedPointer<HSSFilter>(new HSSEachFilter(controller));
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

HSSFilter::HSSFilter(const HSSFilter &orig)
: HSSParserNode(orig)
{
    this->filterType = orig.filterType;
    this->_negating = orig._negating;
}

HSSFilter::~HSSFilter()
{

}

AXRString HSSFilter::toString()
{
    return "Generic filter";
}

const bool HSSFilter::isA(HSSFilterType type) const
{
    return this->filterType == type;
}

const HSSFilterType HSSFilter::getFilterType() const
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

QSharedPointer<HSSFilter> HSSFilter::shared_from_this()
{
    return qSharedPointerCast<HSSFilter > (HSSParserNode::shared_from_this());
}
