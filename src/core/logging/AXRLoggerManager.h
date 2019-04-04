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

#ifndef AXRLOGGERMANAGER_H
#define AXRLOGGERMANAGER_H

#include "AXRGlobal.h"
#include "AXRLoggerEnums.h"

namespace AXR
{
    class AXRAbstractLogger;
    class AXRLoggerManagerPrivate;

    class AXR_API AXRLoggerManager
    {
        Q_DISABLE_COPY(AXRLoggerManager)

    public:
        static AXRLoggerManager& instance();
        ~AXRLoggerManager();

        void addLogger(AXRAbstractLogger *logger);
        bool containsLogger(AXRAbstractLogger *logger);
        void removeLogger(AXRAbstractLogger *logger);
        void clearLoggers();

        void log(AXRLoggerChannels channels, const AXRString &message, bool newLine = false);
        void logLine(AXRLoggerChannels channels, const AXRString &message);

    private:
        AXRLoggerManager();
        AXRLoggerManagerPrivate *d;
    };
}

/*!
 * Convenience macro for \a AXRLoggerManager::logLine.
 */
#define axr_log(channels, message) AXRLoggerManager::instance().logLine(static_cast<AXRLoggerChannels>(channels), message)

#endif // AXRLOGGERMANAGER_H
