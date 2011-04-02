//
//  macAppDelegate.h
//  mac
//
//  Created by Miro Keller on 13/02/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface macAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

void listHSSStatements(NSString * filepath);
void listHSSTokens(NSString * filepath);
void listXMLElements(NSString * filepath);

- (IBAction)listStatements:(id)sender;
- (IBAction)listTokens:(id)sender;

- (void)listXMLElements;

- (IBAction)listXMLElements:(id)sender;

@end