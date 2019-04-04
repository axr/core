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

#include "axr.h"
#include "AXRError.h"

using namespace AXR;

class AXRError::Data : public QSharedData
{
public:
    Data(const AXRString &origin, const AXRString &message, const AXRString &url, HSSUnit line, HSSUnit column)
    : origin(origin), message(message), url(url), line(line), column(column) { }

    Data(const Data &other)
    : QSharedData(other), origin(other.origin), message(other.message),
    url(other.url), line(other.line), column(other.column) { }

    ~Data() { }

    /**
     * The origin of the error message, typically the class from which it was triggered.
     */
    AXRString origin;

    /**
     * A human-readable error message describing the problem.
     */
    AXRString message;

    /**
     * The URL of the XML or HSS file in which the problem occurred.
     */
    AXRString url;

    /**
     * The line in the XML or HSS file on which the problem occurred.
     */
    qint64 line;

    /**
     * The column in the XML or HSS file on which the problem occurred.
     */
    qint64 column;
};

AXRError::AXRError(const AXRString &origin, const AXRString &message, const AXRString &url, HSSUnit line, HSSUnit column)
: d(new Data(origin, message, url, line, column))
{
}

AXRError::AXRError(const AXRError &other)
: d(other.d)
{
}

AXRError::~AXRError()
{
}

void AXRError::raise() const
{
    axr_log(LoggerChannelUserError, toString());
}

AXRString AXRError::toString() const
{
    return toProblemString("Error");
}

AXRString AXRError::toProblemString(const AXRString &label) const
{
    std::vector<AXRString> parts;
    parts.push_back(HSSString::format("%s: %s", label.chardata(), d->message.chardata()));

    // If we have a URL the source is a file, otherwise it's from a memory buffer
    if (!d->url.isEmpty())
        parts.push_back(HSSString::format("in %s", d->url.chardata()));

    if (d->line > 0 || d->column > 0)
    {
        parts.push_back("on");

        if (d->line > 0)
            parts.push_back(HSSString::format("line %d", d->line));

        if (d->column > 0)
            parts.push_back(HSSString::format("column %d", d->column));
    }

    AXRString ret;
    for (size_t i = 0, j = parts.size(); i<j; ++i)
    {
        ret += parts[i];
        if (i < j-1)
        {
            ret += " ";
        }
    }
    return ret;
}

qint64 AXRError::getLine() const
{
    return d->line;
}
qint64 AXRError::getColumn() const
{
    return d->column;
}

AXRString AXRError::getMessage() const
{
    return d->message;
}

AXRError& AXRError::operator=(const AXRError &other)
{
    d = other.d;
    return *this;
}
