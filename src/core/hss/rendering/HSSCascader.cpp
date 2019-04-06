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

#include "axr.h"
#include "HSSCascader.h"

using namespace AXR;

class HSSCascader::Private
{
public:
    Private() : document()
    {
    }

    AXRDocument *document;
};

HSSCascader::HSSCascader()
: d(new Private)
{
    this->setFilterFlags(HSSVisitorFilterCascading);
}

HSSCascader::~HSSCascader()
{
    delete d;
}

void HSSCascader::setDocument(AXRDocument* document)
{
    d->document = document;
}

void HSSCascader::visit(HSSContainer &container)
{
    if(container._needsRereadRules){
        HSSUnit specificity = 0.;
        container.setDefaults();
        container.clearProperties();
        container.rulesInit();
        container.setExpectsIsAIncluded(true);
        while (container._needsRereadRules)
        {
            container._needsRereadRules = false;
            this->applyStack(container, specificity);
            this->applyIsARules(container, specificity);
        }
        this->applyOverrides(container, specificity);
        container.commitStackValues();
        container.fillWithDefaults();
    }
    container.raiseFlagEventIfNeeded();
}

void HSSCascader::applyOverrides(HSSContainer & container, HSSUnit & specificity)
{
    std::map<AXRString, QSharedPointer<HSSPropertyDefinition> >::const_iterator it;
    std::map<AXRString, QSharedPointer<HSSPropertyDefinition> > overrides = container.getOverrides();
    for (it = overrides.begin(); it != overrides.end(); ++it)
    {
        const QSharedPointer<HSSPropertyDefinition>& propertyDefinition = it->second;
        specificity += 1.0;
        std::vector<QSharedPointer<HSSPropertyPath> > propertyPaths = propertyDefinition->getPaths();
        std::vector<QSharedPointer<HSSPropertyPath> >::iterator it2;
        for (it2 = propertyPaths.begin(); it2 != propertyPaths.end(); ++it2)
        {
            QSharedPointer<HSSPropertyPath> path = *it2;
            QSharedPointer<HSSParserNode> clonedNode = propertyDefinition->getValue()->clone();
            if (clonedNode->getSpecificity() == 0.0)
            {
                clonedNode->setSpecificity(specificity);
            }
            else
            {
                clonedNode->setSpecificity(clonedNode->getSpecificity() + (specificity * 0.000001));
            }
            if (path->size() == 1)
            {
                container.setStackNode(path->front()->getPropertyName(), clonedNode);
            }
            else
            {
                path->setStackNode(container.shared_from_this(), clonedNode);
            }
        }
    }
}

void HSSCascader::applyStack(HSSContainer & container, HSSUnit & specificity)
{
    QSharedPointer<HSSRuleStatus> ruleStatus;
    while((ruleStatus = container.ruleStatusNext()))
    {
        switch (ruleStatus->state)
        {
            case HSSRuleStateActivate:
            {
                ruleStatus->state = HSSRuleStateOn;
                //fall through
            }

            case HSSRuleStateOn:
            {
                QSharedPointer<HSSRule> & rule = ruleStatus->rule;
                std::vector<QSharedPointer<HSSPropertyDefinition> >::const_iterator it;
                std::vector<QSharedPointer<HSSPropertyDefinition> > properties = rule->getProperties();
                for (it = properties.begin(); it != properties.end(); ++it)
                {
                    const QSharedPointer<HSSPropertyDefinition>& propertyDefinition = *it;
                    specificity += 1.0;
                    std::vector<QSharedPointer<HSSPropertyPath> > propertyPaths = propertyDefinition->getPaths();
                    std::vector<QSharedPointer<HSSPropertyPath> >::const_iterator it2;
                    for (it2 = propertyPaths.begin(); it2 != propertyPaths.end(); it2++)
                    {
                        const QSharedPointer<HSSPropertyPath> & path = *it2;
                        QSharedPointer<HSSParserNode> node = propertyDefinition->getValue();
                        if (!node) {
                            continue;
                        }
                        QSharedPointer<HSSParserNode> clonedNode = node->clone();
                        if (clonedNode->getSpecificity() == 0.0)
                        {
                            clonedNode->setSpecificity(specificity);
                        }
                        else
                        {
                            clonedNode->setSpecificity(clonedNode->getSpecificity() + (specificity * 0.000001));
                        }
                        if (path->size() == 1)
                        {
                            container.setStackNode(path->front()->getPropertyName(), clonedNode);
                        }
                        else
                        {
                            path->setStackNode(container.shared_from_this(), clonedNode);
                        }
                    }
                }
                break;
            }

            case HSSRuleStatePurge:
            {
                ruleStatus->state = HSSRuleStateOff;
                container.unlinkPropertyPaths();
                break;
            }

            default:
                break;
        }
    }
}

