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
#include "HSSPropertyPath.h"

using namespace AXR;

HSSPropertyPath::HSSPropertyPath(AXRController * controller)
: HSSScopedParserNode(HSSParserNodeTypePropertyPath, controller)
, _initialized(false)
{

}

HSSPropertyPath::HSSPropertyPath(const HSSPropertyPath & orig)
: HSSScopedParserNode(orig)
, _initialized(orig._initialized)
{
    std::deque<QSharedPointer<HSSPropertyPathNode> >::const_iterator it;
    for (it = orig._nodes.begin(); it != orig._nodes.end(); ++it)
    {
        const QSharedPointer<HSSPropertyPathNode> & ppn = *it;
        this->_nodes.push_back(ppn->clone());
    }
}

QSharedPointer<HSSPropertyPath> HSSPropertyPath::clone() const
{
    return qSharedPointerCast<HSSPropertyPath> (this->cloneImpl());
}

HSSPropertyPath::~HSSPropertyPath()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSPropertyPath: destructing property path");
    this->cleanTrackedObservers();
}

AXRString HSSPropertyPath::toString()
{
    AXRString tempstr = AXRString("HSSPropertyPath with the following nodes:\n");

    for (std::deque<QSharedPointer<HSSPropertyPathNode> >::iterator it = this->_nodes.begin(); it != this->_nodes.end(); ++it)
    {
        QSharedPointer<HSSPropertyPathNode> value = *it;
        tempstr.append(value->toString().append("\n"));
    }

    return tempstr;
}

AXRString HSSPropertyPath::stringRep()
{
    AXRString tempstr;
    for (std::deque<QSharedPointer<HSSPropertyPathNode> >::iterator it = this->_nodes.begin(); it != this->_nodes.end(); ++it)
    {
        QSharedPointer<HSSPropertyPathNode> value = *it;
        if (value->hasName() && tempstr.length() > 0)
        {
            tempstr.append(".");
        }
        tempstr.append(value->stringRep());
        
    }
    return tempstr;
}

AXRString HSSPropertyPath::toKeyString() const
{
    AXRString tempstr;
    for (std::deque<QSharedPointer<HSSPropertyPathNode> >::const_iterator it = this->_nodes.begin(); it != this->_nodes.end(); ++it)
    {
        QSharedPointer<HSSPropertyPathNode> value = *it;
        tempstr.append(value->toString().append("."));
    }
    
    return tempstr;
}

bool HSSPropertyPath::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSPropertyPath> castedNode = qSharedPointerCast<HSSPropertyPath>(otherNode);
    if ( this->_nodes.size() != castedNode->_nodes.size() ) return false;
    std::deque<QSharedPointer<HSSPropertyPathNode> >::const_iterator it1, it2;
    it2 = castedNode->_nodes.begin();
    for (it1 = this->_nodes.begin(); it1 != this->_nodes.end(); ++it1)
    {
        const QSharedPointer<HSSPropertyPathNode> & nod = (*it1);
        const QSharedPointer<HSSPropertyPathNode> & otherNod = (*it2);
        if ( ! nod->equalTo(otherNod) ) return false;
        ++it2;
    }
    return true;
}

void HSSPropertyPath::initialize()
{
    this->_initialized = true;
    this->_iterator = this->_nodes.begin();
    this->_endIterator = this->_nodes.end();
}

void HSSPropertyPath::goToNext()
{
    ++this->_iterator;
}

