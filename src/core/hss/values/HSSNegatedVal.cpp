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
#include "HSSNegatedVal.h"

using namespace AXR;

HSSNegatedVal::HSSNegatedVal(AXRController * controller)
: HSSParserNode(HSSParserNodeTypeNegatedVal, controller)
{
    
}

HSSNegatedVal::HSSNegatedVal(const HSSNegatedVal & orig)
: HSSParserNode(orig)
{

}

QSharedPointer<HSSNegatedVal> HSSNegatedVal::clone() const
{
    return qSharedPointerCast<HSSNegatedVal> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSNegatedVal::cloneImpl() const
{
    QSharedPointer<HSSNegatedVal> clone(new HSSNegatedVal(*this));
    clone->setValue(this->_value->clone());
    return clone;
}

HSSNegatedVal::~HSSNegatedVal()
{
    
}

AXRString HSSNegatedVal::toString()
{
    return this->_value->toString();
}

AXRString HSSNegatedVal::stringRep()
{
    return this->_value->stringRep();
}

bool HSSNegatedVal::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSNegatedVal> castedNode = qSharedPointerCast<HSSNegatedVal>(otherNode);
    if (castedNode->_value != this->_value) return false;
    return true;
}

void HSSNegatedVal::setValue(QSharedPointer<HSSParserNode> value)
{
    value->setParentNode(this->shared_from_this());
    this->_value = value;
}

const QSharedPointer<HSSParserNode> & HSSNegatedVal::getValue() const
{
    return this->_value;
}

QSharedPointer<HSSParserNode> HSSNegatedVal::evaluate()
{
    return this->_evaluate(this->_value);
}

QSharedPointer<HSSParserNode> HSSNegatedVal::_evaluate(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeBooleanConstant:
        {
            QSharedPointer<HSSBooleanConstant> theBool = qSharedPointerCast<HSSBooleanConstant>(parserNode);
            return QSharedPointer<HSSBooleanConstant>(new HSSBooleanConstant(!theBool->getValue(), this->getController()));
        }
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> theFunc = qSharedPointerCast<HSSFunction>(parserNode);
            QSharedPointer<HSSObject> remoteObj = theFunc->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                if (valueNode)
                {
                    return this->_evaluate(valueNode);
                }
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
                if (valueNode)
                {
                    return this->_evaluate(valueNode);
                }
                
            }
            break;
        }
        default:
        {
            AXRError("HSSNegatedVal", "Unexpected parser node type in negated value").raise();
            break;
        }
    }
    return QSharedPointer<HSSBooleanConstant>(new HSSBooleanConstant(false, this->getController()));
}

void HSSNegatedVal::setThisObj(QSharedPointer<AXR::HSSDisplayObject> value)
{
    HSSParserNode::setThisObj(value);
    this->_value->setThisObj(value);
}

void HSSNegatedVal::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    switch (this->_value->getType()) {
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> theFunc = qSharedPointerCast<HSSFunction>(this->_value);
            theFunc->setScope(newScope);
            break;
        }
        case HSSParserNodeTypeStringConstant:
        {
            QSharedPointer<HSSStringConstant> theStr = qSharedPointerCast<HSSStringConstant>(this->_value);
            theStr->setScope(newScope);
            break;
        }
        default:
            break;
    }
}
