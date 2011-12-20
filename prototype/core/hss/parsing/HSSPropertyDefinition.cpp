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
 *      Last changed: 2011/12/19
 *      HSS version: 1.0
 *      Core version: 0.42
 *      Revision: 5
 *
 ********************************************************************/

#include "HSSPropertyDefinition.h"
#include "HSSMultipleValueDefinition.h"

using namespace AXR;

HSSPropertyDefinition::HSSPropertyDefinition()
{
    this->name = "";
    this->type = HSSStatementTypePropertyDefinition;
}

HSSPropertyDefinition::HSSPropertyDefinition(std::string name)
{
    this->name = name;
}

HSSPropertyDefinition::HSSPropertyDefinition(std::string name, HSSParserNode::p value)
{
    this->name = name;
}

HSSPropertyDefinition::~HSSPropertyDefinition()
{
    
}

std::string HSSPropertyDefinition::toString()
{
    std::string ret = "HSSPropertyDefinition: " + this->name;
    if(this->value != NULL){
        ret.append(" with value: "+this->value->toString());
    }
    return ret;
}

void HSSPropertyDefinition::setName(std::string name)
{
    this->name = name;
}

std::string HSSPropertyDefinition::getName()
{
    return this->name;
}

void HSSPropertyDefinition::setValue(HSSParserNode::p value){
    this->value = value;
}

void HSSPropertyDefinition::addValue(HSSParserNode::p value)
{
    if (this->value){
        if(this->value->isA(HSSParserNodeTypeMultipleValueDefinition)) {
            HSSMultipleValueDefinition::p mvDef = boost::static_pointer_cast<HSSMultipleValueDefinition>(this->value);
            mvDef->getValues().push_back(value);
        } else {
            HSSMultipleValueDefinition::p mvDef = HSSMultipleValueDefinition::p(new HSSMultipleValueDefinition());
            mvDef->add(this->value);
            mvDef->add(value);
            this->value = mvDef;
        }
    } else {
        this->value = value;
    }
}

HSSParserNode::p HSSPropertyDefinition::getValue()
{
    return this->value;
}

