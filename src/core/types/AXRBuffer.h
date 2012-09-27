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

#ifndef AXRBUFFER_H
#define AXRBUFFER_H

#include <boost/shared_ptr.hpp>
#include <QByteArray>
#include <QFileInfo>
#include <QUrl>
#include "AXRString.h"
#include "AXRGlobal.h"

namespace AXR
{
    /**
     *  @brief Provides an array of bytes and references to the name of the file
     *  it was loaded from.
     */
    class AXR_API AXRBuffer
    {
    public:
        /**
         *  The shared pointer to a file.
         */
        typedef boost::shared_ptr<AXRBuffer> p;

        /**
         *  Creates a new instance of an in-memory buffer.
         */
        AXRBuffer();
        AXRBuffer(const QByteArray &data);
        AXRBuffer(const QFileInfo &filePath);
        AXRBuffer(const QFileInfo &filePath, const QByteArray &data);

        /**
         *  Destructor.
         */
        virtual ~AXRBuffer();

        QUrl url() const;

        /**
         *  Getter for fileName
         *  @return A string containing the file name, including extension.
         */
        AXRString getFileName() const;

        /**
         *  Getter for the base path. This + / + the filename form the full path to
         *  the file.
         *  @return A string containing the base path of the file.
         */
        AXRString getBasePath() const;

        /**
         *  Getter for the buffer where the contents of the file will be stored.
         *  @return A reference to the internal buffer.
         */
        const QByteArray& getBuffer() const;

        bool isValid() const;

        /**
         *  @return A textual representation of the buffer.
         */
        AXRString toString() const;

    private:
        bool valid;
        QByteArray buffer;
        QUrl sourceUrl;
    };
}

#endif
