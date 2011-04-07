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

#ifndef HSSRULE_H
#define HSSRULE_H

#import "HSSStatement.h"
#import "HSSSelectorChain.h"
#import <string>
#import <vector>
using namespace std;

class HSSRule : public HSSStatement
{    
public:
    HSSRule(HSSSelectorChain * selectorChain);
    ~HSSRule();
    
    HSSSelectorChain * selectorChain;
    
    string toString();
    
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
};

#endif