/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef KNOTESTRAY_H
#define KNOTESTRAY_H

#include <KStatusNotifierItem>
#include <QIcon>
class KNotesTray : public KStatusNotifierItem
{
    Q_OBJECT
public:
    explicit KNotesTray(QWidget *parent = nullptr);
    ~KNotesTray();

    void updateNumberOfNotes(int value);

private:
    QIcon mIcon;
};

#endif // KNOTESTRAY_H
