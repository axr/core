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
: GenericAXRWrapper()
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
