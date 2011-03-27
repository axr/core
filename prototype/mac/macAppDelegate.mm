//
//  macAppDelegate.m
//  mac
//
//  Created by Miro Keller on 13/02/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "macAppDelegate.h"
#import <string>
#import "AXR.h"
#import <iostream>


#define BUFFSIZE 8192
char Buff[BUFFSIZE];

@implementation macAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	//load the sample HSS file
	NSArray *hssFileType = [NSArray arrayWithObject:@"hss"];
	NSOpenPanel *openPanel = [NSOpenPanel openPanel];
	int result;
	[openPanel setCanChooseFiles:TRUE];
	[openPanel setAllowsMultipleSelection:FALSE];
	result = [openPanel runModalForTypes:hssFileType];
	if(result == NSOKButton){
		if([[openPanel filenames] count] > 0){
			NSString *filepath = [[openPanel filenames] objectAtIndex:0];
            listHSSStatements(filepath);
		}
	}
}


void listHSSStatements(NSString *filepath)
{
    std::cout << "reading all statements from " << [filepath UTF8String] << std::endl;
    
    unsigned buflen = 8000;
    char buffer[buflen];
    FILE * hssfile = fopen([filepath UTF8String], "r");
    int len = (int)fread(buffer, 1, buflen, hssfile);
    fclose(hssfile);
    HSSParser parser(buffer, len);
    HSSStatement * statement = NULL;
    bool done = FALSE;
    int security_count = 0;
    while (!done) {
        std::cout << "read statement\n";
        if(statement != NULL){
            delete statement;
            statement = NULL;
        }
        try {
            statement = parser.readNextStatement();
        }
        catch(HSSUnexpectedTokenException e){
            std::cout << e.toString() << std::endl;
            continue;
        }
        catch(HSSUnexpectedEndOfSourceException e){
            std::cout << e.toString() << std::endl;
        }
        catch(HSSUnexpectedObjectTypeException e){
            std::cout << e.toString() << std::endl;
        }
        catch(HSSExpectedTokenException e){
            std::cout << e.toString() << std::endl;
        }
        catch(HSSWrongHexLengthException e){
            std::cout << e.toString() << std::endl;
        }
        
        
        if(statement == NULL){
            done = TRUE;
        } else {
            std::cout << std::endl << "-----------------------------" << std::endl
            <<  statement->toString() << std::endl << "-----------------------------" << std::endl;
        }
        
        if(security_count == 99999)
        {
            break;
        } else {
            security_count++;
        }
    }
    std::cout << "reached end of source" << std::endl;
    std::cout << "\n\n\n\n";
    delete statement;
#if AXR_DEBUG_LEVEL > 1
    exit(0);
#endif
}

void listHSSTokens(NSString *filepath)
{
    std::cout << "reading all tokens from " << [filepath UTF8String] << std::endl;
    
    unsigned buflen = 8000;
    char buffer[buflen];
    FILE * hssfile = fopen([filepath UTF8String], "r");
    int len = (int)fread(buffer, 1, buflen, hssfile);
    fclose(hssfile);
    HSSTokenizer tokenizer(buffer, len);
    HSSToken * token = NULL;
    bool done = FALSE;
    int security_count = 0;
    while (!done) {
        if(token != NULL){
            delete token;
            token = NULL;
        }
        token = tokenizer.readNextToken();
        if(token == NULL){
            done = TRUE;
        } else {
            std::cout << token->toString() << std::endl;
        }
        
        if(security_count == 99999)
        {
            break;
        } else {
            security_count++;
        }
    }
    std::cout << "reached end of source" << std::endl;
    std::cout << "\n\n\n\n";
    delete token;
#if AXR_DEBUG_LEVEL > 1
    exit(0);
#endif
}

- (IBAction)listStatements:(id)sender {
    //load the sample HSS file
	NSArray *hssFileType = [NSArray arrayWithObject:@"hss"];
	NSOpenPanel *openPanel = [NSOpenPanel openPanel];
	int result;
	[openPanel setCanChooseFiles:TRUE];
	[openPanel setAllowsMultipleSelection:FALSE];
	result = [openPanel runModalForTypes:hssFileType];
	if(result == NSOKButton){
		if([[openPanel filenames] count] > 0){
			NSString *filepath = [[openPanel filenames] objectAtIndex:0];
            listHSSStatements(filepath);
		}
	}
}

- (IBAction)listTokens:(id)sender {
    //load the sample HSS file
	NSArray *hssFileType = [NSArray arrayWithObject:@"hss"];
	NSOpenPanel *openPanel = [NSOpenPanel openPanel];
	int result;
	[openPanel setCanChooseFiles:TRUE];
	[openPanel setAllowsMultipleSelection:FALSE];
	result = [openPanel runModalForTypes:hssFileType];
	if(result == NSOKButton){
		if([[openPanel filenames] count] > 0){
			NSString *filepath = [[openPanel filenames] objectAtIndex:0];
            listHSSTokens(filepath);
		}
	}
}
@end
