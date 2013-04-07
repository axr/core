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
#include "AXRDocument.h"
#include "HSSContainer.h"
#include "HSSDisplayObject.h"
#include "HSSFlag.h"
#include "HSSInstruction.h"
#include "HSSMultipleSelection.h"
#include "HSSParserNode.h"
#include "HSSRule.h"
#include "HSSSelection.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelection.h"
#include "HSSStatement.h"

using namespace AXR;

HSSFlag::HSSFlag(AXRController * controller)
: HSSFilter(HSSFilterTypeFlag, controller)
{
    this->_purging = HSSRuleStateOff;
}

QSharedPointer<HSSFilter> HSSFlag::clone() const
{
    return qSharedPointerCast<HSSFlag> (this->cloneImpl());
}

HSSFlag::HSSFlag(const HSSFlag &orig)
: HSSFilter(orig)
{
    this->_name = orig._name;
}

HSSFlag::~HSSFlag()
{
}

AXRString HSSFlag::toString()
{
    return "Flag with name " + this->_name;
}

AXRString HSSFlag::stringRep()
{
    return AXRString("::").append(this->_name);
}

AXRString HSSFlag::getName()
{
    return this->_name;
}

void HSSFlag::setName(AXRString newValue)
{
    this->_name = newValue;
}

void HSSFlag::flagChanged(HSSRuleState newStatus)
{
    //parent is simple selector, grandparent is selector chain, grandgrandparent is the rule
    QSharedPointer<HSSParserNode> selectorChainNode = this->getParentNode()->getParentNode();
    QSharedPointer<HSSParserNode> ruleNode = selectorChainNode->getParentNode();
    if (ruleNode->isA(HSSParserNodeTypeStatement))
    {
        QSharedPointer<HSSStatement> ruleStatement = qSharedPointerCast<HSSStatement > (ruleNode);
        if (ruleStatement->isA(HSSStatementTypeRule))
        {
            QSharedPointer<HSSRule> theRule = qSharedPointerCast<HSSRule > (ruleStatement);
            this->setPurging(newStatus);
            if (this->getNegating()) {
                if (newStatus == HSSRuleStateActivate)
                {
                    newStatus = HSSRuleStatePurge;
                }
                else if (newStatus == HSSRuleStatePurge)
                {
                    newStatus = HSSRuleStateActivate;
                }
            }
            if(newStatus != HSSRuleStateActivate || !theRule->hasParent())
            {
                this->getController()->recursiveSetRuleState(theRule, theRule->getOriginalScope(), this->getThisObj(), newStatus);
            } else {
                this->getController()->setRuleStateOnSelection(theRule, this->_selectFromTop(theRule), newStatus);
            }
            this->setPurging(HSSRuleStateOff);
        }
    }
}

QSharedPointer<HSSSimpleSelection> HSSFlag::_selectFromTop(QSharedPointer<HSSRule> theRule)
{
    //climb the tree
    QSharedPointer<HSSRule> rule = theRule;
    std::deque<QSharedPointer<HSSRule> > rules;
    while (rule->hasParent())
    {
        rules.push_front(rule);
        rule = rule->getParent();
    }
    rules.push_front(rule);
    //select from the topmost rule down to our subjects
    std::deque<QSharedPointer<HSSRule> >::iterator it = rules.begin();
    QSharedPointer<HSSRule> topRule = *it;
    QSharedPointer<HSSContainer> thisObj = this->getController()->getRoot();
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
    QSharedPointer<HSSSimpleSelection> scope = topRule->getOriginalScope();
    QSharedPointer<HSSSimpleSelection> selection;
    for (; it != rules.end(); ++it)
    {
        QSharedPointer<HSSRule> itRule = *it;
        // TODO: check @this object
        QSharedPointer<HSSInstruction> instruction = itRule->getInstruction();
        if (instruction) {
            selection = QSharedPointer<HSSSimpleSelection>(new HSSSimpleSelection(itRule->getAppliedTo()));
        } else {
            selection = this->getController()->select(itRule->getSelectorChains(), scope, itRule->getThisObj())->joinAll();
        }
        if(selection->size() == 0)
        {
            break;
        }
        if ((it != rules.end()) && (it+1 != rules.end()))
        {
            QSharedPointer<HSSSimpleSelection> nextLevel(new HSSSimpleSelection());
            for (HSSSimpleSelection::const_iterator it2 = selection->begin(); it2 != selection->end(); ++it2)
            {
                const QSharedPointer<HSSDisplayObject> & displayObject = *it2;
                //if it is a container it may have children
                if (displayObject->isA(HSSObjectTypeContainer))
                {
                    QSharedPointer<HSSContainer> selectedContainer = qSharedPointerCast<HSSContainer>(displayObject);
                    QSharedPointer<HSSSimpleSelection> children = selectedContainer->getChildren();
                    if(children)
                    {
                        nextLevel->addSelection(children);
                    }
                }
            }
            scope = nextLevel;
        }
    }

    return selection;
}

QSharedPointer<HSSSelection> HSSFlag::apply(QSharedPointer<HSSSelection> scope, bool processing)
{
    if (processing)
    {
        QSharedPointer<HSSSimpleSelection> inner = scope->joinAll();
        for (HSSSimpleSelection::const_iterator it = inner->begin(); it != inner->end(); ++it)
        {
            const QSharedPointer<HSSDisplayObject> & theDO = *it;
            //parent is simple selector, grandparent is selector chain, grandgrandparent is the rule
            QSharedPointer<HSSParserNode> ruleNode = this->getParentNode()->getParentNode()->getParentNode();
            if (ruleNode->isA(HSSParserNodeTypeStatement))
            {
                QSharedPointer<HSSStatement> ruleStatement = qSharedPointerCast<HSSStatement > (ruleNode);
                if (ruleStatement->isA(HSSStatementTypeRule))
                {
                    QSharedPointer<HSSRule> theRule = qSharedPointerCast<HSSRule > (ruleStatement);

                    theDO->createFlag(this->shared_from_this(), HSSRuleStateOff);
                }
            }
        }

        return scope->splitAll();
    }
    else
    {
        QSharedPointer<HSSSimpleSelection> ret(new HSSSimpleSelection());
        QSharedPointer<HSSSimpleSelection> inner = scope->joinAll();
        for (HSSSimpleSelection::const_iterator it = inner->begin(); it != inner->end(); ++it)
        {
            const QSharedPointer<HSSDisplayObject> & theDO = *it;
            HSSRuleState purgingState = this->getPurging();
            if (purgingState)
            {
                HSSRuleState state = theDO->flagState(this->getName());
                if (state == purgingState)
                {
                    ret->add(theDO);
                }
            }
            else
            {
                bool firstMatch = (theDO->flagState(this->getName()) == HSSRuleStateOn) && !this->getNegating();
                bool secondMatch = (theDO->flagState(this->getName()) == HSSRuleStateOff) && this->getNegating();
                if (firstMatch || secondMatch)
                {
                    ret->add(theDO);
                }
            }
        }
        return ret;
    }
    return scope;
}

HSSRuleState HSSFlag::getPurging()
{
    return this->_purging;
}

void HSSFlag::setPurging(HSSRuleState newValue)
{
    this->_purging = newValue;
}

QSharedPointer<HSSClonable> HSSFlag::cloneImpl() const
{
    return QSharedPointer<HSSFlag>(new HSSFlag(*this));
}

QSharedPointer<HSSFlag> HSSFlag::shared_from_this()
{
    return qSharedPointerCast<HSSFlag > (HSSParserNode::shared_from_this());
}
