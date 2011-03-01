//
//  macAppDelegate.m
//  mac
//
//  Created by Miro Keller on 13/02/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "macAppDelegate.h"
#import <string.h>
#import <stdio.h>
#import "AXRXmlParser.h"

#define BUFFSIZE 8192
char Buff[BUFFSIZE];

@implementation macAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	//load the sample XML file
	NSArray *xmlFileType = [NSArray arrayWithObject:@"xml"];
	NSOpenPanel *openPanel = [NSOpenPanel openPanel];
	int result;
	[openPanel setCanChooseFiles:TRUE];
	[openPanel setAllowsMultipleSelection:FALSE];
	result = [openPanel runModalForTypes:xmlFileType];
	if(result == NSOKButton){
		if([[openPanel filenames] count] > 0){
			NSString *filepath = [[openPanel filenames] objectAtIndex:0];
			AXRXmlParser parser;
			parser.parse([filepath UTF8String]);
		}
	}
	
}

@end
