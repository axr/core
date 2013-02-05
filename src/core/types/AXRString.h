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

#ifndef AXRSTRING_H
#define AXRSTRING_H

#include <QMetaType>
#include <string>
#include "AXRGlobal.h"

class QChar;
class QString;
typedef const struct __CFString * CFStringRef;

#ifdef __OBJC__
@class NSString;
#else
typedef struct objc_object NSString;
#endif

namespace AXR
{
    typedef QChar AXRChar;
    typedef QString AXRString;

    extern AXR_API std::string toStdString(const AXRString &string);
    extern AXR_API AXRString fromStdString(const std::string &string);

    extern AXR_API QString toQString(const AXRString &string);
    extern AXR_API AXRString fromQString(const QString &string);

    extern AXR_API AXRString fromCFStringRef(CFStringRef string);
    extern AXR_API CFStringRef toCFStringRef(const AXRString &string);

    extern AXR_API NSString* toNSString(const AXRString &string);
    extern AXR_API AXRString fromNSString(const NSString *string);
}

Q_DECLARE_METATYPE(AXR::AXRString)
Q_DECLARE_METATYPE(AXR::AXRString*)

#endif
