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
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QScriptEngine>
#include <QStringList>
#include <QTextStream>
#include <QUrl>
#include <iostream>

#include "AXRController.h"
#include "HSSContainer.h"
#include "AXRDocument.h"
#include "json.h"

bool executeLayoutTests(const QString &filePath);
bool testFile(const QString &href, const QString &expect);
bool recursiveCheckExpectations(const QMap<QString, QVariant> &resulted, const QMap<QString, QVariant> &expected);
void output(const QString &message, bool onlyIfVerbose = false, const QString &newlines = "\n", bool addWhitespace = true);
void voutput(const QString &message, const QString &newlines = "\n");
void newline(bool onlyIfVerbose = false, int amount = 1);
void title(const QString &message);
void xmlOpen(const QString &styleSheet);
void xmlClose();
QString xmlTag(const QString &message, const QString &tagName, const QString &attributes = "");
QString xmlTagOpen(const QString &tagName, const QString &attributes = "");
QString xmlTagClose(const QString &tagName);
void increaseWhitespace();
void decreaseWhitespace();
QString reindent(const QString &message);

using namespace AXR;

bool outputAsXML;
bool verbose;
QString whitespace;

void printUsage()
{
    QTextStream stream(stderr);
    QFileInfo fi(QCoreApplication::applicationFilePath());
    stream << QString(QLatin1String("Usage: %1 json-file [-v] [-format plain|xml] [-stylesheet style.hss]"))
        .arg(fi.fileName());
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();
    outputAsXML = false;
    verbose = false;

    if (args.size() < 2)
    {
        printUsage();
        return 1;
    }

    // Path of the JSON file describing the test suite to execute
    const QString jsonFilePath = args.at(1);

    // Verbose output?
    int vPos = args.indexOf("-v");
    if (vPos > 1)
        verbose = true;

    // Get the desired output format
    int formatPos = args.indexOf("-format");
    if (formatPos > 1 && args.size() >= formatPos + 1)
        outputAsXML = args.at(formatPos + 1).toLower() == "xml";

    // Get the path to the HSS stylesheet for the test report (only applies for XML output format)
    QString styleSheetPath;
    int stylesheetPos = args.indexOf("-stylesheet");
    if (stylesheetPos > 1 && args.length() >= stylesheetPos + 1)
        styleSheetPath = args.at(stylesheetPos + 1);
    
    whitespace = "";
    xmlOpen(styleSheetPath);
    increaseWhitespace();
    title("Executing layout tests:");
    bool allPassed = executeLayoutTests(jsonFilePath);

    if (!allPassed)
    {
        output(xmlTag("\nWARNING: some tests failed", "result"));
    }
    else
    {
        output(xmlTag("All tests passed", "result"));
    }
    decreaseWhitespace();
    xmlClose();

    return allPassed ? 0 : 1;
}

bool executeLayoutTests(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "QIODevice:" << file.errorString();
        return false;
    }

    QTextStream textStream(&file);
    const QString tests = textStream.readAll();

    QMap<QString, QVariant> decoded = Json::decode(tests);
    QMapIterator<QString, QVariant> i(decoded);
    bool allPassed = true;
    while (i.hasNext()) {
        i.next();
        QVariant test = i.value();
        if (test.canConvert<QMap<QString, QVariant> >()) {
            output(xmlTagOpen("test"));
            increaseWhitespace();
            QMap<QString, QVariant> testMap = test.value<QMap<QString, QVariant> >();
            QString testName = testMap.value("name").toString();
            if (!outputAsXML)
            {
                output(QString("Executing test: ").append(testName));
            }
            else
            {
                output(xmlTag(testName, "name"));
            }

            QString href = testMap.value("href").toString();
            href = QFileInfo(QFileInfo(filePath).canonicalPath(), href).absoluteFilePath();
            QString expect = testMap.value("expect").toString();
            expect = QFileInfo(QFileInfo(filePath).canonicalPath(), expect).absoluteFilePath();
            if (verbose)
            {
                if(!outputAsXML) output("Checking file:");
                output(xmlTag(href, "results_file"));
                if(!outputAsXML) output("against:");
                output(xmlTag(expect, "expectations_file"), true, "\n");
                if (!outputAsXML) output("");
            }
            bool testPassed = testFile(href, expect);
            if(!testPassed)
            {
                output(xmlTag("Failed", "result_failed"));
                allPassed = false;
            } else {
                output(xmlTag("Passed", "result_passed"));
            }
            if (!outputAsXML) voutput("----------------------------", "\n");
            decreaseWhitespace();
            output(xmlTagClose("test"));
        }
    }
    return allPassed;
}

