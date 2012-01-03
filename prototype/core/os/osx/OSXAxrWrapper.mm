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
 ********************************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2012/01/03
 *      HSS version: 1.0
 *      Core version: 0.44
 *      Revision: 1
 *
 ********************************************************************/

#include "OSXAxrWrapper.h"
#include "../../AXR.h"
#include "../../axr/errors/AXRError.h"
#include "../../axr/AXRDebugging.h"
#include <Cocoa/Cocoa.h>

using namespace AXR;

OSXAxrWrapper::OSXAxrWrapper()
: AXRWrapper()
{
    
}

OSXAxrWrapper::~OSXAxrWrapper()
{
    
}

AXRFile::p OSXAxrWrapper::getFile(std::string url)
{
    AXRFile::p ret = AXRFile::p(new AXRFile());
    
    if(url.substr(0, 7) == "file://"){
        std::string clean_path = url.substr(7, url.size());
        int slashpos = clean_path.rfind("/");
        ret->fileName = clean_path.substr(slashpos+1, clean_path.size());
        ret->basePath = clean_path.substr(0, slashpos);
        
        ret->bufferSize = 1024;
        ret->buffer = new char[ret->bufferSize];
        ret->fileHandle = fopen(clean_path.c_str(), "r");
        if( ret->fileHandle == NULL ){
            AXRError::p(new AXRError("OSXAxrWrapper", "the file"+ret->fileName+" doesn't exist"))->raise();
        } else if( ferror(ret->fileHandle) ){
            AXRError::p(new AXRError("OSXAxrWrapper", "the file"+ret->fileName+" couldn't be read"))->raise();
        }
        
    } else {
        std_log("http is not implemented yet");
    }
    
    return ret;
}

size_t OSXAxrWrapper::readFile(AXRFile::p theFile)
{
    size_t size = fread(theFile->buffer, sizeof(theFile->buffer[0]), theFile->bufferSize, theFile->fileHandle);
    if (ferror(theFile->fileHandle)) {
        fclose(theFile->fileHandle);
        return -1;
    }
    return size;
}

void OSXAxrWrapper::closeFile(AXRFile::p theFile)
{
    fclose(theFile->fileHandle);
}

void OSXAxrWrapper::handleError()
{
    
}


bool OSXAxrWrapper::openFileDialog(std::string &filePath)
{
    //load a file
	NSArray *fileTypes = [NSArray arrayWithObjects: @"xml", @"hss", nil];
	NSOpenPanel *openPanel = [NSOpenPanel openPanel];
	int result;
	[openPanel setCanChooseFiles:TRUE];
	[openPanel setAllowsMultipleSelection:FALSE];
	result = [openPanel runModalForTypes:fileTypes];
	if(result == NSOKButton){
		if([[openPanel filenames] count] > 0){
			NSString *filepath_s = [[openPanel filenames] objectAtIndex:0];
            std_log1(std::string("******************************************************************\n* opening document:\n* ").append([filepath_s UTF8String]).append("\n******************************************************************"));
            filePath = std::string([filepath_s UTF8String]);
            return true;
        }
    }
    
    return false;
}
