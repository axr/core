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
 *      Last changed: 2011/10/22
 *      HSS version: 1.0
 *      Core version: 0.4
 *      Revision: 30
 *
 ********************************************************************/

#include "HSSContainer.h"
#include "../../axr/AXRDebugging.h"
#include "../../axr/errors/errors.h"
#include "../../axr/AXRController.h"
#include "../parsing/HSSObjectDefinition.h"
#include "../parsing/HSSExpression.h"
#include "../parsing/HSSConstants.h"
#include "../parsing/HSSObjectNameConstant.h"
#include <map>
#include <string>
#include <sstream>
#include <boost/pointer_cast.hpp>
#include <cmath>
#include "HSSShapes.h"
#include <vector>

using namespace AXR;


HSSDisplayObject::p HSSContainer::asDisplayObject(HSSContainer::p theContainer)
{
    return boost::static_pointer_cast<HSSDisplayObject>(theContainer);
}

//always test the return of this function. E.g. "HSSContainer::p cont = HSSContainer::asContainer(myDO); if(cont) { /* ... */ }
HSSContainer::p HSSContainer::asContainer(HSSDisplayObject::p theDisplayObject)
{
    HSSContainer::p ret;
    if (theDisplayObject->isA(HSSObjectTypeContainer)) {
        ret = boost::static_pointer_cast<HSSContainer>(theDisplayObject);
    }
    return ret;
}

HSSContainer::HSSContainer()
: HSSDisplayObject()
{
    this->contentText = std::string();
    this->type = HSSObjectTypeContainer;
    
    this->contentAlignX = this->contentAlignY = 0;
    this->directionPrimary = HSSDirectionLeftToRight;
    this->directionSecondary = HSSDirectionTopToBottom;
    
    this->observedContentAlignX = this->observedContentAlignY
    = this->observedDirectionPrimary = this->observedDirectionSecondary
    = this->observedShape
    = NULL;
    
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("isA");
    shorthandProperties.push_back("width");
    shorthandProperties.push_back("height");
    shorthandProperties.push_back("contentAlignX");
    shorthandProperties.push_back("contentAlignY");
    shorthandProperties.push_back("font");
    shorthandProperties.push_back("background");
    shorthandProperties.push_back("shape");
    
    this->setShorthandProperties(shorthandProperties);
}

HSSContainer::HSSContainer(std::string name)
: HSSDisplayObject(name)
{
    this->contentText = std::string();
    this->type = HSSObjectTypeContainer;
    
    this->contentAlignX = this->contentAlignY = 0;
    this->directionPrimary = HSSDirectionLeftToRight;
    this->directionSecondary = HSSDirectionTopToBottom;
    
    this->observedContentAlignX = this->observedContentAlignY
    = this->observedDirectionPrimary = this->observedDirectionSecondary
    = this->observedShape
    = NULL;
    
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("isA");
    shorthandProperties.push_back("width");
    shorthandProperties.push_back("height");
    shorthandProperties.push_back("contentAlignX");
    shorthandProperties.push_back("contentAlignY");
    shorthandProperties.push_back("font");
    shorthandProperties.push_back("background");
    shorthandProperties.push_back("shape");
    
    this->setShorthandProperties(shorthandProperties);
}

HSSContainer::~HSSContainer()
{
    unsigned i;
    for(i=0; i<this->children.size(); i++){
        this->children.pop_back();
    }
}

std::string HSSContainer::toString()
{
    std::string tempstr;
    unsigned i;
    std::map<std::string, std::string>::iterator it;
    
    if (this->isNamed()) {
        tempstr = std::string("HSSContainer: ").append(this->name);
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

std::string HSSContainer::defaultObjectType()
{
    return "container";
}

std::string HSSContainer::defaultObjectType(std::string property)
{
    if (property == "shape"){
        return "roundedRect";
    } else if (property == "innerMargin"){
        return "margin";
    } else if (property == "background"){
        return "rgba";
    } else {
        return HSSDisplayObject::defaultObjectType(property);
    }
}

bool HSSContainer::isKeyword(std::string value, std::string property)
{
    if (value == "center"){
        if (property == "contentAlignX" || property == "contentAlignY") {
            return true;
        }
    } else if (value == "leftToRight" || value == "rightToLeft" || value == "topToBottom" || value == "bottomToTop"){
        if ( property == "directionPrimary" || property == "directionSecondary"){
            return true;
        }
    }
    
    //if we reached this far, let the superclass handle it
    return HSSDisplayObject::isKeyword(value, property);
}

void HSSContainer::add(HSSDisplayObject::p child)
{
    HSSContainer::p sharedThis = this->shared_from_this();
    child->setParent(sharedThis);
    std_log1("HSSContainer: added child "+child->getElementName()+" to "+this->getElementName());
    child->setIndex(this->children.size());
    this->children.push_back(child);
}

void HSSContainer::remove(unsigned index)
{
    this->children.erase(this->children.begin()+index);
    this->resetChildrenIndexes();
}

void HSSContainer::resetChildrenIndexes()
{
    unsigned i,size;
    for (i=0, size=this->children.size(); i<size; i++) {
        this->children[i]->setIndex(i);
    }
}

void HSSContainer::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name) {
        case HSSObservablePropertyContentAlignX:
            this->setDContentAlignX(value);
            break;
        case HSSObservablePropertyContentAlignY:
            this->setDContentAlignY(value);
            break;
        case HSSObservablePropertyDirectionPrimary:
            this->setDDirectionPrimary(value);
            break;
        case HSSObservablePropertyDirectionSecondary:
            this->setDDirectionSecondary(value);
            break;
        case HSSObservablePropertyShape:
            this->setDShape(value);
            break;
            
        default:
            HSSDisplayObject::setProperty(name, value);
            break;
    }
}

