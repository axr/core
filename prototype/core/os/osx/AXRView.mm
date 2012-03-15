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
 *      Last changed: 2012/02/23
 *      HSS version: 1.0
 *      Core version: 0.45
 *      Revision: 11
 *
 ********************************************************************/

#import "AXRView.h"
#include "OSXAxrWrapper.h"
#include <iostream>
#include "../../AXR.h"
#include "../../axr/AXRDebugging.h"
#include "../../axr/AXRController.h"
#include <cairo/cairo-quartz.h>
#include "../../hss/objects/HSSEvent.h"
#include "../../hss/objects/HSSContainer.h"

@implementation AXRView
@synthesize needsFile;

//hack to make it work with IB from a dependent target
+(void)_keepAtLinkTime
{
    //do nothing
}

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        //nothing here
    }
    
    return self;
}

- (void)awakeFromNib
{
    [self setNeedsFile:YES];
    AXR::OSXAxrWrapper * wrapper = new AXR::OSXAxrWrapper(self);
    
    [self setAxrWrapper:wrapper];
}

- (void)dealloc
{
    AXR::OSXAxrWrapper * wrapper = (AXR::OSXAxrWrapper *)[self axrWrapper];
    delete wrapper;
    [super dealloc];
}

- (BOOL)isOpaque
{
    return YES;
}

- (void)drawRect:(NSRect)dirtyRect
{
    
    NSRect bounds = [self bounds];
    
    if(![self needsFile]){
        AXR::OSXAxrWrapper * wrapper = (AXR::OSXAxrWrapper *)[self axrWrapper];
        AXR::AXRCore::p core = wrapper->getCore();
        if(core){
            
            [[NSColor whiteColor] set];
            NSRectFill(bounds);
            
            AXR::AXRRect axrRect;
            axrRect.size.width = dirtyRect.size.width;
            axrRect.size.height = dirtyRect.size.height;
            axrRect.origin.x = dirtyRect.origin.x;
            axrRect.origin.y = dirtyRect.origin.y;
            AXR::AXRRect axrBounds;
            axrBounds.size.width = bounds.size.width;
            axrBounds.size.height = bounds.size.height;
            axrBounds.origin.x = bounds.origin.x;
            axrBounds.origin.y = bounds.origin.y;
            
            
            CGContextRef ctxt = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
            //invert the coordinate system
            CGContextTranslateCTM (ctxt, 0.0, (int)bounds.size.height);
            CGContextScaleCTM (ctxt, 1.0, -1.0);
            
            cairo_surface_t * targetSurface = cairo_quartz_surface_create_for_cg_context(ctxt, axrRect.size.width, axrRect.size.height);
            cairo_t * tempcairo = cairo_create(targetSurface);
            cairo_surface_destroy(targetSurface);
            core->setCairo(tempcairo);
            core->drawInRectWithBounds(axrRect, axrBounds);
            cairo_destroy(tempcairo);
            core->setCairo(NULL);
        }
    } else {
        //fill with light gray
        [[NSColor colorWithCalibratedWhite:0.8 alpha:1.] set];
        NSRectFill(bounds);
    }
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)viewDidMoveToWindow
{
    if([self window] != nil){
        [[self window] setAcceptsMouseMovedEvents: TRUE];
    }
}

- (void)mouseDown:(NSEvent *)theEvent
{
    AXR::OSXAxrWrapper * wrapper = (AXR::OSXAxrWrapper *)[self axrWrapper];
    AXR::AXRCore::p core = wrapper->getCore();
    AXR::HSSContainer::p root = core->getController()->getRoot();
    if(root){
        AXR::HSSPoint thePoint;
        NSPoint sysPoint = [self convertPoint:[theEvent locationInWindow] fromView:nil];
        NSRect bounds = [self bounds];
        thePoint.x = sysPoint.x;
        thePoint.y = bounds.size.height - sysPoint.y;
        root->handleEvent(AXR::HSSEventTypeMouseDown, (void*) &thePoint);
    }
}

- (void)mouseUp:(NSEvent *)theEvent
{
    AXR::OSXAxrWrapper * wrapper = (AXR::OSXAxrWrapper *)[self axrWrapper];
    AXR::AXRCore::p core = wrapper->getCore();
    AXR::HSSContainer::p root = core->getController()->getRoot();
    if(root){
        AXR::HSSPoint thePoint;
        NSPoint sysPoint = [self convertPoint:[theEvent locationInWindow] fromView:nil];
        NSRect bounds = [self bounds];
        thePoint.x = sysPoint.x;
        thePoint.y = bounds.size.height - sysPoint.y;
        root->handleEvent(AXR::HSSEventTypeMouseUp, (void*) &thePoint);
    }
}

- (void)mouseMoved:(NSEvent *)theEvent
{
    AXR::OSXAxrWrapper * wrapper = (AXR::OSXAxrWrapper *)[self axrWrapper];
    AXR::AXRCore::p core = wrapper->getCore();
    AXR::HSSContainer::p root = core->getController()->getRoot();
    if(root){
        AXR::HSSPoint thePoint;
        NSPoint sysPoint = [self convertPoint:[theEvent locationInWindow] fromView:nil];
        NSRect bounds = [self bounds];
        thePoint.x = sysPoint.x;
        thePoint.y = bounds.size.height - sysPoint.y;
        root->handleEvent(AXR::HSSEventTypeMouseMove, (void*) &thePoint);
    }
}

- (void)setAxrWrapper:(void *)theWrapper
{
    axrWrapper = theWrapper;
}

- (void *)axrWrapper
{
    return axrWrapper;
}

- (bool)loadFile
{
    AXR::OSXAxrWrapper * wrapper = (AXR::OSXAxrWrapper *)[self axrWrapper];
    bool loaded = false;
    if(wrapper!=NULL){
        loaded = wrapper->loadFile();
    }
    if(loaded){
        [self setNeedsDisplay:YES];
        [self setNeedsFile:NO];
    }
    
    return loaded;
}

- (bool)loadFile:(NSString *)xmlPath
{
    AXR::OSXAxrWrapper * wrapper = (AXR::OSXAxrWrapper *)[self axrWrapper];
    bool loaded = false;
    if(wrapper!=NULL){
        loaded = wrapper->loadXMLFile(std::string([xmlPath UTF8String]));
    }
    if(loaded){
        [self setNeedsDisplay:YES];
        [self setNeedsFile:NO];
        [self setAxrWrapper:wrapper];
    }
    
    return loaded;
}

- (bool)reload
{
    AXR::OSXAxrWrapper * wrapper = (AXR::OSXAxrWrapper *)[self axrWrapper];
    if(wrapper->hasLoadedFile()){
        std_log1("reloading file");
        bool loaded = wrapper->reload();
        [self setNeedsDisplay:YES];
        return loaded;
    }
    
    return false;
}

@end
