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

#include "HSSInstruction.h"

HSSInstruction::HSSInstruction(HSSInstructionType type)
{
    this->type = type;
}


HSSInstruction::HSSInstruction(HSSInstructionType type, string value)
{
    this->type = type;
    this->value = value;
}

HSSInstruction::~HSSInstruction()
{
    
}

string HSSInstruction::toString()
{
    string tempstr = "HSSInstruction of type: "+this->instructionStringRepresentation(this->type);
    if (this->type == HSSColorInstruction)
    {
        tempstr.append(" and value: "+this->value);
    }
    return tempstr;
}

string HSSInstruction::instructionStringRepresentation(HSSInstructionType type){
    string types[20];
	types[HSSNewInstruction] = "HSSNewInstruction";
	types[HSSEnsureInstruction] = "HSSEnsureInstruction";
	types[HSSImportInstruction] = "HSSImportInstruction";
	types[HSSColorInstruction] = "HSSColorInstruction";
    return types[type];
}