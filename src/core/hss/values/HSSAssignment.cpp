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
#include "HSSAssignment.h"

using namespace AXR;

HSSAssignment::HSSAssignment(AXRController * controller)
: HSSStatement(HSSStatementTypeAssignment, controller)
{
    
}

HSSAssignment::HSSAssignment(const HSSAssignment & orig)
: HSSStatement(orig)
{

}

QSharedPointer<HSSAssignment> HSSAssignment::clone() const
{
    return qSharedPointerCast<HSSAssignment> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSAssignment::cloneImpl() const
{
    QSharedPointer<HSSAssignment> clone(new HSSAssignment(*this));
    clone->setPropertyPath(this->_propertyPath->clone());
    clone->setValue(this->_value->clone());
    return clone;
}

HSSAssignment::~HSSAssignment()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSAssignment: destructing instance");
    this->cleanTrackedObservers();
}

AXRString HSSAssignment::toString()
{
    AXRString tempstr = AXRString("HSSAssignment with property path:\n");
    tempstr += this->_propertyPath->toString();
    tempstr += " and value ";
    tempstr += this->_value->toString();
    return tempstr;
}

QSharedPointer<HSSObject> HSSAssignment::evaluate()
{
    if (this->_value)
    {
        return this->evaluate(this->_value);
    }
    return QSharedPointer<HSSObject>();
}

QSharedPointer<HSSObject> HSSAssignment::evaluate(QSharedPointer<HSSParserNode> node)
{
    QSharedPointer<HSSObject> errorState;
    
    QSharedPointer<HSSDisplayObject> root = this->getController()->root();
    QSharedPointer<HSSSimpleSelection> rootScope(new HSSSimpleSelection(this->getController()));
    rootScope->add(root);
    
    switch (node->getType())
    {
        case HSSParserNodeTypeExpression:
        {
            QSharedPointer<HSSExpression> theExp = qSharedPointerCast<HSSExpression>(node);
            return HSSValue::valueFromParserNode(this->getController(), theExp->evaluateToConst(), node->getSpecificity(), root, rootScope);
        }
        case HSSParserNodeTypeStringConstant:
        {
            QSharedPointer<HSSStringConstant> strConst = qSharedPointerCast<HSSStringConstant>(node);
            if (strConst->hasArguments())
            {
                QSharedPointer<HSSStringConstant> newStrConst(new HSSStringConstant(strConst->evaluate(), this->getController()));
                return HSSValue::valueFromParserNode(this->getController(), newStrConst, node->getSpecificity(), root, rootScope);
            }
            else
            {
                QSharedPointer<HSSObject> valueObj = HSSValue::valueFromParserNode(this->getController(), node, node->getSpecificity(), root, rootScope);
                return valueObj;
            }
        }

        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeBooleanConstant:
        {
            QSharedPointer<HSSObject> valueObj = HSSValue::valueFromParserNode(this->getController(), node, node->getSpecificity(), root, rootScope);
            return valueObj;
        }
            
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> theFunction = qSharedPointerCast<HSSFunction>(node);
            //if we are assigning an user defined function, return it instead of evaluating
            if (theFunction->isA(HSSFunctionTypeFunction))
            {
                QSharedPointer<HSSObject> valueObj = HSSValue::valueFromParserNode(this->getController(), node, node->getSpecificity(), root, rootScope);
                return valueObj;
            }
            else
            {
                theFunction->setThisObj(root);
                theFunction->setScope(rootScope);
                QSharedPointer<HSSObject> remoteObj = theFunction->evaluate();
                return remoteObj;
            }
        }
        case HSSParserNodeTypeStatement:
        {
            switch (node->getStatementType())
            {
                case HSSStatementTypeObjectDefinition:
                {
                    QSharedPointer<HSSObjectDefinition> objDef = qSharedPointerCast<HSSObjectDefinition>(node);
                    QSharedPointer<HSSObject> theObj = objDef->getObject();
                    theObj->commitStackValues();
                    return theObj;
                }
                    
                default:
                    break;
            }
        }
        case HSSParserNodeTypeUnaryExpression:
        {
            QSharedPointer<HSSUnaryExpression> uExp = qSharedPointerCast<HSSUnaryExpression>(node);
            if (uExp->isA(HSSUnaryExpressionTypeSum))
            {
                QSharedPointer<HSSUnarySum> uSum = qSharedPointerCast<HSSUnarySum>(node);
                QSharedPointer<HSSPropertyPath> ppath = this->propertyPath();
                QSharedPointer<HSSObject> currentValue = ppath->evaluate();
                if (uSum->isStringOperation())
                {
                    QSharedPointer<HSSStringConstant> newStr = uSum->appendString(currentValue);
                    QSharedPointer<HSSObject> valueObj = HSSValue::valueFromParserNode(this->getController(), newStr, node->getSpecificity(), root, rootScope);
                    return valueObj;
                }
                else
                {
                    QSharedPointer<HSSNumberConstant> newNum = uSum->appendNumber(currentValue);
                    if (newNum)
                    {
                        QSharedPointer<HSSObject> valueObj = HSSValue::valueFromParserNode(this->getController(), newNum, node->getSpecificity(), root, rootScope);
                        return valueObj;
                    }
                }
            }
            else
            {
                QSharedPointer<HSSPropertyPath> ppath = this->propertyPath();
                QSharedPointer<HSSObject> currentValue = ppath->evaluate();
                QSharedPointer<HSSNumberConstant> newNum = uExp->appendNumber(currentValue);
                if (newNum)
                {
                    QSharedPointer<HSSObject> valueObj = HSSValue::valueFromParserNode(this->getController(), newNum, node->getSpecificity(), root, rootScope);
                    return valueObj;
                }
            }
            break;
        }
        case HSSParserNodeTypeObjectNameConstant:
        {
            QSharedPointer<HSSObjectNameConstant> theObjName = qSharedPointerCast<HSSObjectNameConstant>(node);
            return this->getVar(theObjName->getValue());
        }
        case HSSParserNodeTypePropertyPath:
        {
            QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath>(node);
            return ppath->evaluate();
        }
        case HSSParserNodeTypeNegatedVal:
        {
            QSharedPointer<HSSNegatedVal> theNeg = qSharedPointerCast<HSSNegatedVal>(node);
            this->evaluate(theNeg->evaluate());
            break;
        }

        default:
        {
            AXRError("HSSAssignment", HSSString("Unknown parser node type in assignment: ") + node->stringRep()).raise();
            break;
        }
    }
    return errorState;
}