void HSSPropertyPath::setStackNode(QSharedPointer<HSSObject> object, QSharedPointer<HSSParserNode> value, bool initializing)
{
    //short path
    if (this->size() == 1)
    {
        object->setStackNode(this->front()->evaluate(), value);
        return;
    }
    //long path
    if (initializing)
    {
        this->initialize();
    }
    QSharedPointer<HSSPropertyPathNode> ppn = *this->_iterator;
    this->goToNext();
    AXRString propertyName = ppn->evaluate();
    if(!object->hasStackValue(propertyName))
    {
        if (this->_iterator != this->_endIterator)
        {
            AXRController * controller = this->getController();
            QSharedPointer<HSSObject> newObject = HSSObject::newObjectWithType(object->defaultObjectType(propertyName), controller);
            newObject->setDefaults();
            newObject->setDefaultPropertyPathObject(true);
            this->_setModifiers(newObject, propertyName, value, object);
            object->setStackValue(propertyName, newObject);
            this->setStackNode(newObject, value, false);
        }
        else
        {
            object->setStackNode(propertyName, value);
            this->_iterator = this->_nodes.begin();
        }
    }
    else
    {
        if (this->_iterator != this->_endIterator)
        {
            QSharedPointer<HSSObject> existingObject = object->getStackValue(propertyName);
            if (existingObject->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(existingObject)->getValue();
                if (parserNode)
                {
                    if (parserNode->isA(HSSParserNodeTypePropertyPath))
                    {
                        QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath>(parserNode);
                        QSharedPointer<HSSObject> remoteObj = ppath->evaluate();
                        QSharedPointer<HSSObject> objDefObj;
                        if (remoteObj)
                        {
                            if (remoteObj->isA(HSSObjectTypeValue))
                            {
                                QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(remoteObj)->getValue();
                                if (valueNode)
                                {
                                    if (valueNode->isA(HSSStatementTypeObjectDefinition))
                                    {
                                        objDefObj = qSharedPointerCast<HSSObjectDefinition>(valueNode)->getObject();
                                    }
                                }
                            }
                            if (objDefObj)
                            {
                                remoteObj = objDefObj;
                            }
                            else
                            {
                                remoteObj = remoteObj->clone();
                            }
                            remoteObj->setSpecificity(parserNode->getSpecificity());
                        }
                        else
                        {
                            AXRWarning("HSSPropertyPath", "The path did not yield any results").raise();
                            return;
                        }
                        this->_setModifiers(remoteObj, propertyName, value, object);
                        this->setStackNode(remoteObj, value, false);
                        remoteObj->commitStackValues();
                        object->setStackValue(propertyName, remoteObj);
                    }
                    else if (parserNode->isA(HSSStatementTypeObjectDefinition))
                    {
                        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition>(parserNode);
                        QSharedPointer<HSSObject> remoteObj = objdef->getObject();
                        this->_setModifiers(remoteObj, propertyName, value, object);
                        this->setStackNode(remoteObj, value, false);
                    }
                    else if (parserNode->isA(HSSFunctionTypeRef))
                    {
                        QSharedPointer<HSSObject> remoteObj = qSharedPointerCast<HSSRefFunction>(parserNode)->evaluate();
                        if (remoteObj)
                        {
                            this->setStackNode(remoteObj, value, false);
                            this->_setModifiers(remoteObj, propertyName, value, object);
                            object->setStackValue(propertyName, remoteObj);
                        }
                        ///@todo observe for future changes. For that, the object definition needs to be cloned.
                    }
                }
                else
                {
                    this->_setModifiers(existingObject, propertyName, value, object);
                    this->setStackNode(existingObject, value, false);
                }
            }
            else
            {
                this->_setModifiers(existingObject, propertyName, value, object);
                this->setStackNode(existingObject, value, false);
            }
        }
        else
        {
            object->setStackNode(propertyName, value);
            this->_iterator = this->_nodes.begin();
        }
    }
}

void HSSPropertyPath::_setModifiers(QSharedPointer<HSSObject> theObj, AXRString propertyName, QSharedPointer<HSSParserNode> value, QSharedPointer<HSSObject> baseObj)
{
    theObj->observe("__impl_private__replace", propertyName, baseObj.data(), new HSSValueChangedCallback<HSSObject>(baseObj.data(), &HSSObject::replaceByPropertyPath));
    QSharedPointer<HSSPropertyDefinition> propDef(new HSSPropertyDefinition(this->getController()));
    std::vector<QSharedPointer<HSSPropertyPath> > paths;
    paths.push_back(this->cloneFromCurrentIterator());
    propDef->setPaths(paths);
    propDef->setValue(value);
    theObj->modifierAdd(propertyName, propDef);
}

