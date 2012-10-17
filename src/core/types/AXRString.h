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

#ifndef AXRSTRING_H
#define AXRSTRING_H

#include <QMetaType>
#include <QString>
#include <string>

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif
#endif

namespace AXR
{
    typedef QChar AXRChar;
    typedef QString AXRString;

    inline static std::string toStdString(const AXRString &string)
    {
        return string.toStdString();
    }

    inline static AXRString fromStdString(const std::string &string)
    {
        return AXRString::fromStdString(string);
    }

#ifdef QT_VERSION
    inline static QString toQString(const AXRString &string)
    {
        return string;
    }

    inline static AXRString fromQString(const QString &string)
    {
        return string;
    }
#endif

#ifdef __APPLE__
    inline static AXRString fromCFStringRef(CFStringRef string)
    {
        if (!string)
            return AXRString();

        CFIndex length = CFStringGetLength(string);
        const UniChar *chars = CFStringGetCharactersPtr(string);
        if (chars)
            return AXRString(reinterpret_cast<const AXRChar*>(chars), length);

        UniChar buffer[length];
        CFStringGetCharacters(string, CFRangeMake(0, length), buffer);
        return AXRString(reinterpret_cast<const AXRChar*>(buffer), length);
    }

    inline static CFStringRef toCFStringRef(const AXRString &string)
    {
        return CFStringCreateWithCharacters(0, reinterpret_cast<const UniChar*>(string.unicode()), string.length());
    }

#ifdef __OBJC__
    inline static NSString* toNSString(const AXRString &string)
    {
        // The const cast below is safe: CFStringRef and NSString are immutable
        return [const_cast<NSString*>(reinterpret_cast<const NSString*>(toCFStringRef(string))) autorelease];
    }

    inline static AXRString fromNSString(const NSString *string)
    {
        return fromCFStringRef(reinterpret_cast<CFStringRef>(string));
    }
#endif
#endif
}

Q_DECLARE_METATYPE(AXR::AXRString);
Q_DECLARE_METATYPE(AXR::AXRString*);

#endif
