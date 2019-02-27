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

#include <QVector>
#include "AXRController.h"
#include "AXRDocument.h"
#include "AXRError.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSCascader.h"
#include "HSSContainer.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSPropertyDefinition.h"
#include "HSSPropertyPath.h"
#include "HSSPropertyPathNode.h"
#include "HSSRule.h"
#include "HSSTextBlock.h"
#include "HSSValue.h"

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
            this->applyOverrides(container, specificity);
            container.commitStackValues();
        }
        container.fillWithDefaults();
    }
}

void HSSCascader::applyOverrides(HSSContainer & container, HSSUnit & specificity)
{
    Q_FOREACH(const QSharedPointer<HSSPropertyDefinition>& propertyDefinition, container.getOverrides())
    {
        specificity += 1.0;
        QVector<QSharedPointer<HSSPropertyPath> > propertyPaths = propertyDefinition->getPaths();
        Q_FOREACH(QSharedPointer<HSSPropertyPath> path, propertyPaths){
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
                Q_FOREACH(const QSharedPointer<HSSPropertyDefinition>& propertyDefinition, rule->getProperties())
                {
                    specificity += 1.0;
                    QVector<QSharedPointer<HSSPropertyPath> > propertyPaths = propertyDefinition->getPaths();
                    Q_FOREACH(QSharedPointer<HSSPropertyPath> path, propertyPaths){
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
