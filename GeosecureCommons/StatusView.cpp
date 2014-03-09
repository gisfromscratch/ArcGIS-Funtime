#include "StatusView.h"

StatusView::StatusView(QMainWindow *parent) :
    QObject(parent),
    _statusBar(new QStatusBar(parent)),
    _statusLabel(new QLabel(_statusBar))
{
    if (nullptr != parent)
    {
        parent->setStatusBar(_statusBar);
    }
    _statusLabel->setAlignment(Qt::AlignLeft);
    _statusBar->addPermanentWidget(_statusLabel, 1);
}

void StatusView::setStatusMessage(const QString &message)
{
    _statusLabel->setText(message);
}

void StatusView::showDefaultStatusMessage()
{
    setStatusMessage(tr("Ready"));
}
