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

#ifndef HSSTEXTBLOCK_H
#define HSSTEXTBLOCK_H

#include "HSSDisplayObject.h"

namespace AXR
{
    enum AXR_API HSSTextTransformType
    {
        HSSTextTransformTypeNone = 0,
        HSSTextTransformTypeLowercase,
        HSSTextTransformTypeUppercase,
        HSSTextTransformTypeCapitalize,
        HSSTextTransformTypeHumanize
    };

    enum AXR_API HSSTextAlignType
    {
        HSSTextAlignTypeNone = 0,
        HSSTextAlignTypeLeft,
        HSSTextAlignTypeRight,
        HSSTextAlignTypeCenter,
        HSSTextAlignTypeJustify
    };

    class AXR_API HSSTextBlock : public HSSDisplayObject
    {
    public:

        static HSSTextTransformType textTransformTypeFromString(AXRString value);
        static HSSTextAlignType textAlignTypeFromString(AXRString value);

        typedef QSharedPointer<HSSTextBlock> p;
        typedef std::vector<HSSTextBlock::p>::iterator it;

        /**
         *  Constructor for HSSTextBlock objects
         */
        HSSTextBlock();
        /**
         *  Copy constructor for HSSTextBlock objects
         */
        HSSTextBlock(const HSSTextBlock & orig);
        /**
         *  Clones an instance of HSSTextBlock and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSTextBlock
         */
        p clone() const;
        virtual ~HSSTextBlock();

        virtual AXRString defaultObjectType();
        virtual bool isKeyword(AXRString value, AXRString property);
        virtual AXRString toString();
        virtual void setDefaults();
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        virtual void drawForeground();
        virtual void layout();

        //transform
        HSSTextTransformType getTransform();
        HSSParserNode::p getDTransform();
        void setDTransform(HSSParserNode::p value);
        void transformChanged(HSSObservableProperty source, void*data);

        //textAlign
        HSSTextAlignType getTextAlign();
        HSSParserNode::p getDTextAlign();
        void setDTextAlign(HSSParserNode::p value);
        void textAlignChanged(HSSObservableProperty source, void*data);

        //text
        AXRString getText();
        HSSParserNode::p getDText();
        void setDText(HSSParserNode::p value);
        void textChanged(HSSObservableProperty source, void*data);

        void trimContentText();

    protected:
        //transform
        HSSTextTransformType transform;
        HSSParserNode::p dTransform;
        HSSObservable * observedTransform;
        HSSObservableProperty observedTransformProperty;

        //textAlign
        HSSTextAlignType textAlign;
        HSSParserNode::p dTextAlign;
        HSSObservable * observedTextAlign;
        HSSObservableProperty observedTextAlignProperty;

        //text
        AXRString text;
        HSSParserNode::p dText;
        HSSObservable * observedText;
        HSSObservableProperty observedTextProperty;

    private:
        QFont getFont() const;

        HSSClonable::p cloneImpl() const;
    };
}

Q_DECLARE_METATYPE(AXR::HSSTextTransformType);
Q_DECLARE_METATYPE(AXR::HSSTextTransformType*);
Q_DECLARE_METATYPE(AXR::HSSTextAlignType);
Q_DECLARE_METATYPE(AXR::HSSTextAlignType*);

#endif
