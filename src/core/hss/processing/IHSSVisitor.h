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

#ifndef IHSSVISITOR_H
#define IHSSVISITOR_H
#include "AXRGlobal.h"
#include <vector>

namespace AXR
{

    class HSSContainer;
    class HSSDisplayObject;
    class HSSTextBlock;

    class AXR_API IHSSVisitor
    {
    public:
        typedef enum VisitorFilterFlags {
            FLAG_All             = 0x01,
            FLAG_Skip            = 0x02,
            FLAG_Rendering       = 0x04,
            FLAG_Diagnostic      = 0x08,
            FLAG_Custom_1        = 0x10,
            FLAG_Custom_2        = 0x20,
            FLAG_Custom_3        = 0x40,
            FLAG_Custom_4        = 0x80
        } VisitorFilterFlags;

        IHSSVisitor() : _filterFlags(FLAG_All){};

        int getFilterFlags(){ return _filterFlags; }
        void setFilterFlags(VisitorFilterFlags filterFlags)
        {
            _filterFlags = FLAG_All | filterFlags;
        }


        virtual void initializeVisit() = 0;
        virtual void visit(HSSContainer &container) = 0;
        virtual void visit(HSSTextBlock &textBlock) = 0;
        virtual void finalizeVisit() = 0;
        virtual void reset() = 0;
        virtual ~IHSSVisitor(){}

        typedef std::vector<IHSSVisitor*>::iterator iterator;

    private:
        int _filterFlags;

    };
}

#endif // defined(IHSSVISITOR_H)