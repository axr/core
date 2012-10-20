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
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#include "AXRDebugging.h"
#include "AXRDocument.h"
#include "HSSExpression.h"
#include "HSSFunction.h"

using namespace AXR;

HSSFunction::HSSFunction(HSSFunctionType type, AXRController * controller)
: HSSParserNode(HSSParserNodeTypeFunctionCall, controller)
{
    this->functionType = type;
    this->scope = NULL;
    this->percentageObserved = NULL;
    this->_isDirty = true;
    this->_value = QVariant();
}

HSSFunction::HSSFunction(const HSSFunction & orig)
: HSSParserNode(orig)
{
    this->functionType = orig.functionType;
    this->scope = NULL;
    this->percentageObserved = NULL;
    this->_isDirty = orig._isDirty;
    this->_value = orig._value;
    this->_name = orig._name;
}

HSSFunction::p HSSFunction::clone() const
{
    return qSharedPointerCast<HSSFunction, HSSClonable > (this->cloneImpl());
}

HSSClonable::p HSSFunction::cloneImpl() const
{
    HSSFunction::p clone = HSSFunction::p(new HSSFunction(*this));

    for (std::deque<HSSParserNode::p>::const_iterator it = this->_arguments.begin(); it != this->_arguments.end(); ++it)
    {
        HSSParserNode::p clonedArgument = (*it)->clone();
        clone->_arguments.push_back(clonedArgument);
    }

    return clone;
}

HSSFunction::~HSSFunction()
{
    //    if (this->percentageObserved) {
    //        this->percentageObserved->removeObserver(this->percentageObservedProperty, HSSObservablePropertyValue, this);
    //        this->percentageObserved = NULL;
    //    }
}

AXRString HSSFunction::toString()
{
    AXRString tempstr = AXRString("HSSFunction\n");
    return tempstr;
}

QVariant HSSFunction::evaluate()
{
    if (this->_isDirty)
    {
        this->_isDirty = false;

        this->_value = this->_evaluate();
    }

    return this->_value;
}

QVariant HSSFunction::evaluate(std::deque<HSSParserNode::p> arguments)
{
    if (this->_isDirty)
    {
        this->_isDirty = false;

        this->_value = this->_evaluate(arguments);
    }

    return this->_value;
}

QVariant HSSFunction::_evaluate()
{
    return this->_evaluate(std::deque<HSSParserNode::p > ());
}

QVariant HSSFunction::_evaluate(std::deque<HSSParserNode::p> arguments)
{
    AXRDocument* document = AXRDocument::getInstance();
    document->evaluateCustomFunction(this->getName(), (void*) &arguments);
    return QVariant();
}

void HSSFunction::propertyChanged(HSSObservableProperty property, void* data)
{
    this->notifyObservers(HSSObservablePropertyValue, data);
}

void HSSFunction::setPercentageBase(HSSUnit value)
{
    this->percentageBase = value;
}

void HSSFunction::setPercentageObserved(HSSObservableProperty property, HSSObservable *observed)
{
    //    if(this->percentageObserved)
    //    {
    //        this->percentageObserved->removeObserver(this->percentageObservedProperty, HSSObservablePropertyValue, this);
    //        this->percentageObserved = NULL;
    //    }
    this->percentageObservedProperty = property;
    this->percentageObserved = observed;
    //observed->observe(property, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSFunction>(this, &HSSFunction::propertyChanged));
}

void HSSFunction::setScope(const std::vector<HSSDisplayObject::p> * newScope)
{
    this->scope = newScope;
    std::deque<HSSParserNode::p>::const_iterator it;
    for (it = this->_arguments.begin(); it != this->_arguments.end(); ++it)
    {
        const HSSParserNode::p node = (*it);
        switch (node->getType())
        {
        case HSSParserNodeTypeFunctionCall:
        {
            HSSFunction::p func = qSharedPointerCast<HSSFunction > (node);
            func->setScope(newScope);
            break;
        }

        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p exp = qSharedPointerCast<HSSExpression > (node);
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
            HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (node);
            objdef->setScope(newScope);
            break;
        }

        default:
            break;
        }
    }
}

void HSSFunction::setDirty(bool value)
{
    this->_isDirty = value;
}

bool HSSFunction::isDirty()
{
    return this->_isDirty;
}

QVariant HSSFunction::getValue()
{
    return this->_value;
}

bool HSSFunction::isA(HSSFunctionType type)
{
    return this->functionType == type;
}

HSSFunctionType HSSFunction::getFunctionType()
{
    return this->functionType;
}

void HSSFunction::setArguments(std::deque<HSSParserNode::p> arguments)
{
    this->_arguments = arguments;
}

std::deque<HSSParserNode::p> HSSFunction::getArguments()
{
    return this->_arguments;
}

void HSSFunction::setName(AXRString newName)
{
    this->_name = newName;
}

AXRString HSSFunction::getName()
{
    return this->_name;
}
