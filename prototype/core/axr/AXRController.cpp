/********************************************************************
 *             a  A                                                        
 *            AM\/MA                                                         
 *           (MA:MMD                                                         
 *            :: VD
 *           ::  º                                                         
 *          ::                                                              
 *         ::   NZ      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6             
 +       6MMZ. MMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6            
 *      6MDAMMDJMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6              
 *      $$  MMMMMC         ___MMMM     VMMMMMMM6       MMMM                   
 *     CMM  MMMMMMM,     AMMMMMMMM      VMMMMM6        MMMM                  
 *    ::MMM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM                   
 *   ::  MMMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM                   
 *  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM                   
 *         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM                   
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
 *      Last changed: 2011/04/09
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 2
 *
 ********************************************************************/

#include "AXRController.h"

using namespace AXR;

AXRController::AXRController()
{
    this->root = NULL;
    this->current = NULL;
    this->cairo = NULL;
}

AXRController::~AXRController()
{
    unsigned i;
    if(this->root != NULL)
    {
        delete this->root;
    }
    for (i=0; i<this->objectTree.size(); i++) {
        this->objectTree.pop_back();
    }
    if(this->cairo != NULL)
    {
        cairo_destroy(this->cairo);
    }
}

std::string AXRController::toString()
{
    std::string tempstr;
    if(this->root != NULL){
      tempstr = this->root->toString();  
    }
    unsigned i;
    if(this->objectTree.size() > 0)
    {
        for (i=0; i<this->objectTree.size(); i++) {
            tempstr.append("\n").append(this->objectTree[i]->toString());
        }
    }
    
    return tempstr;
}

HSSContainer * AXRController::getRoot()
{
    return this->root;
}

void AXRController::setRoot(HSSContainer * newRoot){
    delete this->root;
    this->root = newRoot;
}

void AXRController::add(HSSContainer * newContainer)
{
    if(this->root == NULL){
        this->root = newContainer;
    } else {
        this->current->add(newContainer);
    }
}

HSSContainer * AXRController::getCurrent()
{
    return this->current;
}

void AXRController::setCurrent(HSSContainer *newCurrent)
{
    this->current = newCurrent;
}

void AXRController::objectTreeAdd(HSSObject * newObject)
{
    this->objectTree.push_back(newObject);
}

void AXRController::objectTreeRemove(unsigned index)
{
    this->objectTree.erase(this->objectTree.begin()+index);
}

HSSObject * AXRController::objectTreeGet(unsigned index)
{
    return this->objectTree[index];
}








