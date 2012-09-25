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

#include <QTextStream>
#include "AXRDebugging.h"

using namespace AXR;

unsigned int axr_debug_level = 1;

void std_log_level(const AXRString &message, unsigned int debugLevel, bool newline)
{
    if (debugLevel <= axr_debug_level)
    {
        QTextStream out(stdout);
        out << message;
        if (newline)
            endl(out);

        out.flush();
    }
}

void std_log(const AXRString &message, bool newline)
{
    std_log_level(message, 0, newline);
}

void std_log1(const AXRString &message, bool newline)
{
    std_log_level(message, 1, newline);
}

void std_log2(const AXRString &message, bool newline)
{
    std_log_level(message, 2, newline);
}

void std_log3(const AXRString &message, bool newline)
{
    std_log_level(message, 3, newline);
}

void std_log4(const AXRString &message, bool newline)
{
    std_log_level(message, 4, newline);
}

quint32 axr_debug_active_channels = 0;

void axr_log(quint32 channels, const AXRString &message)
{
    if (axr_debug_channels_active(AXR_DEBUG_CH_ON) && axr_debug_channels_active(channels))
        std_log(message);
}

void axr_log_inline(quint32 channels, const AXRString &message)
{
    if (axr_debug_channels_active(AXR_DEBUG_CH_ON) && axr_debug_channels_active(channels))
        std_log(message, false);
}

bool axr_debug_channels_active(quint32 channels)
{
    return axr_debug_active_channels & channels;
}

void axr_debug_activate_channel(quint32 channels)
{
    axr_debug_active_channels |= channels;
}
