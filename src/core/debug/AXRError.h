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
 *      COPYRIGHT: ©2013 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axrproject.org
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#ifndef AXRERROR_H
#define AXRERROR_H

#include <QSharedDataPointer>
#include "AXRGlobal.h"

namespace AXR
{
    /**
     *  @brief  This class, and it's subclass AXRWarning, are used for error handling in
     *  the rendering engine. They can be displayed using raise() or thrown.
     */
    class AXR_API AXRError
    {
    public:
        /**
         *  Creates a new instance of an error, with information about the filename,
         *  the line and column where it happened.
         *  @param origin   The name of the class where the error happened.
         *  @param message  The error message to be shown.
         *  @param url      The URL of the file where the error happened.
         *  @param line     The line index (starting at 1) in the file where the error happened.
         *  @param column   The column index (starting at 1) in the line where the error happened.
         */
        AXRError(const AXRString &origin, const AXRString &message, const QUrl &url = QUrl(), qint64 line = 0, qint64 column = 0);
        AXRError(const AXRError &other);
        virtual ~AXRError();

        /**
         *  Call this if you want the message to be printed to the logger.
         */
        virtual void raise() const;

        /**
         *  Print itself as a textual representation.
         *  @return A string containing the textual representation of the error.
         */
        virtual AXRString toString() const;

        /**
         *  Getter for message.
         *  @return The message in this error.
         */
        AXRString getMessage() const;
        
        qint64 getLine() const;
        qint64 getColumn() const;

        AXRError& operator=(const AXRError &other);

    protected:
        virtual AXRString toProblemString(const AXRString &label) const;

    private:
        class Data;
        QSharedDataPointer<Data> d;
    };
}

#endif
