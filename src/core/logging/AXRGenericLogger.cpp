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

#include "AXRGenericLogger.h"
#include <QFile>
#include <QTextStream>

using namespace AXR;

namespace AXR
{
    class AXRGenericLoggerPrivate
    {
    public:
        AXRGenericLoggerPrivate()
        : autoFlush(true), outputDevices(), stdoutDevice(), stderrDevice()
        {
            stdoutDevice.open(stdout, QIODevice::WriteOnly);
            stderrDevice.open(stderr, QIODevice::WriteOnly);
        }

        bool autoFlush;
        QMap<AXRLoggerChannel, QIODevice*> outputDevices;
        QFile stdoutDevice;
        QFile stderrDevice;
    };
}

AXRGenericLogger::AXRGenericLogger()
: d(new AXRGenericLoggerPrivate)
{
}

AXRGenericLogger::~AXRGenericLogger()
{
    delete d;
}

bool AXRGenericLogger::autoFlush() const
{
    return d->autoFlush;
}

void AXRGenericLogger::setAutoFlush(bool flush)
{
    d->autoFlush = flush;
}

/*!
 * Returns the current output device for the given logger channel,
 * or \c NULL if there is no output device set for the channel.
 *
 * If \a channel is \a LoggerChannelAll, this method returns \c NULL.
 */
QIODevice* AXRGenericLogger::outputDevice(AXRLoggerChannel channel) const
{
    if (channel == LoggerChannelAll)
        return NULL;

    return d->outputDevices[channel];
}

void AXRGenericLogger::setOutputDevice(AXRLoggerChannels channels, QIODevice *device)
{
    foreach (AXRLoggerChannel channel, loggerFlagsToList(channels))
        d->outputDevices[channel] = device;
}

void AXRGenericLogger::setStandardOutputDevice(AXRLoggerChannels channels)
{
    setOutputDevice(channels, &d->stdoutDevice);
}

void AXRGenericLogger::setStandardErrorDevice(AXRLoggerChannels channels)
{
    setOutputDevice(channels, &d->stderrDevice);
}

void AXRGenericLogger::log(AXRLoggerChannel channel, const AXRString &message, bool newLine)
{
    QIODevice *device = outputDevice(channel);
    if (device && device->isWritable())
    {
        QTextStream out(device);
        out << message;
        if (newLine)
            endl(out);

        if (d->autoFlush)
            out.flush();
    }
}