void HSSPropertyPath::applyModifier(QSharedPointer<HSSObject> object, QSharedPointer<HSSParserNode> value, bool initializing)
{
    //short path
    if (this->size() == 1)
    {
        AXRString propertyName = this->front()->evaluate();
        QSharedPointer<HSSObject> theObj = object->computeValue(propertyName, value);
        theObj->setSpecificity(value->getSpecificity());
        object->setStackValue(propertyName, theObj);
        object->setComputed(propertyName, theObj);
        return;
    }
    //long path
    if (initializing)
    {
        this->initialize();
    }
    QSharedPointer<HSSPropertyPathNode> ppn = *this->_iterator;
    this->goToNext();
    AXRString propertyName = ppn->evaluate();
    if(!object->getComputedValue(propertyName))
    {
        if (this->_iterator != this->_endIterator)
        {
            AXRController * controller = this->getController();
            QSharedPointer<HSSObject> newObject = HSSObject::newObjectWithType(object->defaultObjectType(propertyName), controller);
            newObject->setDefaults();
            this->applyModifier(newObject, value, false);
            QSharedPointer<HSSObject> theObj = object->computeValue(propertyName, value);
            newObject->setSpecificity(value->getSpecificity());
            object->setStackValue(propertyName, newObject);
            object->setComputed(propertyName, newObject);
        }
        else
        {
            QSharedPointer<HSSObject> theObj = object->computeValue(propertyName, value);
            theObj->setSpecificity(value->getSpecificity());
            object->setStackValue(propertyName, theObj);
            object->setComputed(propertyName, theObj);
            this->_iterator = this->_nodes.begin();
        }
    }
    else
    {
        if (this->_iterator != this->_endIterator)
        {
            QSharedPointer<HSSObject> existingObject = object->getComputedValue(propertyName);
            if (existingObject->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(existingObject)->getValue();
                if (parserNode)
                {
                    if (parserNode->isA(HSSStatementTypeObjectDefinition))
                    {
                        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition>(parserNode);
                        this->applyModifier(objdef->getObject(), value, false);
                    }
                    else if (parserNode->isA(HSSFunctionTypeRef))
                    {
                        QSharedPointer<HSSObject> remoteObj = qSharedPointerCast<HSSRefFunction>(parserNode)->evaluate();
                        if (remoteObj)
                        {
                            this->applyModifier(remoteObj, value, false);
                            object->setStackValue(propertyName, remoteObj);
                            object->setComputed(propertyName, remoteObj);
                        }
                    }
                }
            }
            else
            {
                this->applyModifier(existingObject, value, false);
            }
        }
        else
        {
            QSharedPointer<HSSObject> theObj = object->computeValue(propertyName, value);
            theObj->setSpecificity(value->getSpecificity());
            object->setStackValue(propertyName, theObj);
            object->setComputed(propertyName, theObj);
            this->_iterator = this->_nodes.begin();
        }
    }
}

HSSPropertyPath::query HSSPropertyPath::getComputed(QSharedPointer<HSSObject> object)
{
    query ret;
    if (!this->_initialized)
        this->initialize();
    
    if (this->_iterator != this->_endIterator)
    {
        QSharedPointer<HSSPropertyPathNode> ppn = *this->_iterator;
        this->goToNext();
        AXRString propertyName = ppn->evaluate();
        QSharedPointer<HSSObject> computedValue = object->getComputedValue(propertyName);
        if(computedValue)
        {
            if (this->_iterator != this->_endIterator)
            {
                return this->getComputed(computedValue);
            }
            else
            {
                ret.propertyName = propertyName;
                ret.value = computedValue;
                ret.object = object;
                this->_iterator = this->_nodes.begin();
            }
        }
        else
        {
            ret.object = object;
            ret.propertyName = propertyName;
            this->_iterator = this->_nodes.begin();
        }
    }
    else
    {
        this->_iterator = this->_nodes.begin();
    }
    return ret;
}

std::deque<QSharedPointer<HSSPropertyPathNode> > HSSPropertyPath::getNodes()
{
    return this->_nodes;
}

void HSSPropertyPath::setNodes(std::deque<QSharedPointer<HSSPropertyPathNode> > newNodes)
{
    this->_nodes = newNodes;
}

void HSSPropertyPath::add(QSharedPointer<HSSPropertyPathNode> newValue)
{
    this->_nodes.push_back(newValue);
}

void HSSPropertyPath::add(AXRString newValue)
{
    QSharedPointer<HSSPropertyPathNode> ppn(new HSSPropertyPathNode(HSSPropertyNameConstant::createConstant(newValue, this->getController()), this->getController()));
    this->add(ppn);
}

void HSSPropertyPath::prepend(QSharedPointer<HSSPropertyPathNode> newValue)
{
    this->_nodes.push_front(newValue);
}

void HSSPropertyPath::prepend(AXRString newValue)
{
    QSharedPointer<HSSPropertyPathNode> ppn(new HSSPropertyPathNode(HSSPropertyNameConstant::createConstant(newValue, this->getController()), this->getController()));
    this->prepend(ppn);
}

QSharedPointer<HSSPropertyPathNode> HSSPropertyPath::front() const
{
    return this->_nodes.front();
}

size_t HSSPropertyPath::size() const
{
    return this->_nodes.size();
}

QSharedPointer<HSSPropertyPathNode> HSSPropertyPath::popFront()
{
    QSharedPointer<HSSPropertyPathNode> ppn = this->_nodes.front();
    this->_nodes.pop_front();
    return ppn;
}

