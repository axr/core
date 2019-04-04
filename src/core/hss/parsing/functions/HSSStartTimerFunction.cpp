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
#include "HSSStartTimerFunction.h"

using namespace AXR;

HSSStartTimerFunction::HSSStartTimerFunction(AXRController * controller)
: HSSFunction(HSSFunctionTypeStartTimer, controller)
{
    this->repeats = true;
}

HSSStartTimerFunction::~HSSStartTimerFunction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSStartTimerFunction: destructing start timer function");
    this->cleanTrackedObservers();
}

HSSStartTimerFunction::HSSStartTimerFunction(const HSSStartTimerFunction & orig)
: HSSFunction(orig)
{
    this->timerName = orig.timerName;
    this->function = orig.function;
    this->ms = orig.ms;
    this->repeats = orig.repeats;
}

QSharedPointer<HSSFunction> HSSStartTimerFunction::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSStartTimerFunction::cloneImpl() const
{
    QSharedPointer<HSSStartTimerFunction> clone = QSharedPointer<HSSStartTimerFunction>(new HSSStartTimerFunction(*this));
    
    return clone;
}

AXRString HSSStartTimerFunction::toString()
{
    AXRString tempstr = AXRString("HSSStartTimerFunction\n");
    return tempstr;
}

QSharedPointer<HSSObject> HSSStartTimerFunction::_evaluate()
{
    AXRString timerName = this->getTimerName();
    if (timerName == "once")
    {
        this->getController()->document()->startTimerOnce(this, this->getComputedMs());
    }
    else
    {
        this->getController()->document()->startTimer(timerName, this, this->getComputedMs(), this->getRepeats());
    }

    this->setDirty(true);
    return this->_value;
}

void HSSStartTimerFunction::fireTimer(AXRString timerName)
{
    if (this->function->getType() == HSSParserNodeTypeFunctionCall)
    {
        QSharedPointer<HSSFunction> fun = qSharedPointerCast<HSSFunction>(this->getFunction());
        fun->evaluate();
    }
}

void HSSStartTimerFunction::setTimerName(AXRString _timerName)
{
    this->timerName = _timerName;
}
AXRString HSSStartTimerFunction::getTimerName() const
{
    return this->timerName;
}

void HSSStartTimerFunction::setFunction(QSharedPointer<HSSParserNode> _fun)
{
    if (!_fun)
        return;
    
    this->function = _fun;
}
QSharedPointer<HSSParserNode> HSSStartTimerFunction::getFunction() const
{
    return this->function;
}

void HSSStartTimerFunction::setMs(QSharedPointer<HSSParserNode> _ms)
{
    if (!_ms)
        return;

    this->ms = _ms;
}
QSharedPointer<HSSParserNode> HSSStartTimerFunction::getMs() const
{
    return this->ms;
}

HSSUnit HSSStartTimerFunction::getComputedMs()
{
    switch (this->ms->getType()) {
        case HSSParserNodeTypeNumberConstant:
        {
            QSharedPointer<HSSNumberConstant> numConst = qSharedPointerCast<HSSNumberConstant>(this->ms);
            return numConst->getValue();
        }
        default:
            break;
    }
    return 100;
}

void HSSStartTimerFunction::setRepeats(bool _repeats)
{
    this->repeats = _repeats;
}
bool HSSStartTimerFunction::getRepeats()
{
    return this->repeats;
}

void HSSStartTimerFunction::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    HSSFunction::setScope(newScope);
    //propagate values
    if (this->function)
    {
        switch (this->function->getType())
        {
            case HSSParserNodeTypeExpression:
                qSharedPointerCast<HSSExpression>(this->function)->setScope(newScope);
                break;
            case HSSParserNodeTypeFunctionCall:
                qSharedPointerCast<HSSFunction>(this->function)->setScope(newScope);
                break;
            default:
                break;
        }
    }
    if (this->ms)
    {
        switch (this->ms->getType())
        {
            case HSSParserNodeTypeExpression:
                qSharedPointerCast<HSSExpression>(this->function)->setScope(newScope);
                break;
            case HSSParserNodeTypeFunctionCall:
                qSharedPointerCast<HSSFunction>(this->function)->setScope(newScope);
                break;
            default:
                break;
        }
    }
}

void HSSStartTimerFunction::setThisObj(QSharedPointer<HSSDisplayObject> newThis)
{
    HSSFunction::setThisObj(newThis);
    this->function->setThisObj(newThis);
    this->ms->setThisObj(newThis);
}
