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

#include "axr.h"
#include "HSSSum.h"

using namespace AXR;

HSSSum::HSSSum(QSharedPointer<HSSParserNode> _left, QSharedPointer<HSSParserNode> _right, AXRController * controller)
: HSSExpression(HSSExpressionTypeSum, _left, _right, controller)
{

}

HSSSum::HSSSum(const HSSSum &orig)
: HSSExpression(orig)
{

}

QSharedPointer<HSSSum> HSSSum::clone() const
{
    return qSharedPointerCast<HSSSum> (this->cloneImpl());
}

HSSSum::~HSSSum()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSSum: destructing sum expression");
    this->cleanTrackedObservers();
}

HSSUnit HSSSum::calculate(HSSUnit leftval, HSSUnit rightval)
{
    return leftval + rightval;
}

QSharedPointer<HSSClonable> HSSSum::cloneImpl() const
{
    return QSharedPointer<HSSSum>(new HSSSum(*this));
}

bool HSSSum::isStringOperation() const
{
    return this->isStringNode(this->_leftNode) || this->isStringNode(this->_rightNode);
}

bool HSSSum::isStringNode(QSharedPointer<HSSParserNode> parserNode) const
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeStringConstant:
        {
            return true;
        }
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> rightFunction = qSharedPointerCast<HSSFunction>(parserNode);
            rightFunction->setThisObj(this->getThisObj());
            rightFunction->setScope(this->scope);
            QSharedPointer<HSSObject> remoteObj = rightFunction->evaluate();
            if(remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                if (valueNode)
                {
                    return this->isStringNode(valueNode);
                }
            }
            break;
        }
        case HSSParserNodeTypePropertyPath:
        {
            QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath>(parserNode);
            QSharedPointer<HSSObject> searchResult = ppath->evaluate();
            if (searchResult && searchResult->isA(HSSObjectTypeValue))
            {
                return this->isStringNode(qSharedPointerCast<HSSValue>(searchResult)->getValue());
            }
            else
            {
                AXRWarning("HSSValue", AXRString("The path ") + ppath->stringRep() + " did not yield any results.").raise();
            }
            break;
        }
        default:
        {
            AXRWarning("HSSSum", "Unknown node type while checking string operation mode.").raise();
            break;
        }
    }
    return false;
}

QSharedPointer<HSSStringConstant> HSSSum::appendString()
{
    //left
    HSSString leftStr = this->_getString(this->_leftNode);
    //right
    HSSString rightStr = this->_getString(this->_rightNode);
    QSharedPointer<HSSValue> right = this->getRight();
    //calculate
    QSharedPointer<HSSStringConstant> newStrConst(new HSSStringConstant(leftStr.stripQuotes() + rightStr.stripQuotes(), this->getController()));
    newStrConst->setHasStartQuote(false);
    newStrConst->setHasEndQuote(false);
    return newStrConst;
}

HSSString HSSSum::_getString(QSharedPointer<HSSParserNode> parserNode)
{
    HSSString ret = "";
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeStringConstant:
        {
            QSharedPointer<HSSStringConstant> rightStr = qSharedPointerCast<HSSStringConstant>(parserNode);
            ret = rightStr->getValue();
            break;
        }
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> rightFunction = qSharedPointerCast<HSSFunction>(parserNode);
            rightFunction->setThisObj(this->getThisObj());
            rightFunction->setScope(this->scope);
            QSharedPointer<HSSObject> remoteObj = rightFunction->evaluate();
            if(remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                if (valueNode)
                {
                    return this->_getString(valueNode);
                }
            }
            break;
        }
        case HSSParserNodeTypePropertyPath:
        {
            QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath>(parserNode);
            QSharedPointer<HSSObject> searchResult = ppath->evaluate();
            if (searchResult && searchResult->isA(HSSObjectTypeValue))
            {
                return this->_getString(qSharedPointerCast<HSSValue>(searchResult)->getValue());
            }
            else
            {
                AXRWarning("HSSValue", AXRString("The path ") + ppath->stringRep() + " did not yield any results.").raise();
            }
            break;
        }
        default:
        {
            AXRWarning("HSSSum", "Unknown node type while getting string.").raise();
            break;
        }
    }
    return ret;
}
