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

#include "HSSCombinator.h"
#include "HSSDisplayObject.h"
#include "HSSSelection.h"

using namespace AXR;

HSSCombinator::HSSCombinator(HSSCombinatorType type, AXRController * controller)
: HSSSelector(HSSSelectorTypeCombinator, controller)
{
    this->combinatorType = type;
}

QSharedPointer<HSSCombinator> HSSCombinator::clone() const
{
    return qSharedPointerCast<HSSCombinator > (this->cloneImpl());
}

bool HSSCombinator::isA(HSSCombinatorType otherType)
{
    return this->combinatorType == otherType;
}

HSSCombinatorType HSSCombinator::getCombinatorType()
{
    return this->combinatorType;
}

AXRString HSSCombinator::toString()
{
    return "HSSCombinator of type  " + this->combinatorStringRepresentation(this->combinatorType);
}

AXRString HSSCombinator::combinatorStringRepresentation(HSSCombinatorType type)
{
    AXRString types[20];
    types[ HSSCombinatorTypeSiblings ] = "HSSCombinatorTypeSiblings";
    types[ HSSCombinatorTypePreviousSiblings ] = "HSSCombinatorTypePreviousSiblings";
    types[ HSSCombinatorTypeNextSiblings ] = "HSSCombinatorTypeNextSiblings";
    types[ HSSCombinatorTypeChildren ] = "HSSCombinatorTypeChildren";
    types[ HSSCombinatorTypeDescendants ] = "HSSCombinatorTypeDescendants";
    types[ HSSCombinatorTypeTextSelection ] = "HSSCombinatorTypeTextSelection";
    return types[type];
}

QSharedPointer<HSSClonable> HSSCombinator::cloneImpl() const
{
    return QSharedPointer<HSSCombinator>(new HSSCombinator(*this));
}

//does nothing yet

QSharedPointer<HSSSelection> HSSCombinator::filterSelection(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing)
{
    return scope;
}
