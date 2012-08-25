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

#include "HSSFirstChildFilter.h"

using namespace AXR;

HSSFirstChildFilter::HSSFirstChildFilter()
: HSSFilter(HSSFilterTypeFirst)
{

}

HSSFirstChildFilter::p HSSFirstChildFilter::clone() const
{
    return boost::static_pointer_cast<HSSFirstChildFilter, HSSClonable > (this->cloneImpl());
}

HSSFirstChildFilter::~HSSFirstChildFilter()
{

}

std::string HSSFirstChildFilter::toString()
{
    return "First Child Filter";
}

const std::vector<HSSDisplayObject::p> HSSFirstChildFilter::apply(const std::vector<HSSDisplayObject::p> &scope, bool processing)
{
    std::vector<HSSDisplayObject::p> ret;
    HSSDisplayObject::const_it it;
    for (it = scope.begin(); it != scope.end(); it++)
    {
        const HSSDisplayObject::p & theDO = *it;
        if (this->getNegating())
        {
            if (theDO->getIndex() != 0)
            {
                ret.push_back(theDO);
            }
        }
        else
        {
            if (theDO->getIndex() == 0)
            {
                ret.push_back(theDO);
            }
        }
    }
    return ret;
}

HSSClonable::p HSSFirstChildFilter::cloneImpl() const
{
    return HSSClonable::p(new HSSFirstChildFilter(*this));
}
