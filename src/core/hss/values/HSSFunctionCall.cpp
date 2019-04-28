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
#include "HSSFunctionCall.h"

using namespace AXR;

HSSFunctionCall::HSSFunctionCall(AXRController * controller)
: HSSFunction(HSSFunctionTypeCall, controller)
{
    
}

HSSFunctionCall::HSSFunctionCall(const HSSFunctionCall & orig)
: HSSFunction(orig)
{
    this->scope = orig.scope;
    this->_subject = orig._subject;
}

QSharedPointer<HSSFunction> HSSFunctionCall::clone() const
{
    return qSharedPointerCast<HSSFunctionCall> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSFunctionCall::cloneImpl() const
{
    QSharedPointer<HSSFunctionCall> clone = QSharedPointer<HSSFunctionCall>(new HSSFunctionCall(*this));
    
    for (std::vector<QSharedPointer<HSSArgument> >::const_iterator it = this->_arguments.begin(); it != this->_arguments.end(); ++it)
    {
        QSharedPointer<HSSArgument> clonedArgument = (*it)->clone();
        clone->_arguments.push_back(clonedArgument);
    }
    
    return clone;
}

HSSFunctionCall::~HSSFunctionCall()
{
}

HSSString HSSFunctionCall::toString()
{
    HSSString tempstr = HSSString("HSSFunctionCall\n");
    return tempstr;
}

HSSString HSSFunctionCall::stringRep()
{
    HSSString tempstr = "(";
    for (std::vector<QSharedPointer<HSSArgument> >::const_iterator it = this->_arguments.begin(); it != this->_arguments.end(); /*manually increment*/)
    {
        QSharedPointer<HSSArgument> argument = *it;
        tempstr.append(argument->stringRep());
        ++it;
        if (it != this->_arguments.end())
        {
            tempstr.append(", ");
        }
    }
    tempstr.append(")");
    return tempstr;
}

void HSSFunctionCall::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    this->scope = newScope;
    if (this->_subject)
    {
        switch (this->_subject->getType())
        {
            case HSSParserNodeTypeFunction:
            {
                QSharedPointer<HSSFunctionCall> func = qSharedPointerCast<HSSFunctionCall > (this->_subject);
                func->setScope(newScope);
                break;
            }
                
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> exp = qSharedPointerCast<HSSExpression > (this->_subject);
                exp->setScope(newScope);
                break;
            }
                
            default:
                break;
        }
        
        switch (this->_subject->getStatementType())
        {
            case HSSStatementTypeObjectDefinition:
            {
                QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (this->_subject);
                objdef->setScope(newScope);
                break;
            }
                
            default:
                break;
        }
    }

    std::vector<QSharedPointer<HSSArgument> >::const_iterator it;
    for (it = this->_arguments.begin(); it != this->_arguments.end(); ++it)
    {
        const QSharedPointer<HSSParserNode> & node = (*it)->node();
        switch (node->getType())
        {
            case HSSParserNodeTypeFunction:
            {
                QSharedPointer<HSSFunctionCall> func = qSharedPointerCast<HSSFunctionCall > (node);
                func->setScope(newScope);
                break;
            }
                
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> exp = qSharedPointerCast<HSSExpression > (node);
                exp->setScope(newScope);
                break;
            }
                
            default:
                break;
        }
        
        switch (node->getStatementType())
        {
            case HSSStatementTypeObjectDefinition:
            {
                QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (node);
                objdef->setScope(newScope);
                break;
            }
                
            default:
                break;
        }
    }
}

bool HSSFunctionCall::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSCustomFunction> castedNode = qSharedPointerCast<HSSCustomFunction>(otherNode);
    //@todo compare arguments?
    return true;
}

QSharedPointer<HSSObject> HSSFunctionCall::_evaluate()
{
    this->_value = this->__evaluate(this->_subject);
    this->setDirty(true);
    return this->_value;
}

QSharedPointer<HSSObject> HSSFunctionCall::__evaluate(QSharedPointer<HSSParserNode> parserNode)
{
    switch (this->_subject->getType())
    {
        case HSSParserNodeTypePropertyPath:
        {
            QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath>(parserNode);
            QSharedPointer<HSSObject> globalVar = this->getController()->getGlobalVariable(ppath->front()->evaluate());
            ppath->initialize();
            ppath->goToNext();
            QSharedPointer<HSSObject> remoteObj = ppath->getComputed(globalVar).value;
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSValue> theValue = qSharedPointerCast<HSSValue>(remoteObj);
                QSharedPointer<HSSParserNode> valueNode = theValue->getValue();
                if (valueNode->isA(HSSFunctionTypeFunction))
                {
                    QSharedPointer<HSSFunctionFunction> theFunc = qSharedPointerCast<HSSFunctionFunction>(valueNode);
                    return theFunc->evaluate(this->getArguments());
                }
                else
                {
                    return this->__evaluate(valueNode);
                }
            }
        }
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> theFunc = qSharedPointerCast<HSSFunction>(parserNode);
            QSharedPointer<HSSObject> remoteObj = theFunc->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSValue> theValue = qSharedPointerCast<HSSValue>(remoteObj);
                QSharedPointer<HSSParserNode> valueNode = theValue->getValue();
                if (valueNode->isA(HSSFunctionTypeFunction))
                {
                    QSharedPointer<HSSFunctionFunction> theFunc = qSharedPointerCast<HSSFunctionFunction>(valueNode);
                    return theFunc->evaluate(this->getArguments());
                }
                else
                {
                    return this->__evaluate(valueNode);
                }
            }
        }
        default:
            break;
    }

    return QSharedPointer<HSSObject>();
}

QSharedPointer<HSSObject> HSSFunctionCall::evaluate(QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSObject> errorState;

    if (theObj && theObj->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSValue> theValue = qSharedPointerCast<HSSValue>(theObj);
        QSharedPointer<HSSParserNode> valueNode = theValue->getValue();
        if (valueNode->isA(HSSFunctionTypeFunction))
        {
            QSharedPointer<HSSFunctionFunction> theFunc = qSharedPointerCast<HSSFunctionFunction>(valueNode);
            theFunc->setThisObj(this->getThisObj());
            theFunc->setScope(this->scope);
            return theFunc->evaluate(this->getArguments());
        }
    }
    return errorState;
}

void HSSFunctionCall::setThisObj(QSharedPointer<HSSDisplayObject> newThis)
{
    HSSParserNode::setThisObj(newThis);
    std::vector<QSharedPointer<HSSArgument> >::const_iterator it;
    if (this->_subject)
    {
        this->_subject->setThisObj(newThis);
    }
    for (it = this->_arguments.begin(); it != this->_arguments.end(); ++it)
    {
        const QSharedPointer<HSSParserNode> node = (*it)->node();
        node->setThisObj(newThis);
    }
}

void HSSFunctionCall::setArguments(std::vector<QSharedPointer<HSSArgument> > arguments)
{
    this->_arguments = arguments;
}

void HSSFunctionCall::addArgument(QSharedPointer<HSSArgument> argument)
{
    this->_arguments.push_back(argument);
}

std::vector<QSharedPointer<HSSArgument> > HSSFunctionCall::getArguments()
{
    return this->_arguments;
}

void HSSFunctionCall::setSubject(QSharedPointer<HSSParserNode> parserNode)
{
    this->_subject = parserNode;
}

const QSharedPointer<HSSParserNode> & HSSFunctionCall::subject() const
{
    return this->_subject;
}


