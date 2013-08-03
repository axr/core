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

#include "AXRBuffer.h"
#include "AXRError.h"
#include "AXRLoggerManager.h"
#include "HSSToken.h"
#include "HSSTokenizer.h"
#include "HSSValueToken.h"
#include <QSharedPointer>
#include <QTextStream>

using namespace AXR;

bool isLatin1Letter(AXRChar ch)
{
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

namespace AXR
{
    class HSSTokenizerPrivate
    {
    public:
        HSSTokenizerPrivate()
        : file(), textStream(), currentChar(), currentTokenText(),
          currentLine(), currentColumn(), peekPositionOffset(),
          peekLineOffset(), peekColumnOffset(), preferHex()
        {
        }

        // Shared pointer to the data source and stream to read from it
        QSharedPointer<AXRBuffer> file;
        QTextStream *textStream;

        // The Unicode character that is currently being processed
        AXRChar currentChar;

        // The text of a string token that is currently being processed
        AXRString currentTokenText;

        // The line and column number of the current character
        qint64 currentLine;
        qint64 currentColumn;

        // Offsets from the buffer position and line/column numbers used when
        // peeking ahead in the stream
        qint64 peekPositionOffset;
        qint64 peekLineOffset;
        qint64 peekColumnOffset;

        // If you are expecting a hexadecimal number, set this to true
        // don't forget to reset it afterwards
        bool preferHex;
    };
}

HSSTokenizer::HSSTokenizer()
: d(new HSSTokenizerPrivate)
{
    this->reset();
}

HSSTokenizer::~HSSTokenizer()
{
    if (d->textStream)
        delete d->textStream;

    delete d;
}

/*!
 * Resets all properties of the tokenizer to their defaults,
 * and sets the input buffer to \c NULL.
 */
void HSSTokenizer::reset()
{
    if (d->textStream)
    {
        delete d->textStream;
        d->textStream = NULL;
    }

    if (d->file)
        d->file.clear();

    d->currentChar = '\0';
    d->currentTokenText = AXRString();

    d->currentLine = 1;
    d->currentColumn = 1;

    d->peekPositionOffset = 0;
    d->peekLineOffset = 0;
    d->peekColumnOffset = 0;

    // By default, numbers are read as real numbers and A-F will be an identifier
    d->preferHex = false;
}

QSharedPointer<AXRBuffer> HSSTokenizer::getFile() const
{
    return d->file;
}

void HSSTokenizer::setFile(QSharedPointer<AXRBuffer> file)
{
    // We must clear state when setting a new file as any of it would
    // have no meaning from this point on
    this->reset();

    if (!file)
        return;

    d->file = file;
    d->textStream = new QTextStream(file->getBuffer());
}

bool HSSTokenizer::isHexPreferred() const
{
    return d->preferHex;
}

void HSSTokenizer::setHexPreferred(bool prefer)
{
    d->preferHex = prefer;
}

qint64 HSSTokenizer::currentLine() const
{
    return d->currentLine;
}

qint64 HSSTokenizer::currentColumn() const
{
    return d->currentColumn;
}

bool HSSTokenizer::atEndOfSource()
{
    return d->textStream ? d->textStream->atEnd() : true;
}

/*!
 * Reads the next character from the buffer and stores it.
 */
HSS_TOKENIZING_STATUS HSSTokenizer::readNextChar()
{
    if (this->atEndOfSource())
    {
        d->currentChar = '\0';
    }
    else
    {
        *d->textStream >> d->currentChar;
    }

    axr_log(LoggerChannelHSSTokenizer, AXRString("Read character %1 (line %2, col %3)").arg(d->currentChar).arg(d->currentLine).arg(d->currentColumn));

    d->currentColumn++;

    return HSSTokenizerOK;
}

/*!
 * Reads and returns a pointer to the next token in the buffer, or \c NULL if the buffer was empty.
 */
QSharedPointer<HSSToken> HSSTokenizer::readNextToken()
{
    QSharedPointer<HSSToken> ret;

    if (this->atEndOfSource() && d->currentChar == '\0')
    {
        return ret;
    }

    QChar cc = d->currentChar;

    // Identifiers can start with a letter or an underscore
    if (isLatin1Letter(cc) || cc == '_')
    {
        if (d->preferHex)
            return this->readHexOrIdentifier();
        else
            return this->readIdentifier();
    }

    if (cc.isSpace())
        return this->readWhitespace();

    // If it starts with a number it is either a number or a percentage
    if (cc.isDigit())
    {
        if (d->preferHex)
            return this->readHexOrIdentifier();
        else
            return this->readNumberOrPercentage();
    }

    switch (cc.toLatin1())
    {
        // If it starts with quotes, either single or double, it is a string
        case '"':
        case '\'':
            return this->readString();
        case '#':
            ret = QSharedPointer<HSSToken>(new HSSToken(HSSInstructionSign, d->currentLine, d->currentColumn - 1));
            this->readNextChar();
            return ret;
        case '@':
            ret = QSharedPointer<HSSToken>(new HSSToken(HSSObjectSign, d->currentLine, d->currentColumn - 1));
            this->readNextChar();
            return ret;
        case '&':
            ret = QSharedPointer<HSSToken>(new HSSToken(HSSAmpersand, d->currentLine, d->currentColumn - 1));
            this->readNextChar();
            return ret;
        case '{':
            ret = QSharedPointer<HSSToken>(new HSSToken(HSSBlockOpen, d->currentLine, d->currentColumn - 1));
            this->readNextChar();
            return ret;
        case '}':
            ret = QSharedPointer<HSSToken>(new HSSToken(HSSBlockClose, d->currentLine, d->currentColumn - 1));
            this->readNextChar();
            return ret;
        case ',':
            ret = QSharedPointer<HSSToken>(new HSSToken(HSSComma, d->currentLine, d->currentColumn - 1));
            this->readNextChar();
            return ret;
        case ':':
            ret = QSharedPointer<HSSToken>(new HSSToken(HSSColon, d->currentLine, d->currentColumn - 1));
            this->readNextChar();
            return ret;
        case ';':
            ret = QSharedPointer<HSSToken>(new HSSToken(HSSEndOfStatement, d->currentLine, d->currentColumn - 1));
            this->readNextChar();
            return ret;
        case '(':
            ret = QSharedPointer<HSSToken>(new HSSToken(HSSParenthesisOpen, d->currentLine, d->currentColumn - 1));
            this->readNextChar();
            return ret;
        case ')':
            ret = QSharedPointer<HSSToken>(new HSSToken(HSSParenthesisClose, d->currentLine, d->currentColumn - 1));
            this->readNextChar();
            return ret;
        case '/':
            return this->readCommentOrSymbol();
        case '!':
            ret = QSharedPointer<HSSToken>(new HSSToken(HSSNegator, d->currentLine, d->currentColumn - 1));
            this->readNextChar();
            return ret;
        default:
            return this->readSymbol();
    }
}

/*!
 * Reads and returns a pointer to the next token in the buffer, or \c NULL
 * if there is no input buffer.
 *
 * The actual position in the file is saved and can be restored
 * by calling \a resetPeek().
 */
QSharedPointer<HSSToken> HSSTokenizer::peekNextToken()
{
    QSharedPointer<HSSToken> ret;
    if (!d->textStream)
        return ret;

    // Store the current position in the buffer
    qint64 savedPosition = d->textStream->pos();
    qint64 savedLine = d->currentLine;
    qint64 savedColumn = d->currentColumn;

    // Read the next token
    ret = this->readNextToken();

    // Store the new offset
    d->peekPositionOffset += (d->textStream->pos() - savedPosition);
    d->peekLineOffset += (d->currentLine - savedLine);
    d->peekColumnOffset += (d->currentColumn - savedColumn);

    if (ret)
        axr_log(LoggerChannelHSSTokenizer, AXRString("Peeked (offset: %1, current position: %2, saved position: %3), read token %4").arg(d->peekPositionOffset).arg(d->textStream->pos()).arg(savedPosition).arg(ret->toString()));
    else
        axr_log(LoggerChannelHSSTokenizer, AXRString("Peeked (offset: %1, current position: %2, saved position: %3), no token read").arg(d->peekPositionOffset).arg(d->textStream->pos()).arg(savedPosition));

    return ret;
}

/*!
 * Restores state after a call to \a peekNextToken().
 *
 * This method only needs to be called once to restore state
 * regardless of how many times \a peekNextToken() is called.
 */
void HSSTokenizer::resetPeek()
{
    if (!d->textStream)
        return;

    // Restore the saved position in the buffer
    // We start one character before we were before, since we are re-reading the character
    d->textStream->seek(d->textStream->pos() - (d->peekPositionOffset + 1));
    d->currentLine -= d->peekLineOffset;
    d->currentColumn -= (d->peekColumnOffset + 1);

    this->readNextChar();

    // Reset the peek offsets
    d->peekPositionOffset = 0;
    d->peekLineOffset = 0;
    d->peekColumnOffset = 0;

    axr_log(LoggerChannelHSSTokenizer, AXRString("Peek reset"));
}

/*!
 * Skips over any whitespace characters.
 */
HSS_TOKENIZING_STATUS HSSTokenizer::skipWhitespace()
{
    while (d->currentChar.isSpace())
    {
        // We only want to consider something after \n to be a new line, as this
        // effectively matches \n and \r\n. No modern system considers \r alone
        // to be a new line, and checking for it here would cause most Windows
        // files to show incorrect line numbers, as a new line would be registered
        // for both the \r AND the \n.
        if (d->currentChar == '\n')
        {
            d->currentLine++;
            d->currentColumn = 1;
        }

        this->readNextChar();
    }

    return HSSTokenizerOK;
}

/*!
 * Stores the current character in the multi-character token buffer,
 * then reads the next character.
 */
HSS_TOKENIZING_STATUS HSSTokenizer::storeCurrentCharAndReadNext()
{
    d->currentTokenText += d->currentChar;
    this->readNextChar();

    return HSSTokenizerOK;
}

/*!
 * Clears the current text token and returns its value.
 */
AXRString HSSTokenizer::extractCurrentTokenText()
{
    AXRString text = d->currentTokenText;
    d->currentTokenText.clear();
    return text;
}

/*!
 * Reads and returns a whitespace token.
 */
QSharedPointer<HSSToken> HSSTokenizer::readWhitespace()
{
    const qint64 line = d->currentLine;
    const qint64 column = d->currentColumn - 1;

    this->skipWhitespace();
    return QSharedPointer<HSSToken>(new HSSToken(HSSWhitespace, line, column));
}

/*!
 * Reads and returns an identifier token.
 */
QSharedPointer<HSSToken> HSSTokenizer::readIdentifier()
{
    const qint64 line = d->currentLine;
    const qint64 column = d->currentColumn - 1;

    while (isLatin1Letter(d->currentChar) || d->currentChar.isDigit() || d->currentChar == '_')
    {
        this->storeCurrentCharAndReadNext();
    }

    return QSharedPointer<HSSValueToken>(new HSSValueToken(HSSIdentifier, this->extractCurrentTokenText(), line, column));
}

/*!
 * Reads and returns a hexadecimal number or identifier token.
 */
QSharedPointer<HSSToken> HSSTokenizer::readHexOrIdentifier()
{
    const qint64 line = d->currentLine;
    const qint64 column = d->currentColumn - 1;

    security_brake_init();
    bool done = false;
    d->currentTokenText.clear();
    while (!done)
    {
        switch (d->currentChar.toLatin1())
        {
        case 'a':
        case 'A':
        case 'b':
        case 'B':
        case 'c':
        case 'C':
        case 'd':
        case 'D':
        case 'e':
        case 'E':
        case 'f':
        case 'F':
            this->storeCurrentCharAndReadNext();
            continue;

        default:
            if (d->currentChar.isDigit())
            {
                this->storeCurrentCharAndReadNext();
                continue;
            }
            else
            {
                if (isLatin1Letter(d->currentChar))
                {
                    done = true;
                    break;
                }
                else if (d->currentTokenText.size() > 0)
                {
                    return QSharedPointer<HSSValueToken>(new HSSValueToken(HSSHexNumber, this->extractCurrentTokenText(), line, column));
                }
                else
                {
                    done = true;
                    break;
                }
            }
        }
        security_brake();
    }

    // If we reached this far, it is an identifier - finish reading it
    return this->readIdentifier();
}

/*!
 * Reads and returns either a number or a percentage token.
 *
 * NOTE: This function assumes that the current character is a digit
 * (between U+0030 and U+0039, inclusive).
 */
QSharedPointer<HSSToken> HSSTokenizer::readNumberOrPercentage()
{
    const qint64 line = d->currentLine;
    const qint64 column = d->currentColumn - 1;

    bool dotFound = false;
    while (d->currentChar.isDigit() || d->currentChar == '.')
    {
        if (d->currentChar == '.')
        {
            if (dotFound)
                break;
            else
                dotFound = true;
        }

        this->storeCurrentCharAndReadNext();
    }

    QSharedPointer<HSSToken> ret;
    if (d->currentChar == '%')
    {
        ret = QSharedPointer<HSSValueToken>(new HSSValueToken(HSSPercentageNumber, this->extractCurrentTokenText(), line, column));
        this->readNextChar();
    }
    else
    {
        ret = QSharedPointer<HSSValueToken>(new HSSValueToken(HSSNumber, this->extractCurrentTokenText(), line, column));
    }

    return ret;
}

/*!
 * Reads and returns either a single quoted or double quoted string token.
 *
 * NOTE: This function assumes that the current character is " (U+0022) or ' (U+0027).
 */
QSharedPointer<HSSToken> HSSTokenizer::readString()
{
    const qint64 line = d->currentLine;
    const qint64 column = d->currentColumn - 1;

    QSharedPointer<HSSToken> ret;
    if (d->currentChar == '"')
    {
        this->readNextChar();
        while (d->currentChar != '"')
        {
            if (this->atEndOfSource())
                throw AXRError("HSSTokenizer", "Unexpected end of source");

            this->storeCurrentCharAndReadNext();
        }

        ret = QSharedPointer<HSSValueToken>(new HSSValueToken(HSSDoubleQuoteString, this->extractCurrentTokenText(), line, column));
    }
    else if (d->currentChar == '\'')
    {
        this->readNextChar();
        while (d->currentChar != '\'')
        {
            if (this->atEndOfSource())
                throw AXRError("HSSTokenizer", "Unexpected end of source");

            this->storeCurrentCharAndReadNext();
        }

        ret = QSharedPointer<HSSValueToken>(new HSSValueToken(HSSSingleQuoteString, this->extractCurrentTokenText(), line, column));
    }

    this->readNextChar();
    return ret;
}

/*!
 * Reads and returns a comment or a symbol token.
 *
 * NOTE: This function assumes that the current character is / (U+002F).
 */
QSharedPointer<HSSToken> HSSTokenizer::readCommentOrSymbol()
{
    const qint64 line = d->currentLine;
    const qint64 column = d->currentColumn - 1;

    QSharedPointer<HSSValueToken> ret;
    this->readNextChar();
    if (d->currentChar == '/')
    {
        this->readNextChar(); // skip '/'

        // Read all chars until end of line
        while (d->currentChar != '\n' && d->currentChar != '\r' && d->currentChar != '\f' && !this->atEndOfSource())
        {
            this->storeCurrentCharAndReadNext();
        }

        ret = QSharedPointer<HSSValueToken>(new HSSValueToken(HSSLineComment, this->extractCurrentTokenText(), line, column));
    }
    else if (d->currentChar == '*')
    {
        readNextChar(); //skip '*'
        while (true)
        {
            if (d->currentChar == '*')
            {
                readNextChar(); // We won't know if it is the end of the comment until we seek further
                if (d->currentChar == '/')
                {
                    // It is the end, break the loop
                    break;
                }
                else
                {
                    // It's not the end of the comment, store the char and continue reading
                    d->currentTokenText += '*';
                }
            }
            else if (this->atEndOfSource())
            {
                break;
            }

            this->storeCurrentCharAndReadNext();
        }

        ret = QSharedPointer<HSSValueToken>(new HSSValueToken(HSSBlockComment, this->extractCurrentTokenText(), line, column));
        readNextChar();
    }
    else
    {
        ret = QSharedPointer<HSSValueToken>(new HSSValueToken(HSSSymbol, '/', line, column));
    }

    return ret;
}

/*!
 * Reads and returns a symbol token.
 */
QSharedPointer<HSSToken> HSSTokenizer::readSymbol()
{
    const qint64 line = d->currentLine;
    const qint64 column = d->currentColumn - 1;

    QSharedPointer<HSSToken> ret;

    switch (d->currentChar.toLatin1())
    {
    case '.':
        this->storeCurrentCharAndReadNext();
        if (d->currentChar == '.')
        {
            this->storeCurrentCharAndReadNext();
            if (d->currentChar == '.')
            {
                this->storeCurrentCharAndReadNext();
            }

            ret = QSharedPointer<HSSValueToken>(new HSSValueToken(HSSSymbol, this->extractCurrentTokenText(), line, column));
            return ret;
        }

        ret = QSharedPointer<HSSValueToken>(new HSSValueToken(HSSSymbol, this->extractCurrentTokenText(), line, column));
        return ret;

    default:
        ret = QSharedPointer<HSSValueToken>(new HSSValueToken(HSSSymbol, d->currentChar, line, column));
        this->readNextChar();
        return ret;
    }
}
