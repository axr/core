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

#include <QStringList>
#include <QUrl>
#include "AXRDebugging.h"
#include "AXRError.h"

using namespace AXR;

class AXRError::Data : public QSharedData
{
public:
    Data(const AXRString &origin, const AXRString &message, const QUrl &url, int line, int column)
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
    QUrl url;

    /**
     * The line in the XML or HSS file on which the problem occurred.
     */
    int line;

    /**
     * The column in the XML or HSS file on which the problem occurred.
     */
    int column;
};

AXRError::AXRError(const AXRString &origin, const AXRString &message, const QUrl &url, int line, int column)
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
    std_log(toString());
}

AXRString AXRError::toString() const
{
    return toProblemString("Error");
}

AXRString AXRError::toProblemString(const AXRString &label) const
{
    QStringList parts;
    parts << AXRString("%1: %2").arg(label).arg(d->message);

    // If we have a URL the source is a file, otherwise it's from a memory buffer
    if (!d->url.isEmpty())
        parts << AXRString("in %1").arg(d->url.toString());

    if (d->line > 0 || d->column > 0)
    {
        parts << "on";

        if (d->line > 0)
            parts << AXRString("line %1").arg(d->line);

        if (d->column > 0)
            parts << AXRString("column %1").arg(d->column);
    }

    return parts.join(" ");
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
