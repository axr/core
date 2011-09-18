/********************************************************************
 *             a  A                                                        
 *            AM\/MA                                                         
 *           (MA:MMD                                                         
 *            :: VD
 *           ::  º                                                         
 *          ::                                                              
 *         ::   **      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6             
 +       6::Z. TMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6            
 *      6M:AMMJMMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6              
 *      ::  TMMTMC         ___MMMM     VMMMMMMM6       MMMM                   
 *     MMM  TMMMTTM,     AMMMMMMMM      VMMMMM6        MMMM                  
 *    :: MM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM                   
 *   ::   MMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM                   
 *  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM                   
 *         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM                   
 *        TMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM                   
 *       TMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM                   
 *      TMMM MMMM                                                           
 *     TMMM  .MMM                                         
 *     TMM   .MMD       ARBITRARY·······XML········RENDERING                           
 *     TMM    MMA       ====================================                              
 *     TMN    MM                               
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
 *      Last changed: 2011/09/18
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 6
 *
 ********************************************************************/

#include "HSSSelectorChain.h"
#include <iostream>
#include "../../axr/AXRDebugging.h"

using namespace AXR;

HSSSelectorChain::HSSSelectorChain()
{
    
}

HSSSelectorChain::~HSSSelectorChain()
{
    this->nodeList.clear();
}

std::string HSSSelectorChain::toString()
{
    std::string tempstr = "HSSSelectorChain with the following nodes:\n";
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


const HSSParserNode::p & HSSSelectorChain::operator[] (const int i) const
{
    return this->nodeList[i];
}

const HSSParserNode::p & HSSSelectorChain::get(const int i) const
{
    return this->nodeList[i];
}

void HSSSelectorChain::add(HSSParserNode::p newNode)
{
    if(newNode != NULL)
    {
        std_log3("HSSSelectorChain: Added node of type " << newNode->toString());
        this->nodeList.push_back(newNode);
    }
}

void HSSSelectorChain::prepend(HSSParserNode::p newNode)
{
    if(newNode != NULL)
    {
        std_log3("HSSSelectorChain: Added node of type " << newNode->toString() + " to the front of the selector");
        this->nodeList.push_front(newNode);
    }
}

void HSSSelectorChain::removeLast()
{
    this->nodeList.pop_back();
}

HSSParserNode::p HSSSelectorChain::last()
{
    return this->nodeList.back();
}

const int HSSSelectorChain::size()
{
    return this->nodeList.size();
}
