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

#include <QCoreApplication>
#include <QStringList>
#include <QTextStream>
#include "AXRDocument.h"

using namespace AXR;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();
    args.removeFirst();

    if (args.size() != 4)
    {
        QTextStream out(stderr);
        out << QString("Usage: %1 canvas-width canvas-height document-filename output-filename").arg(a.arguments().first()) << endl;
        out << endl;
        out << "Renders an XML or HSS document to an image file" << endl;
        out.flush();
        return 1;
    }

    int canvasWidth = args.at(0).toInt();
    int canvasHeight = args.at(1).toInt();
    QString documentFilename = args.at(2);
    QString outputFilename = args.at(3);

    AXRDocument document;
    if (document.loadFileByPath(QUrl::fromLocalFile(documentFilename)))
    {
        // Create an image object to render onto
        QImage image(canvasWidth, canvasHeight, QImage::Format_ARGB32);
        image.fill(Qt::transparent);
        QPainter painter(&image);

        // Render the document
        if (document.getController()->getRoot())
        {
            document.drawInRectWithBounds(image.rect(), image.rect());
            painter.drawImage(QPoint(), document.getRender()->surface());
        }

        // Save the image to a file
        QFileInfo fi(outputFilename);
        if (image.save(outputFilename, fi.suffix().toUpper().toUtf8().constData()))
        {
            return 0;
        }
        else
        {
            QTextStream out(stderr);
            out << QString("Could not write the image file to %1").arg(outputFilename) << endl;
            out.flush();
        }
    }
    else
    {
        QTextStream out(stderr);
        out << QString("Could not load the document at URL %1").arg(documentFilename) << endl;
        out.flush();
    }

    return 1;
}
