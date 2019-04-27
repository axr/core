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
#include "HSSString.h"
//#include <stdarg.h>
#include <sstream>

using namespace AXR;

namespace AXR
{
    class HSSStringPrivate
    {
        friend class HSSString;

        HSSStringPrivate()
        {
            
        }
        std::string string;
    };
}

HSSString HSSString::number(HSSUnit num)
{
    
    std::ostringstream out;
    out.precision(HSSString::_count_decimals(num));
    out << std::fixed << num;
    
    return HSSString(out.str());
}

unsigned int HSSString::_count_decimals(HSSUnit num)
{
    unsigned int count = 0;
    num = std::abs(num);
    num = num - int(num);
    while (std::abs(num) >= 0.0000001)
    {
        num = num * 10;
        if (std::isinf(num))
            return 0;
        count += 1;
        num = num - int(num);
    }
    return count;
}

HSSString HSSString::format(const HSSString input, ...)
{
    std::string fmt = input.data();
    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::string str;
    va_list ap;
    while (1) {     // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap, input);
        int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return HSSString(str);
        }
        if (n > -1)  // Needed size returned
            size = n + 1;   // For null char
        else
            size *= 2;      // Guess at a larger size (OS specific)
    }
    return HSSString(str);
}

HSSString HSSString::join(std::vector<HSSString> strings, const char * glue)
{
    std::string ret;
    for (size_t i = 0, j = strings.size(); i<j; ++i)
    {
        const HSSString & string = strings[i];
        ret += string.data();
        if (i < j-1)
        {
            ret += glue;
        }
    }
    return HSSString(ret);
}

HSSString::HSSString()
: d(new HSSStringPrivate)
{
    
}

HSSString::HSSString(const HSSString & other)
: d(new HSSStringPrivate)
{
    d->string = other.d->string;
}

HSSString::~HSSString()
{
    delete d;
}

HSSString::HSSString(const char * cStr)
: d(new HSSStringPrivate)
{
    d->string = cStr;
}

HSSString::HSSString(std::string str)
: d(new HSSStringPrivate)
{
    d->string = str;
}

HSSString::HSSString(HSSChar c)
: d(new HSSStringPrivate)
{
    d->string = c.data();
}

const std::string & HSSString::data() const
{
    return d->string;
}

const char * HSSString::chardata() const
{
    return (char *)d->string.data();
}

std::string::iterator HSSString::begin()
{
    return d->string.begin();
}

std::string::iterator HSSString::end()
{
    return d->string.end();
}

HSSString & HSSString::operator=(const HSSString &other)
{
    d->string = other.data();
    return *this;
}

void HSSString::clear()
{
    d->string.clear();
}

HSSString & HSSString::append(const HSSString &other)
{
    d->string = d->string + other.data();
    return *this;
}

HSSString & HSSString::append(const HSSChar &c)
{
    const uint32_t intval = c.data();
    d->string += static_cast<const char>(intval);
    return *this;
}

void HSSString::truncate(size_t pos)
{
    d->string.erase(d->string.begin()+pos, d->string.end());
}

const bool HSSString::operator==(const HSSString &other) const
{
    return d->string == other.data();
}

const double HSSString::toDouble() const
{
    return std::stod(d->string);
}

unsigned int HSSString::toHex() const
{
    return std::stoi(d->string, nullptr, 16);
}

HSSString HSSString::trimmed() const
{
    HSSString ret;
    std::string val = d->string;
    if (val.length() > 0)
    {
        std::string wspc (" \t\f\v\n\r");// These are the whitespaces
        //finding the last valid character
        std::string::size_type posafter = val.find_last_not_of(wspc);
        //finding the first valid character
        std::string::size_type posbefore=val.find_first_not_of(wspc);
        
        if((-1 < (int)posafter) && (-1 < (int)posbefore)) //Just Wsp
        {
            ret = val.substr(posbefore,((posafter+1)-posbefore));
        }
    }
    return ret;
}

HSSString HSSString::stripQuotes() const
{
    std::string string = d->string;
    size_t position = 0;
    size_t n = 0;
    size_t len = string.length();
    if (string.substr(0, 1) == "\"")
    {
        position = 1;
        if (string.substr(len-1, 1) == "\"")
        {
            n = len - 1 - position;
        }
        else
        {
            n = len - position;
        }
    }
    else if (string.substr(0, 1) == "'")
    {
        position = 1;
        if (string.substr(len-1, 1) == "'")
        {
            n = len - 1 - position;
        }
        else
        {
            n = len - position;
        }
    }
    else
    {
        return HSSString(string);
    }
    return HSSString(string.substr(position, n));
}

bool HSSString::isEmpty() const
{
    return d->string.empty();
}

size_t HSSString::length() const
{
    return d->string.length();
}

HSSString HSSString::substr(size_t start, size_t length) const
{
    return HSSString(d->string.substr(start, length));
}

HSSString HSSString::substr(size_t start) const
{
    return HSSString(d->string.substr(start));
}

HSSString HSSString::at(size_t pos) const
{
    return this->substr(pos, 1);
}

bool HSSString::contains(const char * needle) const
{
    size_t pos = d->string.find(needle);
    return pos != std::string::npos;
}
