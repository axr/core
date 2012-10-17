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
 *      AUTHORS: see AUTHORS file
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

#import <iostream>
#import <QPixmap>
#import "AXRController.h"
#import "AXRDebugging.h"
#import "AXRInitializer.h"
#import "AXRView.h"
#import "HSSContainer.h"
#import "HSSEvent.h"
#import "HSSTypeEnums.h"

@implementation AXRView
@synthesize needsFile;

-(id) initWithFrame : (NSRect) frame
{
    self = [super initWithFrame : frame];
    if (self)
    {
    }

    return self;
}

-(void) awakeFromNib
{
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRView: awaking from NIB");
    [self setNeedsFile : YES];
}

-(void) dealloc
{
    [super dealloc];
}

-(BOOL) isOpaque
{
    return YES;
}

-(void) drawRect : (NSRect) dirtyRect
{
    AXR::AXRCore* core = AXR::AXRCore::getInstance();
    AXR::AXRRender::p renderer = core->getRender();
    if (renderer && core->getController()->getRoot())
    {
        NSRect paintRect = [self bounds];

        // Fill the view with our background color...
        [[NSColor whiteColor] set];
        NSRectFill(paintRect);

        // Render the final composite on to the screen
        core->drawInRectWithBounds(dirtyRect, paintRect);
        CGContextDrawImage((CGContextRef)[[NSGraphicsContext currentContext] graphicsPort], paintRect, QPixmap::fromImage(renderer->surface()).toMacCGImageRef());
    }
}

-(BOOL) acceptsFirstResponder
{
    return YES;
}

-(void) viewDidMoveToWindow
{
    if ([self window] != nil)
    {
        [[self window] setAcceptsMouseMovedEvents : TRUE];
    }
}

-(void) mouseDown : (NSEvent *) theEvent
{
    axr_log(AXR_DEBUG_CH_EVENTS | AXR_DEBUG_CH_EVENTS_SPECIFIC, "AXRView: mouse down");
    AXR::HSSContainer::p root = AXR::AXRCore::getInstance()->getController()->getRoot();
    if (root)
    {
        AXR::HSSPoint thePoint;
        NSPoint sysPoint = [self convertPoint : [theEvent locationInWindow] fromView : nil];
        NSRect bounds = [self bounds];
        thePoint.x = sysPoint.x;
        thePoint.y = bounds.size.height - sysPoint.y;
        root->handleEvent(AXR::HSSEventTypeMouseDown, (void*) &thePoint);
    }
}

-(void) mouseUp : (NSEvent *) theEvent
{
    axr_log(AXR_DEBUG_CH_EVENTS | AXR_DEBUG_CH_EVENTS_SPECIFIC, "AXRView: mouse up");
    AXR::HSSContainer::p root = AXR::AXRCore::getInstance()->getController()->getRoot();
    if (root)
    {
        AXR::HSSPoint thePoint;
        NSPoint sysPoint = [self convertPoint : [theEvent locationInWindow] fromView : nil];
        NSRect bounds = [self bounds];
        thePoint.x = sysPoint.x;
        thePoint.y = bounds.size.height - sysPoint.y;
        root->handleEvent(AXR::HSSEventTypeMouseUp, (void*) &thePoint);
        root->handleEvent(AXR::HSSEventTypeClick, (void*) &thePoint);
    }
}

-(void) mouseMoved : (NSEvent *) theEvent
{
    axr_log_inline(AXR_DEBUG_CH_EVENTS_SPECIFIC, ".");
    AXR::HSSContainer::p root = AXR::AXRCore::getInstance()->getController()->getRoot();
    if (root)
    {
        AXR::HSSPoint thePoint;
        NSPoint sysPoint = [self convertPoint : [theEvent locationInWindow] fromView : nil];
        NSRect bounds = [self bounds];
        thePoint.x = sysPoint.x;
        thePoint.y = bounds.size.height - sysPoint.y;
        root->handleEvent(AXR::HSSEventTypeMouseMove, (void*) &thePoint);
    }
}

@end
