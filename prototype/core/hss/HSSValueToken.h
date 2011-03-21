//
//  HSSValueToken.h
//  mac
//
//  Created by Miro Keller on 13/03/11.
//  Copyright 2011 2011 Miro Keller. All rights reserved.
//

#ifndef HSSVALUETOKEN_H
#define HSSVALUETOKEN_H

#include "HSSToken.h"

#define VALUE_TOKEN(thetoken) (static_cast<HSSValueToken *>(thetoken))

class HSSValueToken: public HSSToken
{
public:
	HSSValueToken(HSSTokenType type, string value);
    HSSValueToken(HSSTokenType type, char value);
	string value;
	bool equals(HSSTokenType otherType, string otherValue);
    string toString();
};

#endif