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
#include "HSSTargetedFunction.h"

using namespace AXR;

HSSTargetedFunction::HSSTargetedFunction(HSSFunctionType type, AXRController * controller)
: HSSFunction(type, controller)
{
}

HSSTargetedFunction::HSSTargetedFunction(const HSSTargetedFunction & orig)
: HSSFunction(orig)
{
    
}

QSharedPointer<HSSFunction> HSSTargetedFunction::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

HSSTargetedFunction::~HSSTargetedFunction()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSTargetedFunction: destructing targeted function");
    this->cleanTrackedObservers();
}

bool HSSTargetedFunction::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSFunction::equalTo(otherNode)) return false;
    QSharedPointer<HSSTargetedFunction> castedNode = qSharedPointerCast<HSSTargetedFunction>(otherNode);
    unsigned i = 0;
    Q_FOREACH(QSharedPointer<HSSSelectorChain> selectorChain, this->selectorChains)
    {
        if ( ! selectorChain->equalTo(castedNode->selectorChains[i])) return false;
        ++i;
    }
    return true;
}

const std::vector<QSharedPointer<HSSSelectorChain> > & HSSTargetedFunction::getSelectorChains() const
{
    return this->selectorChains;
}

void HSSTargetedFunction::setSelectorChains(std::vector<QSharedPointer<HSSSelectorChain> > newValues)
{
    this->selectorChains = newValues;
    this->setDirty(true);
    for (HSSSelectorChain::it it = this->selectorChains.begin(); it != this->selectorChains.end(); ++it)
    {
        (*it)->setParentNode(this->shared_from_this());
    }
}

void HSSTargetedFunction::selectorChainsAdd(QSharedPointer<HSSSelectorChain> & newSelectorChain)
{
    if (newSelectorChain)
    {
        newSelectorChain->setParentNode(this->shared_from_this());
        this->selectorChains.push_back(newSelectorChain);
    }
}

void HSSTargetedFunction::selectorChainsRemove(unsigned int index)
{
    this->selectorChains.erase(this->selectorChains.begin() + index);
}

void HSSTargetedFunction::selectorChainsRemoveLast()
{
    this->selectorChains.pop_back();
}

QSharedPointer<HSSSelectorChain> & HSSTargetedFunction::selectorChainsGet(unsigned index)
{
    return this->selectorChains[index];
}

QSharedPointer<HSSSelectorChain> & HSSTargetedFunction::selectorChainsLast()
{
    return this->selectorChains.back();
}

size_t HSSTargetedFunction::selectorChainsSize() const
{
    return this->selectorChains.size();
}

QSharedPointer<HSSClonable> HSSTargetedFunction::cloneImpl() const
{
    QSharedPointer<HSSTargetedFunction> clone = QSharedPointer<HSSTargetedFunction>(new HSSTargetedFunction(*this));
    this->cloneSelectorChains(clone);
    
    return clone;
}

void HSSTargetedFunction::cloneSelectorChains(const QSharedPointer<HSSTargetedFunction> & clone) const
{
    for (HSSSelectorChain::const_it sIt = this->selectorChains.begin(); sIt != this->selectorChains.end(); ++sIt)
    {
        QSharedPointer<HSSSelectorChain> clonedSelectorChain = (*sIt)->clone();
        clone->selectorChainsAdd(clonedSelectorChain);
    }
}
