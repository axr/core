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
#import "HSSDisplayObject.h"
#import "CocoaAppDelegate.h"

@implementation CocoaAppDelegate

@synthesize axrWindow;
@synthesize axrView;
@synthesize needsFile;

-(id) init
{
    wrapper = new AXR::AXRWrapper();

    [self setNeedsFile : YES];
    //    axr_debug_activate_channel(AXR_DEBUG_CH_ON);
    //    axr_debug_activate_channel(AXR_DEBUG_CH_OVERVIEW);
    //    axr_debug_activate_channel(AXR_DEBUG_CH_GENERAL);
    //    axr_debug_activate_channel(AXR_DEBUG_CH_GENERAL_SPECIFIC);
    //    axr_debug_activate_channel(AXR_DEBUG_CH_XML);
    //    axr_debug_activate_channel(AXR_DEBUG_CH_HSS);
    //    axr_debug_activate_channel(AXR_DEBUG_CH_FULL_FILENAMES);
    //    axr_debug_activate_channel(AXR_DEBUG_CH_EVENTS);
    //    axr_debug_activate_channel(AXR_DEBUG_CH_EVENTS_SPECIFIC);
    //    axr_debug_activate_channel(AXR_DEBUG_CH_TOKENIZING);

    return self;
}

-(void) dealloc
{
    delete wrapper;
    [super dealloc];
}

-(void) applicationDidFinishLaunching : (NSNotification *) aNotification
{
    //NSString * filepath = [[NSBundle mainBundle] pathForResource:@"blank" ofType:@"xml" inDirectory:@"views"];
    //[[self axrView] loadFile:filepath];

    if ([self needsFile])
    {
        [self openDocument : self];
        [self setNeedsFile : NO];
    }
}

-(BOOL) application : (NSApplication *) theApplication openFile : (NSString *) filename
{
    [[self axrWindow] makeKeyAndOrderFront : self];

    if (wrapper->loadFileByPath(QUrl::fromLocalFile(AXR::fromNSString(filename))))
    {
        [[self axrView] setNeedsDisplay : YES];
        [self setNeedsFile : NO];
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
        AXR::AXRString filePath = AXR::fromNSString([[openPanel filenames] objectAtIndex: 0]);
        axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "CocoaAXRWrapper: User selected file " + filePath);

        if (wrapper->loadFileByPath(QUrl::fromLocalFile(filePath)))
        {
            [[self axrView] setNeedsDisplay : YES];
            [self setNeedsFile : NO];
        }
    }
}

-(IBAction) reload : (id) sender
{
    wrapper->reload();
    [[self axrView] setNeedsDisplay : YES];
}

@end
