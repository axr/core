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

#include "AXRController.h"
#include "HSSFlagAction.h"

using namespace AXR;

HSSFlagAction::HSSFlagAction()
: HSSAction(HSSActionTypeFlag)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSFlagAction: creating flag action object");
}

HSSFlagAction::HSSFlagAction(const HSSFlagAction & orig)
: HSSAction(orig)
{
    this->_flagFunction = boost::static_pointer_cast<HSSFlagFunction > (orig._flagFunction->clone());
}

HSSFlagAction::p HSSFlagAction::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSFlagAction: cloning flag action object");
    return boost::static_pointer_cast<HSSFlagAction, HSSClonable > (this->cloneImpl());
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

std::string HSSFlagAction::toString()
{
    return "HSSFlag";
}

std::string HSSFlagAction::defaultObjectType()
{
    return "Flag";
}

void HSSFlagAction::fire()
{
    HSSFlagFunction::p flagFunction = this->getFlagFunction();
    std::vector< std::vector<HSSDisplayObject::p> > selection = this->axrController->select(flagFunction->getSelectorChains(), *this->scope, this->getThisObj(), false);
    if (selection.size() == 0)
    {
        // ignore
    }
    else
    {

        std::vector< std::vector<HSSDisplayObject::p> >::iterator outerIt;
        std::vector<HSSDisplayObject::p>::iterator innerIt;

        for (outerIt = selection.begin(); outerIt != selection.end(); outerIt++)
        {
            std::vector<HSSDisplayObject::p> & inner = *outerIt;
            for (innerIt = inner.begin(); innerIt != inner.end(); innerIt++)
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
                    throw AXRWarning::p(new AXRWarning("HSSFlagAction", "Invalid flag function type for flag action"));
                    break;
                }
            }
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
