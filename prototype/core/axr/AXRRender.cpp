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
 *      Last changed: 2012/06/11
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 12
 *
 ********************************************************************/

#include "AXRRender.h"
#include <iostream>
#include "AXRDebugging.h"
#include "errors/errors.h"
#include "../hss/objects/HSSDisplayObject.h"
#include "AXRController.h"
#include "../AXR.h"
#include "../axr/AXRDebugging.h"
#include <boost/lexical_cast.hpp>

using namespace AXR;

AXRRender::AXRRender(AXRController * controller)
{
    this->controller = controller;
    this->windowWidth = 0;
    this->windowHeight = 0;
    this->cairo = NULL;
}

AXRRender::~AXRRender()
{
    
}

void AXRRender::drawInRectWithBounds(AXRRect rect, AXRRect bounds)
{
    if(this->cairo == NULL){
        AXRError::p(new AXRError("AXRRender", "Fatal error: Cairo was not defined"))->raise();
        exit(0);
    }
    
    //prepare values
    HSSContainer::p root = this->controller->getRoot();
    
    if(root){
        ///@todo find out what objects lie in that rect
        
        //if the window size has changed, make new size
        if(bounds.size.width != this->windowWidth || bounds.size.height != this->windowHeight){
            axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRRender: window size changed, setting to width: "+boost::lexical_cast<std::string>(bounds.size.width)+" and height: "+boost::lexical_cast<std::string>(bounds.size.height));
            this->windowWidth = bounds.size.width;
            this->windowHeight = bounds.size.height;
        //    root->setDWidth(HSSNumberConstant::p(new HSSNumberConstant(this->windowWidth)));
        //    root->setDHeight(HSSNumberConstant::p(new HSSNumberConstant(this->windowHeight)));
            root->setNeedsRereadRules(true);
        }
        //draw the elements
        axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRRender: reading object definitions");
        root->recursiveReadDefinitionObjects();
        //    if(this->_needsRereadRules){
        //        std_log1("rereading rules of "+this->elementName);
        //        this->readDefinitionObjects();
        //    }
        axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRRender: laying out elements on page");
        root->recursiveLayout();
        axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRRender: regenerating surfaces");
        root->recursiveRegenerateSurfaces();
        //    if(this->_needsSurface){
        //        std_log1("regenerating surfaces of "+this->elementName);
        //        this->regenerateSurfaces();
        //    }
        axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRRender: drawing tree");
        root->recursiveDraw(this->cairo);
    } else {
        AXRError::p(new AXRError("AXRRender", "Fatal error: No root"))->raise();
    }
}

//this will maintain the reference to controller
void AXRRender::reset()
{
    this->windowWidth = 0;
    this->windowHeight = 0;
}


void AXRRender::mouseDown(long double x, long double y)
{
    //prepare values
    HSSContainer::p root = this->controller->getRoot();
    struct point { long double x; long double y; } thePoint;
    thePoint.x = x;
    thePoint.y = y;
    if(root)
        root->handleEvent(HSSEventTypeMouseDown, (void*)&thePoint);
}

void AXRRender::mouseUp(long double x, long double y)
{
    //prepare values
    HSSContainer::p root = this->controller->getRoot();
    struct point { long double x; long double y; } thePoint;
    thePoint.x = x;
    thePoint.y = y;
    if(root){
        try {
            root->handleEvent(HSSEventTypeMouseUp, (void*)&thePoint);
        } catch (AXRError::p e) {
            e->raise();
        }
    }
}

void AXRRender::setCairo(cairo_t * cairo) {
    this->cairo = cairo;
}

cairo_t * AXRRender::getCairo() { return this->cairo; }

double AXRRender::getWindowWidth()
{
    return this->windowWidth;
}

double AXRRender::getWindowHeight()
{
    return this->windowHeight;
}
