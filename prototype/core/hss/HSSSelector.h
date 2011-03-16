//
//  HSSSelector.h
//  mac
//
//  Created by Miro Keller on 13/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSSELECTOR_H
#define HSSSELECTOR_H

#include "HSSParserNode.h"
#include <string>
using namespace std;


class HSSSelector : public HSSParserNode {
    string elementName;
    
public:
    HSSSelector(string elementName);
    virtual string toString();
};

#endif