void HSSContainer::setProperty(HSSObservableProperty name, void * value)
{
    switch (name) {
        case HSSObservablePropertyContentAlignX:
            this->contentAlignX = *(long double*) value;
            break;
        case HSSObservablePropertyContentAlignY:
            this->contentAlignY = *(long double*) value;
            break;
        case HSSObservablePropertyDirectionPrimary:
            this->directionPrimary = *(HSSDirectionValue*) value;
            break;
        case HSSObservablePropertyDirectionSecondary:
            this->directionSecondary = *(HSSDirectionValue*) value;
            break;
        case HSSObservablePropertyShape:
            this->shape = *(HSSShape::p*) value;
            break;
            
        default:
            return HSSDisplayObject::setProperty(name, value);
    }
    
    this->notifyObservers(name, value);
}

void HSSContainer::recursiveReadDefinitionObjects()
{
    if (this->_needsRereadRules) {
        this->readDefinitionObjects();
    }
    
    unsigned i;
    for (i=0; i<this->children.size(); i++) {
        this->children[i]->recursiveReadDefinitionObjects();
    }
}

void HSSContainer::recursiveRegenerateSurfaces()
{
    this->regenerateSurfaces();
    
    unsigned i, size;
    for (i=0, size = this->children.size(); i<size; i++) {
        this->children[i]->regenerateSurfaces();
    }
}

void HSSContainer::recursiveDraw(cairo_t * cairo)
{
    this->draw(cairo);
    unsigned i, size;
    for (i=0, size = this->children.size(); i<size; i++) {
        this->children[i]->recursiveDraw(cairo);
    }
}

void HSSContainer::drawBackground()
{
    long double r = 0., g = 0., b = 0., a = 0;
    if(this->backgroundColor){
        r = this->backgroundColor->getRed();
        g = this->backgroundColor->getGreen();
        b = this->backgroundColor->getBlue();
        a = this->backgroundColor->getAlpha();
    }
    
    cairo_t * cairo = cairo_create(this->backgroundSurface);
    this->shape->draw(cairo, this->width, this->height);
    cairo_set_source_rgba(cairo, (r/255), (g/255), (b/255), (a/255));
    cairo_fill(cairo);
    
    
    
    //draw a small rectangle to be able to see the anchor X and Y
    //    cairo_rectangle(cairo, this->anchorX-1., this->anchorY-1., 2, 2);
    //    cairo_set_source_rgb(cairo, 1,0,0);
    //    cairo_fill(cairo);
    
    cairo_destroy(cairo);
}

//void HSSContainer::drawShape(cairo_t *cairo)
//{
//    switch (this->shape->getShapeType()) {
//        case HSSShapeTypeRectangle:
//            cairo_rectangle(cairo, 0., 0., this->width, this->height);
//            break;
//        case HSSShapeTypeCircle:
//        {
//            double long halfWidth = this->width * 0.5;
//            cairo_arc(cairo, halfWidth, this->height * 0.5, halfWidth, 0., 2*M_PI);
//            break;
//        }
//            
//        default:
//            throw AXRError::p(new AXRError("HSSContainer", "Unknown shape type"));
//            break;
//    }
//}

