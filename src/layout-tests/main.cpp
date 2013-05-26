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
#include <QScriptEngine>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QUrl>
#include <iostream>

#include "AXRController.h"
#include "HSSContainer.h"
#include "AXRDocument.h"
#include "json.h"

bool executeLayoutTests(QString filePath, bool verbose);
bool testFile(QString href, QString expect, bool verbose);
bool recursiveCheckExpectations(QMap<QString, QVariant> resulted, QMap<QString, QVariant> expected);

using namespace AXR;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();
    bool outputAsXML = false;
    bool verbose = false;
    int formatPos = args.indexOf("-format");
    if (formatPos > 0 && args.length() >= formatPos+1)
    {
        outputAsXML = args.at(formatPos+1).toLower() == "xml";
    }

    int vPos = args.indexOf("-v");
    if (vPos > 0)
    {
        verbose = true;
    }

    bool allPassed = executeLayoutTests(args.at(1), verbose);

    if (!allPassed)
    {
        std::cout << "\nWARNING: some tests failed" << std::endl;
    }
    else
    {
        std::cout << "\nAll tests passed" << std::endl;
    }

    return allPassed ? 0 : -1;
}

bool executeLayoutTests(QString filePath, bool verbose)
{
    std::cout << "Executing layout tests:\n\n";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QString tests = QString(file.readAll());
    QMap<QString, QVariant> decoded = Json::decode(tests);
    QMapIterator<QString, QVariant> i(decoded);
    bool allPassed = true;
    while (i.hasNext()) {
        i.next();
        QVariant test = i.value();
        if (test.canConvert<QMap<QString, QVariant> >()) {
            QMap<QString, QVariant> testMap = test.value<QMap<QString, QVariant> >();
            QString testName = testMap.value("name").toString();
            std::cout << "Executing test: " << testName.toStdString() << std::endl;

            QString href = testMap.value("href").toString();
            href = QFileInfo(QFileInfo(filePath).canonicalPath(), href).absoluteFilePath();
            QString expect = testMap.value("expect").toString();
            expect = QFileInfo(QFileInfo(filePath).canonicalPath(), expect).absoluteFilePath();
            if (verbose) {
                std::cout << "\nChecking file: \n    " << href.toStdString() << "\n against:\n    " << expect.toStdString() << std::endl << std::endl;
            }
            bool testPassed = testFile(href, expect, verbose);
            if(!testPassed)
            {
                std::cout << "Failed\n";
                allPassed = false;
            } else {
                std::cout << "Passed\n";
            }
            if (verbose) std::cout << std::endl << "----------------------------\n\n";
        }
    }
    return allPassed;
}

bool testFile(QString href, QString expect, bool verbose)
{
    AXRDocument document;
    document.setWindowSize(400., 400.);
    if (!document.loadFileByPath(QUrl(QString("file://").append(href))))
    {
        std::cout << "Failed to load URL" << href.toStdString() << std::endl;
        return false;
    }
    QSharedPointer<HSSContainer> root = document.controller()->root();
    AXRString jsonRep = root->toJSON("");
    if (verbose)
    {
        std::cout << "The resulted JSON representation of the AXR document:" << std::endl;
        std::cout << jsonRep.toStdString() << std::endl << std::endl;
    }

    QMap<QString, QVariant> resultedJson = Json::decode(jsonRep);

    QFile file(expect);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QString expected = QString(file.readAll());
    if (verbose)
    {
        std::cout << "The expectations:" << std::endl;
        std::cout << expected.toStdString() << std::endl;
    }

    QMap<QString, QVariant> expectedJson = Json::decode(expected);
    return recursiveCheckExpectations(resultedJson, expectedJson);
}

bool recursiveCheckExpectations(QMap<QString, QVariant> resulted, QMap<QString, QVariant> expected)
{
    QList<QVariant> expectations = expected.value("expect").value<QList<QVariant> >();
    Q_FOREACH(QVariant expectation, expectations)
    {
        QMap<QString, QVariant> expectationMap = expectation.value<QMap<QString, QVariant> >();
        QString eProperty = expectationMap.value("property").value<QString>();
        QVariant eValue = expectationMap.value("value");
        QString eType = expectationMap.value("type").value<QString>();
        if(eType == "" || eType == "matches"){
            if (resulted.value("properties").canConvert<QMap<QString, QVariant> >())
            {
                QMap<QString, QVariant> rProperties = resulted.value("properties").value<QMap<QString, QVariant> >();
                bool check = rProperties.value(eProperty) == eValue;
                if(!check)
                    return false;
            }
        }
    }
    QList<QVariant> rChildren = resulted.value("children").value<QList<QVariant> >();
    QList<QVariant> eChildren = expected.value("children").value<QList<QVariant> >();
    Q_FOREACH(QVariant eChild, eChildren)
    {
        QMap<QString, QVariant> eChildMap = eChild.value<QMap<QString, QVariant> >();
        Q_FOREACH(QVariant rChild, rChildren)
        {
            QMap<QString, QVariant> rChildMap = rChild.value<QMap<QString, QVariant> >();
            if (eChildMap.value("name") == rChildMap.value("name"))
            {
                bool check = recursiveCheckExpectations(rChildMap, eChildMap);
                if(!check)
                    return false;
            }
        }
    }
    return true;
}

