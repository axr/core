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
 *      AUTHORS: Miro Keller
 *      
 *      COPYRIGHT: ©2011 - All Rights Reserved
 *
 *      LICENSE: see License.txt file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2011/12/26
 *      HSS version: 1.0
 *      Core version: 0.43
 *      Revision: 10
 *
 ********************************************************************/

#include "HSSObjectDefinition.h"
#include <iostream>
#include "../../axr/AXRDebugging.h"

using namespace AXR;

//this acquires ownership of the pointer
HSSObjectDefinition::HSSObjectDefinition(HSSObject::p prototype)
{
    this->prototype = prototype;
    this->type = HSSStatementTypeObjectDefinition;
    this->nodeType = HSSParserNodeTypeObjectDefinition;
    this->scope = NULL;
}

HSSObjectDefinition::~HSSObjectDefinition()
{
    
}

std::string HSSObjectDefinition::toString()
{    
    std::string tempstr = std::string("HSSObjectDefinition with object of type:");
    tempstr.append(this->prototype->toString());
    
    const int pccount = this->propertiesSize();
    if(pccount > 0){
        tempstr.append(" and the following properties: \n");
        unsigned j;
        for (j=0; j<pccount; j++) {
            tempstr.append("   ").append(this->properties[j]->toString()).append("\n");
        }
    }
    
    return tempstr;
}

void HSSObjectDefinition::apply()
{
    unsigned i,size;
    for (i=0, size=this->propertiesSize(); i<size; i++) {
        HSSPropertyDefinition::p theProperty = this->properties[i];
        this->prototype->setPropertyWithName(theProperty->getName(), theProperty->getValue());
    }
}

void HSSObjectDefinition::propertiesAdd(HSSPropertyDefinition::p &newProperty)
{
    if(newProperty)
    {
        std_log3("HSSObjectDefinition: Added node of type " << newProperty->toString());
        this->properties.push_back(newProperty);
    }
}

void HSSObjectDefinition::propertiesAdd(const HSSPropertyDefinition::p &newProperty)
{
    if(newProperty)
    {
        std_log3("HSSObjectDefinition: Added node of type " << newProperty->toString());
        this->properties.push_back(newProperty);
        //this->prototype->setPropertyWithName(newProperty->getName(), newProperty->getValue());
    }
}

void HSSObjectDefinition::propertiesPrepend(HSSPropertyDefinition::p &newProperty)
{
    if(newProperty)
    {
        std_log3("HSSObjectDefinition: Prepended node of type " << newProperty->toString());
        this->properties.push_front(newProperty);
    }
}

void HSSObjectDefinition::propertiesPrepend(const HSSPropertyDefinition::p &newProperty)
{
    if(newProperty)
    {
        std_log3("HSSObjectDefinition: Prepended node of type " << newProperty->toString());
        this->properties.push_front(newProperty);
    }
}

void HSSObjectDefinition::propertiesRemoveLast()
{
    this->properties.pop_back();
}

HSSPropertyDefinition::p &HSSObjectDefinition::propertiesLast()
{
    return this->properties.back();
}

const int HSSObjectDefinition::propertiesSize()
{
    return this->properties.size();
}

std::deque<HSSPropertyDefinition::p> HSSObjectDefinition::getProperties()
{
    return this->properties;
}

void HSSObjectDefinition::childrenAdd(HSSObjectDefinition::p &child)
{
    if(child)
    {
        this->children.push_back(child);
    }
}

void HSSObjectDefinition::childrenAdd(const HSSObjectDefinition::p &child)
{
    if(child)
    {
        this->children.push_back(child);
    }
}

void HSSObjectDefinition::childrenRemoveLast()
{
    this->children.pop_back();
}

HSSObjectDefinition::p &HSSObjectDefinition::childrenLast()
{
    return this->children.back();
}

const int HSSObjectDefinition::childrenSize()
{
    return this->children.size();
}

const std::vector<HSSObjectDefinition::p> HSSObjectDefinition::getChildren() const
{
    return this->children;
}

HSSObject::p HSSObjectDefinition::getObject()
{
    return this->prototype;
}


HSSObjectDefinition::p HSSObjectDefinition::getParent()
{
    return this->parent.lock();
}

void HSSObjectDefinition::setParent(HSSObjectDefinition::p newParent)
{
    this->parent = boost::weak_ptr<HSSObjectDefinition>(newParent);
}

void HSSObjectDefinition::setScope(const std::vector<HSSDisplayObject::p> * newScope)
{
    this->scope = newScope;
    //propagate values
    this->prototype->setScope(newScope);
}

void HSSObjectDefinition::setThisObj(HSSDisplayObject::p value)
{
    this->thisObj = value;
    //propagate values
    this->prototype->setThisObj(value);
}

HSSDisplayObject::p HSSObjectDefinition::getThisObj()
{
    return thisObj;
}