void HSSContainer::layout()
{
    //reset flag
    this->_needsLayout = false;
    std::vector<displayGroup>primaryGroups;
    std::vector<displayGroup>secondaryGroups;
    unsigned i, size, j, k;
    //long double acc2 = 0;
    security_brake_init(); 
    
    //bool secondaryIsHorizontal = (this->directionSecondary == HSSDirectionLeftToRight || this->directionSecondary == HSSDirectionRightToLeft);
    
    //create groups and lines
    for (i=0, size = this->children.size(); i<size; i++) {
        HSSDisplayObject::p child = this->children[i];
        //place it on the alignment point
        //horizontal
        child->x = child->alignX - child->anchorX;
        if ((child->x + child->width) > this->width) child->x = this->width - child->width;
        if (child->x < 0) child->x = 0;
        //vertical
        child->y = child->alignY - child->anchorY;
        if ((child->y + child->height) > this->height) child->y = this->height - child->height;
        if (child->y < 0) child->y = 0;
        
        bool addedToGroup = false;
        
        if( i!=0 ) {
            j = 0;
            while (j<primaryGroups.size()) {
                if(primaryGroups[j].lines.size() == 0){
                    displayGroup & currentPGroup = primaryGroups[j];
                    addedToGroup = this->_addChildToGroupIfNeeded(child, currentPGroup, this->directionPrimary);
                    if (!addedToGroup && currentPGroup.complete){
                        //transform the current group into a line
                        displayGroup newGroup;
                        newGroup.x = currentPGroup.x;
                        newGroup.y = currentPGroup.y;
                        newGroup.width = currentPGroup.width;
                        newGroup.height = currentPGroup.height;
                        newGroup.complete = false;
                        newGroup.lines.push_back(currentPGroup);
                        
                        displayGroup newLine;
                        newLine.x = child->x;
                        newLine.y = child->y;
                        newLine.width = child->width;
                        newLine.height = child->height;
                        newLine.complete = false;
                        newLine.objects.push_back(child);
                        newGroup.lines.push_back(newLine);
                        
                        primaryGroups[j] = newGroup;
                        
                        addedToGroup = true;
                    }
                    
                    if(addedToGroup){
                        k=0;
                        while (k<primaryGroups.size()){
                            if(k != j){
                                displayGroup & otherPGroup = primaryGroups[k];
                                bool merged = this->_mergeGroupsIfNeeded(otherPGroup, currentPGroup,  this->directionPrimary);
                                if(merged){
                                    primaryGroups.erase(primaryGroups.begin()+j);
                                    j = k;
                                } else {
                                    k++;
                                }
                            } else {
                                k++;
                            }
                        }
                    }
                    
                } else {
                    displayGroup & currentPGroup = primaryGroups[j].lines.back();
                    addedToGroup = this->_addChildToGroupIfNeeded(child, currentPGroup, this->directionPrimary);
                    if (!addedToGroup && currentPGroup.complete){
                        //create new line
                        displayGroup newLine;
                        newLine.x = child->x;
                        newLine.y = child->y;
                        newLine.width = child->width;
                        newLine.height = child->height;
                        newLine.complete = false;
                        newLine.objects.push_back(child);
                        primaryGroups[j].lines.push_back(newLine);
                        
                        addedToGroup = true;
                    }
                    
                    if(addedToGroup){
                        k=0;
                        while (k<primaryGroups.size()){
                            if(k != j){
                                displayGroup & otherPGroup = primaryGroups[k];
                                bool merged = this->_mergeGroupsIfNeeded(otherPGroup, currentPGroup,  this->directionPrimary);
                                if(merged){
                                    primaryGroups.erase(primaryGroups.begin()+j);
                                    j = k;
                                } else {
                                    k++;
                                }
                            } else {
                                k++;
                            }
                        }
                    }
                }
                
                j++;
                security_brake();
            }
        }
        
        if(!addedToGroup){
            displayGroup newGroup;
            newGroup.x = child->x;
            newGroup.y = child->y;
            newGroup.width = child->width;
            newGroup.height = child->height;
            newGroup.complete = false;
            newGroup.objects.push_back(child);
            primaryGroups.push_back(newGroup);
        }
        
        
    }
    
    security_brake_reset();
    
    //now align the lines in the secondary direction
    for(i=0, size = this->children.size(); i<size; i++){
        HSSDisplayObject::p &child = this->children[i];
        
        bool addedToGroup = false;
        if( i!=0 ) {
            j = 0;
            while (j<secondaryGroups.size()) {
                displayGroup & currentSGroup = secondaryGroups[j];
                addedToGroup = this->_addChildToGroupIfNeeded(child, currentSGroup, this->directionSecondary);
                
                if(addedToGroup){
                    k=0;
                    while (k<secondaryGroups.size()){
                        if(k != j){
                            displayGroup & otherSGroup = secondaryGroups[k];
                            bool merged = this->_mergeGroupsIfNeeded(otherSGroup, currentSGroup,  this->directionSecondary);
                            if(merged){
                                secondaryGroups.erase(secondaryGroups.begin()+j);
                                j = k;
                            } else {
                                k++;
                            }
                        } else {
                            k++;
                        }
                    }
                }
                
                j++;
                security_brake();
            }
        }
        if(!addedToGroup){
            displayGroup newGroup;
            newGroup.x = child->x;
            newGroup.y = child->y;
            newGroup.width = child->width;
            newGroup.height = child->height;
            newGroup.complete = false;
            newGroup.objects.push_back(child);
            secondaryGroups.push_back(newGroup);
        }
    }
    //assign the globalX and globalY
    for(i=0, size = this->children.size(); i<size; i++){
        HSSDisplayObject::p &child = this->children[i];
        child->setGlobalX(floor(this->globalX + child->x));
        child->setGlobalY(floor(this->globalY + child->y));
    }
    
    if(this->heightByContent){
        long double maxHeight = 0.;
        
        for (i=0, size = secondaryGroups.size(); i<size; i++) {
            if(secondaryGroups[i].height > maxHeight){
                maxHeight = secondaryGroups[i].height;
            }
        }
        if(size > 0){
            this->height = maxHeight;
            this->setNeedsSurface(true);
            this->setDirty(true);
            this->notifyObservers(HSSObservablePropertyHeight, &this->height);
        }
    }
}

