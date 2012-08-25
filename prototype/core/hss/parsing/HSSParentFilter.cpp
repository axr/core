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

#warning __FILE__ " is using deprecated enum members. Remove this message once the project compiles cleanly with AXR_NO_DEPRECATED_ENUMS defined."

#include "HSSContainer.h"
#include "HSSParentFilter.h"

using namespace AXR;

HSSParentFilter::HSSParentFilter()
: HSSFilter(HSSFilterTypeParent)
{

}

HSSParentFilter::p HSSParentFilter::clone() const
{
    return boost::static_pointer_cast<HSSParentFilter, HSSClonable > (this->cloneImpl());
}

HSSParentFilter::~HSSParentFilter()
{

}

std::string HSSParentFilter::toString()
{
    return "Parent Filter";
}

const std::vector<HSSDisplayObject::p> HSSParentFilter::apply(const std::vector<HSSDisplayObject::p> &scope, bool processing)
{
    if (scope.size() > 0)
    {
        std::vector<HSSDisplayObject::p> ret;
        int size = scope.size();
        HSSContainer::p container;
        if (!this->getNegating())
            for (int i = 0; i < size; i++)
            {
                container = HSSContainer::asContainer(scope[i]);
                if (container && container->getChildren(false).size() > 0)
                    ret.push_back(scope[i]);
            }

        else
            for (int i = 0; i < size; i++)
            {
                container = HSSContainer::asContainer(scope[i]);
                if (container && container->getChildren(false).size() == 0)
                    ret.push_back(scope[i]);
            }
        return ret;
    }
    else
    {
        return scope;
    }
}

HSSClonable::p HSSParentFilter::cloneImpl() const
{
    return HSSClonable::p(new HSSParentFilter(*this));
}
