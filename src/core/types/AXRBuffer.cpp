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

#include "AXRBuffer.h"

using namespace AXR;

AXRBuffer::AXRBuffer()
: valid(false)
{
}

AXRBuffer::AXRBuffer(const QByteArray &data)
: valid(true), buffer(data)
{
}

AXRBuffer::AXRBuffer(const QFileInfo &filePath)
{
    QFile file(filePath.canonicalFilePath());
    if (file.open(QIODevice::ReadOnly))
    {
        this->buffer = file.readAll();
        this->sourceUrl = filePath.canonicalFilePath();
        this->valid = this->buffer.size() == file.size();
    }
}

AXRBuffer::AXRBuffer(const QFileInfo &filePath, const QByteArray &data)
: valid(true), buffer(data), sourceUrl(filePath.canonicalFilePath())
{
}

AXRBuffer::~AXRBuffer()
{
}

AXRString AXRBuffer::getFileName() const
{
    QFileInfo fi(this->sourceUrl.toLocalFile());
    return fi.fileName();
}

AXRString AXRBuffer::getBasePath() const
{
    QFileInfo fi(this->sourceUrl.toLocalFile());
    return fi.canonicalPath();
}

const QByteArray& AXRBuffer::getBuffer() const
{
    return this->buffer;
}

bool AXRBuffer::isValid() const
{
    return this->valid;
}

AXRString AXRBuffer::toString() const
{
    return "AXRBuffer:\nFilename: " + getFileName() + "\nBasepath: " + getBasePath() + "\n-------------------------\n";
}
