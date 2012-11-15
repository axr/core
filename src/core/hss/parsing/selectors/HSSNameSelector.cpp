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
#include "HSSSimpleSelector.h"
#include "HSSMultipleSelection.h"

using namespace AXR;

HSSNameSelector::HSSNameSelector(AXRString elementName, AXRController * controller)
: HSSSelector(HSSSelectorTypeNameSelector, controller)
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
    return qSharedPointerCast<HSSNameSelector> (this->cloneImpl());
}

AXRString HSSNameSelector::getElementName()
{
    return this->elementName;
}

AXRString HSSNameSelector::toString()
{
    return "Name selector targeting an element with name " + this->elementName;
}

HSSClonable::p HSSNameSelector::cloneImpl() const
{
    return HSSNameSelector::p(new HSSNameSelector(*this));
}

HSSSelection::p HSSNameSelector::filterSelection(HSSSelection::p scope, HSSDisplayObject::p thisObj, bool processing)
{
    HSSSimpleSelection::p ret(new HSSSimpleSelection());
    if (scope->isA(HSSSelectionTypeMultipleSelection))
    {
        HSSMultipleSelection::p multiSel = qSharedPointerCast<HSSMultipleSelection>(scope);
        for (HSSMultipleSelection::const_iterator it = multiSel->begin(); it!=multiSel->end(); ++it)
        {
            this->_filterSimpleSelection(ret, *it);
        }
    }
    else if (scope->isA(HSSSelectionTypeSimpleSelection))
    {
        HSSSimpleSelection::p simpleSel = qSharedPointerCast<HSSSimpleSelection>(scope);
        this->_filterSimpleSelection(ret, simpleSel);
    }
    
    return ret;
}

inline void HSSNameSelector::_filterSimpleSelection(HSSSimpleSelection::p & ret, HSSSimpleSelection::p selection)
{
    //select only elements with matching element name
    for (HSSSimpleSelection::const_iterator it = selection->begin(); it != selection->end(); ++it)
    {
        HSSDisplayObject::p theDO = *it;
        bool match = theDO->getElementName() == this->getElementName();
        if ((match && !this->getNegating()) || (!match && this->getNegating()))
        {
            ret->add(theDO);
        }
    }
}
