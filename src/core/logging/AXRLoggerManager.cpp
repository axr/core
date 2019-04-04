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
        std::list<AXRAbstractLogger*> loggers;
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
        d->loggers.push_back(logger);
}

bool AXRLoggerManager::containsLogger(AXRAbstractLogger *logger)
{
    return std::find(d->loggers.begin(), d->loggers.end(), logger) != d->loggers.end();
}

void AXRLoggerManager::removeLogger(AXRAbstractLogger *logger)
{
    d->loggers.remove(logger);
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
    std::list<AXRLoggerChannel> loggerFlagsToList(AXRLoggerChannels channels)
    {
        std::list<AXRLoggerChannel> channelList;

        // It's necessary to check for > 0 since after i <= LoggerChannelMax
        // evaluates to 2^31 - 1 <= 2^31 - 1 we'll overflow to negative
        for (int i = 1; i > 0 && i <= LoggerChannelMax; i *= 2)
            if (channels & i)
                channelList.push_back(static_cast<AXRLoggerChannel>(i));

        return channelList;
    }

    AXRString loggerChannelToString(AXRLoggerChannel channel)
    {
        switch (channel)
        {
            case LoggerChannelAll:
                return AXRString("LoggerChannelAll");
            case LoggerChannelUserError:
                return AXRString("LoggerChannelUserError");
            case LoggerChannelUserWarning:
                return AXRString("LoggerChannelUserWarning");
            case LoggerChannelOverview:
                return AXRString("LoggerChannelOverview");
            case LoggerChannelGeneral:
                return AXRString("LoggerChannelGeneral");
            case LoggerChannelGeneralSpecific:
                return AXRString("LoggerChannelGeneralSpecific");
            case LoggerChannelIO:
                return AXRString("LoggerChannelIO");
            case LoggerChannelNetwork:
                return AXRString("LoggerChannelNetwork");
            case LoggerChannelXMLParser:
                return AXRString("LoggerChannelXMLParser");
            case LoggerChannelHSSParser:
                return AXRString("LoggerChannelHSSParser");
            case LoggerChannelHSSTokenizer:
                return AXRString("LoggerChannelHSSTokenizer");
            case LoggerChannelLayout:
                return AXRString("LoggerChannelLayout");
            case LoggerChannelRendering:
                return AXRString("LoggerChannelRendering");
            case LoggerChannelObserving:
                return AXRString("LoggerChannelObserving");
            case LoggerChannelEvents:
                return AXRString("LoggerChannelEvents");
            case LoggerChannelEventsSpecific:
                return AXRString("LoggerChannelEventsSpecific");
            case LoggerChannelController:
                return AXRString("LoggerChannelController");
            case LoggerChannelControllerSpecific:
                return AXRString("LoggerChannelControllerSpecific");
            case LoggerChannelLogFunction:
                return AXRString("LoggerChannelLogFunction");

            case LoggerChannelObsolete0:
                return AXRString("LoggerChannelObsolete0");
            case LoggerChannelObsolete1:
                return AXRString("LoggerChannelObsolete1");
            case LoggerChannelObsolete3:
                return AXRString("LoggerChannelObsolete3");

            default:
                return AXRString();
        }
    }

    AXRLoggerChannel stringToLoggerChannel(const AXRString &channelName)
    {
        if (channelName == AXRString("LoggerChannelAll"))
            return LoggerChannelAll;
        else if (channelName == AXRString("LoggerChannelUserError"))
            return LoggerChannelUserError;
        else if (channelName == AXRString("LoggerChannelUserWarning"))
            return LoggerChannelUserWarning;
        else if (channelName == AXRString("LoggerChannelOverview"))
            return LoggerChannelOverview;
        else if (channelName == AXRString("LoggerChannelGeneral"))
            return LoggerChannelGeneral;
        else if (channelName == AXRString("LoggerChannelGeneralSpecific"))
            return LoggerChannelGeneralSpecific;
        else if (channelName == AXRString("LoggerChannelIO"))
            return LoggerChannelIO;
        else if (channelName == AXRString("LoggerChannelNetwork"))
            return LoggerChannelNetwork;
        else if (channelName == AXRString("LoggerChannelXMLParser"))
            return LoggerChannelXMLParser;
        else if (channelName == AXRString("LoggerChannelHSSParser"))
            return LoggerChannelHSSParser;
        else if (channelName == AXRString("LoggerChannelHSSTokenizer"))
            return LoggerChannelHSSTokenizer;
        else if (channelName == AXRString("LoggerChannelLayout"))
            return LoggerChannelLayout;
        else if (channelName == AXRString("LoggerChannelRendering"))
            return LoggerChannelRendering;
        else if (channelName == AXRString("LoggerChannelObserving"))
            return LoggerChannelObserving;
        else if (channelName == AXRString("LoggerChannelEvents"))
            return LoggerChannelEvents;
        else if (channelName == AXRString("LoggerChannelEventsSpecific"))
            return LoggerChannelEventsSpecific;
        else if (channelName == AXRString("LoggerChannelController"))
            return LoggerChannelController;
        else if (channelName == AXRString("LoggerChannelControllerSpecific"))
            return LoggerChannelControllerSpecific;
        else if (channelName == AXRString("LoggerChannelLogFunction"))
            return LoggerChannelLogFunction;

        else if (channelName == AXRString("LoggerChannelObsolete0"))
            return LoggerChannelObsolete0;
        else if (channelName == AXRString("LoggerChannelObsolete1"))
            return LoggerChannelObsolete1;
        else if (channelName == AXRString("LoggerChannelObsolete3"))
            return LoggerChannelObsolete3;

        else
            return LoggerChannelNone;
    }
}
