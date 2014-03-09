#ifndef STATUSVIEW_H
#define STATUSVIEW_H

#include "geosecurecommons_global.h"

#include <QObject>
#include <QLabel>
#include <QMainWindow>
#include <QStatusBar>

/*!
 * \brief The view of the status bar.
 */
class GEOSECURECOMMONSSHARED_EXPORT StatusView : public QObject
{
    Q_OBJECT
public:
    explicit StatusView(QMainWindow *parent = 0);

    /*!
     * \brief setStatusMessage  Updates the status message of the status bar.
     * \param message           The message which should be shown in the status bar.
     */
    void setStatusMessage(const QString &message);

    /*!
     * \brief showDefaultStatusMessage  Sets the status message to the default value e.g "Ready"
     */
    void showDefaultStatusMessage();

signals:

public slots:

private:
    QStatusBar *_statusBar;
    QLabel *_statusLabel;
};

#endif // STATUSVIEW_H
