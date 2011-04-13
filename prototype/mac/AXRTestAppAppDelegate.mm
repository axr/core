//
//  macAppDelegate.m
//  mac
//
//  Created by Miro Keller on 13/02/11.
//  Copyright 2011 Miro Kelelr. All rights reserved.
//

#import "AXRTestAppAppDelegate.h"
#import <string>
#import <iostream>
#import "../core/AXR.h"

#define BUFFSIZE 8192
char Buff[BUFFSIZE];

@implementation AXRTestAppAppDelegate

@synthesize testsWindow;
@synthesize axrWindow;
@synthesize axrView;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    //[self openDocument:self];
}

void listHSSStatements(NSString *filepath)
{
    std_log1(std::string("******************************************************************\n* reading all statements from\n* ").append([filepath UTF8String]).append("\n******************************************************************\n"));
    
    //FIXME: this is uh-gly
    AXR::AXRController::p controller(AXR::AXRController::create());
    AXR::HSSParser::p hssparser(new AXR::HSSParser(controller));
    bool loaded = hssparser->loadFile(std::string([filepath UTF8String]));
    if(loaded){
        unsigned i;
        const std::vector<AXR::HSSStatement::p> statements = controller->getStatements();
        for(i=0; i<statements.size(); i++){
            std_log1(statements[i]->toString());
        }
    } else {
        std_log1("error loading file");
    }
    
    std::cout << "reached end of source" << std::endl;
    std::cout << "\n\n\n\n";

    //std_log1("sorry, this is not working yet");
#if AXR_DEBUG_LEVEL > 1
    exit(0);
#endif
}

void listHSSTokens(NSString *filepath)
{
    std_log1(std::string("******************************************************************\n* reading all tokens from\n* ").append([filepath UTF8String]).append("\n******************************************************************\n"));
    
    FILE * hssfile = fopen([filepath UTF8String], "r");
    AXR::HSSTokenizer::p tokenizer = AXR::HSSTokenizer::p(new AXR::HSSTokenizer());
    int len = (int)fread(tokenizer->getBuffer().get(), 1, AXR_HSS_BUFFER_SIZE, hssfile);
    tokenizer->setBufferLength(len);
    tokenizer->readNextChar();
    fclose(hssfile);
    
    
    AXR::HSSToken::p token;
    bool done = FALSE;
    int security_count = 0;
    while (!done) {
        if(token){
            token.reset();
        }
        token = tokenizer->readNextToken();
        if(!token){
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
#if AXR_DEBUG_LEVEL > 1
    exit(0);
#endif
}

void listXMLElements(NSString *filepath)
{
//    std_log1(std::string("******************************************************************\n* reading all XML elements from\n* ").append([filepath UTF8String]).append("\n******************************************************************"));
//    
//    AXR::AXRController * controller = new AXR::AXRController();
//    AXR::XMLParser parser = AXR::XMLParser(controller, [filepath UTF8String], [[filepath lastPathComponent] UTF8String]);
//    try {
//        parser.Parse();
//    }
//    catch(AXR::XMLUnexpectedEndOfSourceException e){
//        std::cout << e.toString() << std::endl;
//    }
//    catch(AXR::XMLUnknownProcessingInstructionException e){
//        std::cout << e.toString() << std::endl;
//    }
//    catch(AXR::XMLMalformedProcessingInstructionException e){
//        std::cout << e.toString() << std::endl;
//    }
//    
//    std::cout << std::endl << "-----------------------------" << std::endl
//    <<  controller->toString() << std::endl << "-----------------------------" << std::endl;
}

- (void)listXMLElements
{
    //load the XML file
	NSArray *xmlFileType = [NSArray arrayWithObject:@"xml"];
	NSOpenPanel *openPanel = [NSOpenPanel openPanel];
	int result;
	[openPanel setCanChooseFiles:TRUE];
	[openPanel setAllowsMultipleSelection:FALSE];
	result = [openPanel runModalForTypes:xmlFileType];
	if(result == NSOKButton){
		if([[openPanel filenames] count] > 0){
			NSString *filepath = [[openPanel filenames] objectAtIndex:0];
            listXMLElements(filepath);
		}
	}
    std::cout << "reached end of source" << std::endl;
    std::cout << "\n\n\n\n";
}

- (IBAction)listStatements:(id)sender {
    //load the HSS file
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
    //load the HSS file
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

- (IBAction)listXMLElements:(id)sender {
    [self listXMLElements];
}

- (IBAction)openDocument:(id)sender {
    [[self axrWindow] makeKeyAndOrderFront:self];
    //sending loadfile
    [[self axrView] loadFile];
    
}

- (IBAction)reload:(id)sender {
    bool loaded = [[self axrView] reload];
    if(!loaded){
        NSLog(@"Could not reload file because of an unknown error.");
    }
}


@end