bool HSSContainer::_addChildToGroupIfNeeded(HSSDisplayObject::p &child, AXR::HSSContainer::displayGroup &group, HSSDirectionValue direction)
{
    unsigned i, size;
    bool isHorizontal = (direction == HSSDirectionLeftToRight || direction == HSSDirectionRightToLeft);
    bool addedToGroup = false;
    long double lineTotalPrimary = 0;
    
    long double originalX = child->x;
    long double originalY = child->y;
    
    for (i=0, size = group.objects.size(); i<size; i++) {
        HSSDisplayObject::p & otherChild = group.objects[i];
        if( isHorizontal ){
            lineTotalPrimary += otherChild->width;
        } else {
            lineTotalPrimary += otherChild->height;
        }
        if (
            ((child->x + child->width)  > otherChild->x) && (child->x < (otherChild->x + otherChild->width))
            && ((child->y + child->height) > otherChild->y) && (child->y < (otherChild->y + otherChild->height))
            ){
            //it will go into a group
            
            //if it is the last one
            if(i>=size-1){
                //check if we have enough space to add it to the end of the line
                if( isHorizontal ){
                    if( lineTotalPrimary + child->width > this->width){
                        group.complete = true;
                    }
                } else {
                    if( lineTotalPrimary + child->height > this->height){
                        group.complete = true;
                    }
                }
                
                if (!group.complete){
                    //put it into the group
                    group.objects.push_back(child);
                    addedToGroup = true;
                    
                    switch (direction) {
                        case HSSDirectionTopToBottom:
                        case HSSDirectionBottomToTop:
                            group.height += child->height;
                            break;
                            
                        case HSSDirectionRightToLeft:
                        default:
                            group.width += child->width;
                            break;
                    }
                    
                    this->_arrange(group, direction);
                } else {
                    //restore the original position
                    child->x = originalX;
                    child->y = originalY;
                }
                
            } else {
                
                //push it further in the primary direction, and check again
                switch (direction) {
                    case HSSDirectionRightToLeft:
                    {
                        child->x = otherChild->x - child->width;
                        break;
                    }
                        
                    case HSSDirectionTopToBottom:
                    {
                        child->y = otherChild->y + otherChild->height;
                        break;
                    }
                        
                    case HSSDirectionBottomToTop:
                    {
                        child->y = otherChild->y - child->height;
                        break;
                    }
                        
                    default:
                    {
                        child->x = otherChild->x + otherChild->width;
                        break;
                    }
                }
                
            }
        }//if overlap
        
    }// for each child
    
    return addedToGroup;
}

bool HSSContainer::_mergeGroupsIfNeeded(displayGroup &group, displayGroup &otherGroup, HSSDirectionValue direction)
{
    unsigned i, size, j, size2;
    if (
        ((group.x + group.width) > otherGroup.x) && (group.x < (otherGroup.x + otherGroup.width))
        && ((group.y + group.height) > otherGroup.y) && (group.y < (otherGroup.y + otherGroup.height))
        ){
        //if the group bounds overlap, check each individual element against each other
        for (i=0, size = group.objects.size(); i<size; i++) {
            HSSDisplayObject::p & child = group.objects[i];
            for (j=0, size2 = otherGroup.objects.size(); j<size; j++) {
                HSSDisplayObject::p &otherChild = otherGroup.objects[j];
                if (
                    ((child->x + child->width)  > otherChild->x) && (child->x < (otherChild->x + otherChild->width))
                    && ((child->y + child->height) > otherChild->y) && (child->y < (otherChild->y + otherChild->height))
                    ){
                    //there is an overlap, merge the groups
                    //add all the elements of this group to the overlapping one
                    group.objects.insert(group.objects.end(), otherGroup.objects.begin(), otherGroup.objects.end());
                    if(direction == HSSDirectionLeftToRight || direction == HSSDirectionRightToLeft){
                        group.width += otherGroup.width;
                    } else {
                        group.height += otherGroup.height;
                    }
                    
                    this->_arrange(group, direction);
                       
                    return true;
                }
            }
        }
    }
    
    
    
    return false;
}

