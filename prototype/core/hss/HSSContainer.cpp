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

#include "HSSContainer.h"
#include "AXR.h"
#include <map>

HSSContainer::HSSContainer()
: HSSDisplayObject()
{
}

HSSContainer::HSSContainer(string name)
: HSSDisplayObject(name)
{
    
}

HSSContainer::~HSSContainer()
{
    unsigned i;
    for(i=0; i<this->children.size(); i++){
        //delete this->children.back();
        this->children.pop_back();
    }
}

string HSSContainer::toString()
{
    string tempstr;
    unsigned i;
    map<string, string>::iterator it;
    
    if (this->isNamed()) {
        tempstr = string("HSSContainer: ").append(this->name);
    } else {
        tempstr = "Annonymous HSSContainer";
    }
    
    if (this->attributes.size() > 0) {
        inc_output_indent();
        tempstr.append("\n").append(output_indent("with the following attributes:"));
        for(it=this->attributes.begin(); it!=this->attributes.end(); it++)
        {
            tempstr.append("\n").append(output_indent("- ").append((*it).first).append(": ").append((*it).second));
        }
        dec_output_indent();
        tempstr.append("\n");
    }
    
    if(this->children.size() > 0){
        inc_output_indent();
        tempstr.append("\n").append(output_indent("with the following children objects:"));
        for (i=0; i<this->children.size(); i++) {
            tempstr.append("\n").append(output_indent(this->children[i]->toString()));
        }
        dec_output_indent();
        tempstr.append("\n");
    }
    
    return tempstr;
}

string HSSContainer::defaultObjectType(string property)
{
    if (property == "shape"){
        return "roundedRect";
    } else if (property == "innerMargin"){
        return "margin";
    } else if (property == "background"){
        return "image";
    } else {
        return HSSDisplayObject::defaultObjectType(property);
    }
}

bool HSSContainer::isKeyword(string value, string property)
{
    if (value == "center"){
        if (   property == "contentAlignX"
            || property == "contentAlignY" ) {
            return true;
        }
    }
    
    //if we reached this far, let the superclass handle it
    return HSSDisplayObject::isKeyword(value, property);
}

void HSSContainer::add(HSSDisplayObject *child)
{
    this->children.push_back(child);
    child->setParent(this);
}

void HSSContainer::remove(unsigned index)
{
    //delete this->children[index];
    this->children.erase(this->children.begin()+index);
}


string HSSContainer::getContentText()
{
    return this->contentText;
}

void HSSContainer::setContentText(string newText)
{
    this->contentText = newText;
}









