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

#include "axr.h"
#include "AXRLoggerManager.h"

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

    AXRString loggerChannelToString(AXRLoggerChannel channel)
    {
        switch (channel)
        {
            case LoggerChannelAll:
                return QLatin1String("LoggerChannelAll");
            case LoggerChannelUserError:
                return QLatin1String("LoggerChannelUserError");
            case LoggerChannelUserWarning:
                return QLatin1String("LoggerChannelUserWarning");
            case LoggerChannelOverview:
                return QLatin1String("LoggerChannelOverview");
            case LoggerChannelGeneral:
                return QLatin1String("LoggerChannelGeneral");
            case LoggerChannelGeneralSpecific:
                return QLatin1String("LoggerChannelGeneralSpecific");
            case LoggerChannelIO:
                return QLatin1String("LoggerChannelIO");
            case LoggerChannelNetwork:
                return QLatin1String("LoggerChannelNetwork");
            case LoggerChannelXMLParser:
                return QLatin1String("LoggerChannelXMLParser");
            case LoggerChannelHSSParser:
                return QLatin1String("LoggerChannelHSSParser");
            case LoggerChannelHSSTokenizer:
                return QLatin1String("LoggerChannelHSSTokenizer");
            case LoggerChannelLayout:
                return QLatin1String("LoggerChannelLayout");
            case LoggerChannelRendering:
                return QLatin1String("LoggerChannelRendering");
            case LoggerChannelObserving:
                return QLatin1String("LoggerChannelObserving");
            case LoggerChannelEvents:
                return QLatin1String("LoggerChannelEvents");
            case LoggerChannelEventsSpecific:
                return QLatin1String("LoggerChannelEventsSpecific");
            case LoggerChannelController:
                return QLatin1String("LoggerChannelController");
            case LoggerChannelControllerSpecific:
                return QLatin1String("LoggerChannelControllerSpecific");
            case LoggerChannelLogFunction:
                return QLatin1String("LoggerChannelLogFunction");

            case LoggerChannelObsolete0:
                return QLatin1String("LoggerChannelObsolete0");
            case LoggerChannelObsolete1:
                return QLatin1String("LoggerChannelObsolete1");
            case LoggerChannelObsolete3:
                return QLatin1String("LoggerChannelObsolete3");

            default:
                return AXRString();
        }
    }

    AXRLoggerChannel stringToLoggerChannel(const AXRString &channelName)
    {
        if (channelName == QLatin1String("LoggerChannelAll"))
            return LoggerChannelAll;
        else if (channelName == QLatin1String("LoggerChannelUserError"))
            return LoggerChannelUserError;
        else if (channelName == QLatin1String("LoggerChannelUserWarning"))
            return LoggerChannelUserWarning;
        else if (channelName == QLatin1String("LoggerChannelOverview"))
            return LoggerChannelOverview;
        else if (channelName == QLatin1String("LoggerChannelGeneral"))
            return LoggerChannelGeneral;
        else if (channelName == QLatin1String("LoggerChannelGeneralSpecific"))
            return LoggerChannelGeneralSpecific;
        else if (channelName == QLatin1String("LoggerChannelIO"))
            return LoggerChannelIO;
        else if (channelName == QLatin1String("LoggerChannelNetwork"))
            return LoggerChannelNetwork;
        else if (channelName == QLatin1String("LoggerChannelXMLParser"))
            return LoggerChannelXMLParser;
        else if (channelName == QLatin1String("LoggerChannelHSSParser"))
            return LoggerChannelHSSParser;
        else if (channelName == QLatin1String("LoggerChannelHSSTokenizer"))
            return LoggerChannelHSSTokenizer;
        else if (channelName == QLatin1String("LoggerChannelLayout"))
            return LoggerChannelLayout;
        else if (channelName == QLatin1String("LoggerChannelRendering"))
            return LoggerChannelRendering;
        else if (channelName == QLatin1String("LoggerChannelObserving"))
            return LoggerChannelObserving;
        else if (channelName == QLatin1String("LoggerChannelEvents"))
            return LoggerChannelEvents;
        else if (channelName == QLatin1String("LoggerChannelEventsSpecific"))
            return LoggerChannelEventsSpecific;
        else if (channelName == QLatin1String("LoggerChannelController"))
            return LoggerChannelController;
        else if (channelName == QLatin1String("LoggerChannelControllerSpecific"))
            return LoggerChannelControllerSpecific;
        else if (channelName == QLatin1String("LoggerChannelLogFunction"))
            return LoggerChannelLogFunction;

        else if (channelName == QLatin1String("LoggerChannelObsolete0"))
            return LoggerChannelObsolete0;
        else if (channelName == QLatin1String("LoggerChannelObsolete1"))
            return LoggerChannelObsolete1;
        else if (channelName == QLatin1String("LoggerChannelObsolete3"))
            return LoggerChannelObsolete3;

        else
            return LoggerChannelNone;
    }
}
