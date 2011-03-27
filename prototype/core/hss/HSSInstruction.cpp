//
//  HSSInstruction.cpp
//  mac
//
//  Created by Miro Keller on 26/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

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