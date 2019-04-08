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
#include "HSSCustomFunction.h"

using namespace AXR;

HSSCustomFunction::HSSCustomFunction(AXRController * controller)
: HSSFunction(HSSFunctionTypeCustom, controller)
{
    
}

HSSCustomFunction::HSSCustomFunction(const HSSCustomFunction & orig)
: HSSFunction(orig)
{
    
}

QSharedPointer<HSSFunction> HSSCustomFunction::clone() const
{
    return qSharedPointerCast<HSSCustomFunction> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSCustomFunction::cloneImpl() const
{
    QSharedPointer<HSSCustomFunction> clone = QSharedPointer<HSSCustomFunction>(new HSSCustomFunction(*this));
    
    for (std::vector<QSharedPointer<HSSArgument> >::const_iterator it = this->_arguments.begin(); it != this->_arguments.end(); ++it)
    {
        QSharedPointer<HSSArgument> clonedArgument = (*it)->clone();
        clone->_arguments.push_back(clonedArgument);
    }
    
    return clone;
}

HSSCustomFunction::~HSSCustomFunction()
{
}

AXRString HSSCustomFunction::toString()
{
    AXRString tempstr = AXRString("HSSCustomFunction\n");
    return tempstr;
}

bool HSSCustomFunction::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSCustomFunction> castedNode = qSharedPointerCast<HSSCustomFunction>(otherNode);
    //@todo compare arguments?
    return true;
}

QSharedPointer<HSSObject> HSSCustomFunction::_evaluate()
{
    AXRDocument* document = this->getController()->document();
    QSharedPointer<HSSMultipleValue> multiValue = QSharedPointer<HSSMultipleValue>(new HSSMultipleValue(this->getController()));
    const std::vector<QSharedPointer<HSSArgument> > & arguments = this->getArguments();
    std::vector<QSharedPointer<HSSArgument> >::const_iterator it;
    for (it = arguments.begin(); it != arguments.end(); ++it)
    {
        const QSharedPointer<HSSParserNode> & node = *it;
        multiValue->add(HSSValue::valueFromParserNode(this->getController(), node, 0, this->getThisObj(), this->scope));
    }
    multiValue->setThisObj(this->getThisObj());
    document->evaluateCustomFunction(this->getName(), multiValue);
    //next time we call this custom function we want to reevaluate
    this->setDirty(true);
    return QSharedPointer<HSSObject>();
}

void HSSCustomFunction::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    HSSFunction::setScope(newScope);
    this->scope = newScope;
    std::vector<QSharedPointer<HSSArgument> >::const_iterator it;
    for (it = this->_arguments.begin(); it != this->_arguments.end(); ++it)
    {
        const QSharedPointer<HSSParserNode> node = (*it);
        switch (node->getType())
        {
            case HSSParserNodeTypeFunction:
            {
                QSharedPointer<HSSCustomFunction> func = qSharedPointerCast<HSSCustomFunction > (node);
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

void HSSCustomFunction::setThisObj(QSharedPointer<HSSDisplayObject> newThis)
{
    HSSFunction::setThisObj(newThis);
    std::vector<QSharedPointer<HSSArgument> >::const_iterator it;
    for (it = this->_arguments.begin(); it != this->_arguments.end(); ++it)
    {
        const QSharedPointer<HSSParserNode> node = (*it);
        node->setThisObj(newThis);
    }
}

void HSSCustomFunction::setArguments(std::vector<QSharedPointer<HSSArgument> > arguments)
{
    this->_arguments = arguments;
}

void HSSCustomFunction::addArgument(QSharedPointer<HSSArgument> argument)
{
    this->_arguments.push_back(argument);
}

std::vector<QSharedPointer<HSSArgument> > HSSCustomFunction::getArguments()
{
    return this->_arguments;
}
