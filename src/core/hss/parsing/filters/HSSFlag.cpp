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

#include "AXRController.h"
#include "AXRDocument.h"
#include "HSSDisplayObject.h"
#include "HSSFlag.h"
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
            QSharedPointer<HSSSimpleSelection> scope = theRule->getOriginalScope();
            AXRController * controller = this->getController();
            this->setPurging(newStatus);
            std::vector<QSharedPointer<HSSSelectorChain> > chains;
            chains.push_back(qSharedPointerCast<HSSSelectorChain>(selectorChainNode));
            QSharedPointer<HSSSimpleSelection> selection = controller->select(chains, scope, this->getThisObj())->joinAll();
            this->setPurging(HSSRuleStateOff);

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
            for (HSSSimpleSelection::const_iterator it=selection->begin(); it!=selection->end(); ++it)
            {
                const QSharedPointer<HSSDisplayObject> & theDO = *it;
                theDO->setRuleStatus(theRule, newStatus);
            }
        }
    }
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
