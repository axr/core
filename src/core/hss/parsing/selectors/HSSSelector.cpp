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
#include "HSSCallback.h"
#include "HSSContainer.h"
#include "HSSDisplayObject.h"
#include "HSSRule.h"
#include "HSSSelector.h"
#include "HSSSimpleSelection.h"
#include "HSSStatement.h"

using namespace AXR;

HSSSelector::HSSSelector(HSSSelectorType type, AXRController * controller)
: HSSParserNode(HSSParserNodeTypeSelector, controller)
{
    this->_selectorType = type;
    this->_negating = false;
}

HSSSelector::~HSSSelector()
{
    this->cleanTrackedObservers();
}

bool HSSSelector::getNegating() const
{
    return this->_negating;
}

void HSSSelector::setNegating(bool value)
{
    this->_negating = value;
}

bool HSSSelector::isA(HSSSelectorType otherType) const
{
    return this->_selectorType == otherType;
}

HSSSelectorType HSSSelector::getSelectorType() const
{
    return this->_selectorType;
}
void HSSSelector::setSelectorType(HSSSelectorType newType)
{
    this->_selectorType = newType;
}

bool HSSSelector::isA(HSSCombinatorType otherType) const
{
    return HSSParserNode::isA(otherType);
}

bool HSSSelector::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSSelector> castedNode = qSharedPointerCast<HSSSelector>(otherNode);
    if ( this->_selectorType != castedNode->_selectorType ) return false;
    if ( this->_negating != castedNode->_negating ) return false;
    return true;
}

const void HSSSelector::observeForContentTreeChanges(QSharedPointer<HSSSelection> scope, bool getParents)
{
    if (getParents)
    {
        const std::vector<QSharedPointer<HSSDisplayObject> > items = scope->getParents();
        for (HSSSimpleSelection::const_iterator it = items.begin(); it != items.end(); ++it)
        {
            const QSharedPointer<HSSDisplayObject> & theDO = *it;
            theDO->observe("__impl_private__contentTreeChanged", "__impl_private__simpleSelection", this, new HSSValueChangedCallback<HSSSelector > (this, &HSSSelector::contentTreeChanged));
        }
    }
    else
    {
        if(!scope) return;
        Q_FOREACH(const QSharedPointer<HSSDisplayObject> & theDO, *(scope->joinAll().data()))
        {
            theDO->observe("__impl_private__contentTreeChanged", "__impl_private__simpleSelection", this, new HSSValueChangedCallback<HSSSelector > (this, &HSSSelector::contentTreeChanged));
        }
    }
}

void HSSSelector::contentTreeChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSParserNode> selectorChainNode = this->getParentNode();
    QSharedPointer<HSSParserNode> ruleNode = selectorChainNode->getParentNode();
    if (ruleNode->isA(HSSParserNodeTypeStatement))
    {
        QSharedPointer<HSSStatement> ruleStatement = qSharedPointerCast<HSSStatement > (ruleNode);
        if (ruleStatement->isA(HSSStatementTypeRule))
        {
            QSharedPointer<HSSRule> theRule = qSharedPointerCast<HSSRule > (ruleStatement);
            AXRController * theController = this->getController();
            QSharedPointer<HSSContainer> theContainer = qSharedPointerCast<HSSContainer>(theObj);
            QSharedPointer<HSSSimpleSelection> scope = theContainer->getChildren();
            if (scope)
            {
                theRule->fastForwardSelectorChain(this);
                theController->recursiveMatchRulesToDisplayObjects(theRule, scope, theContainer, true);
                theController->setRuleStateOnSelection(theRule, scope, HSSRuleStateOn);
                theRule->resetFastForwardSelectorChain();
            }
        }
    }
}
