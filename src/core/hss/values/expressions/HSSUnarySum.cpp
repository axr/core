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
#include "HSSUnarySum.h"

using namespace AXR;

HSSUnarySum::HSSUnarySum(AXRController * controller)
: HSSUnaryExpression(HSSUnaryExpressionTypeSum, controller)
, _stringMode(false)
{
    
}

HSSUnarySum::HSSUnarySum(const HSSUnarySum &orig)
: HSSUnaryExpression(orig)
, _stringMode(orig._stringMode)
{
    
}

QSharedPointer<HSSUnarySum> HSSUnarySum::clone() const
{
    return qSharedPointerCast<HSSUnarySum> (this->cloneImpl());
}

HSSUnarySum::~HSSUnarySum()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSUnarySum: destructing unary sum expression");
    this->cleanTrackedObservers();
}

HSSUnit HSSUnarySum::calculate(HSSUnit leftval, HSSUnit rightval)
{
    return leftval + rightval;
}

QSharedPointer<HSSClonable> HSSUnarySum::cloneImpl() const
{
    return QSharedPointer<HSSUnarySum>(new HSSUnarySum(*this));
}

void HSSUnarySum::addValue(QSharedPointer<AXR::HSSParserNode> value)
{
    if (value->isA(HSSParserNodeTypeStringConstant))
    {
        this->_stringMode = true;
    }
    this->_rightNode = value;
}

void HSSUnarySum::mergeNumber(QSharedPointer<HSSNumberConstant> numConst, QSharedPointer<HSSUnaryExpression> other)
{
    QSharedPointer<HSSNumberConstant> thisNumConst = qSharedPointerCast<HSSNumberConstant>(this->_rightNode);
    HSSUnit thisNumber = thisNumConst->getValue();
    
    switch (other->getUnaryExpressionType()) {
        case HSSUnaryExpressionTypeSum:
        {
            thisNumConst->setValue(thisNumber + numConst->getValue());
            break;
        }
        case HSSUnaryExpressionTypeSubtraction:
        {
            thisNumConst->setValue(thisNumber - numConst->getValue());
            break;
        }
            
        default:
            break;
    }
}

QSharedPointer<HSSStringConstant> HSSUnarySum::appendString(QSharedPointer<HSSObject> currentValue)
{
    QSharedPointer<HSSStringConstant> ret;
    if (currentValue && currentValue->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> currentValueNode = qSharedPointerCast<HSSValue>(currentValue)->getValue();
        switch (currentValueNode->getType())
        {
            case HSSParserNodeTypeStringConstant:
            {
                QSharedPointer<HSSStringConstant> currentStr = qSharedPointerCast<HSSStringConstant>(currentValueNode);
                QSharedPointer<HSSParserNode> rightNode = this->getRightNode();
                
                QSharedPointer<HSSStringConstant> newStrConst(new HSSStringConstant(currentStr->getValue().stripQuotes() + this->_getRightString(rightNode).stripQuotes(), this->getController()));
                newStrConst->setHasStartQuote(false);
                newStrConst->setHasEndQuote(false);
                return newStrConst;
            }
            default:
                break;
        }
    }
    return ret;
}

bool HSSUnarySum::isStringOperation() const
{
    return this->_stringMode;
}

HSSString HSSUnarySum::_getRightString(QSharedPointer<HSSParserNode> rightNode)
{
    HSSString rightString;
    switch (rightNode->getType())
    {
        case HSSParserNodeTypeStringConstant:
        {
            QSharedPointer<HSSStringConstant> rightStr = qSharedPointerCast<HSSStringConstant>(rightNode);
            rightString = rightStr->getValue();
            break;
        }
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> rightFunction = qSharedPointerCast<HSSFunction>(rightNode);
            rightFunction->setThisObj(this->getThisObj());
            rightFunction->setScope(this->scope);
            QSharedPointer<HSSObject> remoteObj = rightFunction->evaluate();
            if(remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                if (valueNode)
                {
                    return this->_getRightString(valueNode);
                }
            }
            break;
        }
        case HSSParserNodeTypeObjectNameConstant:
        {
            QSharedPointer<HSSObjectNameConstant> theObjName = qSharedPointerCast<HSSObjectNameConstant>(rightNode);
            QSharedPointer<HSSObject> remoteObj = this->getController()->getGlobalVariable(theObjName->getValue());
            if(remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                if (valueNode)
                {
                    return this->_getRightString(valueNode);
                }
            }
        }
        default:
        {
            AXRWarning("HSSUnarySum", "Unknown node type while getting right string.").raise();
            break;
        }
    }
    return rightString;
}
