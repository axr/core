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

#include "AXRController.h"
#include "HSSContainer.h"
#include "AXRDocument.h"
#include "TestSuite.h"
#include "TestCase.h"
#include "Json.h"

class TestCasePrivate
{
public:
    TestCasePrivate() :
        suite(),
        name(),
        load(),
        path(),
        strict(),
        verbose(),
        failed(),
        message(),
        resultedJSON(),
        expected()
    {
    }

    void recursiveCompare(const QVariantMap &resulted, const QVariantMap &expected);
    void setFailed();
    void setFailed(const QString &message);
    void setPassed();

    TestSuite *suite;
    QString name;
    QString load;
    QString path;
    bool strict;
    bool verbose;
    bool failed;
    QString message;
    QString resultedJSON;
    QMap<QString, QVariant> expected;
};

TestCase::TestCase(TestSuite *suite, const QString &path) :
    d(new TestCasePrivate)
{
    d->suite = suite;
    d->path = path;

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        d->setFailed(QString("Could not find test case at %1").arg(path));
        return;
    }

    QTextStream textStream(&file);
    const QString infoString = textStream.readAll();
    QVariantMap info = Json::decode(infoString);

    d->name = info.value("name", "Unnamed").toString();
    d->load = info.value("load").toString();
    d->strict = info.value("strict", 1).toInt() == 1 ? true : false;
    d->expected = info.value("expect").toMap();
}

TestCase::~TestCase()
{
}

void TestCase::buildResults(QXmlStreamWriter *xmlWriter)
{
    xmlWriter->writeStartElement("testCase");
    xmlWriter->writeAttribute("name", d->name);
    xmlWriter->writeAttribute("path", d->path);
    xmlWriter->writeAttribute("status", d->failed ? "failed" : "passed");
    xmlWriter->writeEndElement();
}

void TestCase::buildResults(QString &output, int indent)
{
    QString out = QString("[%1] %2\n%3\n\n")
        .arg(d->failed ? "FAILED" : "PASSED")
        .arg(d->name)
        .arg(d->path);

    if (!d->message.isEmpty())
    {
        out.append(QString("    ").append(d->message));
        out.append("\n\n");
    }

    if (d->failed && d->verbose && !d->resultedJSON.isEmpty())
    {
        out.append("Resulting JSON:\n");
        out.append(d->resultedJSON);
        out.append("\n\n");
    }

    QString whitespace = QString(" ").repeated(indent * 4);
    output.append(out.replace("\n", whitespace.append("\n")));
}

void TestCase::execute()
{
    AXR::AXRDocument document;
    document.setWindowSize(400, 400);

    QDir* dir = new QDir(d->path);
    dir->cdUp();
    QString path = dir->absolutePath().append("/").append(d->load);

    if (!document.loadFileByPath(QUrl::fromLocalFile(path)))
    {
        d->setFailed(QString("Failed to load URL %1").arg(path));
        return;
    }

    QSharedPointer<AXR::HSSContainer> root = document.controller()->root();

    QVariantMap resulted = root->toMap();
    QVariantMap expected(d->expected);

    d->resultedJSON = Json::encode(resulted);
    d->recursiveCompare(resulted, expected);

    if (!d->failed)
    {
        d->setPassed();
    }
}

void TestCase::setVerbose(bool verbose)
{
    d->verbose = verbose;
}


void TestCasePrivate::recursiveCompare(const QVariantMap &resulted, const QVariantMap &expected)
{
    if (resulted.value("message").canConvert<QString>() &&
        resulted.value("message").toString() == "Parse error")
    {
        this->setFailed("Unexpected resulted JSON parse error");
        return;
    }

    QList<QVariant> expectedProperties(expected.value("properties").toList());
    QVariantMap resultedProperties;

    if (resulted.value("properties").canConvert<QVariantMap>())
    {
        resultedProperties = resulted.value("properties").toMap();
    }

    for (int i = 0; i < expectedProperties.size(); i++)
    {
        QVariantMap property = expectedProperties.at(i).toMap();

        QString expectedProperty = property.value("name").toString();
        QVariant expectedValue = property.value("value");
        QString type = property.value("type").toString();

        if (!resultedProperties.isEmpty())
        {
            if (type.isEmpty() || type == "match")
            {
                if (resultedProperties.value(expectedProperty) != expectedValue)
                {
                    this->setFailed("Condition `match` failed");
                    return;
                }
            }
            else if (type == "no_match")
            {
                if (resultedProperties.value(expectedProperty) == expectedValue)
                {
                    this->setFailed("Condition `no_match` failed");
                    return;
                }
            }
        }
        else
        {
            this->setFailed();
            return;
        }
    }

    QList<QVariant> expectedChildren = expected.value("children").toList();
    QList<QVariant> resultedChildren = resulted.value("children").toList();

    for (int i = 0; i < expectedChildren.size(); i++)
    {
        QVariantMap expectedChild = expectedChildren.at(i).toMap();
        QString expectedChildName = expectedChild.value("name").toString();
        expectedChildName.append("_source_obj");
        bool childFound = false;

        for (int j = 0; j < resultedChildren.size(); j++)
        {
            QVariantMap resultedChild = resultedChildren.at(j).toMap();

            if (expectedChildName == resultedChild.value("name").toString())
            {
                childFound = true;
                this->recursiveCompare(resultedChild, expectedChild);

                if (this->failed)
                {
                    // Stop scanning
                    return;
                }
            }
        }

        if (!childFound)
        {
            this->setFailed(QString("Child %1 not present in the resulting document")
                .arg(expectedChild.value("name").toString()));
            return;
        }
    }

    // All expected children are present so if the counts don't match it means
    // that the resulted document has too many children
    if (this->strict && expectedChildren.size() != resultedChildren.size())
    {
        this->setFailed("Unwanted children in resulting document");
        return;
    }
}

void TestCasePrivate::setFailed()
{
    if (this->suite && !this->failed)
    {
        this->suite->setFailed();
    }

    this->failed = true;
}

void TestCasePrivate::setFailed(const QString &message)
{
    this->message = message;
    this->setFailed();
}

void TestCasePrivate::setPassed()
{
    if (this->suite && !this->failed)
    {
        this->suite->setPassed();
    }
}
