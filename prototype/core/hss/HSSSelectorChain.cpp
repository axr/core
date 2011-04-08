/********************************************************************
 *             a  A                                                        
 *            AM\/MA                                                         
 *           (MMMMMD                                                         
 *            :: VD
 *           ::  .                                                         
 *          ::                                                              
 *          M   NZ      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6             
 +       .MMZ. MMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6            
 *      AMMMMMDJMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6              
 *      MM  MMMMMC         ___MMMM     VMMMMMMM6       MMMM                   
 *      MM  MMMMMMM,     AMMMMMMMM      VMMMMM6        MMMM                  
 *     :MM AMMMMMMMD    MMMMMMMMMM       MMMMMM        MMMM                   
 *    :: MMMMMMMMMM    MMMMMMMMMMM      AMMMMMMD       MMMM                   
 *   .     MMMMM~     MMMM    MMMM     AMMMMMMMMD      MMMM                   
 *         MMMMM      MMMM    MMMM    AMMM6  MMMMD     MMMM                   
 *        MMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM                   
 *       MMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM                   
 *      MMMM MMMM                                                           
 *     AMMM  .MMM                                         
 *     MMM   .MMD       ARBITRARY·······XML········RENDERING                           
 *     MMM    MMA       ====================================                              
 *     DMN    MM                               
 *      MN    ZM                       
 *            MM,
 *
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
 ********************************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2011/04/04
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 ********************************************************************/

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
