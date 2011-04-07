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

#include "HSSRule.h"
#include <string>
#include <iostream>
#include "AXR.h"

HSSRule::HSSRule(HSSSelectorChain * selectorChain)
{
    this->selectorChain = selectorChain;
}

HSSRule::~HSSRule()
{
    delete this->selectorChain;
    unsigned i;
    for(i=0; i<this->properties.size(); i++){
        this->propertiesRemoveLast();
    }
}

string HSSRule::toString()
{
    string tempstr = string("HSSRule with the following selector chain: \n");
    if (this->selectorChain != NULL){
        tempstr.append("   ").append(this->selectorChain->toString());
    } else {
        tempstr.append("WARNING: this rule has no selector chain!\n");
    }
    
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

void HSSRule::propertiesAdd(HSSStatement * newStatement)
{
    if(newStatement != NULL)
    {
        std_log3("Added node of type " << newStatement->toString());
        this->properties.push_back(newStatement);
    }
}

void HSSRule::propertiesRemoveLast()
{
    delete this->properties.back();
    this->properties.pop_back();
}

HSSStatement * HSSRule::propertiesLast()
{
    return this->properties.back();
}

const int HSSRule::propertiesSize()
{
    return this->properties.size();
}