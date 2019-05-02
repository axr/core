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
#include "HSSReturnFunction.h"

using namespace AXR;

HSSReturnFunction::HSSReturnFunction(AXRController * controller)
: HSSFunction(HSSFunctionTypeReturn, controller)
{
    
}

HSSReturnFunction::HSSReturnFunction(const HSSReturnFunction & orig)
: HSSFunction(orig)
{
    
}

QSharedPointer<HSSFunction> HSSReturnFunction::clone() const
{
    return qSharedPointerCast<HSSReturnFunction> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSReturnFunction::cloneImpl() const
{
    QSharedPointer<HSSReturnFunction> clone = QSharedPointer<HSSReturnFunction>(new HSSReturnFunction(*this));
    if (this->_node)
    {
        clone->addValue(this->_node->clone());
    }
    return clone;
}

HSSReturnFunction::~HSSReturnFunction()
{
    this->cleanTrackedObservers();
}

AXRString HSSReturnFunction::toString()
{
    AXRString tempstr = AXRString("HSSReturnFunction\n");
    return tempstr;
}

bool HSSReturnFunction::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSCustomFunction> castedNode = qSharedPointerCast<HSSCustomFunction>(otherNode);
    //@todo compare arguments?
    return true;
}

QSharedPointer<HSSObject> HSSReturnFunction::_evaluate()
{
    if (this->_node)
    {
        QSharedPointer<HSSParserNode> parentNode = this->getParentNode();
        if (parentNode->isA(HSSParserNodeTypeFunction))
        {
            QSharedPointer<HSSFunction> theFunc = qSharedPointerCast<HSSFunction>(parentNode);
            if (theFunc->isA(HSSFunctionTypeFunction))
            {
                QSharedPointer<HSSFunctionFunction> theFuncFunc = qSharedPointerCast<HSSFunctionFunction>(theFunc);
                theFuncFunc->setReturnValue(this->_evaluateNode(this->_node));
            }
        }
    }
    this->setDirty(true);
    return this->_value;
}

QSharedPointer<HSSObject> HSSReturnFunction::_evaluateNode(QSharedPointer<HSSParserNode> node)
{
    switch (node->getType())
    {
        case HSSParserNodeTypeExpression:
        {
            QSharedPointer<HSSExpression> theExp = qSharedPointerCast<HSSExpression>(node);
            QSharedPointer<HSSNumberConstant> numConst = HSSNumberConstant::number(theExp->evaluate(), this->getController());
            QSharedPointer<HSSObject> valueObj = HSSValue::valueFromParserNode(this->getController(), numConst, node->getSpecificity(), this->getThisObj(), this->scope);
            return valueObj;
        }
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypeStringConstant:
        case HSSParserNodeTypeBooleanConstant:
        {
            QSharedPointer<HSSObject> valueObj = HSSValue::valueFromParserNode(this->getController(), node, node->getSpecificity(), this->getThisObj(), this->scope);
            return valueObj;
        }
        case HSSParserNodeTypeObjectNameConstant:
        {
            //todo delete this case
            QSharedPointer<HSSObjectNameConstant> theObjName = qSharedPointerCast<HSSObjectNameConstant>(node);
            return this->getVar(theObjName->getValue());
        }
        case HSSParserNodeTypeStatement:
        {
            if (node->isA(HSSStatementTypeObjectDefinition))
            {
                QSharedPointer<HSSObjectDefinition> objDef = qSharedPointerCast<HSSObjectDefinition>(node);
                QSharedPointer<HSSObject> theObj = objDef->getObject();
                theObj->commitStackValues();
                return theObj;
            }
        }
        case HSSParserNodeTypeNegatedVal:
        {
            QSharedPointer<HSSNegatedVal> theNeg = qSharedPointerCast<HSSNegatedVal>(node);
            QSharedPointer<HSSParserNode> remoteNode = theNeg->evaluate();
            if (remoteNode)
            {
                QSharedPointer<HSSObject> valueObj = HSSValue::valueFromParserNode(this->getController(), remoteNode, node->getSpecificity(), this->getThisObj(), this->scope);
                return valueObj;
            }
            break;
        }
        case HSSParserNodeTypePropertyPath:
        {
            QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath>(node);
            return ppath->evaluate();
        }
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> theFunc = qSharedPointerCast<HSSFunction>(node);
            return theFunc->evaluate();
        }
        default:
        {
            AXRError("HSSReturnFunction", "Unexpected parser node type while evaluating return function").raise();
            break;
        }
    }
    return QSharedPointer<HSSObject>();
}

void HSSReturnFunction::addValue(QSharedPointer<HSSParserNode> parserNode)
{
    parserNode->setParentNode(this->shared_from_this());
    this->_node = parserNode;
}

void HSSReturnFunction::setThisObj(QSharedPointer<HSSDisplayObject> newThis)
{
    HSSParserNode::setThisObj(newThis);
    if (this->_node)
    {
        this->_node->setThisObj(newThis);
    }
}

void HSSReturnFunction::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    HSSFunction::setScope(newScope);
    if (this->_node)
    {
        switch (this->_node->getType())
        {
            case HSSParserNodeTypeFunction:
            {
                QSharedPointer<HSSFunctionCall> func = qSharedPointerCast<HSSFunctionCall > (this->_node);
                func->setScope(newScope);
                break;
            }
                
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> exp = qSharedPointerCast<HSSExpression > (this->_node);
                exp->setScope(newScope);
                break;
            }
                
            default:
                break;
        }
        
        switch (this->_node->getStatementType())
        {
            case HSSStatementTypeObjectDefinition:
            {
                QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (this->_node);
                objdef->setScope(newScope);
                break;
            }
                
            default:
                break;
        }
    }
}
