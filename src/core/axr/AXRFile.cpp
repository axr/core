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

#include <cstdio>
#include "AXRFile.h"

using namespace AXR;

AXRFile::AXRFile()
{
    this->fileName = "";
    this->mimeType = "";
    this->basePath = "";
    this->extension = "";
    this->fileHandle = NULL;
    this->_atEndOfFile = false;
}

AXRFile::~AXRFile()
{
    if (this->fileHandle)
    {
        fclose(this->fileHandle);
    }
}

void AXRFile::setFileName(std::string value)
{
    this->fileName = value;
}

std::string AXRFile::getFileName()
{
    return this->fileName;
}

void AXRFile::setBuffer(const QByteArray &buffer)
{
    this->buffer = buffer;
}

QByteArray& AXRFile::getBuffer()
{
    return this->buffer;
}

int AXRFile::getFileSize()
{
    return this->buffer.size();
}

int AXRFile::getBufferSize()
{
    return this->buffer.size();
}

void AXRFile::setBasePath(std::string path)
{
    this->basePath = path;
}

std::string AXRFile::getBasePath()
{
    return this->basePath;
}

void AXRFile::setMimeType(std::string mime)
{
    this->mimeType = mime;
}

std::string AXRFile::getMimeType()
{
    return this->mimeType;
}

void AXRFile::setExtension(std::string value)
{
    this->extension = value;
}

std::string AXRFile::getExtension()
{
    return this->extension;
}

void AXRFile::setFileHandle(FILE * path)
{
    this->fileHandle = path;
}

FILE * AXRFile::getFileHandle()
{
    return this->fileHandle;
}

std::string AXRFile::toString()
{
    return "AXRFile:\nFilename: " + this->fileName + "\nBasepath: " + this->basePath + "\n-------------------------\n";
}

bool AXRFile::isAtEndOfFile()
{
    return this->_atEndOfFile;
}

void AXRFile::setAtEndOfFile(bool newValue)
{
    this->_atEndOfFile = newValue;
}