bool testFile(const QString &href, const QString &expect)
{
    AXRDocument document;
    document.setWindowSize(400, 400);
    if (!document.loadFileByPath(QUrl(QString("file://").append(href))))
    {
        output(QString("Failed to load URL ").append(href));
        return false;
    }

    QSharedPointer<HSSContainer> root = document.controller()->root();
    if (verbose)
    {
        if(!outputAsXML) output("The resulted JSON representation of the AXR document:");
        output(xmlTagOpen("resulted_json"));
        increaseWhitespace();
    }
    AXRString jsonRep = root->toJSON(whitespace);
    if (verbose) {
        output(jsonRep, "\n\n");
        decreaseWhitespace();
        output(xmlTagClose("resulted_json"));
    }

    QMap<QString, QVariant> resultedJson = Json::decode(jsonRep);

    QFile file(expect);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "QIODevice:" << file.errorString();
        return false;
    }

    QTextStream ts(&file);
    QString expected = ts.readAll();

    if (verbose)
    {
        if(!outputAsXML) output("The resulted JSON representation of the AXR document:");
        output(xmlTagOpen("expected_json"));
        increaseWhitespace();
    }
    if (verbose) {
        output(reindent(expected.trimmed()), true, "", false);
        decreaseWhitespace();
        output(xmlTagClose("expected_json"));
    }

    QMap<QString, QVariant> expectedJson = Json::decode(expected);
    return recursiveCheckExpectations(resultedJson, expectedJson);
}

bool recursiveCheckExpectations(const QMap<QString, QVariant> &resulted, const QMap<QString, QVariant> &expected)
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

void output(const QString &message, bool onlyIfVerbose, const QString &newlines, bool addWhitespace)
{
    if (!onlyIfVerbose || verbose)
    {
        QTextStream ts(stdout);
        if (addWhitespace)
            ts << whitespace;

        ts << message << newlines;
    }
}

void voutput(const QString &message, const QString &newlines)
{
    output(message, true, newlines);
}

void newline(bool onlyIfVerbose, int amount)
{
    if (!outputAsXML)
        output("", onlyIfVerbose, QString(amount, '\n'));
}

void title(const QString &message)
{
    output(xmlTag(message, "title"));
    if (!outputAsXML)
        output(QString(message.size(), '='), false, "\n\n");
}

void xmlOpen(const QString &styleSheet)
{
    if (outputAsXML)
    {
        output("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        if (!styleSheet.isEmpty())
            output(QString("<?xml-stylesheet type=\"application/x-hss\" href=\"").append(styleSheet).append("\" version=\"0.4.9\"?>"));

        output("<tests>");
    }
}

void xmlClose()
{
    if (outputAsXML)
        output("</tests>");
}

QString xmlTag(const QString &message, const QString &tagName, const QString &attributes)
{
    return xmlTagOpen(tagName, attributes).append(message).append(xmlTagClose(tagName));
}

QString xmlTagOpen(const QString &tagName, const QString &attributes)
{
    if (outputAsXML)
    {
        return !attributes.isEmpty()
                    ? QString("<%1 %2>").arg(tagName).arg(attributes)
                    : QString("<%1>").arg(tagName);
    }

    return QString();
}

QString xmlTagClose(const QString &tagName)
{
    return outputAsXML ? QString("</%1>").arg(tagName) : QString();
}

void increaseWhitespace()
{
    whitespace.append("    ");
}

void decreaseWhitespace()
{
    if (whitespace.length() >= 4)
        whitespace.remove(whitespace.length() - 4, 4);
    else
        whitespace = "";
}

QString reindent(const QString &message)
{
    QString ret;
    QStringList lines = message.split("\n", QString::SkipEmptyParts);
    Q_FOREACH (const QString &line, lines)
    {
        ret.append(QString("%1%2\n").arg(whitespace).arg(line));
    }

    return ret;
}
