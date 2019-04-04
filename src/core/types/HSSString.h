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

#ifndef HSSSTRING_H
#define HSSSTRING_H

namespace AXR
{
    typedef double HSSUnit;
    class HSSStringPrivate;
    
    class HSSString
    {
    public:
        static HSSString number(HSSUnit num);
        static HSSString format(const HSSString format, ...);
        static HSSString join(std::vector<HSSString> strings, const char * glue);

        HSSString();
        HSSString(const HSSString & other);
        virtual ~HSSString();
        HSSString(const char * cStr);
        HSSString(std::string str);
        HSSString(HSSChar c);
        const std::string & data() const;
        const char * chardata() const;
        
        std::string::iterator begin();
        std::string::iterator end();
        
        HSSString & operator=(const HSSString &other);
        inline HSSString &operator+=(const HSSString &s) { return append(s); }
        inline HSSString &operator+=(const HSSChar &c) { return append(c.data()); }
        void clear();
        HSSString & append(const HSSString &other);
        HSSString & append(const HSSChar &c);
        void truncate(size_t pos);
        
        const bool operator==(const HSSString &other) const;
        friend inline bool operator!=(const HSSString &s1, const HSSString &s2) { return !(s1 == s2); }
        const double toDouble() const;
        unsigned int toHex() const;
        HSSString trimmed() const;
        HSSString stripQuotes() const;
        bool isEmpty() const;
        size_t length() const;
        HSSString substr(size_t start, size_t length) const;
        HSSString substr(size_t start) const;
        HSSString at(size_t pos) const;
        bool contains(const char * needle) const;

    private:
        HSSStringPrivate * d;
        static unsigned int _count_decimals(HSSUnit num);
    };
    
    inline const HSSString operator+(const HSSString & s1, const HSSString & s2)
    { HSSString t(s1); t += s2; return t; }
    
    inline const HSSString operator+(const HSSString & s1, const char * s2)
    { HSSString t(s1); t += s2; return t; }
    
    inline const bool operator<(const HSSString & s1, const HSSString & s2)
    { return s1.data() < s2.data(); }
}


#endif /* HSSSTRING_H */
