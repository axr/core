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

#ifndef AXRLOGGERENUMS_H
#define AXRLOGGERENUMS_H

#include "AXRGlobal.h"

namespace AXR
{
    /*!
     * Lists the available channels the logging system can write to.
     */
    enum AXR_API AXRLoggerChannel
    {
        /*!
         * No logger channel.
         */
        LoggerChannelNone = 0,

        /*!
         * Errors that are displayed to the user.
         */
        LoggerChannelUserError = 1 << 0,

        /*!
         * Warnings are are displayed to the user.
         */
        LoggerChannelUserWarning = 1 << 1,

        /*!
         * Basic high level actions.
         */
        LoggerChannelOverview = 1 << 2,

        /*!
         * General actions in the system.
         */
        LoggerChannelGeneral = 1 << 3,

        /*!
         * General actions in the system, more fine grained.
         */
        LoggerChannelGeneralSpecific = 1 << 4,

        /*!
         * Events related to disk I/O.
         */
        LoggerChannelIO = 1 << 5,

        /*!
         * Events related to network I/O.
         */
        LoggerChannelNetwork = 1 << 6,

        /*!
         * XML parsing events.
         */
        LoggerChannelXMLParser = 1 << 7,

        /*!
         * HSS parsing events.
         */
        LoggerChannelHSSParser = 1 << 8,

        /*!
         * Prints the characters read by the HSS tokenizer.
         */
        LoggerChannelHSSTokenizer = 1 << 9,

        /*!
         * HSS document layout calculation.
         */
        LoggerChannelLayout = 1 << 10,

        /*!
         * Rendering (painting) events.
         */
        LoggerChannelRendering = 1 << 11,

        /*!
         * Adding, removing and notifying of observers.
         */
        LoggerChannelObserving = 1 << 12,

        /*!
         * Messages related to event handling.
         */
        LoggerChannelEvents = 1 << 13,

        /*!
         * Messages related to event handling, more fine grained.
         */
        LoggerChannelEventsSpecific = 1 << 14,

        /*!
         * Messages related to the document controller.
         */
        LoggerChannelController = 1 << 15,

        /*!
         * Messages related to the document controller, more fine grained.
         */
        LoggerChannelControllerSpecific = 1 << 16,
        /*!
         * Values that were given to log() functions in hss or js.
         */
        LoggerChannelLogFunction = 1 << 17,

        /*!
         * \deprecated
         * This member is obsolete. Use a more suitable channel instead.
         */
        LoggerChannelObsolete0 = 1 << 28,

        /*!
         * \deprecated
         * This member is obsolete. Use a more suitable channel instead.
         */
        LoggerChannelObsolete1 = 1 << 29,

        /*!
         * \deprecated
         * This member is obsolete. Use a more suitable channel instead.
         */
        LoggerChannelObsolete3 = 1 << 30,

        /*!
         * Do not use. No member may exceed this value as it is the largest
         * flag guaranteed to fit into a 32-bit signed integer.
         */
        LoggerChannelMax = 1 << 30,

        /*!
         * All logger channels.
         */
        LoggerChannelAll = 0x7fffffff
    };

    Q_DECLARE_FLAGS(AXRLoggerChannels, AXRLoggerChannel)

    AXR_API QList<AXRLoggerChannel> loggerFlagsToList(AXRLoggerChannels channels);
    AXR_API AXRString loggerChannelToString(AXRLoggerChannel channel);
    AXR_API AXRLoggerChannel stringToLoggerChannel(const AXRString &channelName);
}

Q_DECLARE_OPERATORS_FOR_FLAGS(AXR::AXRLoggerChannels)

#endif // AXRLOGGERENUMS_H
