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
 *      Last changed: 2011/04/17
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 6
 *
 ********************************************************************/

#import "AXRView.h"
//#include <cairo/cairo.h>
#include "../../hss/objects/HSSContainer.h"
#include "../../axr/AXRController.h"
#include "OSXRender.h"
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
    AXR::AXRController * theController = new AXR::AXRController::AXRController();
    [self setAxrController:theController];
    AXR::OSXRender * theRender =  new AXR::OSXRender::OSXRender(theController);
    [self setAxrRender:theRender];
}

- (void)dealloc
{
    delete (AXR::AXRController *)[self axrController];
    delete (AXR::OSXRender *)[self axrRender];
    [super dealloc];
}

- (BOOL)isOpaque
{
    return YES;
}

- (void)drawRect:(NSRect)dirtyRect
{
    
    NSRect bounds = [self bounds];
    
    AXR::AXRController * controller = (AXR::AXRController *)[self axrController];
    AXR::OSXRender * render = (AXR::OSXRender *)[self axrRender];
    
    if(controller != NULL && controller->hasLoadedFile() && render != NULL){
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
        render->drawInRectWithBounds(axrRect, axrBounds);
        //std_log1(controller->toString());
                
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

- (void)setAxrRender:(void *)theRender
{
    axrRender = theRender;
}

- (void *)axrRender
{
    return axrRender;
}

- (bool)loadFile
{
    std_log1("loading file");
    
    AXR::AXRController * controller = (AXR::AXRController *)[self axrController];
    bool loaded = controller->loadFile();
    [self setNeedsDisplay:YES];
    return loaded;
}

- (bool)reload
{
    AXR::AXRController * controller = (AXR::AXRController *)[self axrController];
    if(controller->hasLoadedFile()){
        std_log1("reloading file");
        bool loaded = controller->reload();
        [self setNeedsDisplay:YES];
        return loaded;
    }
    
    return false;
}

@end
