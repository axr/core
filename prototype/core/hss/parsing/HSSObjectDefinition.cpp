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
 *      Last changed: 2011/09/11
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 6
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

void HSSObjectDefinition::propertiesAdd(HSSPropertyDefinition::p &newProperty)
{
    if(newProperty != NULL)
    {
        std_log3("HSSObjectDefinition: Added node of type " << newProperty->toString());
        this->properties.push_back(newProperty);
        this->prototype->setProperty(newProperty->getName(), newProperty->getValue());
    }
}

void HSSObjectDefinition::propertiesAdd(const HSSPropertyDefinition::p &newProperty)
{
    if(newProperty != NULL)
    {
        std_log3("HSSObjectDefinition: Added node of type " << newProperty->toString());
        this->properties.push_back(newProperty);
        this->prototype->setProperty(newProperty->getName(), newProperty->getValue());
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

HSSObject::p HSSObjectDefinition::getObject()
{
    return this->prototype;
}


