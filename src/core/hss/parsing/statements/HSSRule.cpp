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

#include "AXRError.h"
#include "AXRController.h"
#include "AXRLoggerManager.h"
#include "HSSContainer.h"
#include "HSSDisplayObject.h"
#include "HSSFilter.h"
#include "HSSInstruction.h"
#include "HSSPropertyDefinition.h"
#include "HSSRule.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

HSSRule::HSSRule(AXRController * controller)
: HSSStatement(HSSStatementTypeRule, controller)
{
    this->_clonedFromRule = NULL;
}

HSSRule::HSSRule(const HSSRule & orig)
: HSSStatement(orig)
{
    this->_originalScope = orig._originalScope;
    this->_clonedFromRule = &orig;
}

QSharedPointer<HSSRule> HSSRule::clone() const
{
    QSharedPointer<HSSRule> clone = qSharedPointerCast<HSSRule>(this->cloneImpl());
    if (this->instruction)
        clone->instruction = this->instruction->clone();
    return clone;
}

HSSRule::~HSSRule()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRule: destructing rule");
    this->cleanTrackedObservers();
    for (unsigned i = 0; i<this->properties.size(); ++i)
    {
        this->propertiesRemoveLast();
    }
}

AXRString HSSRule::toString()
{
    AXRString tempstr = AXRString("HSSRule with the following selector chain: \n");
    if (this->selectorChains.size() > 0)
    {
        for (HSSSelectorChain::const_it sIt = this->selectorChains.begin(); sIt != this->selectorChains.end(); ++sIt)
        {
            tempstr.append("   ").append((*sIt)->toString());
        }
    }
    else
    {
        tempstr.append("WARNING: this rule has no selector chain!\n");
    }

    const size_t pccount = this->propertiesSize();
    if (pccount > 0)
    {
        tempstr.append(" and the following properties: \n");

        for (size_t j = 0; j < pccount; ++j)
        {
            tempstr.append("   ").append(this->properties[j]->toString()).append("\n");
        }
    }

    const size_t srcount = this->children.size();
    if (srcount > 0)
    {
        tempstr.append(" with the following sub-rules: \n");

        for (size_t k = 0; k < srcount; ++k)
        {
            tempstr.append("    ").append(this->children[k]->toString()).append("\n");
        }
    }

    return tempstr;
}

void HSSRule::setSelectorChains(std::vector<QSharedPointer<HSSSelectorChain> > newChains)
{
    this->selectorChains = newChains;

    for (HSSSelectorChain::it it = this->selectorChains.begin(); it != this->selectorChains.end(); ++it)
    {
        (*it)->setParentNode(this->shared_from_this());
    }
}

const std::vector<QSharedPointer<HSSSelectorChain> > & HSSRule::getSelectorChains() const
{
    return this->selectorChains;
}

void HSSRule::selectorChainsAdd(QSharedPointer<HSSSelectorChain> & newSelectorChain)
{
    if (newSelectorChain)
    {
        newSelectorChain->setParentNode(this->shared_from_this());
        this->selectorChains.push_back(newSelectorChain);
    }
}

void HSSRule::selectorChainsRemove(off_t index)
{
    this->selectorChains.erase(this->selectorChains.begin() + index);
}

void HSSRule::selectorChainsRemoveLast()
{
    this->selectorChains.pop_back();
}

QSharedPointer<HSSSelectorChain> & HSSRule::selectorChainsGet(size_t index)
{
    return this->selectorChains[index];
}

QSharedPointer<HSSSelectorChain> & HSSRule::selectorChainsLast()
{
    return this->selectorChains.back();
}

size_t HSSRule::selectorChainsSize() const
{
    return this->selectorChains.size();
}

const std::vector<QSharedPointer<HSSPropertyDefinition> > & HSSRule::getProperties() const
{
    return this->properties;
}

void HSSRule::propertiesAdd(QSharedPointer<HSSPropertyDefinition> & newProperty)
{
    if (newProperty)
    {
        newProperty->setParentNode(this->shared_from_this());
        this->properties.push_back(newProperty);
    }
}

void HSSRule::propertiesRemove(off_t index)
{
    this->properties.erase(this->properties.begin() + index);
}

void HSSRule::propertiesRemoveLast()
{
    this->properties.pop_back();
}

QSharedPointer<HSSPropertyDefinition> & HSSRule::propertiesGet(size_t index)
{
    return this->properties[index];
}

QSharedPointer<HSSPropertyDefinition> & HSSRule::propertiesLast()
{
    return this->properties.back();
}

size_t HSSRule::propertiesSize() const
{
    return this->properties.size();
}

void HSSRule::childrenAdd(QSharedPointer<HSSRule> newRule)
{
    newRule->setParentNode(this->shared_from_this()); //parent in the node tree
    this->children.push_back(newRule);
}

void HSSRule::childrenPrepend(QSharedPointer<HSSRule> newRule)
{
    newRule->setParentNode(this->shared_from_this()); //parent in the node tree
    this->children.push_front(newRule);
}

void HSSRule::childrenPrependOnce(QSharedPointer<HSSRule> newRule)
{
    bool found = false;
    std::deque<QSharedPointer<HSSRule> >::iterator it;
    for (it = this->children.begin(); it != this->children.end(); ++it)
    {
        QSharedPointer<HSSRule> existingRule = *it;
        if (newRule == existingRule || existingRule->clonedFromSameRule(newRule))
        {
            found = true;
        }
    }
    if(!found){
        this->childrenPrepend(newRule);
    }
}

QSharedPointer<HSSRule> HSSRule::childrenGet(size_t index)
{
    return this->children[index];
}

