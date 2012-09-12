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

#ifndef AXRDEBUGGING_H
#define AXRDEBUGGING_H

#include <iostream>
#include <sstream>
#include <string>
#include <QString>

// Debug levels:
// - 0 No debug
// - 1
// - 2
// - 3
// - 4 All messages

extern unsigned int axr_debug_level;

void std_log_level(const std::string &message, unsigned int debugLevel = 0, bool newline = true);
void std_log(const std::string &message, bool newline = true);
void std_log1(const std::string &message, bool newline = true);
void std_log2(const std::string &message, bool newline = true);
void std_log3(const std::string &message, bool newline = true);
void std_log4(const std::string &message, bool newline = true);

// Security brakes for while loops
#define AXR_DEBUG_BRAKE 9999
#define security_brake_init() int __axr_security_count = 0;
#define security_brake_reset() __axr_security_count = 0;
#define security_brake() \
    if (__axr_security_count > AXR_DEBUG_BRAKE) \
    { \
        std::stringstream msg; \
        msg << "WARNING: Loop iterated over " << AXR_DEBUG_BRAKE << " times. Broke out of possible infinite loop."; \
        std_log1(msg.str()); \
        break; \
    } \
    else \
        __axr_security_count++;

// Debug channels
/**
 *  These are the available channels:
 *
 *  Bit layout:
 *  0000 0000  0000 0000  0000 0000  0000 0001
 *  fedc abZY  XWVU TSRQ  PONM LKJI  HGFE DCBA
 *
 *  A:  AXR_DEBUG_CH_ON                 Debug on/off
 *  B:  AXR_DEBUG_CH_OVERVIEW           Basic high level actions
 *  C:  AXR_DEBUG_CH_GENERAL            General actions in the system
 *  D:  AXR_DEBUG_CH_GENERAL_SPECIFIC   General actions, more fine grained
 *  E:  AXR_DEBUG_CH_
 *  F:  AXR_DEBUG_CH_
 *  G:  AXR_DEBUG_CH_XML                XML parsing events
 *  H:  AXR_DEBUG_CH_
 *  I:  AXR_DEBUG_CH_
 *  J:  AXR_DEBUG_CH_
 *  K:  AXR_DEBUG_CH_HSS                HSS parsing events
 *  L:  AXR_DEBUG_CH_
 *  M:  AXR_DEBUG_CH_OBSERVING          Adding, removing and notifying of observers
 *  N:  AXR_DEBUG_CH_EVENTS             Prints messages about the event handling
 *  O:  AXR_DEBUG_CH_EVENTS_SPECIFIC    Event handling, more specific
 *  P:  AXR_DEBUG_CH_
 *  Q:  AXR_DEBUG_CH_
 *  R:  AXR_DEBUG_CH_
 *  S:  AXR_DEBUG_CH_TOKENIZING         Prints the chars that are read by the tokenizer
 *  T:  AXR_DEBUG_CH_
 *  U:  AXR_DEBUG_CH_
 *  V:  AXR_DEBUG_CH_
 *  W:  AXR_DEBUG_CH_
 *  X:  AXR_DEBUG_CH_
 *  Y:  AXR_DEBUG_CH_
 *  Z:  AXR_DEBUG_CH_
 *  a:  AXR_DEBUG_CH_
 *  b:  AXR_DEBUG_CH_
 *  c:  AXR_DEBUG_CH_FULL_FILENAMES     Normally only the filename is shown, activate this for full paths.
 *  d:  AXR_DEBUG_CH_
 *  e:  AXR_DEBUG_CH_
 *  f:  AXR_DEBUG_CH_
 *
 */

quint32 extern axr_debug_active_channels;

enum axr_debug_ch
{
    AXR_DEBUG_CH_ON = 1, //A
    AXR_DEBUG_CH_OVERVIEW = 1 << 1, //B
    AXR_DEBUG_CH_GENERAL = 1 << 2, //C
    AXR_DEBUG_CH_GENERAL_SPECIFIC = 1 << 3, //D
    //E
    //F
    AXR_DEBUG_CH_XML = 1 << 6, //G
    //H
    //I
    //J
    AXR_DEBUG_CH_HSS = 1 << 10, //K

    AXR_DEBUG_CH_OBSERVING = 1 << 12, //M
    AXR_DEBUG_CH_EVENTS = 1 << 13, //N
    AXR_DEBUG_CH_EVENTS_SPECIFIC = 1 << 14, //O

    //

    AXR_DEBUG_CH_TOKENIZING = 1 << 18, //S

    //

    AXR_DEBUG_CH_FULL_FILENAMES = 1 << 28, //c
};

void axr_log(quint32 channels, const std::string &message);
void axr_log_inline(quint32 channels, const std::string &message);
void axr_log(quint32 channels, const QString &message);
void axr_log_inline(quint32 channels, const QString &message);
void axr_log(quint32 channels, const char *message);
void axr_log_inline(quint32 channels, const char *message);
void axr_debug_activate_channel(quint32 channels);

#endif //AXRDEBUGGING_H