void HSSContainer::_arrange(displayGroup &group, HSSDirectionValue direction)
{
    unsigned i, size;
    
    switch (direction) {
        case HSSDirectionRightToLeft:
        {            
            //calculate the new alignment and anchor point for the group
            HSSDisplayObject::p & groupFirst = group.objects.front();
            long double alignmentTotal = 0;
            long double accWidth = groupFirst->anchorX;
            long double anchorsTotal = 0;
            for (i=0, size = group.objects.size(); i<size; i++) {
                HSSDisplayObject::p & currentChild = group.objects[i];
                alignmentTotal += currentChild->alignX;
                if(i>0){
                    anchorsTotal += accWidth + currentChild->anchorX;
                    accWidth += currentChild->width;
                }
            }
            double long groupAlignX = alignmentTotal / size;
            double long groupAnchorX = anchorsTotal / size;
            
            //reposition the elements in the group
            double long startX = groupAlignX +  groupAnchorX + (groupFirst->width - groupFirst->anchorX);
            if(startX - group.width < 0) startX = group.width;
            if(startX > this->width) startX = this->width;
            accWidth = 0;
            for (i=0, size = group.objects.size(); i<size; i++) {
                HSSDisplayObject::p & otherChild2 = group.objects[i];
                otherChild2->x = startX - otherChild2->width - accWidth;
                accWidth += otherChild2->width;
            }
            group.x = group.objects.front()->x;
            group.y = group.objects.front()->y;
            break;
        }
            
        case HSSDirectionTopToBottom:
        {            
            //calculate the new alignment and anchor point for the group
            HSSDisplayObject::p & groupFirst = group.objects.front();
            long double alignmentTotal = 0;
            long double accHeight = groupFirst->height - groupFirst->anchorY;
            long double anchorsTotal = 0;
            for (i=0, size = group.objects.size(); i<size; i++) {
                HSSDisplayObject::p & currentChild = group.objects[i];
                alignmentTotal += currentChild->alignY;
                if(i>0){
                    anchorsTotal += accHeight + currentChild->anchorY;
                    accHeight += currentChild->height;
                }
            }
            double long groupAlignY = alignmentTotal / size;
            double long groupAnchorY = anchorsTotal / size;
            
            //reposition the elements in the group
            double long startY = groupAlignY - groupAnchorY - groupFirst->anchorY;
            if(startY > this->height - group.height) startY = this->height - group.height;
            if(startY < 0) startY = 0;
            accHeight = 0;
            for (i=0, size = group.objects.size(); i<size; i++) {
                HSSDisplayObject::p & otherChild2 = group.objects[i];
                otherChild2->y = startY + accHeight;
                accHeight += otherChild2->height;
            }
            group.x = group.objects.front()->x;
            group.y = group.objects.front()->y;
            break;
        }
            
        case HSSDirectionBottomToTop:
        {            
            //calculate the new alignment and anchor point for the group
            HSSDisplayObject::p & groupFirst = group.objects.front();
            long double alignmentTotal = 0;
            long double accHeight = groupFirst->anchorY;
            long double anchorsTotal = 0;
            for (i=0, size = group.objects.size(); i<size; i++) {
                HSSDisplayObject::p & currentChild = group.objects[i];
                alignmentTotal += currentChild->alignY;
                if(i>0){
                    anchorsTotal += accHeight + currentChild->anchorY;
                    accHeight += currentChild->height;
                }
            }
            double long groupAlignY = alignmentTotal / size;
            double long groupAnchorY = anchorsTotal / size;
            
            //reposition the elements in the group
            double long startY = groupAlignY +  groupAnchorY + (groupFirst->height - groupFirst->anchorY);
            if(startY - group.height < 0) startY = group.height;
            if(startY > this->height) startY = this->height;
            accHeight = 0;
            for (i=0, size = group.objects.size(); i<size; i++) {
                HSSDisplayObject::p & otherChild2 = group.objects[i];
                otherChild2->y = startY - otherChild2->height - accHeight;
                accHeight += otherChild2->height;
            }
            group.x = group.objects.front()->x;
            group.y = group.objects.front()->y;
            break;
        }
            
        default:
        {            
            //calculate the new alignment and anchor point for the group
            HSSDisplayObject::p & groupFirst = group.objects.front();
            long double alignmentTotal = 0;
            long double accWidth = groupFirst->width - groupFirst->anchorX;
            long double anchorsTotal = 0;
            for (i=0, size = group.objects.size(); i<size; i++) {
                HSSDisplayObject::p & currentChild = group.objects[i];
                alignmentTotal += currentChild->alignX;
                if(i>0){
                    anchorsTotal += accWidth + currentChild->anchorX;
                    accWidth += currentChild->width;
                }
            }
            double long groupAlignX = alignmentTotal / size;
            double long groupAnchorX = anchorsTotal / size;
            
            //reposition the elements in the group
            double long startX = groupAlignX - groupAnchorX - groupFirst->anchorX;
            if(startX > this->width - group.width) startX = this->width - group.width;
            if(startX < 0) startX = 0;
            accWidth = 0;
            for (i=0, size = group.objects.size(); i<size; i++) {
                HSSDisplayObject::p & otherChild2 = group.objects[i];
                otherChild2->x = startX + accWidth;
                accWidth += otherChild2->width;
            }
            group.x = group.objects.front()->x;
            group.y = group.objects.front()->y;
            break;
        }
    }
}

