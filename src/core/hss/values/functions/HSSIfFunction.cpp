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
#include "HSSIfFunction.h"

using namespace AXR;

HSSIfFunction::HSSIfFunction(AXRController * controller)
: HSSEvaluableFunction(HSSFunctionTypeIf, controller)
, _comparisonType(HSSComparisonTypeNone)
, _condition()
, _readEvaluables(false)
{
    
}

HSSIfFunction::~HSSIfFunction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSIfFunction: destructing if function");
    this->cleanTrackedObservers();
}

HSSIfFunction::HSSIfFunction(const HSSIfFunction & orig)
: HSSEvaluableFunction(orig)
{
    this->_comparisonType = orig._comparisonType;
    this->_readEvaluables = orig._readEvaluables;
}

QSharedPointer<HSSFunction> HSSIfFunction::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSIfFunction::cloneImpl() const
{
    QSharedPointer<HSSIfFunction> clone = QSharedPointer<HSSIfFunction>(new HSSIfFunction(*this));
    if (this->_condition)
    {
        clone->addValue(this->_condition->clone());
    }
    this->cloneEvaluables(clone);
    if (this->_else)
    {
        clone->setElse(this->_else->clone());
    }
    return clone;
}

AXRString HSSIfFunction::toString()
{
    AXRString tempstr = AXRString("HSSIfFunction\n");
    return tempstr;
}

QSharedPointer<HSSObject> HSSIfFunction::_evaluate()
{
    if (this->evaluateCondition())
    {
        this->evaluateEvaluables();
    }
    this->setDirty(true);
    return this->_value;
}

bool HSSIfFunction::evaluateCondition()
{
    return (!this->_condition || this->_evaluateCondition(this->_condition));
}

bool HSSIfFunction::_evaluateCondition(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType()) {
        case HSSParserNodeTypeBooleanConstant:
        {
            return qSharedPointerCast<HSSBooleanConstant>(parserNode)->getValue();
        }
        case HSSParserNodeTypeNumberConstant:
        {
            return qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue() > 0;
        }
        case HSSParserNodeTypeStringConstant:
        {
            HSSString theString = qSharedPointerCast<HSSStringConstant>(parserNode)->getValue();
            if (theString == "1" || theString == "true" || theString == "yes")
            {
                return true;
            }
            else if (theString == "0" || theString == "false" || theString == "no")
            {
                return false;
            }
            return theString.length() > 0;
        }
        case HSSParserNodeTypeExpression:
        {
            return qSharedPointerCast<HSSExpression>(parserNode)->evaluate() > 0;
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
            return theComp->evaluate();
        }
        case HSSParserNodeTypeNegatedVal:
        {
            QSharedPointer<HSSNegatedVal> theNeg = qSharedPointerCast<HSSNegatedVal>(parserNode);
            return this->_evaluateCondition(theNeg->evaluate());
        }
        default:
        {
            AXRError("HSSIfFunction", "Unknown parser node type while evaluating condition").raise();
            break;
        }
    }
    return false;
}

void HSSIfFunction::addValue(QSharedPointer<HSSParserNode> parserNode)
{
    parserNode->setParentNode(this->HSSParserNode::shared_from_this());
    if (this->_readEvaluables)
    {
        this->addEvaluable(parserNode);
    }
    else
    {
        this->_condition = parserNode;
    }
}

void HSSIfFunction::setReadEvaluables(bool newVal)
{
    this->_readEvaluables = newVal;
}
const bool HSSIfFunction::readEvaluables() const
{
    return this->_readEvaluables;
}
void HSSIfFunction::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    HSSEvaluableFunction::setScope(newScope);
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
    if (this->_else)
    {
        switch (this->_else->getType())
        {
            case HSSParserNodeTypeExpression:
                qSharedPointerCast<HSSExpression>(this->_else)->setScope(newScope);
                break;
            case HSSParserNodeTypeFunction:
                qSharedPointerCast<HSSFunction>(this->_else)->setScope(newScope);
                break;
            default:
                break;
        }
    }
}

void HSSIfFunction::setThisObj(QSharedPointer<HSSDisplayObject> newThis)
{
    HSSEvaluableFunction::setThisObj(newThis);
    if (this->_condition)
    {
        this->_condition->setThisObj(newThis);
    }
    if (this->_else)
    {
        this->_else->setThisObj(newThis);
    }
}

void HSSIfFunction::setElse(QSharedPointer<HSSParserNode> newElse)
{
    newElse->setParentNode(this->getParentNode());
    this->_else = newElse;
}

QSharedPointer<HSSParserNode> HSSIfFunction::getElse()
{
    return this->_else;
}
