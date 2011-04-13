//
//  macAppDelegate.h
//  mac
//
//  Created by Miro Keller on 13/02/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "../core/AXR.h"
#import "../core/os/osx/AXRView.h"

@interface AXRTestAppAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *testsWindow;
@property (assign) IBOutlet NSWindow *axrWindow;
@property (assign) IBOutlet AXRView *axrView;


void listHSSStatements(NSString * filepath);
void listHSSTokens(NSString * filepath);
void listXMLElements(NSString * filepath);

- (IBAction)listStatements:(id)sender;
- (IBAction)listTokens:(id)sender;

- (void)listXMLElements;

- (IBAction)listXMLElements:(id)sender;

- (IBAction)openDocument:(id)sender;
- (IBAction)reload:(id)sender;

@end