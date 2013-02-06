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

#include "AXRAbstractLogger.h"
#include "AXRLoggerManager.h"
#include <QList>

using namespace AXR;

namespace AXR
{
    class AXRLoggerManagerPrivate
    {
    public:
        QList<AXRAbstractLogger*> loggers;
    };
}

AXRLoggerManager::AXRLoggerManager()
: d(new AXRLoggerManagerPrivate)
{
}

AXRLoggerManager::~AXRLoggerManager()
{
    delete d;
}

AXRLoggerManager& AXRLoggerManager::instance()
{
    static AXRLoggerManager instance;
    return instance;
}

void AXRLoggerManager::addLogger(AXRAbstractLogger *logger)
{
    if (!containsLogger(logger))
        d->loggers.append(logger);
}

bool AXRLoggerManager::containsLogger(AXRAbstractLogger *logger)
{
    return d->loggers.contains(logger);
}

void AXRLoggerManager::removeLogger(AXRAbstractLogger *logger)
{
    d->loggers.removeAll(logger);
}

void AXRLoggerManager::clearLoggers()
{
    d->loggers.clear();
}

void AXRLoggerManager::log(AXRLoggerChannels channels, const AXRString &message, bool newLine)
{
    foreach (AXRAbstractLogger *logger, d->loggers)
        if (logger)
        {
            if (newLine)
                logger->logLine(channels, message);
            else
                logger->log(channels, message);
        }
}

void AXRLoggerManager::logLine(AXRLoggerChannels channels, const AXRString &message)
{
    foreach (AXRAbstractLogger *logger, d->loggers)
        if (logger)
            logger->logLine(channels, message);
}

namespace AXR
{
    QList<AXRLoggerChannel> loggerFlagsToList(AXRLoggerChannels channels)
    {
        QList<AXRLoggerChannel> channelList;

        // It's necessary to check for > 0 since after i <= LoggerChannelMax
        // evaluates to 2^31 - 1 <= 2^31 - 1 we'll overflow to negative
        for (int i = 1; i > 0 && i <= LoggerChannelMax; i *= 2)
            if (channels & i)
                channelList.append(static_cast<AXRLoggerChannel>(i));

        return channelList;
    }
}
