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
#include "AXRAbstractLogger.h"

using namespace AXR;

namespace AXR
{
    class AXRAbstractLoggerPrivate
    {
    public:
        AXRAbstractLoggerPrivate() : name(), activeChannels()
        {
        }

        AXRString name;
        AXRLoggerChannels activeChannels;
    };
}

AXRAbstractLogger::AXRAbstractLogger(const AXRString &name)
: d(new AXRAbstractLoggerPrivate)
{
    setName(name);
}

AXRAbstractLogger::~AXRAbstractLogger()
{
    delete d;
}

/*!
 * Gets the name used to identify the logger to the user.
 *
 * This has no effect on the operation of the logger itself
 * and can be set to an empty string if desired.
 */
AXRString AXRAbstractLogger::name() const
{
    return d->name;
}

void AXRAbstractLogger::setName(const AXRString &name)
{
    d->name = name;
}

/*!
 * Gets the currently active logger channels.
 * Any output written to channels that are not active will be discarded.
 * No logging channels are active by default.
 */
AXRLoggerChannels AXRAbstractLogger::activeChannels() const
{
    return d->activeChannels;
}

/*!
 * Sets the currently active logger channels.
 */
void AXRAbstractLogger::setActiveChannels(AXRLoggerChannels channels)
{
    d->activeChannels = channels;
}

bool AXRAbstractLogger::allChannelsActive() const
{
    return d->activeChannels == LoggerChannelAll;
}

bool AXRAbstractLogger::areChannelsActive(AXRLoggerChannels channels) const
{
    return d->activeChannels & channels;
}

void AXRAbstractLogger::activateChannels(AXRLoggerChannels channels)
{
    d->activeChannels |= channels;
}

void AXRAbstractLogger::deactivateChannels(AXRLoggerChannels channels)
{
    d->activeChannels &= ~channels;
}

void AXRAbstractLogger::log(AXRLoggerChannels channels, const AXRString &message)
{
    foreach (AXRLoggerChannel channel, loggerFlagsToList(channels))
        if (d->activeChannels.testFlag(channel))
            log(channel, message, false);
}

void AXRAbstractLogger::logLine(AXRLoggerChannels channels, const AXRString &message)
{
    foreach (AXRLoggerChannel channel, loggerFlagsToList(channels))
        if (d->activeChannels.testFlag(channel))
            log(channel, message, true);
}
