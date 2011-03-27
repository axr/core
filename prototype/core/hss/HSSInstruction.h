//
//  HSSInstruction.h
//  mac
//
//  Created by Miro Keller on 26/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSINSTRUCTION_H
#define HSSINSTRUCTION_H

#include "HSSStatement.h"

enum HSSInstructionType {
    HSSNewInstruction,
    HSSEnsureInstruction,
    HSSImportInstruction,
    HSSColorInstruction
};

class HSSInstruction : public HSSStatement
{
public:
    static string instructionStringRepresentation(HSSInstructionType type);
    
    HSSInstruction(HSSInstructionType type);
    HSSInstruction(HSSInstructionType type, string value);
    ~HSSInstruction();
    virtual string toString();
    
protected:
    HSSInstructionType type;
    string value;
};

#endif