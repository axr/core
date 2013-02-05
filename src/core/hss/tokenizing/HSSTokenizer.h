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

#ifndef HSSTOKENIZER_H
#define HSSTOKENIZER_H

#include "AXRGlobal.h"

template <class T> class QSharedPointer;

namespace AXR
{
    class AXRBuffer;
    class HSSToken;
    class HSSTokenizerPrivate;

    /*!
     * List of possible states that the tokenizer may be in.
     */
    enum AXR_API HSS_TOKENIZING_STATUS
    {
        HSSTokenizerUnknownError = -1,
        HSSTokenizerDone = 0,
        HSSTokenizerOK = 1
    };

    class AXR_API HSSTokenizer
    {
    public:
        HSSTokenizer();
        ~HSSTokenizer();

        void reset();

        QSharedPointer<AXRBuffer> getFile() const;
        void setFile(QSharedPointer<AXRBuffer> file);

        bool isHexPreferred() const;
        void setHexPreferred(bool prefer);

        qint64 currentLine() const;
        qint64 currentColumn() const;

        bool atEndOfSource();

        HSS_TOKENIZING_STATUS readNextChar();

        QSharedPointer<HSSToken> readNextToken();
        QSharedPointer<HSSToken> peekNextToken();
        void resetPeek();

    private:
        HSS_TOKENIZING_STATUS skipWhitespace();
        HSS_TOKENIZING_STATUS storeCurrentCharAndReadNext();

        AXRString extractCurrentTokenText();

        QSharedPointer<HSSToken> readWhitespace();
        QSharedPointer<HSSToken> readIdentifier();
        QSharedPointer<HSSToken> readHexOrIdentifier();
        QSharedPointer<HSSToken> readNumberOrPercentage();
        QSharedPointer<HSSToken> readString();
        QSharedPointer<HSSToken> readCommentOrSymbol();
        QSharedPointer<HSSToken> readSymbol();

        HSSTokenizerPrivate *d;
    };
}

#endif
