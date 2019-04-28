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
#include "HSSToNumFunction.h"

using namespace AXR;

HSSToNumFunction::HSSToNumFunction(AXRController * controller)
: HSSFunction(HSSFunctionTypeToNum, controller)
{
    
}

HSSToNumFunction::HSSToNumFunction(const HSSToNumFunction & orig)
: HSSFunction(orig)
{
    
}

QSharedPointer<HSSFunction> HSSToNumFunction::clone() const
{
    return qSharedPointerCast<HSSToNumFunction> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSToNumFunction::cloneImpl() const
{
    QSharedPointer<HSSToNumFunction> clone = QSharedPointer<HSSToNumFunction>(new HSSToNumFunction(*this));
    if (this->_node)
    {
        clone->addValue(this->_node->clone());
    }
    return clone;
}

HSSToNumFunction::~HSSToNumFunction()
{
}

AXRString HSSToNumFunction::toString()
{
    AXRString tempstr = AXRString("HSSToNumFunction\n");
    return tempstr;
}

bool HSSToNumFunction::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSCustomFunction> castedNode = qSharedPointerCast<HSSCustomFunction>(otherNode);
    //@todo compare arguments?
    return true;
}

HSSUnit HSSToNumFunction::evaluate()
{
    return this->_evaluateNode(this->_node);
}

HSSUnit HSSToNumFunction::_evaluateNode(QSharedPointer<HSSParserNode> node)
{
    switch (node->getType())
    {
        case HSSParserNodeTypeExpression:
        {
            QSharedPointer<HSSExpression> theExp = qSharedPointerCast<HSSExpression>(node);
            return theExp->evaluate();
        }
        case HSSParserNodeTypeNumberConstant:
        {
            return qSharedPointerCast<HSSNumberConstant>(node)->getValue();
        }
        case HSSParserNodeTypeStringConstant:
        {
            return std::stold(qSharedPointerCast<HSSStringConstant>(node)->getValue().data());
        }
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> theFunc = qSharedPointerCast<HSSFunction>(node);
            QSharedPointer<HSSObject> remoteObj = theFunc->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                if (valueNode)
                {
                    return this->_evaluateNode(valueNode);
                }
            }
            break;
        }
        case HSSParserNodeTypePropertyPath:
        {
            QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath>(node);
            QSharedPointer<HSSObject> remoteObj = ppath->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                if (valueNode)
                {
                    return this->_evaluateNode(valueNode);
                }
            }
            break;
        }
        default:
        {
            AXRError("HSSLogFunction", "Unexpected parser node type in log function").raise();
            break;
        }
    }
    return 0;
}

void HSSToNumFunction::addValue(QSharedPointer<HSSParserNode> parserNode)
{
    parserNode->setParentNode(this->shared_from_this());
    this->_node = parserNode;
}

void HSSToNumFunction::setThisObj(QSharedPointer<HSSDisplayObject> newThis)
{
    HSSParserNode::setThisObj(newThis);
    if (this->_node)
    {
        this->_node->setThisObj(newThis);
    }
}

void HSSToNumFunction::setScope(QSharedPointer<HSSSimpleSelection> newScope)
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
