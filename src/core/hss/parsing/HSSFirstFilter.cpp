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

#include "HSSFirstFilter.h"

using namespace AXR;

HSSFirstFilter::HSSFirstFilter()
: HSSFilter(HSSFilterTypeFirst)
{

}

HSSFirstFilter::p HSSFirstFilter::clone() const
{
    return boost::static_pointer_cast<HSSFirstFilter, HSSClonable > (this->cloneImpl());
}

HSSFirstFilter::~HSSFirstFilter()
{

}

std::string HSSFirstFilter::toString()
{
    return "First Filter";
}

const std::vector<HSSDisplayObject::p> HSSFirstFilter::apply(const std::vector<HSSDisplayObject::p> &scope, bool processing)
{
    if (scope.size() > 0)
    {
        if (this->getNegating())
        {
            std::vector<HSSDisplayObject::p> ret;
            HSSDisplayObject::const_it it;
            ret.insert(ret.begin(), scope.begin() + 1, scope.end());
            return ret;
        }
        else
        {
            std::vector<HSSDisplayObject::p> ret;
            ret.push_back(scope[0]);
            return ret;
        }
    }
    else
    {
        return scope;
    }
}

HSSClonable::p HSSFirstFilter::cloneImpl() const
{
    return HSSClonable::p(new HSSFirstFilter(*this));
}
