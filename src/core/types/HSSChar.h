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

#ifndef AXRCHAR_H
#define AXRCHAR_H


namespace AXR
{
    class HSSCharPrivate;
    
    class HSSChar
    {
    public:
        HSSChar();
        HSSChar(const HSSChar & other);
        HSSChar(uint32_t value);
        virtual ~HSSChar();
        
        HSSChar & operator=(const HSSChar &other);
        HSSChar & operator=(const char * other);
        
        uint32_t data() const;
        bool isSpace() const;
        bool isDigit() const;
        
    private:
        HSSCharPrivate * d;
    };
    
    inline bool operator<(const HSSChar & c1, const HSSChar & c2)
    { return c1.data() < c2.data(); }
    
    inline bool operator<=(const HSSChar & c1, const HSSChar & c2)
    { return c1.data() <= c2.data(); }
    
    inline bool operator>(const HSSChar & c1, const HSSChar & c2)
    { return c1.data() > c2.data(); }
    
    inline bool operator>=(const HSSChar & c1, const HSSChar & c2)
    { return c1.data() >= c2.data(); }
    
    inline bool operator==(const HSSChar & c1, const HSSChar & c2)
    { return c1.data() == c2.data(); }
    
    inline bool operator==(const HSSChar & c1, const char c2)
    { return c1.data() == c2; }
    
    inline bool operator==(const char c1, const HSSChar & c2)
    { return c1 == c2.data(); }
    
    inline bool operator!=(const HSSChar & c1, const HSSChar & c2)
    { return ! operator==(c1, c2); }
    
    inline bool operator!=(const char c1, const HSSChar & c2)
    { return ! operator==(c1, c2); }
}


#endif /* AXRCHAR_H */
