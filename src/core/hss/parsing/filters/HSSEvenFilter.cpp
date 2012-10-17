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

#include "HSSEvenFilter.h"

using namespace AXR;

HSSEvenFilter::HSSEvenFilter()
: HSSFilter(HSSFilterTypeFirst)
{

}

HSSEvenFilter::p HSSEvenFilter::clone() const
{
    return qSharedPointerCast<HSSEvenFilter, HSSClonable > (this->cloneImpl());
}

HSSEvenFilter::~HSSEvenFilter()
{

}

AXRString HSSEvenFilter::toString()
{
    return "Even Filter";
}

const std::vector<HSSDisplayObject::p> HSSEvenFilter::apply(const std::vector<HSSDisplayObject::p> &scope, bool processing)
{
    std::vector<HSSDisplayObject::p> ret;
    unsigned i, size;
    for (i = 0, size = scope.size(); i < size; ++i)
    {
        if (this->getNegating())
        {
            if (i % 2 == 0)
            {
                ret.push_back(scope[i]);
            }
        }
        else
        {
            if (i % 2 != 0)
            {
                ret.push_back(scope[i]);
            }
        }
    }
    return ret;
}

HSSClonable::p HSSEvenFilter::cloneImpl() const
{
    return HSSClonable::p(new HSSEvenFilter(*this));
}
