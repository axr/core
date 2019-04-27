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
#include "HSSEvaluableFunction.h"

using namespace AXR;

HSSEvaluableFunction::HSSEvaluableFunction(HSSFunctionType type, AXRController * controller)
: HSSFunction(type, controller)
{
    
}

HSSEvaluableFunction::HSSEvaluableFunction(const HSSEvaluableFunction & orig)
: HSSFunction(orig)
{
    
}

HSSEvaluableFunction::~HSSEvaluableFunction()
{
}

AXRString HSSEvaluableFunction::toString()
{
    AXRString tempstr = AXRString("HSSEvaluableFunction\n");
    return tempstr;
}

bool HSSEvaluableFunction::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    
    return true;
}

bool HSSEvaluableFunction::evaluateEvaluables()
{
    bool needsReturn = false;
    bool evaluateNextElseFunction = false;
    std::vector<QSharedPointer<HSSParserNode> >::const_iterator it;
    for (it = this->_evaluables.begin(); it != this->_evaluables.end(); ++it)
    {
        if (needsReturn)
            break;

        const QSharedPointer<HSSParserNode> & evaluable = *it;
        switch (evaluable->getType()) {
                
            case HSSParserNodeTypeFunction:
            {
                QSharedPointer<HSSFunction> theFunc = qSharedPointerCast<HSSFunction>(evaluable);
                switch (theFunc->getFunctionType())
                {
                    case HSSFunctionTypeIf:
                    {
                        QSharedPointer<HSSIfFunction> ifFunc = qSharedPointerCast<HSSIfFunction>(evaluable);
                        if (ifFunc->evaluateCondition())
                        {
                            needsReturn = ifFunc->evaluateEvaluables();
                        }
                        else
                        {
                            evaluateNextElseFunction = true;
                        }
                        break;
                    }
                    case HSSFunctionTypeElseIf:
                    {
                        if (evaluateNextElseFunction)
                        {
                            QSharedPointer<HSSIfFunction> elIfFunc = qSharedPointerCast<HSSIfFunction>(evaluable);
                            if (elIfFunc->evaluateCondition())
                            {
                                evaluateNextElseFunction = false;
                                needsReturn = elIfFunc->evaluateEvaluables();
                            }
                        }
                        break;
                    }
                    case HSSFunctionTypeElse:
                    {
                        if (evaluateNextElseFunction)
                        {
                            QSharedPointer<HSSIfFunction> elseFunc = qSharedPointerCast<HSSIfFunction>(evaluable);
                            evaluateNextElseFunction = false;
                            needsReturn = elseFunc->evaluateEvaluables();
                        }
                        break;
                    }
                    case HSSFunctionTypeReturn:
                    {
                        theFunc->evaluate();
                        needsReturn = true;
                        break;
                    }
                    default:
                        theFunc->evaluate();
                        break;
                }
                break;
            }
            case HSSParserNodeTypePropertyPath:
            {
                QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath>(evaluable);
                ppath->evaluate();
                break;
            }
            case HSSParserNodeTypeStatement:
            {
                switch (evaluable->getStatementType())
                {
                    case HSSStatementTypeVarDeclaration:
                    {
                        QSharedPointer<HSSVarDeclaration> varDecl = qSharedPointerCast<HSSVarDeclaration>(evaluable);
                        if (varDecl->assignment())
                        {
                            this->setLocalVariable(varDecl->name(), varDecl->assignment()->evaluate());
                        }
                        else
                        {
                            this->setLocalVariable(varDecl->name(), QSharedPointer<AXR::HSSObject>(new HSSObject(HSSObjectTypeNull, this->getController())));
                        }
                        
                        break;
                    }
                    case HSSStatementTypeAssignment:
                    {
                        QSharedPointer<HSSAssignment> theAsgmt = qSharedPointerCast<HSSAssignment>(evaluable);
                        QSharedPointer<HSSPropertyPath> ppath = theAsgmt->propertyPath();
                        ppath->evaluateSet(theAsgmt->evaluate());
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
                
            default:
            {
                AXRWarning("HSSEvaluableFunction", "Ignoring evaluable of unknown type").raise();
                break;
            }
        }
    }
    return needsReturn;
}

void HSSEvaluableFunction::addEvaluable(QSharedPointer<HSSParserNode> parserNode)
{
    parserNode->setParentNode(this->shared_from_this());
    this->_evaluables.push_back(parserNode);
}

const std::vector<QSharedPointer<HSSParserNode> > HSSEvaluableFunction::getEvaluables()
{
    return this->_evaluables;
}

void HSSEvaluableFunction::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    HSSFunction::setScope(newScope);
    //propagate values
    std::vector<QSharedPointer<HSSParserNode> >::iterator it;
    for (it = this->_evaluables.begin(); it != this->_evaluables.end(); ++it)
    {
        QSharedPointer<HSSParserNode> evaluable = *it;
        switch (evaluable->getType())
        {
            case HSSParserNodeTypeExpression:
                qSharedPointerCast<HSSExpression>(evaluable)->setScope(newScope);
                break;
            case HSSParserNodeTypeFunction:
                qSharedPointerCast<HSSFunction>(evaluable)->setScope(newScope);
                break;
            case HSSParserNodeTypePropertyPath:
                qSharedPointerCast<HSSPropertyPath>(evaluable)->setScope(newScope);
                break;
            default:
                break;
        }
    }
}

void HSSEvaluableFunction::setThisObj(QSharedPointer<HSSDisplayObject> newThis)
{
    HSSFunction::setThisObj(newThis);
    std::vector<QSharedPointer<HSSParserNode> >::iterator it;
    for (it = this->_evaluables.begin(); it != this->_evaluables.end(); ++it)
    {
        (*it)->setThisObj(newThis);
    }
}

void HSSEvaluableFunction::cloneEvaluables(QSharedPointer<HSSEvaluableFunction> clone) const
{
    std::vector<QSharedPointer<HSSParserNode> >::const_iterator evalsIt;
    for (evalsIt = this->_evaluables.begin(); evalsIt != this->_evaluables.end(); ++evalsIt)
    {
        clone->addEvaluable((*evalsIt)->clone());
    }
}

void HSSEvaluableFunction::setLocalVariable(AXRString name, QSharedPointer<HSSObject> value)
{
    if (value)
    {
        this->_locals[name] = value;
    }
    else
    {
        int testvar = 1;
        testvar += 1;
    }
}

QSharedPointer<HSSObject> HSSEvaluableFunction::getLocalVariable(AXRString name)
{
    return this->_locals[name];
}

QSharedPointer<HSSObject> HSSEvaluableFunction::getVariable(AXRString name)
{
    //return local if there
    if (this->_locals.count(name))
    {
        return this->_locals[name];
    }
    return QSharedPointer<HSSObject>();
}

bool HSSEvaluableFunction::hasLocalVariable(AXR::HSSString name)
{
    return this->_locals.count(name) > 0;
}
