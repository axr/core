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

#include "AXRLoggerManager.h"
#include "HSSDisplayObject.h"
#include "HSSObject.h"
#include "HSSObjectDefinition.h"
#include "HSSPropertyDefinition.h"
#include "HSSPropertyPath.h"
#include "HSSPropertyPathNode.h"
#include "HSSRule.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

HSSObjectDefinition::HSSObjectDefinition(QSharedPointer<HSSObject> prototype, AXRController * controller)
: HSSStatement(HSSStatementTypeObjectDefinition, controller)
{
    this->prototype = prototype;
}

HSSObjectDefinition::HSSObjectDefinition(const HSSObjectDefinition & orig)
: HSSStatement(orig)
{
    this->prototype = orig.prototype->clone();

    for (std::deque<QSharedPointer<HSSPropertyDefinition> >::const_iterator p_it = orig.properties.begin(); p_it != orig.properties.end(); ++p_it)
    {
        this->properties.push_back((*p_it)->clone());
    }

    for (std::vector<QSharedPointer<HSSObjectDefinition> >::const_iterator c_it = orig.children.begin(); c_it != orig.children.end(); ++c_it)
    {
        this->children.push_back((*c_it)->clone());
    }

    for (std::deque<QSharedPointer<HSSRule> >::const_iterator r_it = orig._rules.begin(); r_it != orig._rules.end(); ++r_it)
    {
        this->_rules.push_back((*r_it)->clone());
    }
}

QSharedPointer<HSSObjectDefinition> HSSObjectDefinition::clone() const
{
    return qSharedPointerCast<HSSObjectDefinition> (this->cloneImpl());
}

HSSObjectDefinition::~HSSObjectDefinition()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSObjectDefinition: destructing object definition");
    this->cleanTrackedObservers();
}

AXRString HSSObjectDefinition::toString()
{
    AXRString tempstr = AXRString("HSSObjectDefinition with object of type:");
    tempstr.append(this->prototype->toString());

    const size_t pccount = this->propertiesSize();
    if (pccount > 0)
    {
        tempstr.append(" and the following properties: \n");

        for (size_t j = 0; j < pccount; ++j)
        {
            tempstr.append(this->properties[j]->toString().append("\n"));
        }
    }

    return tempstr;
}

bool HSSObjectDefinition::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSStatement::equalTo(otherNode)) return false;
    QSharedPointer<HSSObjectDefinition> castedNode = qSharedPointerCast<HSSObjectDefinition>(otherNode);
    if ( ! this->prototype->equalTo(castedNode->prototype)) return false;
    return true;
}

void HSSObjectDefinition::applyStack()
{
    HSSUnit objDefSpecificity = this->getSpecificity();
    HSSUnit specificity = 0.;
    this->prototype->clearProperties();
    this->prototype->setDefaults();
    Q_FOREACH(const QSharedPointer<HSSPropertyDefinition>& propertyDefinition, this->properties)
    {
        QVector<QSharedPointer<HSSPropertyPath> > propertyPaths = propertyDefinition->getPaths();
        Q_FOREACH(QSharedPointer<HSSPropertyPath> path, propertyPaths){
            QSharedPointer<HSSParserNode> nodeValue = propertyDefinition->getValue();
            nodeValue->setSpecificity(objDefSpecificity + (specificity * 0.000001));
            path->setStackNode(this->prototype, nodeValue);
        }
        specificity += 1.;
    }
}

void HSSObjectDefinition::applyRules()
{
    ///@todo this should target all display object types
    if (this->prototype->isA(HSSObjectTypeContainer))
    {
        QSharedPointer<HSSDisplayObject> theDO = qSharedPointerCast<HSSDisplayObject>(this->prototype);
        Q_FOREACH(QSharedPointer<HSSRule> rule, this->_rules)
        {
            rule->setSpecificity(this->getSpecificity());
            theDO->objDefRulesAdd(rule);
        }
    }
}

void HSSObjectDefinition::propertiesAdd(QSharedPointer<HSSPropertyDefinition> &newProperty)
{
    if (newProperty)
    {
        newProperty->setParentNode(this->shared_from_this());
        this->properties.push_back(newProperty);
    }
}

