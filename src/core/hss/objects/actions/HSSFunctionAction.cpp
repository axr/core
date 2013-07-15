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

#include "AXRController.h"
#include "AXRLoggerManager.h"
#include "HSSDisplayObject.h"
#include "HSSFunction.h"
#include "HSSFunctionAction.h"

using namespace AXR;

HSSFunctionAction::HSSFunctionAction(AXRController * controller)
: HSSAction(HSSActionTypeFunction, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSFunctionAction: creating function action object");
}

HSSFunctionAction::HSSFunctionAction(const HSSFunctionAction & orig)
: HSSAction(orig)
{
    //this->_function = orig._function->clone();
}

QSharedPointer<HSSFunctionAction> HSSFunctionAction::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSFunctionAction: cloning function action object");
    return qSharedPointerCast<HSSFunctionAction> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSFunctionAction::cloneImpl() const
{
    QSharedPointer<HSSFunctionAction> clone = QSharedPointer<HSSFunctionAction>(new HSSFunctionAction(*this));
    return clone;
}

HSSFunctionAction::~HSSFunctionAction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSFunctionAction: destructing function action object");
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
    QSharedPointer<HSSFunction> function = this->getFunction();
    function->_evaluate();
}

QSharedPointer<HSSFunction> HSSFunctionAction::getFunction()
{
    return this->_function;
}

void HSSFunctionAction::setFunction(QSharedPointer<HSSFunction> newValue)
{
    this->_function = newValue;
}
