//
//  HSSRule.h
//  mac
//
//  Created by Miro Keller on 13/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSOBJECTDEFINITION_H
#define HSSOBJECTDEFINITION_H

#import "HSSStatement.h"
#import <string>
#import <vector>
#import "HSSParserNodes.h"
#import "HSSObject.h"

using namespace std;

class HSSObjectDefinition : public HSSStatement
{
public:
    HSSObjectDefinition(HSSObject * prototype);
    ~HSSObjectDefinition();
    virtual string toString();
    
private:
    HSSObject * prototype;
};

#endif