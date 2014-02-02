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

#include "AXRController.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSDisplayObject.h"
#include "HSSFlagFunction.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

HSSFlagFunctionType HSSFlagFunction::flagFunctionTypeFromString(AXRString name)
{
    if (name == "flag")
    {
        return HSSFlagFunctionTypeFlag;
    }
    if (name == "unflag")
    {
        return HSSFlagFunctionTypeUnflag;
    }
    if (name == "toggleFlag")
    {
        return HSSFlagFunctionTypeToggleFlag;
    }
    if (name == "takeFlag")
    {
        return HSSFlagFunctionTypeTakeFlag;
    }

    return HSSFlagFunctionTypeNone;
}

HSSFlagFunction::HSSFlagFunction(HSSFlagFunctionType flagFunctionType, AXRController * controller)
: HSSFunction(HSSFunctionTypeFlag, controller)
{
    this->_flagFunctionType = flagFunctionType;
}

HSSFlagFunction::HSSFlagFunction(const HSSFlagFunction & orig)
: HSSFunction(orig)
{
    this->_name = orig._name;
    this->_flagFunctionType = orig._flagFunctionType;
}

QSharedPointer<HSSFunction> HSSFlagFunction::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

HSSFlagFunction::~HSSFlagFunction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSFlagFunction: destructing flag function");
    this->cleanTrackedObservers();
}

bool HSSFlagFunction::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSFunction::equalTo(otherNode)) return false;
    QSharedPointer<HSSFlagFunction> castedNode = qSharedPointerCast<HSSFlagFunction>(otherNode);
    if ( this->_flagFunctionType != castedNode->_flagFunctionType ) return false;
    unsigned i = 0;
    Q_FOREACH(QSharedPointer<HSSSelectorChain> selectorChain, this->selectorChains)
    {
        if ( ! selectorChain->equalTo(castedNode->selectorChains[i])) return false;
        ++i;
    }
    return true;
}

const AXRString & HSSFlagFunction::getName()
{
    return this->_name;
}

void HSSFlagFunction::setName(AXRString newValue)
{
    this->_name = newValue;
    this->setDirty(true);
}

const std::vector<QSharedPointer<HSSSelectorChain> > & HSSFlagFunction::getSelectorChains() const
{
    return this->selectorChains;
}

void HSSFlagFunction::setSelectorChains(std::vector<QSharedPointer<HSSSelectorChain> > newValues)
{
    this->selectorChains = newValues;
    this->setDirty(true);
}

void HSSFlagFunction::selectorChainsAdd(QSharedPointer<HSSSelectorChain> & newSelectorChain)
{
    if (newSelectorChain)
    {
        newSelectorChain->setParentNode(this->shared_from_this());
        this->selectorChains.push_back(newSelectorChain);
    }
}

void HSSFlagFunction::selectorChainsRemove(unsigned int index)
{
    this->selectorChains.erase(this->selectorChains.begin() + index);
}

void HSSFlagFunction::selectorChainsRemoveLast()
{
    this->selectorChains.pop_back();
}

QSharedPointer<HSSSelectorChain> & HSSFlagFunction::selectorChainsGet(unsigned index)
{
    return this->selectorChains[index];
}

QSharedPointer<HSSSelectorChain> & HSSFlagFunction::selectorChainsLast()
{
    return this->selectorChains.back();
}

size_t HSSFlagFunction::selectorChainsSize() const
{
    return this->selectorChains.size();
}

QSharedPointer<HSSObject> HSSFlagFunction::_evaluate()
{
    QSharedPointer<HSSDisplayObject> thisObj = this->getThisObj();
    QSharedPointer<HSSSelection> selection = this->getController()->select(this->getSelectorChains(), this->scope, thisObj);
    QSharedPointer<HSSSimpleSelection> inner = selection->joinAll();
    HSSFlagFunctionType type = this->getFlagFunctionType();
    const AXRString & flagName = this->getName();
    if(type == HSSFlagFunctionTypeTakeFlag){
        for (HSSSimpleSelection::iterator innerIt = inner->begin(); innerIt != inner->end(); ++innerIt)
        {
            QSharedPointer<HSSDisplayObject> container = *innerIt;
            if(container != thisObj)
            {
                container->flagsDeactivate(flagName);
            }
        }
        thisObj->flagsActivate(flagName);
    }
    else
    {
        for (HSSSimpleSelection::iterator innerIt = inner->begin(); innerIt != inner->end(); ++innerIt)
        {
            QSharedPointer<HSSDisplayObject> container = *innerIt;
            switch (this->getFlagFunctionType())
            {
                case HSSFlagFunctionTypeFlag:
                    container->flagsActivate(flagName);
                    break;
                case HSSFlagFunctionTypeUnflag:
                    container->flagsDeactivate(flagName);
                    break;
                case HSSFlagFunctionTypeToggleFlag:
                    container->flagsToggle(flagName);
                    break;

                default:
                    throw AXRWarning("HSSFlagAction", "Invalid flag function type for flag action");
                    break;
            }
        }
    }
    //reset dirty to always re evaluate
    this->setDirty(true);
    return QSharedPointer<HSSObject>();
}

QSharedPointer<HSSObject> HSSFlagFunction::_evaluate(std::deque<QSharedPointer<HSSParserNode> > arguments)
{
    return this->_evaluate();
}

void HSSFlagFunction::valueChanged(const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->setDirty(true);
    this->_value = theObj;
    this->notifyObservers("__impl_private__value", theObj);
}

HSSFlagFunctionType HSSFlagFunction::getFlagFunctionType() const
{
    return this->_flagFunctionType;
}

QSharedPointer<HSSClonable> HSSFlagFunction::cloneImpl() const
{
    QSharedPointer<HSSFlagFunction> clone = QSharedPointer<HSSFlagFunction>(new HSSFlagFunction(*this));

    for (HSSSelectorChain::const_it sIt = this->selectorChains.begin(); sIt != this->selectorChains.end(); ++sIt)
    {
        QSharedPointer<HSSSelectorChain> clonedSelectorChain = (*sIt)->clone();
        clone->selectorChainsAdd(clonedSelectorChain);
    }

    return clone;
}
