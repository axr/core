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
#include "HSSFunctionAction.h"

using namespace AXR;

HSSFunctionAction::HSSFunctionAction()
: HSSAction(HSSActionTypeFunction)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSFunctionAction: creating function action object");
}

HSSFunctionAction::HSSFunctionAction(const HSSFunctionAction & orig)
: HSSAction(orig)
{
    //this->_function = orig._function->clone();
}

HSSFunctionAction::p HSSFunctionAction::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSFunctionAction: cloning function action object");
    return qSharedPointerCast<HSSFunctionAction, HSSClonable > (this->cloneImpl());
}

HSSClonable::p HSSFunctionAction::cloneImpl() const
{
    HSSFunctionAction::p clone = HSSFunctionAction::p(new HSSFunctionAction(*this));
    return clone;
}

HSSFunctionAction::~HSSFunctionAction()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSFunctionAction: destructing function action object");
}

AXRString HSSFunctionAction::toString()
{
    return "HSSFunctionAction";
}

AXRString HSSFunctionAction::defaultObjectType()
{
    return "value";
}

void HSSFunctionAction::fire()
{
    HSSFunction::p function = this->getFunction();
    function->_evaluate(function->getArguments());
}

HSSFunction::p HSSFunctionAction::getFunction()
{
    return this->_function;
}

void HSSFunctionAction::setFunction(HSSFunction::p newValue)
{
    this->_function = newValue;
}
