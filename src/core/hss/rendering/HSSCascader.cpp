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
    if (container._needsRereadRules)
    {
        container.clearProperties();
        container.setDefaults();
        Q_FOREACH(const QSharedPointer<HSSRuleStatus> & ruleStatus, container.rules)
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
                        QVector<QVector<AXRString> > propertyPaths = propertyDefinition->getPaths();
                        Q_FOREACH(QVector<AXRString> path, propertyPaths){
                            QSharedPointer<HSSParserNode> clonedNode = propertyDefinition->getValue()->clone();
                            if (clonedNode->isA(HSSStatementTypeObjectDefinition))
                            {
                                qSharedPointerCast<HSSObjectDefinition>(clonedNode)->applyStack();
                            }
                            if (path.size() == 1)
                            {
                                container.setStackValue(path.front(), clonedNode);
                            }
                            else
                            {
                                this->_applyProperty(container, path, clonedNode);
                            }
                        }
                    }
                    break;
                }
                    
                case HSSRuleStatePurge:
                {
                    ruleStatus->state = HSSRuleStateOff;
                    break;
                }
                    
                default:
                    break;
            }
        }
        container.commitStackValues();
        container.fillWithDefaults();
        container._needsRereadRules = false;
    }
}

void HSSCascader::_applyProperty(HSSObject & object, QVector<AXRString> path, QSharedPointer<HSSParserNode> value)
{
    AXRController * controller = this->d->document->controller().data();
    AXRString property = path.front();
    path.pop_front();
    QSharedPointer<HSSObjectDefinition> objDef;
    if (!path.empty())
    {
        if(object.hasStackValue(property))
        {
            QSharedPointer<HSSParserNode> stackValue = object.getStackValue(property);
            if (stackValue && stackValue->isA(HSSStatementTypeObjectDefinition))
            {
                objDef = qSharedPointerCast<HSSObjectDefinition>(stackValue);
            }
        }
        else
        {
            QSharedPointer<HSSObject> prototype = HSSObject::newObjectWithType(object.defaultObjectType(property), controller);
            objDef = QSharedPointer<HSSObjectDefinition>(new HSSObjectDefinition(prototype, controller));
        }
        this->_applyProperty(*(objDef->getObject().data()), path, value);
    }
    else
    {
        object.setStackValue(property, value);
    }
}

void HSSCascader::visit(HSSTextBlock &textBlock)
{
    if (textBlock._needsRereadRules)
    {
        textBlock.setDefaults();
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


