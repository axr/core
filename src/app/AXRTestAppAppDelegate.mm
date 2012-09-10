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
#import <string>
#import "AXRDebugging.h"
#import "AXRTestAppAppDelegate.h"
#import "HSSCallback.h"
#import "HSSDisplayObject.h"
#import "CocoaAXRWrapper.h"

#define BUFFSIZE 8192
char Buff[BUFFSIZE];

@implementation AXRTestAppAppDelegate

@synthesize testsWindow;
@synthesize errorsWindow;
@synthesize errorLog;
@synthesize axrWindow;
@synthesize axrView;
@synthesize needsFile;

-(id) init
{
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

-(void) applicationDidFinishLaunching : (NSNotification *) aNotification
{

    //    NSString * filepath = [[NSBundle mainBundle] pathForResource:@"blank" ofType:@"xml" inDirectory:@"views"];
    //    [[self axrView] loadFile:filepath];

    if ([self needsFile])
    {
        [self openDocument : self];
        [self setNeedsFile : NO];
    }
}

-(BOOL) application : (NSApplication *) theApplication openFile : (NSString *) filename
{
    [[self axrWindow] makeKeyAndOrderFront : self];
    bool ret = [[self axrView] loadFile : filename];
    [self setNeedsFile : NO];
    return ret;
}

void listHSSStatements(NSString *filepath)
{
    //    std_log1(std::string("******************************************************************\n* reading all statements from\n* ").append([filepath UTF8String]).append("\n******************************************************************\n"));
    //
    //    //FIXME: this is uh-gly
    //    AXR::AXRController controller = AXR::AXRController() ;
    //    AXR::HSSParser hssparser = AXR::HSSParser(&controller);
    //    bool loaded = hssparser.loadFile(std::string([filepath UTF8String]));
    //    if(loaded){
    //        unsigned i;
    //        const std::vector<AXR::HSSStatement::p> statements = controller.getStatements();
    //        for(i=0; i<statements.size(); ++i){
    //            std_log1(statements[i]->toString());
    //        }
    //    } else {
    //        std_log1("error loading file");
    //    }
    //
    //    std_log1("reached end of source");
    //    std_log1("\n\n\n\n");
    //
    //    //std_log1("sorry, this is not working yet");
    //#if AXR_DEBUG_LEVEL > 1
    //    exit(0);
    //#endif
}

void listHSSTokens(NSString *filepath)
{
    //    std_log1(std::string("******************************************************************\n* reading all tokens from\n* ").append([filepath UTF8String]).append("\n******************************************************************\n"));
    //
    //    FILE * hssfile = fopen([filepath UTF8String], "r");
    //    AXR::HSSTokenizer tokenizer = AXR::HSSTokenizer();
    //    int len = (int)fread(tokenizer.getBuffer().get(), 1, AXR_HSS_BUFFER_SIZE, hssfile);
    //    tokenizer.setBufferLength(len);
    //    tokenizer.readNextChar();
    //    fclose(hssfile);
    //
    //
    //    AXR::HSSToken::p token;
    //    bool done = FALSE;
    //    int security_count = 0;
    //    while (!done) {
    //        if(token){
    //            token.reset();
    //        }
    //        token = tokenizer.readNextToken();
    //        if(!token){
    //            done = TRUE;
    //        } else {
    //            std::cout << token->toString() << std::endl;
    //        }
    //
    //        if(security_count == 99999)
    //        {
    //            break;
    //        } else {
    //            security_count++;
    //        }
    //    }
    //    std_log1("reached end of source");
    //    std_log1("\n\n\n\n");
    //#if AXR_DEBUG_LEVEL > 1
    //    exit(0);
    //#endif
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

-(void) listXMLElements
{
    //    //load the XML file
    //  NSArray *xmlFileType = [NSArray arrayWithObject:@"xml"];
    //  NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    //  int result;
    //  [openPanel setCanChooseFiles:TRUE];
    //  [openPanel setAllowsMultipleSelection:FALSE];
    //  result = [openPanel runModalForTypes:xmlFileType];
    //  if(result == NSOKButton){
    //      if([[openPanel filenames] count] > 0){
    //          NSString *filepath = [[openPanel filenames] objectAtIndex:0];
    //            listXMLElements(filepath);
    //      }
    //  }
    //    std_log1("reached end of source");
    //    std_log1("\n\n\n\n");
}

-(IBAction) listStatements : (id) sender
{
    //    //load the HSS file
    //  NSArray *hssFileType = [NSArray arrayWithObject:@"hss"];
    //  NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    //  int result;
    //  [openPanel setCanChooseFiles:TRUE];
    //  [openPanel setAllowsMultipleSelection:FALSE];
    //  result = [openPanel runModalForTypes:hssFileType];
    //  if(result == NSOKButton){
    //      if([[openPanel filenames] count] > 0){
    //          NSString *filepath = [[openPanel filenames] objectAtIndex:0];
    //            listHSSStatements(filepath);
    //      }
    //  }
}

-(IBAction) listTokens : (id) sender
{
    //    //load the HSS file
    //  NSArray *hssFileType = [NSArray arrayWithObject:@"hss"];
    //  NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    //  int result;
    //  [openPanel setCanChooseFiles:TRUE];
    //  [openPanel setAllowsMultipleSelection:FALSE];
    //  result = [openPanel runModalForTypes:hssFileType];
    //  if(result == NSOKButton){
    //      if([[openPanel filenames] count] > 0){
    //          NSString *filepath = [[openPanel filenames] objectAtIndex:0];
    //            listHSSTokens(filepath);
    //      }
    //  }
}

-(IBAction) listXMLElements : (id) sender
{
    [self listXMLElements];
}

-(IBAction) openDocument : (id) sender
{
    [[self axrWindow] makeKeyAndOrderFront : self];
    //sending loadfile
    [[self axrView] loadFile];
}

-(IBAction) layoutTests : (id) sender
{

    [[self axrWindow] makeKeyAndOrderFront : self];
    NSString * filepath = [[NSBundle mainBundle] pathForResource : @"layoutTests" ofType : @"hss" inDirectory : @"views"];
    AXR::CocoaAXRWrapper * wrapper = (AXR::CocoaAXRWrapper *)[[self axrView] axrWrapper];
    AXR::AXRCore::tp & core = AXR::AXRCore::getInstance();
    core->registerCustomFunction("AXRLayoutTestsExecute", new AXR::HSSValueChangedCallback<AXR::CocoaAXRWrapper > (wrapper, &AXR::AXRWrapper::executeLayoutTests));
    [[self axrView] loadFile : filepath];
    return;
}

-(IBAction) reload : (id) sender
{
    [[self axrView] reload];
}

-(IBAction) previousLayoutStep : (id) sender
{
    AXR::CocoaAXRWrapper * wrapper = (AXR::CocoaAXRWrapper *)[[self axrView] axrWrapper];
    wrapper->setShowLayoutSteps(true);
    wrapper->previousLayoutStep();
}

-(IBAction) nextLayoutStep : (id) sender
{
    AXR::CocoaAXRWrapper * wrapper = (AXR::CocoaAXRWrapper *)[[self axrView] axrWrapper];
    wrapper->setShowLayoutSteps(true);
    wrapper->nextLayoutStep();
}

@end
