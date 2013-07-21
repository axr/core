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

#include "Json.h"
#include "TestCase.h"
#include "TestSuite.h"

class TestSuitePrivate
{
public:
    TestSuitePrivate() :
        parentSuite(),
        name(),
        failed(),
        failedCount(),
        passedCount(),
        childSuites(),
        cases()
    {
    }

    TestSuite *parentSuite;
    QString name;
    bool failed;
    int failedCount;
    int passedCount;
    QList<TestSuite *> childSuites;
    QList<TestCase *> cases;
};

TestSuite::TestSuite(TestSuite *parentSuite, const QString &path) :
    d(new TestSuitePrivate)
{
    d->parentSuite = parentSuite;
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << QString("Could not find test suite at %1").arg(path);
        d->failed = true;
        return;
    }

    QDir dir = QDir(path);
    dir.cdUp();
    const QString suiteDirectory = dir.path().append("/");

    QTextStream textStream(&file);
    const QString infoString = textStream.readAll();
    QVariantMap info = Json::decode(infoString);

    d->name = info.value("name", "Unnamed").toString();

    if (info.value("include").canConvert<QList<QVariant> >())
    {
        QList<QVariant> include = info.value("include").toList();
        for (int i = 0; i < include.size(); i++)
        {
            if (include.at(i).canConvert<QString>())
            {
                QString includePath = QString(suiteDirectory);
                includePath.append(include.at(i).toString());

                d->childSuites.append(new TestSuite(this, includePath));
            }
        }
    }

    if (info.value("cases").canConvert<QList<QVariant> >())
    {
        QList<QVariant> cases = info.value("cases").toList();
        for (int i = 0; i < cases.size(); i++)
        {
            QVariant casePath = cases.at(i);

            if (casePath.canConvert<QString>())
            {
                QString casePath = QString(suiteDirectory);
                casePath.append(cases.at(i).toString());

                d->cases.append(new TestCase(this, casePath));
            }
        }
    }
}

TestSuite::~TestSuite()
{
    for (int i = 0; i < d->childSuites.size(); i++)
    {
        delete d->childSuites.at(i);
    }

    for (int i = 0; i < d->cases.size(); i++)
    {
        delete d->cases.at(i);
    }
}

void TestSuite::execute()
{
    for (int i = 0; i < d->childSuites.size(); i++)
    {
        d->childSuites.at(i)->execute();
    }

    for (int i = 0; i < d->cases.size(); i++)
    {
        d->cases.at(i)->execute();
    }
}

QString TestSuite::getResults(const QString &format)
{
    if (format == "xml")
    {
        QByteArray byteArray;
        QBuffer outputBuffer(&byteArray);
        outputBuffer.open(QIODevice::WriteOnly);

        QXmlStreamWriter *xmlWriter = new QXmlStreamWriter();
        xmlWriter->setDevice(&outputBuffer);
        xmlWriter->setAutoFormatting(true);

        xmlWriter->writeStartDocument();
        this->buildResults(xmlWriter);
        xmlWriter->writeEndDocument();

        delete xmlWriter;
        return QString(byteArray);
    }
    else if (format == "plain")
    {
        QString output = QString();
        this->buildResults(output, 0);
        return output;
    }

    return QString("%1 is not supported\n").arg(format);
}

void TestSuite::buildResults(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("testSuite");
    xmlWriter->writeAttribute("name", d->name);
    xmlWriter->writeAttribute("failed", QString::number(d->failedCount));
    xmlWriter->writeAttribute("passed", QString::number(d->passedCount));
    xmlWriter->writeAttribute("total", QString::number(d->failedCount + d->passedCount));

    for (int i = 0; i < d->childSuites.size(); i++)
    {
        d->childSuites.at(i)->buildResults(xmlWriter);
    }

    for (int i = 0; i < d->cases.size(); i++)
    {
        d->cases.at(i)->buildResults(xmlWriter);
    }

    xmlWriter->writeEndElement();
}

void TestSuite::buildResults(QString &output, int indent)
{
    QString whitespace = QString(" ").repeated(indent * 4);
    QString out = QString("Executing test suite \"%2\"\n\n").arg(d->name);
    out.prepend(whitespace);

    for (int i = 0; i < d->childSuites.size(); i++)
    {
        d->childSuites.at(i)->buildResults(out, indent + 1);
    }

    for (int i = 0; i < d->cases.size(); i++)
    {
        d->cases.at(i)->buildResults(out, indent + 1);
    }

    out.append(QString("Test suite [%1]\n").arg(d->failed ? "FAILED" : "PASSED"));

    QString summary = QString("Passed: %1   Failed: %2   Total: %3\n")
        .arg(d->passedCount)
        .arg(d->failedCount)
        .arg(d->failedCount + d->passedCount);

    out.append(summary);
    out.append(QString("-").repeated(summary.length() - 1));
    out.append("\n");

    output.append(out.replace("\n", QString(whitespace).prepend("\n")));
    output.append("\n");
}

void TestSuite::setFailed()
{
    d->failedCount++;
    d->failed = true;

    if (d->parentSuite)
    {
        d->parentSuite->setFailed();
    }
}

void TestSuite::setPassed()
{
    d->passedCount++;

    if (d->parentSuite)
    {
        d->parentSuite->setPassed();
    }
}

void TestSuite::setVerbose(bool verbose)
{
    for (int i = 0; i < d->childSuites.size(); i++)
    {
        d->childSuites.at(i)->setVerbose(verbose);
    }

    for (int i = 0; i < d->cases.size(); i++)
    {
        d->cases.at(i)->setVerbose(verbose);
    }
}
