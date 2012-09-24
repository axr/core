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

#include <sstream>
#include <QStringList>
#include "AXRDebugging.h"
#include "AXRError.h"
#include "AXRInitializer.h"

using namespace AXR;

AXRError::AXRError(const AXRString &origin, const AXRString &message, const AXRString &filename, int line, int column)
{
    this->origin = origin;
    this->message = message;
    this->filename = filename;
    this->in_file = !filename.isEmpty();
    this->line = line;
    this->column = column;
}

AXRError::~AXRError()
{
}

void AXRError::raise() const
{
    AXRCore::tp & core = AXRCore::getInstance();
    AXRWrapper * wrapper = core->getWrapper();
    wrapper->handleError(this->shared_from_this());
}

AXRString AXRError::toString() const
{
    return toProblemString("Error");
}

AXRString AXRError::toProblemString(const AXRString &label) const
{
    QStringList parts;
    parts << AXRString("%1: %2").arg(label).arg(message);

    if (in_file)
        parts << AXRString("in %1").arg(filename);

    if (line > 0 || column > 0)
    {
        parts << "on";

        if (line > 0)
            parts << AXRString("line %1").arg(line);

        if (column > 0)
            parts << AXRString("column %1").arg(column);
    }

    return parts.join(" ");
}

AXRString AXRError::getMessage() const
{
    return this->message;
}
