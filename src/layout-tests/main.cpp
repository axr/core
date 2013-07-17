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

#include <QCoreApplication>
#include <QDir>
#include <QTextStream>
#include "TestSuite.h"

void printUsage()
{
    QTextStream stream(stderr);
    QFileInfo fi(QCoreApplication::applicationFilePath());
    stream << QString(QLatin1String("Usage: %1 json-file [-v|--verbose] [--format plain|xml] [--color]"))
        .arg(fi.fileName());
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    bool outputAsXML = false;
    bool verbose = false;
    bool color = false;

    if (args.size() < 2)
    {
        printUsage();
        return 1;
    }

    // Path of the JSON file describing the test suite to execute
    const QString testSuitePath = args.at(1);

    // Verbose output?
    int vPos = args.indexOf("-v");
    int verbosePos = args.indexOf("--verbose");
    if (vPos > 1 || verbosePos > 1)
        verbose = true;

    // Get the desired output format
    int formatPos = args.indexOf("--format");
    if (formatPos > 1 && args.size() >= formatPos + 1)
        outputAsXML = args.at(formatPos + 1).toLower() == "xml";

    int colorPos = args.indexOf("--color");
    if (colorPos > 1)
        color = 1;

    TestSuite *suite = new TestSuite(NULL, testSuitePath);
    suite->setVerbose(verbose);
    suite->execute();

    QString output = suite->getResults(outputAsXML ? "xml" : "plain");

    if (!outputAsXML && color)
    {
        output = output.replace("[FAILED]", "\033[0;31m[FAILED]\033[0;0m");
        output = output.replace("[PASSED]", "\033[0;32m[PASSED]\033[0;0m");
    }

    QTextStream ts(stdout);
    ts << output;

    delete suite;

    return 0;
}
