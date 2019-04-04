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
 *      COPYRIGHT: ©2019 - All Rights Reserved
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
#include "HSSToggleTimerFunction.h"

using namespace AXR;

HSSToggleTimerFunction::HSSToggleTimerFunction(AXRController * controller)
: HSSStartTimerFunction(controller)
{
}

HSSToggleTimerFunction::~HSSToggleTimerFunction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSToggleTimerFunction: destructing start timer function");
}

HSSToggleTimerFunction::HSSToggleTimerFunction(const HSSToggleTimerFunction & orig)
: HSSStartTimerFunction(orig)
{

}

QSharedPointer<HSSFunction> HSSToggleTimerFunction::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSToggleTimerFunction::cloneImpl() const
{
    QSharedPointer<HSSToggleTimerFunction> clone = QSharedPointer<HSSToggleTimerFunction>(new HSSToggleTimerFunction(*this));
    
    return clone;
}

QSharedPointer<HSSObject> HSSToggleTimerFunction::_evaluate()
{
    AXRDocument * doc = this->getController()->document();
    AXRString timerName = this->getTimerName();
    if (doc->hasTimer(timerName))
    {
        doc->stopTimer(timerName);
    }
    else
    {
        HSSStartTimerFunction::_evaluate();
    }
    this->setDirty(true);
    return this->_value;
}