QSharedPointer<HSSPropertyPath> HSSPropertyPath::cloneFromCurrentIterator()
{
    QSharedPointer<HSSPropertyPath> newPath(new HSSPropertyPath(this->getController()));
    
    std::deque<QSharedPointer<HSSPropertyPathNode> >::iterator currentIterator = this->_iterator;
    while (this->_iterator != this->_endIterator)
    {
        newPath->add((*this->_iterator)->clone());
        this->goToNext();
    }
    this->_iterator = currentIterator;
    return newPath;
}

QSharedPointer<HSSClonable> HSSPropertyPath::cloneImpl() const
{
    return QSharedPointer<HSSPropertyPath>(new HSSPropertyPath(*this));
}

QSharedPointer<HSSObject> HSSPropertyPath::evaluate()
{
    QSharedPointer<HSSObject> errorState;
    //short path
    if (this->size() == 1)
    {
        QSharedPointer<HSSParserNode> value = this->front()->getValue();
        switch (value->getType())
        {
            case HSSParserNodeTypeObjectNameConstant:
            {
                return this->getVar(qSharedPointerCast<HSSObjectNameConstant>(value)->getValue());
            }
            case HSSParserNodeTypePropertyNameConstant:
            {
                return this->getVar(qSharedPointerCast<HSSPropertyNameConstant>(value)->getValue());
            }
            default:
                break;
        }
    }
    //long path
    this->initialize();
    QSharedPointer<HSSPropertyPathNode> gppn = *this->_iterator;
    AXRString varName = gppn->evaluate();
    QSharedPointer<HSSObject> object = this->getVar(varName);
    if (!object)
        return errorState;

    this->goToNext();
    while (this->_iterator != this->_endIterator)
    {
        
        QSharedPointer<HSSPropertyPathNode> ppn = *this->_iterator;
        if (ppn->hasName())
        {
            AXRString propertyName = ppn->evaluate();
            QSharedPointer<HSSObject> computedValue = object->getComputedValue(propertyName);
            if(computedValue)
            {
                object = computedValue;
            }
            else
            {
                AXRError("HSSPropertyPath", "Object "+object->name+" doesn't have property "+propertyName);
                return errorState;
            }
        }
        else
        {
            QSharedPointer<HSSFunctionCall> fCall = qSharedPointerCast<HSSFunctionCall>(ppn->getValue());
            QSharedPointer<HSSObject> remoteObj = fCall->evaluate(object);
            if (remoteObj)
            {
                object = remoteObj;
            }
        }
        this->goToNext();
    }
    return object;
}

void HSSPropertyPath::observe()
{
    //short path
    if (this->size() == 1)
    {
        QSharedPointer<HSSParserNode> value = this->front()->getValue();
        switch (value->getType())
        {
            case HSSParserNodeTypeObjectNameConstant:
            {
                this->observeVar(qSharedPointerCast<HSSObjectNameConstant>(value)->getValue());
                return;
            }
            default:
                break;
        }
    }
    //long path
    this->initialize();
    QSharedPointer<HSSPropertyPathNode> gppn = *this->_iterator;
    AXRString varName = gppn->evaluate();
    QSharedPointer<HSSObject> object = this->getController()->getGlobalVariable(varName);
    if (!object)
        return;
    
    this->observeVar(varName);
    
    this->goToNext();
    while (this->_iterator != this->_endIterator)
    {
        
        QSharedPointer<HSSPropertyPathNode> ppn = *this->_iterator;
        if (ppn->hasName())
        {
            AXRString propertyName = ppn->evaluate();
            QSharedPointer<HSSObject> computedValue = object->getComputedValue(propertyName);
            if(computedValue)
            {
                object->observe(propertyName, "propertyPath", this,  new HSSValueChangedCallback<HSSPropertyPath > (this, &HSSPropertyPath::valueChanged));

                object = computedValue;
            }
            else
            {
                AXRError("HSSPropertyPath", "Object "+object->name+" doesn't have property "+propertyName);
                return;
            }
        }
        else
        {
            QSharedPointer<HSSFunctionCall> fCall = qSharedPointerCast<HSSFunctionCall>(ppn->getValue());
            QSharedPointer<HSSObject> remoteObj = fCall->evaluate(object);
            if (remoteObj)
            {
                object->observe("__impl_private_valueChanged", this->getHostProperty(), this,  new HSSValueChangedCallback<HSSPropertyPath > (this, &HSSPropertyPath::valueChanged));

                object = remoteObj;
            }
        }
        this->goToNext();
    }
}

