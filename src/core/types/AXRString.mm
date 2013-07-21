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

#include <QString>
#include "AXRString.h"
#import <Foundation/Foundation.h>

using namespace AXR;

AXRString AXR::fromCFStringRef(CFStringRef string)
{
    if (!string)
        return AXRString();

    CFIndex length = CFStringGetLength(string);
    const UniChar *chars = CFStringGetCharactersPtr(string);
    if (chars)
        return AXRString(reinterpret_cast<const AXRChar*>(chars), static_cast<int>(length));

    UniChar buffer[length];
    CFStringGetCharacters(string, CFRangeMake(0, length), buffer);
    return AXRString(reinterpret_cast<const AXRChar*>(buffer), static_cast<int>(length));
}

CFStringRef AXR::toCFStringRef(const AXRString &string)
{
    return CFStringCreateWithCharacters(0, reinterpret_cast<const UniChar*>(string.unicode()), string.length());
}

NSString* AXR::toNSString(const AXRString &string)
{
    // The const cast below is safe: CFStringRef and NSString are immutable
    return [const_cast<NSString*>(reinterpret_cast<const NSString*>(toCFStringRef(string))) autorelease];
}

AXRString AXR::fromNSString(const NSString *string)
{
    return fromCFStringRef(reinterpret_cast<CFStringRef>(string));
}