void HSSContainer::recursiveLayout()
{
    this->layout();
    unsigned i, size;
    for (i=0, size = this->children.size(); i<size; i++) {
        this->children[i]->recursiveLayout();
    }
}

void HSSContainer::setGlobalX(long double newValue)
{
    long double delta = newValue - this->globalX;
    HSSDisplayObject::setGlobalX(newValue);
    unsigned i, size;
    for (i=0, size=this->children.size(); i<size; i++){
        HSSDisplayObject::p theChild = this->children[i];
        theChild->setGlobalX(theChild->globalX + delta);
    }
}

void HSSContainer::setGlobalY(long double newValue)
{
    long double delta = newValue - this->globalY;
    HSSDisplayObject::setGlobalY(newValue);
    unsigned i, size;
    for (i=0, size=this->children.size(); i<size; i++){
        HSSDisplayObject::p theChild = this->children[i];
        theChild->setGlobalY(theChild->globalY + delta);
    }
}

void HSSContainer::setChildren(std::vector<HSSDisplayObject::p> newChildren)
{
    this->children = newChildren;
    unsigned i, size;
    for (i=0, size = this->children.size(); i<size; i++) {
        this->children[i]->setParent(this->shared_from_this());
    }
}

const std::vector<HSSDisplayObject::p>& HSSContainer::getChildren() const
{
    return this->children;
}

//contentAlignX
HSSParserNode::p HSSContainer::getDContentAlignX() { return this->dContentAlignX; }
void HSSContainer::setDContentAlignX(HSSParserNode::p value)
{
    this->dContentAlignX = value;
    if(this->observedContentAlignX != NULL)
    {
        this->observedContentAlignX->removeObserver(this->observedContentAlignXProperty, HSSObservablePropertyAlignX, this);
    }
    HSSContainer::p parentContainer = this->getParent();
    const std::vector<HSSDisplayObject::p> * scope;
    if(parentContainer){
        scope = &(parentContainer->getChildren());
    } else {
        scope = NULL;
    }
    this->contentAlignX = this->_setLDProperty(
                                        &HSSContainer::contentAlignXChanged,
                                        value,
                                        this->width,
                                        HSSObservablePropertyWidth,
                                        this,
                                        HSSObservablePropertyAlignX,
                                        this->observedContentAlignX,
                                        this->observedContentAlignXProperty,
                                        scope
                                        );
    this->notifyObservers(HSSObservablePropertyContentAlignX, &this->contentAlignX);
#if AXR_DEBUG_LEVEL > 0
    this->setDirty(true);
#endif
}

void HSSContainer::contentAlignXChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dContentAlignX->getType();
    switch (nodeType) {
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant>(this->dContentAlignX);
            this->contentAlignX = percentageValue->getValue(*(long double*)data);
            break;
        }
            
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression>(this->dContentAlignX);
            this->contentAlignX = expressionValue->evaluate();
            break;
        }
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyContentAlignX, &this->contentAlignX);
#if AXR_DEBUG_LEVEL > 0
    this->setDirty(true);
#endif
}

//contentAlignY
HSSParserNode::p HSSContainer::getDContentAlignY() { return this->dContentAlignX; }
void HSSContainer::setDContentAlignY(HSSParserNode::p value)
{
    this->dContentAlignY = value;
    HSSObservableProperty observedProperty = HSSObservablePropertyHeight;
    if(this->observedContentAlignY != NULL)
    {
        this->observedContentAlignY->removeObserver(this->observedContentAlignYProperty, HSSObservablePropertyAlignY, this);
    }
    HSSContainer::p parentContainer = this->getParent();
    const std::vector<HSSDisplayObject::p> * scope;
    if(parentContainer){
        scope = &(parentContainer->getChildren());
    } else {
        scope = NULL;
    }
    this->contentAlignY = this->_setLDProperty(
                                        &HSSContainer::contentAlignYChanged,
                                        value,
                                        this->height,
                                        observedProperty,
                                        this,
                                        HSSObservablePropertyAlignY,
                                        this->observedContentAlignY,
                                        this->observedContentAlignYProperty,
                                        scope
                                        );
    this->notifyObservers(HSSObservablePropertyContentAlignY, &this->contentAlignY);
#if AXR_DEBUG_LEVEL > 0
    this->setDirty(true);
#endif
}