void HSSCascader::applyIsARules(HSSContainer & container, HSSUnit & specificity)
{
    if (!container._objDefRulesChanged) {
        return;
    }

    QSharedPointer<HSSContainer> thisContainer = qSharedPointerCast<HSSContainer>(container.shared_from_this());
    std::vector<QSharedPointer<HSSRule> > currentRules(container._appliedIsARules);
    std::vector<QSharedPointer<HSSRule> > newRules;
    
    //find the new rules to add
    const std::deque<QSharedPointer<HSSRule> > & objDefRules = container.getObjDefRules();
    std::deque<QSharedPointer<HSSRule> >::const_iterator it;
    for (it = objDefRules.begin(); it != objDefRules.end(); ++it)
    {
        const QSharedPointer<HSSRule> & rule = *it;
        bool found = false;
        
        std::vector<QSharedPointer<HSSRule> >::const_iterator it2;
        for (it2 = currentRules.begin(); it2 != currentRules.end(); ++it2)
        {
            const QSharedPointer<HSSRule> & tmprule = *it2;
            if (tmprule->equalTo(rule))
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            newRules.push_back(rule);
        }
    }
    //remove obsolete rules
    std::vector<QSharedPointer<HSSRule> >::const_iterator it3;
    for (it3 = currentRules.begin(); it3 != currentRules.end(); ++it3)
    {
        const QSharedPointer<HSSRule> & tmprule = *it3;
        bool found = false;
        
        std::deque<QSharedPointer<HSSRule> >::const_iterator it4;
        for (it4 = objDefRules.begin(); it4 != objDefRules.end(); ++it4)
        {
            const QSharedPointer<HSSRule> & tmprule2 = *it4;
            if (tmprule->equalTo(tmprule2))
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            tmprule->removeFromDisplayObjects();
            for (size_t i = 0, size = container._appliedIsARules.size(); i<size; ++i)
            {
                if (container._appliedIsARules[i] == tmprule)
                {
                    container._appliedIsARules.erase(container._appliedIsARules.begin() + i);
                    container.setNeedsRereadRules(true);
                    --i;
                    --size;
                }
            }
        }
    }
    std::vector<QSharedPointer<HSSRule> >::iterator it5;
    for (it5 = newRules.begin(); it5 != newRules.end(); ++it5)
    {
        const QSharedPointer<HSSRule> & rule = *it5;
        rule->setSpecificity(specificity);
        container._appliedIsARules.push_back(rule);
        container.objDefRulesPrepend(rule);
        AXRController * controller = container.getController();
        controller->currentContextPush(thisContainer);
        controller->recursiveMatchRulesToDisplayObjects(rule, thisContainer->getChildren(), thisContainer, true);
        controller->recursiveSetRuleState(rule, thisContainer->getChildren(), thisContainer, HSSRuleStateOn);
        controller->currentContextPop();
    }
    container._objDefRulesChanged = false;
}


void HSSCascader::visit(HSSTextBlock &textBlock)
{
    if (textBlock._needsRereadRules)
    {
        textBlock.setDefaults();
        textBlock.setExpectsIsAIncluded(true);
        textBlock.commitStackValues();
        textBlock.fillWithDefaults();
        textBlock._needsRereadRules = false;
    }
}

void HSSCascader::initializeVisit()
{
    axr_log(LoggerChannelRendering, "INITIALIZE_CASCADE");
    //prepare values
    QSharedPointer<HSSContainer> root = d->document->controller()->root();
    if (!root)
    {
        AXRError("HSSCascader", "Fatal error: No root").raise();
    }
}

void HSSCascader::finalizeVisit()
{
    axr_log(LoggerChannelRendering, "FINALIZE_CASCADE");
}
