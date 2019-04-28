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
#include "HSSObjectDefinition.h"

using namespace AXR;

HSSObjectDefinition::HSSObjectDefinition(HSSString type, AXRController * controller)
: HSSStatement(HSSStatementTypeObjectDefinition, controller)
, _objType(type)
, _objName()
{

}

HSSObjectDefinition::HSSObjectDefinition(const HSSObjectDefinition & orig)
: HSSStatement(orig)
{
    this->_objType = orig._objType;
    this->_objName = orig._objName;
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

HSSString HSSObjectDefinition::toString()
{
    HSSString tempstr = HSSString("HSSObjectDefinition");

    const size_t pccount = this->propertiesSize();
    if (pccount > 0)
    {
        tempstr.append(" with the following properties: \n");

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
    return true;
}

void HSSObjectDefinition::applyStack(QSharedPointer<HSSObject> theObj)
{
    HSSUnit objDefSpecificity = this->getSpecificity();
    HSSUnit specificity = 0.;
    std::deque<QSharedPointer<HSSPropertyDefinition> >::iterator it;
    for (it = this->properties.begin(); it != this->properties.end(); ++it)
    {
        QSharedPointer<HSSPropertyDefinition> propertyDefinition = *it;
        const std::vector<QSharedPointer<HSSPropertyPath> > propertyPaths = propertyDefinition->getPaths();
        std::vector<QSharedPointer<HSSPropertyPath> >::const_iterator it2;
        for (it2 = propertyPaths.begin(); it2 != propertyPaths.end(); ++it2)
        {
            const QSharedPointer<HSSPropertyPath> & path = *it2;
            QSharedPointer<HSSParserNode> nodeValue = propertyDefinition->getValue()->clone();
            nodeValue->setSpecificity(objDefSpecificity + (specificity * 0.000001));
            path->setStackNode(theObj, nodeValue);
        }
        specificity += 1.;
    }
}

void HSSObjectDefinition::applyRules(QSharedPointer<HSSObject> theObj)
{
    ///@todo this should target all display object types
    if (theObj->isA(HSSObjectTypeContainer))
    {
        QSharedPointer<HSSDisplayObject> theDO = qSharedPointerCast<HSSDisplayObject>(theObj);
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

void HSSObjectDefinition::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    this->scope = newScope;
}

void HSSObjectDefinition::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    this->thisObj = value;
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

void HSSObjectDefinition::setType(HSSString newType)
{
    this->_objType = newType;
}

const HSSString HSSObjectDefinition::type() const
{
    return this->_objType;
}

void HSSObjectDefinition::setName(HSSString newName)
{
    this->_objName = newName;
}

const HSSString HSSObjectDefinition::name() const
{
    return this->_objName;
}

QSharedPointer<HSSObject> HSSObjectDefinition::getObject()
{
    QSharedPointer<HSSObject> ret = HSSObject::newObjectWithType(this->_objType, this->getController());
    if (ret)
    {
        ret->clearProperties();
        ret->setDefaults();
        this->applyStack(ret);
        ret->fillWithDefaults();
    }
    return ret;
}

QSharedPointer<HSSClonable> HSSObjectDefinition::cloneImpl() const
{
    return QSharedPointer<HSSObjectDefinition>(new HSSObjectDefinition(*this));
}
