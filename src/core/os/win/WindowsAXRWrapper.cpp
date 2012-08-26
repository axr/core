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

#include "AXR.h"
#include "AXRDebugging.h"
#include "AXRError.h"
#include "WindowsAXRWrapper.h"

using namespace AXR;

WindowsAXRWrapper::WindowsAXRWrapper()
: AXRWrapper()
{
    this->needsDisplay = true;
}

AXRWrapper * WindowsAXRWrapper::createWrapper()
{
    return new WindowsAXRWrapper();
}

WindowsAXRWrapper::~WindowsAXRWrapper()
{

}

AXRFile::p WindowsAXRWrapper::getFile(std::string url)
{
    AXRFile::p ret = AXRFile::p(new AXRFile());



    if (url.substr(0, 7) == "file://")
    {
        std::string clean_path = url.substr(7, std::string::npos);
        int slashpos = clean_path.rfind("/");
        ret->getFileName() = clean_path.substr(slashpos + 1, clean_path.size());
        ret->basePath = clean_path.substr(0, slashpos);


        ret->bufferSize = 10240;
        ret->buffer = new char[ret->bufferSize];
        ret->fileHandle = fopen(clean_path.c_str(), "r");



        if (ret->fileHandle == NULL)
        {
            AXRError::p(new AXRError("WindowsAXRWrapper", "the file " + ret->getFileName() + " doesn't exist"))->raise();
        }
        else if (ferror(ret->fileHandle))
        {
            AXRError::p(new AXRError("WindowsAXRWrapper", "the file " + ret->getFileName() + " couldn't be read"))->raise();
        }

    }
    else
    {
        std_log("http is not implemented yet");
    }

    return ret;
}

size_t WindowsAXRWrapper::readFile(AXRFile::p theFile)
{
    size_t size = fread(theFile->buffer, sizeof (theFile->buffer[0]), theFile->bufferSize, theFile->fileHandle);
    if (ferror(theFile->fileHandle))
    {
        fclose(theFile->fileHandle);
        return -1;
    }

    return size;
}

void WindowsAXRWrapper::closeFile(AXRFile::p theFile)
{
    fclose(theFile->fileHandle);
    theFile->fileHandle = NULL;
}

void WindowsAXRWrapper::handleError(AXRError::p theError)
{
    std::cout << theError->toString() << "\n";
}

bool WindowsAXRWrapper::openFileDialog(std::string &filePath)
{
    OPENFILENAME ofn;
    HANDLE hf;
    char szFile[260] = {0};

    ZeroMemory(&ofn, sizeof (OPENFILENAME));
    ofn.lStructSize = sizeof (OPENFILENAME);
    ofn.hwndOwner = this->hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof (szFile);
    ofn.lpstrFilter = "xml\0*.xml\0hss\0*.hss\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) != TRUE) return false;

    filePath = szFile;
    std::replace(filePath.begin(), filePath.end(), '\\', '/');

    return true;
}

void WindowsAXRWrapper::setNeedsDisplay(bool newValue)
{
    this->needsDisplay = newValue;
}
