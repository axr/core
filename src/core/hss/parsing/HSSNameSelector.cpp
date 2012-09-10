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

#include "HSSNameSelector.h"

using namespace AXR;

HSSNameSelector::HSSNameSelector(std::string elementName)
: HSSSelector(HSSSelectorTypeNameSelector)
{
    this->elementName = elementName;
}

HSSNameSelector::HSSNameSelector(const HSSNameSelector &orig)
: HSSSelector(orig)
{
    this->elementName = orig.elementName;
}

HSSNameSelector::p HSSNameSelector::clone() const
{
    return boost::static_pointer_cast<HSSNameSelector, HSSClonable > (this->cloneImpl());
}

std::string HSSNameSelector::getElementName()
{
    return this->elementName;
}

std::string HSSNameSelector::toString()
{
    return "Name selector targeting an element with name " + this->elementName;
}

HSSClonable::p HSSNameSelector::cloneImpl() const
{
    return HSSClonable::p(new HSSNameSelector(*this));
}

std::vector<HSSDisplayObject::p> HSSNameSelector::filterSelection(const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj, bool processing)
{
    std::vector< HSSDisplayObject::p> ret;
    unsigned i, size;
    //select only elements with matching element name
    for (i = 0, size = scope.size(); i < size; ++i)
    {
        bool match = scope[i]->getElementName() == this->getElementName();
        if ((match && !this->getNegating()) || (!match && this->getNegating()))
        {
            ret.push_back(scope[i]);
        }
    }
    return ret;
}
