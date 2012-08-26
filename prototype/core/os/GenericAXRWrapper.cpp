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

#include <iostream>
#include "AXR.h"
#include "AXRDebugging.h"
#include "AXRError.h"
#include "GenericAXRWrapper.h"

using namespace AXR;

GenericAXRWrapper::GenericAXRWrapper() : AXRWrapper()
{
    this->needsDisplay = true;
}

AXRWrapper * GenericAXRWrapper::createWrapper()
{
    return new GenericAXRWrapper();
}

GenericAXRWrapper::~GenericAXRWrapper()
{
}

AXRFile::p GenericAXRWrapper::getFile(std::string url)
{
    AXRFile::p ret = AXRFile::p(new AXRFile());

    if (url.substr(0, 7) == "file://")
    {
        std::string clean_path = url.substr(7, std::string::npos);
        int slashpos = clean_path.rfind("/");
        ret->setFileName(clean_path.substr(slashpos + 1, clean_path.size()));
        ret->setBasePath(clean_path.substr(0, slashpos));

        ret->setBufferSize(20240);
        ret->setBuffer(new char[ret->getBufferSize()]);
        ret->setFileHandle(fopen(clean_path.c_str(), "r"));

        if (ret->getFileHandle() == NULL)
        {
            AXRError::p(new AXRError("GenericAXRWrapper", "the file " + ret->getFileName() + " doesn't exist " + ret->getBasePath()))->raise();
        }
        else if (ferror(ret->getFileHandle()))
        {
            AXRError::p(new AXRError("GenericAXRWrapper", "the file " + ret->getFileName() + " couldn't be read"))->raise();
        }
    }
    else
    {
        std_log("http is not implemented yet");
    }

    return ret;
}

size_t GenericAXRWrapper::readFile(AXRFile::p theFile)
{
    size_t size = fread(theFile->getBuffer(), sizeof (theFile->getBuffer()[0]),
                        theFile->getBufferSize(), theFile->getFileHandle());

    if (ferror(theFile->getFileHandle()))
    {
        fclose(theFile->getFileHandle());

        return -1;
    }

    return size;
}

void GenericAXRWrapper::closeFile(AXRFile::p theFile)
{
    fclose(theFile->getFileHandle());
    theFile->setFileHandle(NULL);
}

void GenericAXRWrapper::handleError(AXRError::p theError)
{
    std::cout << theError->toString().c_str() << "\n";
}

bool GenericAXRWrapper::openFileDialog(std::string &filePath)
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

void GenericAXRWrapper::setNeedsDisplay(bool newValue)
{
    this->needsDisplay = newValue;
}

std::string GenericAXRWrapper::getPathToResources()
{
    // Code taken from: http://www.gamedev.net/community/forums/topic.asp?topic_id=459511
    std::string path = "";
    pid_t pid = getpid();
    char buf[20] = {0};
    sprintf(buf, "%d", pid);
    std::string _link = "/proc/";
    _link.append(buf);
    _link.append("/exe");
    char proc[512];
    int ch = readlink(_link.c_str(), proc, 512);
    if (ch != -1)
    {
        proc[ch] = 0;
        path = proc;
        std::string::size_type t = path.find_last_of("/");
        path = path.substr(0, t);
    }

    return path + std::string("/resources");
}

std::string GenericAXRWrapper::getPathToTestsFile()
{
    return this->_layoutTestsFilePath;
}
