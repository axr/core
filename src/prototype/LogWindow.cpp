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

#include <QBuffer>
#include "LogWindow.h"

using namespace AXR;

#include "ui_LogWindow.h"

class LogWindow::Private
{
public:
    QByteArray logBuffer;
    QBuffer *loggingDevice;
};

LogWindow::LogWindow(QWidget *parent)
: QDialog(parent), d(new Private()), ui(new Ui::LogWindow)
{
    this->ui->setupUi(this);

    d->loggingDevice = new QBuffer(&d->logBuffer);
    d->loggingDevice->open(QIODevice::ReadWrite);
    connect(d->loggingDevice, SIGNAL(bytesWritten(qint64)), SLOT(dataLogged(qint64)));
}

LogWindow::~LogWindow()
{
    disconnect(d->loggingDevice, SIGNAL(bytesWritten(qint64)), this, SLOT(dataLogged(qint64)));
    delete d->loggingDevice;
    delete d;

    delete this->ui;
}

AXRString LogWindow::logText() const
{
    return this->ui->logTextEdit->toPlainText();
}

QIODevice* LogWindow::logBuffer() const
{
    return d->loggingDevice;
}

void LogWindow::setLogText(const AXRString &text)
{
    this->ui->logTextEdit->setPlainText(text);
}

void LogWindow::appendLogText(const AXRString &text)
{
    this->setLogText(this->logText() + text);
}

void LogWindow::clearLogText()
{
    this->setLogText(AXRString());
}

void LogWindow::dataLogged(qint64 bytes)
{
    appendLogText(d->logBuffer.right(bytes));
}
