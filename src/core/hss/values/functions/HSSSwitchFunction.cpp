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
#include "HSSSwitchFunction.h"

using namespace AXR;

HSSSwitchFunction::HSSSwitchFunction(AXRController * controller)
: HSSFunction(HSSFunctionTypeSwitch, controller)
, _condition()
{
    
}

HSSSwitchFunction::~HSSSwitchFunction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSSwitchFunction: destructing switch function");
    this->cleanTrackedObservers();
}

HSSSwitchFunction::HSSSwitchFunction(const HSSSwitchFunction & orig)
: HSSFunction(orig)
{
    
}

QSharedPointer<HSSFunction> HSSSwitchFunction::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSSwitchFunction::cloneImpl() const
{
    QSharedPointer<HSSSwitchFunction> clone = QSharedPointer<HSSSwitchFunction>(new HSSSwitchFunction(*this));
    std::vector<QSharedPointer<HSSCase> >::const_iterator it;
    for (it = this->_cases.begin(); it != this->_cases.end(); ++it)
    {
        clone->addCase(qSharedPointerCast<HSSCase>((*it)->clone()));
    }
    if (this->_condition)
    {
        clone->addValue(this->_condition->clone());
    }
    return clone;
}

AXRString HSSSwitchFunction::toString()
{
    AXRString tempstr = AXRString("HSSSwitchFunction\n");
    return tempstr;
}

bool HSSSwitchFunction::evaluateEvaluables()
{
    QSharedPointer<HSSCase> usedCase;
    QSharedPointer<HSSParserNode> conditionResult = this->evaluateCondition();
    QSharedPointer<HSSCase> defaultCase;
    std::vector<QSharedPointer<HSSCase> >::const_iterator it;
    for (it = this->_cases.begin(); it != this->_cases.end(); ++it)
    {
        bool caseFound = false;
        const QSharedPointer<HSSCase> & theCase = *it;
        const std::vector<QSharedPointer<HSSParserNode> > & cases = theCase->getCases();
        //default
        if (cases.size() == 0)
        {
            defaultCase = theCase;
            continue;
        }
        else
        {
            std::vector<QSharedPointer<HSSParserNode> >::const_iterator it2;
            for (it2 = cases.begin(); it2 != cases.end(); ++it2)
            {
                const QSharedPointer<HSSParserNode> & caseValue = *it2;
                if (caseValue->equalTo(conditionResult))
                {
                    usedCase = theCase;
                    caseFound = true;
                    break;
                }
            }
        }
        if (caseFound)
            break;
    }
    bool shouldReturn = false;
    if (usedCase)
    {
        shouldReturn = usedCase->evaluateEvaluables();
    }
    else if (defaultCase)
    {
        shouldReturn = defaultCase->evaluateEvaluables();
    }
    return shouldReturn;
}

QSharedPointer<HSSParserNode> HSSSwitchFunction::evaluateCondition()
{
    return this->_evaluateCondition(this->_condition);
}

QSharedPointer<HSSParserNode> HSSSwitchFunction::_evaluateCondition(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType()) {
        case HSSParserNodeTypeBooleanConstant:
        case HSSParserNodeTypeNumberConstant:
        {
            return parserNode;
        }
        case HSSParserNodeTypeStringConstant:
        {
            QSharedPointer<HSSStringConstant> strConst = qSharedPointerCast<HSSStringConstant>(parserNode);
            if (strConst->hasArguments())
            {
                return HSSStringConstant::string(strConst->evaluate(), this->getController());
            }
            return parserNode;
        }
        case HSSParserNodeTypeExpression:
        {
            return HSSNumberConstant::number(qSharedPointerCast<HSSExpression>(parserNode)->evaluate(), this->getController());
        }
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> theFunc = qSharedPointerCast<HSSFunction>(parserNode);
            QSharedPointer<HSSObject> remoteObj = theFunc->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                return this->_evaluateCondition(valueNode);
            }
            break;
        }
        case HSSParserNodeTypePropertyPath:
        {
            QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath>(parserNode);
            QSharedPointer<HSSObject> remoteObj = ppath->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                return this->_evaluateCondition(valueNode);
            }
            break;
        }
        case HSSParserNodeTypeComparison:
        {
            QSharedPointer<HSSComparison> theComp = qSharedPointerCast<HSSComparison>(parserNode);
            return HSSBooleanConstant::boolean(theComp->evaluate(), this->getController());
        }
        case HSSParserNodeTypeNegatedVal:
        {
            QSharedPointer<HSSNegatedVal> theNeg = qSharedPointerCast<HSSNegatedVal>(parserNode);
            return this->_evaluateCondition(theNeg->evaluate());
        }
        default:
        {
            AXRError("HSSSwitchFunction", "Unknown parser node type while evaluating condition").raise();
            break;
        }
    }
    return QSharedPointer<HSSParserNode>();
}

void HSSSwitchFunction::addValue(QSharedPointer<HSSParserNode> parserNode)
{
    parserNode->setParentNode(this->HSSParserNode::shared_from_this());
    this->_condition = parserNode;
}

void HSSSwitchFunction::addCase(QSharedPointer<HSSCase> newCase)
{
    newCase->setParentNode(this->shared_from_this());
    this->_cases.push_back(newCase);
}

void HSSSwitchFunction::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    HSSFunction::setScope(newScope);
    //propagate values
    if (this->_condition)
    {
        switch (this->_condition->getType())
        {
            case HSSParserNodeTypeExpression:
                qSharedPointerCast<HSSExpression>(this->_condition)->setScope(newScope);
                break;
            case HSSParserNodeTypeFunction:
                qSharedPointerCast<HSSFunction>(this->_condition)->setScope(newScope);
                break;
            default:
                break;
        }
    }
}

void HSSSwitchFunction::setThisObj(QSharedPointer<HSSDisplayObject> newThis)
{
    HSSFunction::setThisObj(newThis);
    if (this->_condition)
    {
        this->_condition->setThisObj(newThis);
    }
}
