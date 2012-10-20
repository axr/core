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

#import "AXRDebugging.h"
#import "AXRDocument.h"
#import "HSSDisplayObject.h"
#import "CocoaAppDelegate.h"

@implementation CocoaAppDelegate

@synthesize axrWindow;
@synthesize axrView;

-(id) init
{
    self = [super init];
    if (self)
    {
        document = AXR::AXRDocument::getInstance();
    }

    return self;
}

-(void) dealloc
{
    // TODO: don't delete until thread pointer is eliminated
    //delete document;
    [super dealloc];
}

-(void) applicationDidFinishLaunching : (NSNotification *) aNotification
{
    axrView.document = document;
    [self openDocument : self];
}

-(BOOL) application : (NSApplication *) theApplication openFile : (NSString *) filename
{
    [[self axrWindow] makeKeyAndOrderFront : self];

    if (document && document->loadFileByPath(QUrl::fromLocalFile(AXR::fromNSString(filename))))
    {
        [[self axrView] setNeedsDisplay : YES];
        return YES;
    }

    return NO;
}

-(IBAction) openDocument : (id) sender
{
    [[self axrWindow] makeKeyAndOrderFront : self];

    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    [openPanel setCanChooseFiles: TRUE];
    [openPanel setAllowsMultipleSelection: FALSE];

    if ([openPanel runModalForTypes: [NSArray arrayWithObjects: @"xml", @"hss", nil]] == NSOKButton)
    {
        // TODO: Should create a new document if NULL...
        if (document && document->loadFileByPath(QUrl::fromLocalFile(AXR::fromNSString([[openPanel filenames] objectAtIndex: 0]))))
        {
            [[self axrView] setNeedsDisplay : YES];
        }
    }
}

-(IBAction) reload : (id) sender
{
    if (!document)
        return;

    document->reload();
    [[self axrView] setNeedsDisplay : YES];
}

@end
