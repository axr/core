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

#ifndef HSSTEXTBLOCK_H
#define HSSTEXTBLOCK_H

#include "HSSDisplayObject.h"
#include "HSSTextEnums.h"

namespace AXR
{
    class AXR_API HSSTextBlock : public HSSDisplayObject
    {
    public:
        friend class HSSCascader;
        friend class HSSRenderer;
        friend class HSSLayout;
        static HSSTextTransformType textTransformTypeFromString(AXRString value);
        static HSSTextAlignType textAlignTypeFromString(AXRString value);

        typedef std::vector<QSharedPointer<HSSTextBlock> >::iterator it;

        /**
         *  Constructor for HSSTextBlock objects
         */
        HSSTextBlock(AXRController * controller);
        /**
         *  Copy constructor for HSSTextBlock objects
         */
        HSSTextBlock(const HSSTextBlock & orig);
        /**
         *  Clones an instance of HSSTextBlock and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSTextBlock
         */
        QSharedPointer<HSSTextBlock> clone() const;
        virtual ~HSSTextBlock();

        virtual void setDefaults();
        virtual AXRString defaultObjectType();
        virtual AXRString toString();

        virtual void accept(HSSAbstractVisitor* visitor, HSSVisitorFilterFlags filterFlags);

        //transform
        HSSTextTransformType getTransform() const;

        //textAlign
        HSSTextAlignType getTextAlign() const;

        //text
        AXRString getText() const;
        void setText(AXRString value, HSSUnit specificity);

        //font
        void notifyFont(QSharedPointer<HSSObject> theObj);

    private:
        void _initialize();

        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        HSSTextAlignType _getTextAlign(QSharedPointer<HSSObject> theObj) const;
    };
}

#endif
