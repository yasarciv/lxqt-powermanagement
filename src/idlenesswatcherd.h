/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright (C) 2013  Alec Moskvin <alecm@gmx.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#ifndef IDLENESSWATCHERD_H
#define IDLENESSWATCHERD_H

#include <xcb/xcb.h>
#include <QMap>
#include <QTimer>
#include <QProcess>
#include <QDateTime>
#include <QDBusContext>
#include <QDBusServiceWatcher>
#include <lxqt/lxqtsettings.h>
#include <lxqt/lxqtnotification.h>

class IdlenessWatcherd : public QObject, protected QDBusContext
{
    Q_OBJECT
public:
    explicit IdlenessWatcherd(QObject* parent = 0);

signals:
    void ActiveChanged(bool in0);

public slots:
    void Lock();
    uint GetSessionIdleTime();
    uint GetActiveTime();
    bool GetActive();
    bool SetActive(bool activate);
    void SimulateUserActivity();
    uint Inhibit(const QString& applicationName, const QString& reasonForInhibit);
    void UnInhibit(uint cookie);
    uint Throttle(const QString& applicationName, const QString& reasonForThrottle);
    void UnThrottle(uint cookie);

private slots:
    void idleTimeout();
    void loadSettings();
    void screenUnlocked(int exitCode, QProcess::ExitStatus exitStatus);
    void notificationAction(int num);
    void serviceUnregistered(const QString& service);

private:
    bool lockScreen();
    void restartTimer();
    uint getIdleTimeMs();

    static xcb_screen_t* screenOfDisplay(xcb_connection_t* mConn, int screen);

    LxQt::Settings mSettings;
    QTimer mTimer;
    QProcess mLockProcess;
    LxQt::Notification mErrorNotification;
    QString mLockCommand;
    QDateTime mLockTime;
    QMap<uint,QString> mInhibitors;
    QDBusServiceWatcher mDBusWatcher;
    xcb_connection_t* mConn;
    xcb_screen_t* mScreen;
    uint mIdleTimeoutMs;
    uint mInhibitorCookie;
    bool mIsLocked;
    bool mTurnOffDisplay;
};

#endif // IDLENESSWATCHERD_H