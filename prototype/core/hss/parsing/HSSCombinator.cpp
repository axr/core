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

#include "HSSCombinator.h"

using namespace AXR;

HSSCombinator::HSSCombinator(HSSCombinatorType type)
: HSSSelector(HSSSelectorTypeCombinator)
{
    this->combinatorType = type;
}

HSSCombinator::p HSSCombinator::clone() const
{
    return boost::static_pointer_cast<HSSCombinator, HSSClonable > (this->cloneImpl());
}

bool HSSCombinator::isA(HSSCombinatorType otherType)
{
    return this->combinatorType == otherType;
}

HSSCombinatorType HSSCombinator::getCombinatorType()
{
    return this->combinatorType;
}

std::string HSSCombinator::toString()
{
    return "HSSCombinator of type  " + this->combinatorStringRepresentation(this->combinatorType);
}

std::string HSSCombinator::combinatorStringRepresentation(HSSCombinatorType type)
{
    std::string types[20];
    types[ HSSCombinatorTypeSiblings ] = "HSSCombinatorTypeSiblings";
    types[ HSSCombinatorTypePreviousSiblings ] = "HSSCombinatorTypePreviousSiblings";
    types[ HSSCombinatorTypeNextSiblings ] = "HSSCombinatorTypeNextSiblings";
    types[ HSSCombinatorTypeChildren ] = "HSSCombinatorTypeChildren";
    types[ HSSCombinatorTypeDescendants ] = "HSSCombinatorTypeDescendants";
    types[ HSSCombinatorTypeAllDescendants ] = "HSSCombinatorTypeAllDescendants";
    types[ HSSCombinatorTypeTextSelection ] = "HSSCombinatorTypeTextSelection";
    return types[type];
}

HSSClonable::p HSSCombinator::cloneImpl() const
{
    return HSSClonable::p(new HSSCombinator(*this));
}

//does nothing yet

std::vector<HSSDisplayObject::p> HSSCombinator::filterSelection(const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj, bool processing)
{
    return scope;
}
