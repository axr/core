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
 *      WEB: http://axr.vg
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
        friend class HSSRenderer;
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

        virtual AXRString defaultObjectType();
        virtual bool isKeyword(AXRString value, AXRString property);
        virtual AXRString toString();
        virtual void setDefaults();
        virtual void setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value);
        virtual void accept(HSSAbstractVisitor* visitor, bool);
        virtual void layout();

        //transform
        HSSTextTransformType getTransform();
        QSharedPointer<HSSParserNode> getDTransform();
        void setDTransform(QSharedPointer<HSSParserNode> value);
        void transformChanged(HSSObservableProperty source, void*data);

        //textAlign
        HSSTextAlignType getTextAlign();
        QSharedPointer<HSSParserNode> getDTextAlign();
        void setDTextAlign(QSharedPointer<HSSParserNode> value);
        void textAlignChanged(HSSObservableProperty source, void*data);

        //text
        AXRString getText();
        QSharedPointer<HSSParserNode> getDText();
        void setDText(QSharedPointer<HSSParserNode> value);
        void textChanged(HSSObservableProperty source, void*data);

        void trimContentText();

    protected:
        //transform
        HSSTextTransformType transform;
        QSharedPointer<HSSParserNode> dTransform;
        HSSObservable * observedTransform;
        HSSObservableProperty observedTransformProperty;

        //textAlign
        HSSTextAlignType textAlign;
        QSharedPointer<HSSParserNode> dTextAlign;
        HSSObservable * observedTextAlign;
        HSSObservableProperty observedTextAlignProperty;

        //text
        AXRString text;
        QSharedPointer<HSSParserNode> dText;
        HSSObservable * observedText;
        HSSObservableProperty observedTextProperty;

    private:
        QFont getFont() const;

        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

Q_DECLARE_METATYPE(AXR::HSSTextTransformType)
Q_DECLARE_METATYPE(AXR::HSSTextTransformType*)
Q_DECLARE_METATYPE(AXR::HSSTextAlignType)
Q_DECLARE_METATYPE(AXR::HSSTextAlignType*)

#endif
