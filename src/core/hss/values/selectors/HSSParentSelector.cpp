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

#include "axr.h"
#include "HSSParentSelector.h"

using namespace AXR;

HSSParentSelector::HSSParentSelector(AXRController * controller)
: HSSNameSelector("@parent", controller)
{
    this->setSelectorType(HSSSelectorTypeParentSelector);
}

QSharedPointer<HSSParentSelector> HSSParentSelector::clone() const
{
    return qSharedPointerCast<HSSParentSelector> (this->cloneImpl());
}

AXRString HSSParentSelector::toString()
{
    return "@parent selector";
}

AXRString HSSParentSelector::stringRep()
{
    return "@parent";
}

QSharedPointer<HSSClonable> HSSParentSelector::cloneImpl() const
{
    return QSharedPointer<HSSParentSelector>(new HSSParentSelector(*this));
}

AXRString HSSParentSelector::getElementName()
{
    return "@parent";
}

QSharedPointer<HSSSelection> HSSParentSelector::filterSelection(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing, bool subscribingToNotifications)
{
    QSharedPointer<HSSSimpleSelection> ret(new HSSSimpleSelection(this->getController()));
    if(!thisObj->isRoot()){
        QSharedPointer<HSSContainer> parent = thisObj->getParent();
        if (parent)
        {
            ret->add(parent);
        }
    }
    return ret;
}
