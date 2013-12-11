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
#include "HSSCallback.h"
#include "HSSObject.h"
#include "HSSObjectDefinition.h"
#include "HSSPropertyDefinition.h"
#include "HSSPropertyPath.h"
#include "HSSPropertyPathNode.h"
#include "HSSRefFunction.h"
#include "HSSValue.h"

using namespace AXR;

HSSPropertyPath::HSSPropertyPath(AXRController * controller)
: HSSParserNode(HSSParserNodeTypePropertyPath, controller),
_iterator(_nodes)
{

}

HSSPropertyPath::HSSPropertyPath(const HSSPropertyPath & orig)
: HSSParserNode(orig),
_iterator(_nodes)
{
    Q_FOREACH(QSharedPointer<HSSPropertyPathNode> ppn, orig._nodes)
    {
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

    for (QVector<QSharedPointer<HSSPropertyPathNode> >::iterator it = this->_nodes.begin(); it != this->_nodes.end(); ++it)
    {
        QSharedPointer<HSSPropertyPathNode> value = *it;
        tempstr.append(value->toString().append("\n"));
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
    QVector<QSharedPointer<HSSPropertyPathNode> >::const_iterator it1, it2;
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

void HSSPropertyPath::setStackNode(QSharedPointer<HSSObject> object, QSharedPointer<HSSParserNode> value, bool initializing)
{
    //short path
    if (this->size() == 1)
    {
        object->setStackNode(this->front()->getPropertyName(), value);
        return;
    }
    //long path
    if (initializing)
    {
        this->_iterator = QVectorIterator<QSharedPointer<HSSPropertyPathNode> >(this->_nodes);
    }
    QSharedPointer<HSSPropertyPathNode> ppn = this->_iterator.next();
    AXRString propertyName = ppn->getPropertyName();
    if(!object->hasStackValue(propertyName))
    {
        if (this->_iterator.hasNext())
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
            this->_iterator.toFront();
        }
    }
    else
    {
        if (this->_iterator.hasNext())
        {
            QSharedPointer<HSSObject> existingObject = object->getStackValue(propertyName);
            if (existingObject->isA(HSSObjectTypeValue))
            {
                QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(existingObject)->getValue();
                if (parserNode)
                {
                    if (parserNode->isA(HSSStatementTypeObjectDefinition))
                    {
                        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition>(parserNode);
                        objdef->applyStack();
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
            this->_iterator.toFront();
        }
    }
}

void HSSPropertyPath::_setModifiers(QSharedPointer<HSSObject> theObj, AXRString propertyName, QSharedPointer<HSSParserNode> value, QSharedPointer<HSSObject> baseObj)
{
    theObj->observe("__impl_private__replace", propertyName, baseObj.data(), new HSSValueChangedCallback<HSSObject>(baseObj.data(), &HSSObject::replaceByPropertyPath));
    QSharedPointer<HSSPropertyDefinition> propDef(new HSSPropertyDefinition(this->getController()));
    QVector<QSharedPointer<HSSPropertyPath> > paths;
    paths.push_back(this->cloneFromCurrentIterator());
    propDef->setPaths(paths);
    propDef->setValue(value);
    baseObj->modifierAdd(propertyName, propDef);
}

void HSSPropertyPath::applyModifier(QSharedPointer<HSSObject> object, QSharedPointer<HSSParserNode> value, bool initializing)
{
    //short path
    if (this->size() == 1)
    {
        AXRString propertyName = this->front()->getPropertyName();
        QSharedPointer<HSSObject> theObj = object->computeValue(propertyName, value);
        theObj->setSpecificity(value->getSpecificity());
        object->setStackValue(propertyName, theObj);
        object->setComputed(propertyName, theObj);
        return;
    }
    //long path
    if (initializing)
    {
        this->_iterator = QVectorIterator<QSharedPointer<HSSPropertyPathNode> >(this->_nodes);
    }
    QSharedPointer<HSSPropertyPathNode> ppn = this->_iterator.next();
    AXRString propertyName = ppn->getPropertyName();
    if(!object->getComputedValue(propertyName))
    {
        if (this->_iterator.hasNext())
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
            this->_iterator.toFront();
        }
    }
    else
    {
        if (this->_iterator.hasNext())
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
                        objdef->applyStack();
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
            this->_iterator.toFront();
        }
    }
}

HSSPropertyPath::query HSSPropertyPath::getComputed(QSharedPointer<HSSObject> object)
{
    query ret;
    if (this->_iterator.hasNext())
    {
        QSharedPointer<HSSPropertyPathNode> ppn = this->_iterator.next();
        AXRString propertyName = ppn->getPropertyName();
        QSharedPointer<HSSObject> computedValue = object->getComputedValue(propertyName);
        if(computedValue)
        {
            if (this->_iterator.hasNext())
            {
                return this->getComputed(computedValue);
            }
            else
            {
                ret.propertyName = propertyName;
                ret.value = computedValue;
                ret.object = object;
                this->_iterator.toFront();
            }
        }
        else
        {
            ret.object = object;
            ret.propertyName = propertyName;
            this->_iterator.toFront();
        }
    }
    else
    {
        this->_iterator.toFront();
    }
    return ret;
}

QVector<QSharedPointer<HSSPropertyPathNode> > HSSPropertyPath::getNodes()
{
    return this->_nodes;
}

void HSSPropertyPath::setNodes(QVector<QSharedPointer<HSSPropertyPathNode> > newNodes)
{
    this->_nodes = newNodes;
}

void HSSPropertyPath::add(QSharedPointer<HSSPropertyPathNode> newValue)
{
    this->_nodes.push_back(newValue);
}

void HSSPropertyPath::add(AXRString newValue)
{
    QSharedPointer<HSSPropertyPathNode> ppn(new HSSPropertyPathNode(newValue, this->getController()));
    this->add(ppn);
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
    QVectorIterator<QSharedPointer<HSSPropertyPathNode> > currentIterator = this->_iterator;
    while (this->_iterator.hasNext())
    {
        newPath->add(this->_iterator.next()->clone());
    }
    this->_iterator = currentIterator;
    return newPath;
}

QSharedPointer<HSSClonable> HSSPropertyPath::cloneImpl() const
{
    return QSharedPointer<HSSPropertyPath>(new HSSPropertyPath(*this));
}
