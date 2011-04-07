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

#include "AXRController.h"

using namespace AXR;

AXRController::AXRController()
{
    this->root = NULL;
    this->current = NULL;
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
}

std::string AXRController::toString()
{
    string tempstr;
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