void HSSObjectDefinition::propertiesAdd(const QSharedPointer<HSSPropertyDefinition> &newProperty)
{
    if (newProperty)
    {
        newProperty->setParentNode(this->shared_from_this());
        this->properties.push_back(newProperty);
        //this->prototype->setPropertyWithName(newProperty->getName(), newProperty->getValue());
    }
}

void HSSObjectDefinition::propertiesPrepend(QSharedPointer<HSSPropertyDefinition> &newProperty)
{
    if (newProperty)
    {
        newProperty->setParentNode(this->shared_from_this());
        this->properties.push_front(newProperty);
    }
}

void HSSObjectDefinition::propertiesPrepend(const QSharedPointer<HSSPropertyDefinition> &newProperty)
{
    if (newProperty)
    {
        newProperty->setParentNode(this->shared_from_this());
        this->properties.push_front(newProperty);
    }
}

void HSSObjectDefinition::propertiesRemoveLast()
{
    this->properties.pop_back();
}

QSharedPointer<HSSPropertyDefinition> &HSSObjectDefinition::propertiesLast()
{
    return this->properties.back();
}

size_t HSSObjectDefinition::propertiesSize() const
{
    return this->properties.size();
}

std::deque<QSharedPointer<HSSPropertyDefinition> > HSSObjectDefinition::getProperties()
{
    return this->properties;
}

void HSSObjectDefinition::childrenAdd(QSharedPointer<HSSObjectDefinition> &child)
{
    if (child)
    {
        this->children.push_back(child);
        child->setParentNode(this->shared_from_this());
    }
}

void HSSObjectDefinition::childrenAdd(const QSharedPointer<HSSObjectDefinition> &child)
{
    if (child)
    {
        this->children.push_back(child);
        child->setParentNode(this->shared_from_this());
    }
}

void HSSObjectDefinition::childrenRemoveLast()
{
    this->children.pop_back();
}

QSharedPointer<HSSObjectDefinition> &HSSObjectDefinition::childrenLast()
{
    return this->children.back();
}

size_t HSSObjectDefinition::childrenSize() const
{
    return this->children.size();
}

const std::vector<QSharedPointer<HSSObjectDefinition> > HSSObjectDefinition::getChildren() const
{
    return this->children;
}

QSharedPointer<HSSObject> HSSObjectDefinition::getObject()
{
    return this->prototype;
}

void HSSObjectDefinition::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    this->scope = newScope;
    //propagate values
    this->prototype->setScope(newScope);
}

void HSSObjectDefinition::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    this->thisObj = value;
    //propagate values
    this->prototype->setThisObj(value);
}

QSharedPointer<HSSDisplayObject> HSSObjectDefinition::getThisObj()
{
    return thisObj;
}

QSharedPointer<HSSObjectDefinition> HSSObjectDefinition::shared_from_this()
{
    return qSharedPointerCast<HSSObjectDefinition > (HSSStatement::shared_from_this());
}

void HSSObjectDefinition::setRules(std::deque<QSharedPointer<HSSRule> > newRules)
{
    this->_rules = newRules;
}

const std::deque<QSharedPointer<HSSRule> > HSSObjectDefinition::getRules() const
{
    return this->_rules;
}

void HSSObjectDefinition::rulesAdd(QSharedPointer<HSSRule> rule)
{
    this->_rules.push_back(rule);
}

void HSSObjectDefinition::rulesPrepend(QSharedPointer<HSSRule> rule)
{
    this->_rules.push_front(rule);
}

void HSSObjectDefinition::rulesRemove(QSharedPointer<HSSRule> rule)
{
    std::deque<QSharedPointer<HSSRule> >::iterator it = find(this->_rules.begin(), this->_rules.end(), rule);
    if (it != this->_rules.end())
    {
        this->_rules.erase(it);
    }
}

QSharedPointer<HSSClonable> HSSObjectDefinition::cloneImpl() const
{
    return QSharedPointer<HSSObjectDefinition>(new HSSObjectDefinition(*this));
}
