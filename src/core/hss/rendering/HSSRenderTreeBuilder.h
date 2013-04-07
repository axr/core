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

#ifndef HSSRenderTreeBuilder_H
#define HSSRenderTreeBuilder_H

#include "HSSAbstractVisitor.h"

namespace AXR
{
    class HSSRenderTreeBuilder : public HSSAbstractVisitor
    {
        Q_DISABLE_COPY(HSSRenderTreeBuilder)
    public:
        HSSRenderTreeBuilder();
        HSSRenderTreeBuilder(const AXRString &outputFile);
        virtual ~HSSRenderTreeBuilder();

        void setOutputFile(AXRString outputFile);
        AXRString getOutputFile();

        QImage getResultAsImage(int width, int height);
        AXRString getResultAsString();

    protected:
        virtual void initializeVisit();
        virtual void visit(HSSContainer &container);
        virtual void visit(HSSTextBlock &textBlock);
        virtual void finalizeVisit();
        virtual void reset();

    private:
        void addTextForObject(HSSDisplayObject &displayObject);

        class Private;
        Private *d;
    };
}


#endif // #ifndef HSSRenderTreeBuilder_H
