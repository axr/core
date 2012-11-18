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

#include "AXRController.h"
#include "AXRDebugging.h"
#include "AXRWarning.h"
#include "HSSFlagAction.h"

using namespace AXR;

HSSFlagAction::HSSFlagAction(AXRController * controller)
: HSSAction(HSSActionTypeFlag, controller)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSFlagAction: creating flag action object");
}

HSSFlagAction::HSSFlagAction(const HSSFlagAction & orig)
: HSSAction(orig)
{
    this->_flagFunction = qSharedPointerCast<HSSFlagFunction > (orig._flagFunction->clone());
}

HSSFlagAction::p HSSFlagAction::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSFlagAction: cloning flag action object");
    return qSharedPointerCast<HSSFlagAction> (this->cloneImpl());
}

HSSClonable::p HSSFlagAction::cloneImpl() const
{
    HSSFlagAction::p clone = HSSFlagAction::p(new HSSFlagAction(*this));
    return clone;
}

HSSFlagAction::~HSSFlagAction()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSFlagAction: destructing flag action object");
}

AXRString HSSFlagAction::toString()
{
    return "HSSFlag";
}

AXRString HSSFlagAction::defaultObjectType()
{
    return "Flag";
}

void HSSFlagAction::fire()
{
    HSSFlagFunction::p flagFunction = this->getFlagFunction();
    HSSSelection::p selection = this->getController()->select(flagFunction->getSelectorChains(), this->scope, this->getThisObj());
    HSSSimpleSelection::p inner = selection->joinAll();
    for (HSSSimpleSelection::iterator innerIt = inner->begin(); innerIt != inner->end(); ++innerIt)
    {
        HSSDisplayObject::p container = *innerIt;
        switch (flagFunction->getFlagFunctionType())
        {
            case HSSFlagFunctionTypeFlag:
                container->flagsActivate(flagFunction->getName());
                break;
            case HSSFlagFunctionTypeUnflag:
                container->flagsDeactivate(flagFunction->getName());
                break;
            case HSSFlagFunctionTypeToggleFlag:
                container->flagsToggle(flagFunction->getName());
                break;

            default:
                throw AXRWarning("HSSFlagAction", "Invalid flag function type for flag action");
                break;
        }
    }
}

HSSFlagFunction::p HSSFlagAction::getFlagFunction()
{
    return this->_flagFunction;
}

void HSSFlagAction::setFlagFunction(HSSFlagFunction::p newValue)
{
    this->_flagFunction = newValue;
}