void HSSContainer::contentAlignYChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dContentAlignY->getType();
    switch (nodeType) {
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant>(this->dContentAlignY);
            this->contentAlignY = percentageValue->getValue(*(long double*)data);
            break;
        }
            
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression>(this->dContentAlignY);
            this->contentAlignY = expressionValue->evaluate();
            break;
        }
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyContentAlignY, &this->contentAlignY);
#if AXR_DEBUG_LEVEL > 0
    this->setDirty(true);
#endif
}


//directionPrimary
HSSParserNode::p HSSContainer::getDDirectionPrimary() { return this->dDirectionPrimary; }
void HSSContainer::setDDirectionPrimary(HSSParserNode::p value)
{
    this->dDirectionPrimary = value;
    if(this->observedDirectionPrimary != NULL)
    {
        this->observedDirectionPrimary->removeObserver(this->observedDirectionPrimaryProperty, HSSObservablePropertyDirectionPrimary, this);
    }
    
    if (value->isA(HSSParserNodeTypeKeywordConstant)){
        std::string stringValue = boost::static_pointer_cast<HSSKeywordConstant>(value)->getValue();
        if(stringValue == "leftToRight" || stringValue == "rightToLeft"){
            if (this->directionSecondary == HSSDirectionLeftToRight || this->directionSecondary == HSSDirectionRightToLeft){
                this->directionSecondary = HSSDirectionTopToBottom;
                this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
            }
            if(stringValue == "leftToRight"){
                this->directionPrimary = HSSDirectionLeftToRight;
            } else {
                this->directionPrimary = HSSDirectionRightToLeft;
            }
            
        } else if (stringValue == "topToBottom" || stringValue == "bottomToTop") {
            if (this->directionSecondary == HSSDirectionTopToBottom || this->directionSecondary == HSSDirectionBottomToTop){
                this->directionSecondary = HSSDirectionLeftToRight;
                this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
            }
            if(stringValue == "topToBottom"){
                this->directionPrimary = HSSDirectionTopToBottom;
            } else {
                this->directionPrimary = HSSDirectionBottomToTop;
            }
        }
    }
    
    this->notifyObservers(HSSObservablePropertyDirectionPrimary, &this->directionPrimary);
}

void HSSContainer::directionPrimaryChanged(HSSObservableProperty source, void *data)
{
    if(this->dDirectionPrimary->isA(HSSParserNodeTypeKeywordConstant)){
        
    }
    
    this->notifyObservers(HSSObservablePropertyDirectionPrimary, &this->directionPrimary);
}

//directionSecondary
HSSParserNode::p HSSContainer::getDDirectionSecondary() { return this->dDirectionSecondary; }
void HSSContainer::setDDirectionSecondary(HSSParserNode::p value)
{
    this->dDirectionSecondary = value;
    if(this->observedDirectionSecondary != NULL)
    {
        this->observedDirectionSecondary->removeObserver(this->observedDirectionSecondaryProperty, HSSObservablePropertyDirectionSecondary, this);
    }
    
    if (value->isA(HSSParserNodeTypeKeywordConstant)){
        std::string stringValue = boost::static_pointer_cast<HSSKeywordConstant>(value)->getValue();
        if(stringValue == "leftToRight" || stringValue == "rightToLeft"){
            if (this->directionSecondary == HSSDirectionLeftToRight || this->directionSecondary == HSSDirectionRightToLeft){
                this->directionSecondary = HSSDirectionTopToBottom;
                this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
            }
            if(stringValue == "leftToRight"){
                this->directionSecondary = HSSDirectionLeftToRight;
            } else {
                this->directionSecondary = HSSDirectionRightToLeft;
            }
            
        } else if (stringValue == "topToBottom" || stringValue == "bottomToTop") {
            if (this->directionSecondary == HSSDirectionTopToBottom || this->directionSecondary == HSSDirectionBottomToTop){
                this->directionSecondary = HSSDirectionLeftToRight;
                this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
            }
            if(stringValue == "topToBottom"){
                this->directionSecondary = HSSDirectionTopToBottom;
            } else {
                this->directionSecondary = HSSDirectionBottomToTop;
            }
        }
    }
    
    this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
}

