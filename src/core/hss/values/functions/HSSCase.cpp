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
#include "HSSCase.h"

using namespace AXR;

HSSCase::HSSCase(AXRController * controller)
: HSSEvaluableFunction(HSSFunctionTypeCase, controller)
, _readEvaluables(false)
{
    
}

HSSCase::~HSSCase()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSCase: destructing case function");
    this->cleanTrackedObservers();
}

HSSCase::HSSCase(const HSSCase & orig)
: HSSEvaluableFunction(orig)
{
    this->_readEvaluables = orig._readEvaluables;
}

QSharedPointer<HSSFunction> HSSCase::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSCase::cloneImpl() const
{
    QSharedPointer<HSSCase> clone = QSharedPointer<HSSCase>(new HSSCase(*this));
    std::vector<QSharedPointer<HSSParserNode> >::const_iterator it;
    for (it = this->_cases.begin(); it != this->_cases.end(); ++it)
    {
        const QSharedPointer<HSSParserNode> & caseValue = *it;
        clone->addValue(caseValue->clone());
    }
    this->cloneEvaluables(clone);
    return clone;
}

AXRString HSSCase::toString()
{
    AXRString tempstr = AXRString("HSSCase\n");
    return tempstr;
}

QSharedPointer<HSSObject> HSSCase::_evaluate()
{
    this->evaluateEvaluables();
    this->setDirty(true);
    return this->_value;
}

void HSSCase::addValue(QSharedPointer<HSSParserNode> parserNode)
{
    if (this->_readEvaluables)
    {
        this->addEvaluable(parserNode);
    }
    else
    {
        parserNode->setParentNode(this->shared_from_this());
        this->_cases.push_back(parserNode);
    }
}

const std::vector<QSharedPointer<HSSParserNode> > & HSSCase::getCases()
{
    return this->_cases;
}

void HSSCase::setReadEvaluables(bool newVal)
{
    this->_readEvaluables = newVal;
}
const bool HSSCase::readEvaluables() const
{
    return this->_readEvaluables;
}

void HSSCase::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    std::vector<QSharedPointer<HSSParserNode> >::const_iterator it;
    for (it = this->_cases.begin(); it != this->_cases.end(); ++it)
    {
        const QSharedPointer<HSSParserNode> & caseValue = *it;
        switch (caseValue->getType())
        {
            case HSSParserNodeTypeExpression:
                qSharedPointerCast<HSSExpression>(caseValue)->setScope(newScope);
                break;
            case HSSParserNodeTypeFunction:
                qSharedPointerCast<HSSFunction>(caseValue)->setScope(newScope);
                break;
            case HSSParserNodeTypeStringConstant:
                qSharedPointerCast<HSSStringConstant>(caseValue)->setScope(newScope);
                break;
            default:
                break;
        }
    }
    
    HSSEvaluableFunction::setScope(newScope);
}

void HSSCase::setThisObj(QSharedPointer<HSSDisplayObject> newThis)
{
    std::vector<QSharedPointer<HSSParserNode> >::const_iterator it;
    for (it = this->_cases.begin(); it != this->_cases.end(); ++it)
    {
        const QSharedPointer<HSSParserNode> & caseValue = *it;
        caseValue->setThisObj(newThis);
    }
    HSSEvaluableFunction::setThisObj(newThis);
}
