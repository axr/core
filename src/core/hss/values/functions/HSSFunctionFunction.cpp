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
#include "HSSFunctionFunction.h"

using namespace AXR;

HSSFunctionFunction::HSSFunctionFunction(AXRController * controller)
: HSSEvaluableFunction(HSSFunctionTypeFunction, controller)
{
    
}

HSSFunctionFunction::~HSSFunctionFunction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSFunctionFunction: destructing if function");
    this->cleanTrackedObservers();
}

HSSFunctionFunction::HSSFunctionFunction(const HSSFunctionFunction & orig)
: HSSEvaluableFunction(orig)
{
    this->_comparisonType = orig._comparisonType;
}

QSharedPointer<HSSFunction> HSSFunctionFunction::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSFunctionFunction::cloneImpl() const
{
    QSharedPointer<HSSFunctionFunction> clone = QSharedPointer<HSSFunctionFunction>(new HSSFunctionFunction(*this));
    this->cloneEvaluables(clone);
    std::vector<QSharedPointer<HSSObjectNameConstant> >::const_iterator it;
    for (it = this->_arguments.begin(); it != this->_arguments.end(); ++it)
    {
        clone->addArgument((*it)->clone());
    }
    return clone;
}

AXRString HSSFunctionFunction::toString()
{
    AXRString tempstr = AXRString("HSSFunctionFunction\n");
    return tempstr;
}

QSharedPointer<HSSObject> HSSFunctionFunction::_evaluate()
{
    this->evaluateEvaluables();
    this->setDirty(true);
    return this->_value;
}

void HSSFunctionFunction::addValue(QSharedPointer<HSSParserNode> parserNode)
{
    this->addEvaluable(parserNode);
}

void HSSFunctionFunction::setReturnValue(QSharedPointer<HSSObject> theObj)
{
    this->_value = theObj;
}

QSharedPointer<HSSObject> HSSFunctionFunction::evaluate(const std::vector<QSharedPointer<HSSArgument> > & arguments)
{
    size_t i, j;
    for (i=0, j=arguments.size(); i<j; ++i) {
        if (this->_arguments.size() > i) {
            AXRString argName = this->_arguments[i]->getValue();
            this->setLocalVariable(argName, HSSValue::valueFromParserNode(this->getController(), arguments[i]->evaluate(), 1, this->getThisObj(), this->scope));
        }
    }
    
    this->evaluateEvaluables();
    return this->_value;
}

void HSSFunctionFunction::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    HSSEvaluableFunction::setScope(newScope);
}

void HSSFunctionFunction::setThisObj(QSharedPointer<HSSDisplayObject> newThis)
{
    HSSEvaluableFunction::setThisObj(newThis);
}

void HSSFunctionFunction::setArguments(std::vector<QSharedPointer<HSSObjectNameConstant> > arguments)
{
    this->_arguments = arguments;
}

void HSSFunctionFunction::addArgument(QSharedPointer<HSSObjectNameConstant> argument)
{
    this->_arguments.push_back(argument);
}

std::vector<QSharedPointer<HSSObjectNameConstant> > HSSFunctionFunction::getArguments()
{
    return this->_arguments;
}
