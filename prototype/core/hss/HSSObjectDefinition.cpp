/**********************************************************
 *
 *      d8888 Y88b   d88P 8888888b.  
 *      d88888  Y88b d88P  888   Y88b 
 *      d88P888   Y88o88P   888    888 
 *      d88P 888    Y888P    888   d88P 
 *      88P  888    d888b    8888888P"  
 *      d88P   888   d88888b   888 T88b   
 *      d8888888888  d88P Y88b  888  T88b  
 *      d88P     888 d88P   Y88b 888   T88b 
 *      
 *      ARBITRARY·······XML········RENDERING
 *      ====================================
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
 **********************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2011/04/04
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 **********************************************************/

#include "HSSObjectDefinition.h"
#include <string>
#include <iostream>
#include "AXR.h"

//this acquires ownership of the pointer
HSSObjectDefinition::HSSObjectDefinition(HSSObject * prototype)
{
    this->prototype = prototype;
}

HSSObjectDefinition::~HSSObjectDefinition()
{
    delete this->prototype;
}

string HSSObjectDefinition::toString()
{    
    string tempstr = string("HSSObjectDefinition with object of type:");
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

void HSSObjectDefinition::propertiesAdd(HSSStatement * newStatement)
{
    if(newStatement != NULL)
    {
        std_log3("Added node of type " << newStatement->toString());
        this->properties.push_back(newStatement);
    }
}

void HSSObjectDefinition::propertiesRemoveLast()
{
    delete this->properties.back();
    this->properties.pop_back();
}

HSSStatement * HSSObjectDefinition::propertiesLast()
{
    return this->properties.back();
}

const int HSSObjectDefinition::propertiesSize()
{
    return this->properties.size();
}