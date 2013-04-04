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

#ifndef AXRGENERICLOGGER_H
#define AXRGENERICLOGGER_H

#include "AXRAbstractLogger.h"

class QIODevice;

namespace AXR
{
    class AXRGenericLoggerPrivate;

    class AXR_API AXRGenericLogger : public AXRAbstractLogger
    {
        Q_DISABLE_COPY(AXRGenericLogger)

    public:
        AXRGenericLogger(const AXRString &name = "");
        ~AXRGenericLogger();

        bool autoFlush() const;
        void setAutoFlush(bool flush);

        QIODevice* outputDevice(AXRLoggerChannel channel) const;
        void setOutputDevice(AXRLoggerChannels channels, QIODevice *device);
        void setStandardOutputDevice(AXRLoggerChannels channels);
        void setStandardErrorDevice(AXRLoggerChannels channels);

    protected:
        void log(AXRLoggerChannel channel, const AXRString &message, bool newLine);

    private:
        AXRGenericLoggerPrivate *d;
    };
}

#endif // AXRLOGGER_H
