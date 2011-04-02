//
//  AXRController.cpp
//  mac
//
//  Created by Miro Keller on 02/04/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "AXRController.h"

using namespace AXR;

AXRController::AXRController()
{
    this->root = NULL;
    this->current = NULL;
}

AXRController::~AXRController()
{
    if(this->root != NULL)
    {
        delete this->root;
    }
}

std::string AXRController::toString()
{
    return this->root->toString();
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