void HSSContainer::directionSecondaryChanged(HSSObservableProperty source, void *data)
{
    if(this->dDirectionSecondary->isA(HSSParserNodeTypeKeywordConstant)){
        
    }
    
    this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
}


//directionSecondary
HSSParserNode::p HSSContainer::getDShape() { return this->dShape; }
void HSSContainer::setDShape(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeObjectDefinition:
        case HSSParserNodeTypeObjectNameConstant:
        case HSSParserNodeTypeKeywordConstant:
        case HSSParserNodeTypeFunctionCall:
            break;
        default:
            throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for font of "+this->getElementName()));
    }
    
    this->dShape = value;
    if(this->observedShape != NULL)
    {
        this->observedShape->removeObserver(this->observedShapeProperty, HSSObservablePropertyShape, this);
    }
    
    switch (value->getType()) {
        case HSSParserNodeTypeKeywordConstant:
        {
            std::string stringValue = boost::static_pointer_cast<HSSKeywordConstant>(value)->getValue();
            if(stringValue == "default"){
                this->shape = HSSRectangle::p(new HSSRectangle());
            }
            break;
        }
        
        case HSSParserNodeTypeObjectDefinition:
        {
            HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition>(value);
            objdef->apply();
            HSSObject::p objValue = objdef->getObject();
            if(objValue->isA(HSSObjectTypeShape)){
                this->shape = boost::static_pointer_cast<HSSShape>(objValue);
            }
            break;
        }
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
                objdef->apply();
                HSSObject::p theObject = objdef->getObject();
                if (theObject->isA(HSSObjectTypeShape)){
                    this->shape = boost::static_pointer_cast<HSSShape>(theObject);
                }
                
            } catch (HSSObjectNotFoundException * e) {
                std_log(e->toString());
            }
            
            break;
        }
            
        default:
            
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyShape, &this->shape);
}

void HSSContainer::shapeChanged(HSSObservableProperty source, void *data)
{
    this->setDirty(true);
    this->shape = *(HSSShape::p *)data;
    this->notifyObservers(HSSObservablePropertyShape, &this->shape);
}



void HSSContainer::setDefaults()
{
    HSSDisplayObject::setDefaults();
    
    //contentAlignX
    HSSPercentageConstant::p newDContentAlignX(new HSSPercentageConstant(50));
    this->setDContentAlignX(newDContentAlignX);
    //contentAlignY
    HSSNumberConstant::p newDContentAlignY(new HSSNumberConstant(0));
    this->setDContentAlignY(newDContentAlignY);
    //directionPrimary
    HSSKeywordConstant::p newDDirectionPrimary(new HSSKeywordConstant("leftToRight"));
    this->setDDirectionPrimary(newDDirectionPrimary);
    //directionSecondary
    HSSKeywordConstant::p newDDirectionSecondary(new HSSKeywordConstant("topToBottom"));
    this->setDDirectionSecondary(newDDirectionSecondary);
    //shape
    HSSKeywordConstant::p newDShape(new HSSKeywordConstant("default"));
    this->setDShape(newDShape);
}



long double HSSContainer::_setLDProperty(
                                             void(HSSContainer::*callback)(HSSObservableProperty property, void* data),
                                             HSSParserNode::p       value,
                                             long double            percentageBase,
                                             HSSObservableProperty  observedProperty,
                                             HSSObservable *        observedObject,
                                             HSSObservableProperty  observedSourceProperty,
                                             HSSObservable *        &observedStore,
                                             HSSObservableProperty  &observedStoreProperty,
                                             const std::vector<HSSDisplayObject::p> * scope
                                             )
{
    long double ret;
    
    HSSParserNodeType nodeType = value->getType();
    switch (nodeType) {
        case HSSParserNodeTypeNumberConstant:
        {
            HSSNumberConstant::p numberValue = boost::static_pointer_cast<HSSNumberConstant>(value);
            ret = numberValue->getValue();
            break;
        }
            
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant>(value);
            ret = percentageValue->getValue(percentageBase);
            if(callback != NULL)
            {
                observedObject->observe(observedProperty, observedSourceProperty, this, new HSSValueChangedCallback<HSSContainer>(this, callback));
                observedStore = observedObject;
                observedStoreProperty = observedProperty;
            }
            break;
        }
            
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression>(value);
            expressionValue->setPercentageBase(percentageBase);
            expressionValue->setPercentageObserved(observedProperty, observedObject);
            expressionValue->setScope(scope);
            ret = expressionValue->evaluate();
            if(callback != NULL){
                expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSContainer>(this, callback));
                observedStore = expressionValue.get();
                observedStoreProperty = HSSObservablePropertyValue;
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
            
            break;
            
        default:
            throw AXRError::p(new AXRError("HSSContainer", "Unknown parser node type while setting LDProperty"));
    }
    
    return ret;
}

