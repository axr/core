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
 *      AUTHORS: Valerij Primachenko, Ragnis Armus
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
 *      Last changed: 2012/03/07
 *      HSS version: 1.0
 *      Core version: 0.44
 *      Revision: 2
 *
 ********************************************************************/

#define AXR_PLATFORM_LINUX
#include <iostream>
#include "LinuxAxrWrapper.h"
#include "../../AXR.h"
#include "../../axr/errors/AXRError.h"
#include "../../axr/AXRDebugging.h"

using namespace AXR;

LinuxAxrWrapper::LinuxAxrWrapper() : AXRWrapper()
{
	this->needsDisplay = true;
}

LinuxAxrWrapper::~LinuxAxrWrapper()
{
}

AXRFile::p LinuxAxrWrapper::getFile(std::string url)
{
	AXRFile::p ret = AXRFile::p(new AXRFile());

	if (url.substr(0, 7) == "file://")
	{
		std::string clean_path = url.substr(7, std::string::npos);
		int slashpos = clean_path.rfind("/");
		ret->setFileName(clean_path.substr(slashpos + 1, clean_path.size()));
		ret->basePath = clean_path.substr(0, slashpos);

		ret->bufferSize = 10240;
		ret->buffer = new char[ret->bufferSize];
		ret->fileHandle = fopen(clean_path.c_str(), "r");

		if (ret->fileHandle == NULL)
		{
			AXRError::p(new AXRError("LinuxAxrWrapper", "the file " + ret->getFileName() + " doesn't exist " + ret->basePath))->raise();
		}
		else if (ferror(ret->fileHandle))
		{
			AXRError::p(new AXRError("LinuxAxrWrapper", "the file " + ret->getFileName() + " couldn't be read"))->raise();
		}
	}
	else
	{
		std_log("http is not implemented yet");
	}

	return ret;
}

size_t LinuxAxrWrapper::readFile(AXRFile::p theFile)
{
	size_t size = fread(theFile->buffer, sizeof(theFile->buffer[0]),
		theFile->bufferSize, theFile->fileHandle);

	if (ferror(theFile->fileHandle))
	{
		fclose(theFile->fileHandle);

		return -1;
	}

	return size;
}

void LinuxAxrWrapper::closeFile(AXRFile::p theFile)
{
	fclose(theFile->fileHandle);
}

void LinuxAxrWrapper::handleError(AXRError::p theError)
{
	std::cout << theError->toString() << "\n";
}

bool LinuxAxrWrapper::openFileDialog(std::string &filePath)
{
	char file[FILENAME_MAX];

	std::cout << "Open file: ";
	fgets(file, FILENAME_MAX, stdin);

	if (file[strlen(file) - 1] == '\n')
	{
		file[strlen(file) - 1] = '\0';
	}

	if (file[0] == '/')
	{
		filePath = file;
	}
	else
	{
		char cwd[FILENAME_MAX];
		getcwd(cwd, FILENAME_MAX);

		std::stringstream ss;
		ss << cwd << "/" << file;
		filePath = ss.str();
	}

	std::cout << std::endl;

	return true;
}

void LinuxAxrWrapper::setNeedsDisplay(bool newValue)
{
	this->needsDisplay = newValue;
}

