//
//  HSSStatement.h
//  mac
//
//  Created by Miro Keller on 13/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSSTATEMENT_H
#define HSSSTATEMENT_H

#include "HSSParserNode.h"
#include <string>

using namespace std;

class HSSStatement : public HSSParserNode
{
public:
    virtual string toString() =0;
};

#endif