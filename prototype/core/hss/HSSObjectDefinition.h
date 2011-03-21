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
    
    //adds a pointer to newStatement to the properties list
    //this function acquires ownership of the statement
    void propertiesAdd(HSSStatement * newStatement);
    //removes last statement from the list and then deletes it
    void propertiesRemoveLast();
    //returns a pointer to the last statement in the list
    HSSStatement * propertiesLast();
    //returns how many statements there are in the properties list
    const int propertiesSize();
protected:
    vector<HSSStatement *> properties;
    
private:
    HSSObject * prototype;
};

#endif