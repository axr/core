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
 *      Last changed: 2012/01/03
 *      HSS version: 1.0
 *      Core version: 0.44
 *      Revision: 10
 *
 ********************************************************************/

#import "AXRView.h"
#include "OSXAxrWrapper.h"
#include <iostream>
#include "../../AXR.h"
#include "../../axr/AXRDebugging.h"
#include "../../axr/AXRController.h"
#include <cairo/cairo-quartz.h>

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
    axrWrapper = NULL;
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

- (void)mouseDown:(NSEvent *)theEvent
{
//    AXR::OSXRender * render = (AXR::OSXRender *)[self axrRender];
//    NSPoint thePoint = [self convertPoint:[theEvent locationInWindow] fromView:nil];
//    NSRect bounds = [self bounds];
//    render->mouseDown(thePoint.x, bounds.size.height - thePoint.y);
//    [self setNeedsDisplay:TRUE];
}

- (void)mouseUp:(NSEvent *)theEvent
{
//    AXR::OSXRender * render = (AXR::OSXRender *)[self axrRender];
//    NSPoint thePoint = [self convertPoint:[theEvent locationInWindow] fromView:nil];
//    NSRect bounds = [self bounds];
//    render->mouseUp(thePoint.x, bounds.size.height - thePoint.y);
//    [self setNeedsDisplay:TRUE];
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
    AXR::OSXAxrWrapper * wrapper = new AXR::OSXAxrWrapper();
    bool loaded = false;
    if(wrapper!=NULL){
        loaded = wrapper->loadFile();
    }
    if(loaded){
        [self setNeedsDisplay:YES];
        [self setNeedsFile:NO];
        [self setAxrWrapper:wrapper];
    } else {
        delete wrapper;
    }
    
    return loaded;
}

- (bool)loadFile:(NSString *)xmlPath
{
    std_log1("loading file");
//    AXR::OSXAxrWrapper * wrapper = (AXR::OSXAxrWrapper *)[self axrWrapper];
    return false;    
//    AXR::AXRController * controller = (AXR::AXRController *)[self axrController];
//    std::string filePath = std::string([xmlPath UTF8String]);
//    std::string fileName = std::string([[xmlPath lastPathComponent] UTF8String]);
//    bool loaded = controller->loadFile(filePath, fileName);
//    [self setNeedsDisplay:YES];
//    return loaded;
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
