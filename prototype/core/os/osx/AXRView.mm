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
 *      Last changed: 2011/04/15
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 5
 *
 ********************************************************************/

#import "AXRView.h"
//#include <cairo/cairo.h>
#include "../../hss/objects/HSSContainer.h"
#include "../../axr/AXRController.h"
#include <cairo/cairo-quartz.h>
#include <iostream>
#include "../../axr/AXRDebugging.h"

@implementation AXRView

//hack to make it work with IB from a dependent target
+(void)_keepAtLinkTime
{
    //do nothing
}

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // nothing here
    }
    
    return self;
}

- (void)awakeFromNib
{
    
}

- (void)dealloc
{
    delete (AXR::AXRController *)[self axrController];
    [super dealloc];
}

- (BOOL)isOpaque
{
    return YES;
}

- (void)drawRect:(NSRect)dirtyRect
{
    
    NSRect bounds = [self bounds];
    float width = bounds.size.width;
    float height = bounds.size.height;
    
    AXR::AXRController * controller;
    if([self axrController] != NULL){
        controller = (AXR::AXRController *)[self axrController];
    }
    
    if(controller != NULL && controller->hasLoadedFile()){
        //std_log1(controller->toString());
        
        //fill with white
        [[NSColor whiteColor] set];
        NSRectFill(bounds);
        
        CGContextRef ctxt = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
        //invert the coordinate system
        CGContextTranslateCTM (ctxt, 0.0, (int)height);
        CGContextScaleCTM (ctxt, 1.0, -1.0);
        
        cairo_surface_t * targetSurface = cairo_quartz_surface_create_for_cg_context(ctxt, width, height);
        cairo_t * cairo = cairo_create(targetSurface);
        cairo_surface_destroy(targetSurface);
        
        cairo_set_line_width(cairo, 2);
        float i;
        unsigned j;
        AXR::HSSContainer::p root = controller->getRoot();
        unsigned size = root->children.size();
        for(i=0; i<size; i++){
            AXR::HSSDisplayObject::p displayObject = root->children[i];
            for(j=0;j<displayObject->rulesSize();j++){
                AXR::HSSRule::p rule = displayObject->rulesGet(j);
                std_log1(rule->toString());
            }
            //std::fprintf(stderr, "rectangle width: %f, height: %f, x: %f, y: %f\n", (i*150.)+((i+1.)*10.), 20., 150., 150.);
            cairo_rectangle(cairo, (i*150.)+((i+1.)*10.), 20., 150., 150.);
            cairo_set_source_rgb(cairo, 1,0.8,0.8);
            cairo_fill_preserve(cairo);
            cairo_set_source_rgb(cairo, 0,0,0);
            cairo_stroke(cairo);
        }
        
        cairo_destroy(cairo);
        
    } else {
        //fill with light gray
        [[NSColor colorWithCalibratedWhite:0.8 alpha:1.] set];
        NSRectFill(bounds);
    }
}

- (void)setAxrController:(void *)theController
{
    axrController = theController;
}

- (void *)axrController
{
    return axrController;
}

- (bool)loadFile
{
    if([self axrController] == NULL){
        [self setAxrController: new AXR::AXRController::AXRController()];
    }
    std_log1("loading file");
    
    AXR::AXRController * controller = (AXR::AXRController *)[self axrController];
    bool loaded = controller->loadFile();
    [self setNeedsDisplay:YES];
    return loaded;
}

- (bool)reload
{
    
    if([self axrController] != NULL){
        AXR::AXRController * controller = (AXR::AXRController *)[self axrController];
        if(controller->hasLoadedFile()){
            std_log1("reloading file");
            bool loaded = controller->reload();
            [self setNeedsDisplay:YES];
            return loaded;
        }
    }
    
    return false;
}

@end
