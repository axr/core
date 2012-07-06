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

#import <Cocoa/Cocoa.h>
#import "../core/AXR.h"
#import "../core/os/osx/AXRView.h"

#if (MAC_OS_X_VERSION_MAX_ALLOWED <= MAC_OS_X_VERSION_10_5)
@interface AXRTestAppAppDelegate : NSObject {
#else
@interface AXRTestAppAppDelegate : NSObject <NSApplicationDelegate> {
#endif
    NSWindow *window;
    NSWindow * testsWindow;
    NSWindow * errorsWindow;
    NSTextView * errorLog;
    NSWindow * axrWindow;
    AXRView * axrView;
    BOOL needsFile;
}

@property (assign) IBOutlet NSWindow *testsWindow;
@property (assign) IBOutlet NSWindow *errorsWindow;
@property (assign) IBOutlet NSTextView *errorLog;
@property (assign) IBOutlet NSWindow *axrWindow;
@property (assign) IBOutlet AXRView *axrView;
@property (assign) BOOL needsFile;

- (id)init;
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification;
- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename;

void listHSSStatements(NSString * filepath);
void listHSSTokens(NSString * filepath);
void listXMLElements(NSString * filepath);

- (IBAction)listStatements:(id)sender;
- (IBAction)listTokens:(id)sender;

- (void)listXMLElements;

- (IBAction)listXMLElements:(id)sender;

- (IBAction)openDocument:(id)sender;
- (IBAction)layoutTests:(id)sender;
- (IBAction)reload:(id)sender;

@end