void HSSPropertyPath::observeVar(HSSString varName)
{
    this->getController()->observe(varName, "propertyPath", this, new HSSValueChangedCallback<HSSPropertyPath > (this, &HSSPropertyPath::valueChanged));
}

void HSSPropertyPath::valueChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> remoteObj)
{
    QSharedPointer<HSSObject> ret;
    if (remoteObj)
    {
        if (remoteObj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSValue> valueObj = qSharedPointerCast<HSSValue>(remoteObj);
            
            QSharedPointer<HSSParserNode> parserNode = valueObj->getValue();
            if (parserNode)
            {
                switch (parserNode->getType())
                {
                    case HSSParserNodeTypeFunction:
                    {
                        QSharedPointer<HSSObject> remoteObj2 = qSharedPointerCast<HSSFunction>(parserNode)->evaluate();
                        if (remoteObj2) {
                            ret = remoteObj2;
                        }
                        break;
                    }
                        
                    case HSSParserNodeTypeExpression:
                        ret = HSSValue::valueFromParserNode(this->getController(), HSSNumberConstant::number(qSharedPointerCast<HSSExpression>(parserNode)->evaluate(), this->getController()), parserNode->getSpecificity(), this->getThisObj(), this->scope());
                        break;
                        
                    default:
                        ret = remoteObj->clone();
                        break;
                }
            }
            else
            {
                ret = remoteObj->clone();
            }
        }
        else
        {
            ret = remoteObj->clone();
        }
    }
    this->notifyObservers("__impl_private__remoteValue", ret);
}

void HSSPropertyPath::evaluateSet(QSharedPointer<HSSObject> theObj)
{
    //short path
    if (this->size() == 1)
    {
        QSharedPointer<HSSParserNode> value = this->front()->getValue();
        switch (value->getType())
        {
            case HSSParserNodeTypeObjectNameConstant:
            {
                HSSString objName = qSharedPointerCast<HSSObjectNameConstant>(value)->getValue();
                if (this->hasLocalVar(objName))
                {
                    this->setLocalVar(objName, theObj);
                }
                else
                {
                    this->getController()->setGlobalVariable(objName, theObj);
                }
            }
            default:
                break;
        }
        return;
    }
    //long path
    this->initialize();
    QSharedPointer<HSSPropertyPathNode> gppn = *this->_iterator;
    AXRString varName = gppn->evaluate();
    QSharedPointer<HSSObject> object = this->getVar(varName);
    if (!object)
        return;

    QSharedPointer<HSSPropertyPathNode> ppn;
    AXRString propertyName;
    
    this->goToNext();

    while (this->_iterator != this->_endIterator)
    {
        ppn = *this->_iterator;
        
        this->goToNext();
        if (this->_iterator == this->_endIterator)
        {
            if (ppn->hasName())
            {
                propertyName = ppn->evaluate();
                theObj->setSpecificity(1.);
                object->setComputed(propertyName, theObj);
            }
            else
            {
                AXRError("HSSPropertyPath", "Can't set value to a function call");
                return;
            }
        }
        else
        {
            
            if (ppn->hasName())
            {
                propertyName = ppn->evaluate();
                QSharedPointer<HSSObject> computedValue = object->getComputedValue(propertyName);
                if(computedValue)
                {
                    object = computedValue;
                }
                else
                {
                    AXRError("HSSPropertyPath", "Object "+object->name+" doesn't have property "+propertyName);
                    return;
                }
            }
            else
            {
                QSharedPointer<HSSFunctionCall> fCall = qSharedPointerCast<HSSFunctionCall>(ppn->getValue());
                QSharedPointer<HSSObject> remoteObj = fCall->evaluate(object);
                if (remoteObj)
                {
                    object = remoteObj;
                }
            }
        }
    }
}

void HSSPropertyPath::setThisObj(QSharedPointer<AXR::HSSDisplayObject> value)
{
    HSSParserNode::setThisObj(value);
    std::deque<QSharedPointer<HSSPropertyPathNode> >::const_iterator it;
    for (it = this->_nodes.begin(); it != this->_nodes.end(); ++it)
    {
        const QSharedPointer<HSSPropertyPathNode> & ppn = *it;
        ppn->setThisObj(value);
    }
}

void HSSPropertyPath::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    HSSScopedParserNode::setScope(newScope);
    std::deque<QSharedPointer<HSSPropertyPathNode> >::const_iterator it;
    for (it = this->_nodes.begin(); it != this->_nodes.end(); ++it)
    {
        const QSharedPointer<HSSPropertyPathNode> & ppn = *it;
        ppn->setScope(newScope);
    }
}
