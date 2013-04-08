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
 *      COPYRIGHT: ©2013 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axrproject.org
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
#import "AXRDocument.h"
#import "HSSContainer.h"
#import "HSSEvent.h"
#import "HSSMouseEvent.h"
#import "HSSPoint.h"
#import "HSSRect.h"
#import "HSSRenderer.h"
#import "HSSTypeEnums.h"
#import "HSSVisitorManager.h"
#import "NSAXRDocument.h"
#import "NSAXRView.h"

using namespace AXR;

@interface NSAXRView (Private)

- (HSSPoint)pointFromNSEvent:(NSEvent *)theEvent;

@end

@implementation NSAXRView

@synthesize document;

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
        document = NULL;
        renderVisitor = new HSSRenderer();
    }

    return self;
}

- (void)dealloc
{
    delete renderVisitor;
    [super dealloc];
}

- (BOOL)isOpaque
{
    return YES;
}

- (void)drawRect:(NSRect)dirtyRect
{
    if (!document)
    {
        [super drawRect:dirtyRect];
        return;
    }

    NSRect paintRect = [self bounds];
    [document documentObject]->setWindowSize(self.bounds.size.width, self.bounds.size.height);

    // Fill the view with our background color...
    [[NSColor whiteColor] set];
    NSRectFill(paintRect);

    // Render the AXR document
    QSharedPointer<HSSVisitorManager> visitorManager = [document documentObject]->visitorManager();
    if (visitorManager)
    {
        // Render the final image to the screen
        renderVisitor->setDirtyRect([self bounds]);
        visitorManager->runVisitors(HSSAbstractVisitor::VisitorFilterAll);

        CGContextDrawImage((CGContextRef)[[NSGraphicsContext currentContext] graphicsPort], paintRect, QPixmap::fromImage(*renderVisitor->getFinalFrame()).toMacCGImageRef());
    }
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)viewDidMoveToWindow
{
    if ([self window] != nil)
    {
        [[self window] setAcceptsMouseMovedEvents:TRUE];
    }
}

- (void)mouseDown:(NSEvent *)theEvent
{
    if (!document)
        return;

    HSSMouseEvent mouseEvent(HSSEventTypeMouseDown, [self pointFromNSEvent:theEvent]);
    [document documentObject]->handleEvent(&mouseEvent);
}

- (void)mouseUp:(NSEvent *)theEvent
{
    if (!document)
        return;

    HSSPoint thePoint = [self pointFromNSEvent:theEvent];
    HSSMouseEvent upMouseEvent(HSSEventTypeMouseUp, thePoint);
    HSSMouseEvent clickMouseEvent(HSSEventTypeClick, thePoint);
    [document documentObject]->handleEvent(&upMouseEvent);
    [document documentObject]->handleEvent(&clickMouseEvent);
}

- (void)mouseMoved:(NSEvent *)theEvent
{
    if (!document)
        return;

    HSSMouseEvent mouseEvent(HSSEventTypeMouseMove, [self pointFromNSEvent:theEvent]);
    [document documentObject]->handleEvent(&mouseEvent);
}

- (AXR::HSSPoint)pointFromNSEvent:(NSEvent *)theEvent
{
    HSSPoint thePoint;
    NSPoint sysPoint = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    NSRect bounds = [self bounds];
    thePoint.x = sysPoint.x;
    thePoint.y = bounds.size.height - sysPoint.y;
    return thePoint;
}

@end
