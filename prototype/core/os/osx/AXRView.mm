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
 ********************************************************************/

#import <iostream>
#import <cairo/cairo-quartz.h>
#import "AXR.h"
#import "AXRController.h"
#import "AXRDebugging.h"
#import "AXRView.h"
#import "HSSContainer.h"
#import "HSSEvent.h"
#import "HSSTypeEnums.h"
#import "CocoaAXRWrapper.h"

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
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRView: awaking from NIB");
    [self setNeedsFile:YES];
    AXR::CocoaAXRWrapper * wrapper = new AXR::CocoaAXRWrapper(self);

    [self setAxrWrapper:wrapper];
}

- (void)dealloc
{
    AXR::CocoaAXRWrapper * wrapper = (AXR::CocoaAXRWrapper *)[self axrWrapper];
    delete wrapper;
    [super dealloc];
}

- (BOOL)isOpaque
{
    return YES;
}

- (void)drawRect:(NSRect)dirtyRect
{
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRView: drawing");

    NSRect bounds = [self bounds];

    if (![self needsFile])
    {
        AXR::AXRCore::tp & core = AXR::AXRCore::getInstance();
        [[NSColor whiteColor] set];
        NSRectFill(bounds);

        CGContextRef ctxt = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
        //invert the coordinate system
        CGContextTranslateCTM (ctxt, 0.0, (int)bounds.size.height);
        CGContextScaleCTM (ctxt, 1.0, -1.0);

        cairo_surface_t * targetSurface = cairo_quartz_surface_create_for_cg_context(ctxt, dirtyRect.size.width, dirtyRect.size.height);
        cairo_t * tempcairo = cairo_create(targetSurface);
        cairo_surface_destroy(targetSurface);
        core->setCairo(tempcairo);
        core->drawInRectWithBounds(dirtyRect, bounds);
        cairo_destroy(tempcairo);
        core->setCairo(NULL);
    }
    else
    {
        // Fill with light gray
        NSColor *lightGrayColor = [NSColor colorWithCalibratedWhite:0.8 alpha:1];
        [lightGrayColor set];
        NSRectFill(bounds);

        // Display an indicator that no file is currently loaded
        NSFont *errorFont = [NSFont fontWithName:@"Helvetica" size:200];
        NSShadow *shadow = [[NSShadow alloc] init];
        [shadow setShadowColor:[NSColor darkGrayColor]];
        [shadow setShadowBlurRadius:5];
        NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:
                                    errorFont, NSFontAttributeName,
                                    lightGrayColor, NSForegroundColorAttributeName,
                                    shadow, NSShadowAttributeName,
                                    nil];
        NSString *message = @"?";
        NSSize size = [message sizeWithAttributes:attributes];
        NSRect stringBounds = NSMakeRect((bounds.size.width - size.width) / 2, (bounds.size.height - size.height) / 2, size.width, size.height);

        [message drawInRect:stringBounds withAttributes:attributes];
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
    axr_log(AXR_DEBUG_CH_EVENTS | AXR_DEBUG_CH_EVENTS_SPECIFIC, "AXRView: mouse down");
    AXR::HSSContainer::p root = AXR::AXRCore::getInstance()->getController()->getRoot();
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
    axr_log(AXR_DEBUG_CH_EVENTS | AXR_DEBUG_CH_EVENTS_SPECIFIC, "AXRView: mouse up");
    AXR::HSSContainer::p root = AXR::AXRCore::getInstance()->getController()->getRoot();
    if(root){
        AXR::HSSPoint thePoint;
        NSPoint sysPoint = [self convertPoint:[theEvent locationInWindow] fromView:nil];
        NSRect bounds = [self bounds];
        thePoint.x = sysPoint.x;
        thePoint.y = bounds.size.height - sysPoint.y;
        root->handleEvent(AXR::HSSEventTypeMouseUp, (void*) &thePoint);
        root->handleEvent(AXR::HSSEventTypeClick, (void*) &thePoint);
    }
}

- (void)mouseMoved:(NSEvent *)theEvent
{
    axr_log_inline(AXR_DEBUG_CH_EVENTS_SPECIFIC, ".");
    AXR::HSSContainer::p root = AXR::AXRCore::getInstance()->getController()->getRoot();
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
    AXR::CocoaAXRWrapper * wrapper = (AXR::CocoaAXRWrapper *)[self axrWrapper];
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

- (bool)loadFile:(NSString *)filePath
{
    AXR::CocoaAXRWrapper * wrapper = (AXR::CocoaAXRWrapper *)[self axrWrapper];
    bool loaded = false;
    if(wrapper!=NULL){
        loaded = wrapper->loadFileByPath(std::string([filePath UTF8String]));
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
    AXR::CocoaAXRWrapper * wrapper = (AXR::CocoaAXRWrapper *)[self axrWrapper];
    if(wrapper->hasLoadedFile()){
        axr_log(AXR_DEBUG_CH_OVERVIEW, "\n\n\nAXRView: reloading file");
        bool loaded = wrapper->reload();
        [self setNeedsDisplay:YES];
        return loaded;
    }

    return false;
}

@end
