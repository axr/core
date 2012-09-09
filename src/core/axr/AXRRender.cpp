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
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#include <iostream>
#include <boost/lexical_cast.hpp>
#include "errors.h"
#include "AXR.h"
#include "AXRController.h"
#include "AXRDebugging.h"
#include "AXRRender.h"
#include "HSSDisplayObject.h"

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

void AXRRender::drawInRectWithBounds(HSSRect rect, HSSRect bounds)
{
    if (!this->cairo)
    {
        AXRError::p(new AXRError("AXRRender", "Fatal error: Cairo was not defined"))->raise();
#warning Do NOT use exit(0) in a library, especially since this feigns a successful exit...
        exit(0);
    }

    //prepare values
    HSSContainer::p root = this->controller->getRoot();
    AXRWrapper * wrapper = AXRCore::getInstance()->getWrapper();

    if (root)
    {
        ///@todo find out what objects lie in that rect

        //if the window size has changed, make new size
        if (bounds.size.width != this->windowWidth || bounds.size.height != this->windowHeight)
        {
            axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRRender: window size changed, setting to width: " + boost::lexical_cast<std::string > (bounds.size.width) + " and height: " + boost::lexical_cast<std::string > (bounds.size.height));
            this->windowWidth = bounds.size.width;
            this->windowHeight = bounds.size.height;
            root->setNeedsRereadRules(true);
        }
        //draw the elements
        axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRRender: reading object definitions");
        root->recursiveReadDefinitionObjects();
        axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRRender: laying out elements on page");
        root->recursiveLayout();
        if (wrapper->showLayoutSteps())
        {
            wrapper->resetLayoutTicks();
        }

        axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRRender: regenerating surfaces");
        root->recursiveRegenerateSurfaces();
        axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRRender: drawing tree");
        wrapper->nextLayoutChild();
        root->recursiveDraw(this->cairo);
        if (wrapper->showLayoutSteps())
        {
            wrapper->resetLayoutTicks();
            wrapper->resetLayoutChild();
        }
    }
    else
    {
        AXRError::p(new AXRError("AXRRender", "Fatal error: No root"))->raise();
    }
}

//this will maintain the reference to controller

void AXRRender::reset()
{
    this->windowWidth = 0;
    this->windowHeight = 0;
}

void AXRRender::mouseDown(HSSUnit x, HSSUnit y)
{
    //prepare values
    HSSContainer::p root = this->controller->getRoot();

    struct point
    {
        HSSUnit x;
        HSSUnit y;
    } thePoint;
    thePoint.x = x;
    thePoint.y = y;
    if (root)
        root->handleEvent(HSSEventTypeMouseDown, (void*) &thePoint);
}

void AXRRender::mouseUp(HSSUnit x, HSSUnit y)
{
    //prepare values
    HSSContainer::p root = this->controller->getRoot();

    struct point
    {
        HSSUnit x;
        HSSUnit y;
    } thePoint;
    thePoint.x = x;
    thePoint.y = y;
    if (root)
    {
        try
        {
            root->handleEvent(HSSEventTypeMouseUp, (void*) &thePoint);
        }
        catch (const AXRError::p &e)
        {
            e->raise();
        }
    }
}

void AXRRender::setCairo(cairo_t * cairo)
{
    this->cairo = cairo;
}

cairo_t * AXRRender::getCairo()
{
    return this->cairo;
}

double AXRRender::getWindowWidth()
{
    return this->windowWidth;
}

double AXRRender::getWindowHeight()
{
    return this->windowHeight;
}