void HSSRule::childrenRemove(off_t index)
{
    this->children.erase(this->children.begin() + index);
}

void HSSRule::childrenRemoveLast()
{
    this->children.pop_back();
}

size_t HSSRule::childrenSize() const
{
    return this->children.size();
}

void HSSRule::setInstruction(QSharedPointer<HSSInstruction> newInstruction)
{
    this->instruction = newInstruction;
    this->instruction->setParentNode(this->shared_from_this());
}

QSharedPointer<HSSInstruction> HSSRule::getInstruction()
{
    return this->instruction;
}

QSharedPointer<HSSRule> HSSRule::shared_from_this()
{
    return qSharedPointerCast<HSSRule > (HSSStatement::shared_from_this());
}

void HSSRule::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    for (HSSSelectorChain::const_it sIt = this->selectorChains.begin(); sIt != this->selectorChains.end(); ++sIt)
    {
        (*sIt)->setThisObj(value);
    }

    for (std::vector<QSharedPointer<HSSPropertyDefinition> >::iterator it = this->properties.begin(); it != this->properties.end(); ++it)
    {
        (*it)->setThisObj(value);
    }

    //    for (std::vector<QSharedPointer<HSSRule>>::iterator it2 = this->children.begin(); it2 != this->children.end(); ++it2) {
    //        (*it2)->setThisObj(value);
    //    }

    HSSStatement::setThisObj(value);
}

void HSSRule::treeChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSDisplayObject> thisObj = this->getThisObj();
    AXRController * theController = thisObj->getController();
    if (thisObj->isA(HSSObjectTypeContainer))
    {
        QSharedPointer<HSSContainer> thisContainer = HSSContainer::asContainer(thisObj);
        theController->recursiveMatchRulesToDisplayObjects(this->shared_from_this(), thisContainer->getChildren(), thisContainer, false);
    }
}

const std::vector<QWeakPointer<HSSDisplayObject> > HSSRule::getAppliedTo() const
{
    return this->appliedTo;
}

void HSSRule::setAppliedTo(std::vector<QWeakPointer<HSSDisplayObject> > newObjects)
{
    this->appliedTo = newObjects;
}

void HSSRule::appliedToAdd(QSharedPointer<HSSDisplayObject> displayObject)
{
    this->appliedTo.push_back(QWeakPointer<HSSDisplayObject > (displayObject));
}

QSharedPointer<HSSClonable> HSSRule::cloneImpl() const
{
    QSharedPointer<HSSRule> clone = QSharedPointer<HSSRule>(new HSSRule(*this));

    for (HSSSelectorChain::const_it sIt = this->selectorChains.begin(); sIt != this->selectorChains.end(); ++sIt)
    {
        QSharedPointer<HSSSelectorChain> clonedSelectorChain = (*sIt)->clone();
        clone->selectorChainsAdd(clonedSelectorChain);
    }

    for (HSSPropertyDefinition::const_it pIt = this->properties.begin(); pIt != this->properties.end(); ++pIt)
    {
        QSharedPointer<HSSPropertyDefinition> clonedPropDef = (*pIt)->clone();
        clone->propertiesAdd(clonedPropDef);
    }

    for (std::deque<QSharedPointer<HSSRule> >::const_iterator rIt = this->children.begin(); rIt != this->children.end(); ++rIt)
    {
        QSharedPointer<HSSRule> clonedRule = (*rIt)->clone();
        clone->childrenAdd(clonedRule);
    }

    if (this->instruction)
        clone->setInstruction(this->instruction->clone());

    return clone;
}

QSharedPointer<HSSSimpleSelection> HSSRule::getOriginalScope() const
{
    return this->_originalScope;
}

void HSSRule::setOriginalScope(QSharedPointer<HSSSimpleSelection> scope)
{
    this->_originalScope = scope;
}

void HSSRule::addOriginalScope(QSharedPointer<HSSSimpleSelection> scope)
{
    if(this->_originalScope == NULL)
    {
        this->_originalScope = scope;
    } else
    {
        this->_originalScope->addSelection(scope);
    }
}

bool HSSRule::hasParent()
{
    return this->getParentNode() != NULL;
}

QSharedPointer<HSSRule> HSSRule::getParent()
{
    return qSharedPointerCast<HSSRule>(this->getParentNode());
}

const bool HSSRule::clonedFromSameRule(const QSharedPointer<HSSRule> & otherRule) const
{
    if(this->_clonedFromRule != NULL)
    {
        return this->_clonedFromRule == otherRule->_clonedFromRule;
    }
    return false;
}
void HSSRule::removeFromDisplayObjects()
{
    Q_FOREACH(QWeakPointer<HSSDisplayObject> weakDO, this->appliedTo)
    {
        QSharedPointer<HSSDisplayObject> theDO = weakDO.toStrongRef();
        AXRError("beforeRemoving", QString("Removing rule from ")+theDO->getElementName()+QString(", rules size: ")+QString::number(theDO->rulesSize())).raise();
        for (int i = 0, size = theDO->rulesSize(); i<size; ++i)
        {
            QSharedPointer<HSSRule> remoteRule = theDO->rulesGet(i);
            if (this->clonedFromSameRule(remoteRule))
            {
                theDO->rulesRemove(i);
                --i;
                --size;
                if (this->instruction)
                {
                    switch (this->instruction->getInstructionType())
                    {
                        case HSSNewInstruction:
                        {
                            theDO->removeFromParent();
                            break;
                        }

                        default:
                            break;
                    }
                }

            }
        }
    }
}
