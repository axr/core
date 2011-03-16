//
//  HSSUniversalSelector.h
//  mac
//
//  Created by Miro Keller on 15/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSUNIVERSALSELECTOR_H
#define HSSUNIVERSALSELECTOR_H

#include "HSSParserNode.h"
#include <string>
using namespace std;


class HSSUniversalSelector : public HSSParserNode {
    
public:
    HSSUniversalSelector();
    virtual string toString();
};

#endif