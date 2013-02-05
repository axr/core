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

#ifndef HSSTOKENIZER_H
#define HSSTOKENIZER_H

#include <vector>
#include <QSharedPointer>

namespace AXR
{
    class AXRBuffer;
    class HSSToken;

    //the return states any part of the tokenizer will report
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
        //destructor
        ~HSSTokenizer();

        //this will reset all the properties of the tokenizer to default
        void reset();

        void setFile(QSharedPointer<AXRBuffer> file);
        QSharedPointer<AXRBuffer> getFile();

        //reads a the next character and stores it, also keeps track of the position in the buffer
        //it will also set the corresponding state if the end of the buffer is reached, but expects
        //more source
        HSS_TOKENIZING_STATUS readNextChar();

        //reads and returns a pointer to the next token in the buffer or NULL if the buffer was empty
        QSharedPointer<HSSToken> readNextToken();


        //reads and returns a pointer to the next token in the buffer
        //BUT doesn't advance the current position of the normal reading flow. Call resetPeek() after
        //you finish peeking
        //the caller acquires ownership of the pointer
        QSharedPointer<HSSToken> peekNextToken();
        void resetPeek();

        //if you are expecting a hexadecimal number, set this to true
        //don't forget to reset it afterwards
        bool preferHex;

        //the current line and column position we're on
        qint64 currentLine;
        qint64 currentColumn;

        void setBufferLength(int length);

    protected:
        QSharedPointer<AXRBuffer> file;

        //the current character that has been read
        char currentChar;
        //here's where the tokens are stored
        std::vector<QSharedPointer<HSSToken> > tokenList;
        //here's where the current token's text is stored
        AXRString currentTokenText;
        //how long is the buffer?
        int buflen;
        //the position into the buffer
        int bufpos;
        //the position into the buffer when peeking - it is an offset from bufpos
        int peekpos;
        unsigned peekLine;
        unsigned peekColumn;

        //skips over any whitespace
        HSS_TOKENIZING_STATUS skipWhitespace();
        //checks if we are at the end of the source
        bool atEndOfSource();
        //to create tokens that have more than one char, this will store it and continue reading
        HSS_TOKENIZING_STATUS storeCurrentCharAndReadNext();
        //in some cases, storing specific chars in the currentTokenText is needed
        HSS_TOKENIZING_STATUS storeChar(char value);
        //returns the stored token text and clears the variable
        AXRString extractCurrentTokenText();
        //reads and returns a whitespace token
        QSharedPointer<HSSToken> readWhitespace();
        //reads and returns an identifier token
        QSharedPointer<HSSToken> readIdentifier();
        //reads and returns a hexadecimal number
        //QSharedPointer<HSSToken> readHex();
        //reads and returns a hexadecimal number or an identifier token
        QSharedPointer<HSSToken> readHexOrIdentifier();
        //reads and returns either a number or a percentage token
        QSharedPointer<HSSToken> readNumberOrPercentage();
        //reads and returns either a single quoted or double quoted string token
        QSharedPointer<HSSToken> readString();
        //reads and returns an object type token
        QSharedPointer<HSSToken> readObjectType();
        //reads and returns a comment or a symbol token
        QSharedPointer<HSSToken> readCommentOrSymbol();
        //reads and returns a symbol token
        QSharedPointer<HSSToken> readSymbol();
    };
}

#endif
