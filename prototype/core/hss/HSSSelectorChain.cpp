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

#include "HSSSelectorChain.h"
#include "AXR.h"
#include <iostream>

using namespace std;

HSSSelectorChain::HSSSelectorChain()
{
    
}

HSSSelectorChain::~HSSSelectorChain()
{
    for (unsigned i = 0; i<this->nodeList.size(); i++) {
        delete this->nodeList[i];
    }
    this->nodeList.clear();
}

string HSSSelectorChain::toString()
{
    string tempstr = "HSSSelectorChain with the following nodes:\n";
    int sccount = (int)this->nodeList.size();
    if(sccount > 0){
        
        unsigned i;
        for (i=0; i<sccount; i++) {
            tempstr.append("      ").append(this->nodeList[i]->toString()).append("\n");
        }
    }

    
    return tempstr;
    return "";
}

void HSSSelectorChain::add(HSSParserNode * newNode)
{
    if(newNode != NULL)
    {
        std_log3("Added node of type " << newNode->toString());
        this->nodeList.push_back(newNode);
    }
}

void HSSSelectorChain::removeLast()
{
    delete nodeList.back();
    this->nodeList.pop_back();
}

HSSParserNode * HSSSelectorChain::last()
{
    return this->nodeList.back();
}

const int HSSSelectorChain::size()
{
    return this->nodeList.size();
}
