/*******************************************************************
 KNotes -- Notes for the KDE project

 SPDX-FileCopyrightText: 2005 Michael Brade <brade@kde.org>

 SPDX-License-Identifier: GPL-2.0-or-later
*******************************************************************/

#ifndef NOTEALARMDIALOG_H
#define NOTEALARMDIALOG_H

#include <QDialog>
#include "noteshared_export.h"
class QDateTime;
class QAbstractButton;
namespace NoteShared {
class NoteAlarmDialogPrivate;
class NOTESHARED_EXPORT NoteAlarmDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NoteAlarmDialog(const QString &caption, QWidget *parent = nullptr);
    ~NoteAlarmDialog();
    void setAlarm(const QDateTime &dateTime);
    Q_REQUIRED_RESULT QDateTime alarm() const;

private:
    void slotButtonChanged(QAbstractButton *button);
    NoteAlarmDialogPrivate *const d;
};
}

#endif
