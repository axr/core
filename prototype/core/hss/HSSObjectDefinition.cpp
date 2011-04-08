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

#include "HSSObjectDefinition.h"
#include <string>
#include <iostream>
#include "AXR.h"

//this acquires ownership of the pointer
HSSObjectDefinition::HSSObjectDefinition(HSSObject * prototype)
{
    this->prototype = prototype;
}

HSSObjectDefinition::~HSSObjectDefinition()
{
    delete this->prototype;
}

string HSSObjectDefinition::toString()
{    
    string tempstr = string("HSSObjectDefinition with object of type:");
    tempstr.append(this->prototype->toString());
    
    const int pccount = this->propertiesSize();
    if(pccount > 0){
        tempstr.append(" and the following properties: \n");
        unsigned j;
        for (j=0; j<pccount; j++) {
            tempstr.append("   ").append(this->properties[j]->toString()).append("\n");
        }
    }
    
    return tempstr;
}

void HSSObjectDefinition::propertiesAdd(HSSStatement * newStatement)
{
    if(newStatement != NULL)
    {
        std_log3("Added node of type " << newStatement->toString());
        this->properties.push_back(newStatement);
    }
}

void HSSObjectDefinition::propertiesRemoveLast()
{
    delete this->properties.back();
    this->properties.pop_back();
}

HSSStatement * HSSObjectDefinition::propertiesLast()
{
    return this->properties.back();
}

const int HSSObjectDefinition::propertiesSize()
{
    return this->properties.size();
}