bool HSSAssignment::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSStatement::equalTo(otherNode)) return false;

    return true;
}

void HSSAssignment::setPropertyPath(QSharedPointer<HSSPropertyPath> newPath)
{
    newPath->setParentNode(this->HSSParserNode::shared_from_this());
    this->_propertyPath = newPath;
}

QSharedPointer<HSSPropertyPath> HSSAssignment::propertyPath()
{
    return this->_propertyPath;
}

void HSSAssignment::setValue(QSharedPointer<HSSParserNode> parserNode)
{
    parserNode->setParentNode(this->HSSParserNode::shared_from_this());
    this->_value = parserNode;
}

QSharedPointer<HSSParserNode> HSSAssignment::value()
{
    return this->_value;
}

void HSSAssignment::addValue(QSharedPointer<AXR::HSSParserNode> value)
{
    this->setValue(value);
}

void HSSAssignment::setThisObj(QSharedPointer<HSSDisplayObject> newThis)
{
    HSSStatement::setThisObj(newThis);
    if (this->_propertyPath)
    {
        this->_propertyPath->setThisObj(newThis);
    }
    if (this->_value)
    {
        this->_value->setThisObj(newThis);
    }
}

void HSSAssignment::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    if (this->_propertyPath)
    {
        this->_propertyPath->setScope(newScope);
    }
    if (this->_value)
    {
        switch (this->_value->getType())
        {
            case HSSParserNodeTypeFunction:
            {
                QSharedPointer<HSSFunctionCall> func = qSharedPointerCast<HSSFunctionCall > (this->_value);
                func->setScope(newScope);
                break;
            }
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> exp = qSharedPointerCast<HSSExpression > (this->_value);
                exp->setScope(newScope);
                break;
            }
            case HSSParserNodeTypePropertyPath:
            {
                QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath > (this->_value);
                ppath->setScope(newScope);
                break;
            }
            case HSSParserNodeTypeStringConstant:
            {
                QSharedPointer<HSSStringConstant> strConst = qSharedPointerCast<HSSStringConstant> (this->_value);
                strConst->setScope(newScope);
            }
            default:
                break;
        }
        
        switch (this->_value->getStatementType())
        {
            case HSSStatementTypeObjectDefinition:
            {
                QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (this->_value);
                objdef->setScope(newScope);
                break;
            }
            default:
                break;
        }
    }
}
