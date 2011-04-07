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

#include "HSSMultipleValue.h"
#include "AXR.h"
#include <iostream>

HSSMultipleValue::HSSMultipleValue()
{
    this->valueList = vector<HSSValue*>();
}

HSSMultipleValue::~HSSMultipleValue()
{
    unsigned i;
    for(i=0; i<this->valueList.size(); i++){
        this->removeLast();
    }
}

string HSSMultipleValue::toString()
{
    string tempstr = string("HSSMultipleValue");
    
    const int vcount = this->size();
    if(vcount > 0){
        tempstr.append(" with the following values: \n");
        unsigned j;
        for (j=0; j<vcount; j++) {
            tempstr.append("   ").append(this->valueList[j]->toString()).append("\n");
        }
    }
    
    return tempstr;
}

void HSSMultipleValue::add(HSSValue * newValue)
{
    if(newValue != NULL)
    {
        std_log3("Added value to HSSMultipleValue: " << newValue->toString());
        this->valueList.push_back(newValue);
    }
}

void HSSMultipleValue::removeLast()
{
    delete this->valueList.back();
    this->valueList.pop_back();
}

HSSValue * HSSMultipleValue::last()
{
    return this->valueList.back();
}

const int HSSMultipleValue::size()
{
    return this->valueList.size();
}