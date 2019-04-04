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
 *      COPYRIGHT: ©2019 - All Rights Reserved
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
#include "HSSChar.h"

using namespace AXR;

namespace AXR
{
    class HSSCharPrivate
    {
        friend class HSSChar;
        HSSCharPrivate()
        {
            
        }
        uint32_t value;
    };
}

HSSChar::HSSChar()
: d(new HSSCharPrivate)
{
    
}

HSSChar::HSSChar(const HSSChar & other)
: d(new HSSCharPrivate)
{
    d->value = other.d->value;
}

HSSChar::HSSChar(uint32_t value)
: d(new HSSCharPrivate)
{
    d->value = value;
}

HSSChar::~HSSChar()
{
    delete d;
}

HSSChar & HSSChar::operator=(const HSSChar &other)
{
    d->value = other.data();
    return *this;
}

HSSChar & HSSChar::operator=(const char * other)
{
    d->value = static_cast<uint32_t>(*other);
    return *this;
}

uint32_t HSSChar::data() const
{
    return d->value;
}

bool HSSChar::isSpace() const
{
    return std::isspace(static_cast<unsigned char>(this->data()));
}

bool HSSChar::isDigit() const
{
    return std::isdigit(static_cast<unsigned char>(this->data()));
}
