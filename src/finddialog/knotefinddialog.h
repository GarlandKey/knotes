/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef KNOTEFINDDIALOG_H
#define KNOTEFINDDIALOG_H

#include <QDialog>
#include <AkonadiCore/Item>
#include "knotes_export.h"
class QPushButton;
class QLineEdit;
class QListWidgetItem;
class QListWidget;
class QLabel;
namespace NoteShared {
class NoteListWidget;
}
class KNoteFindWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KNoteFindWidget(QWidget *parent = nullptr);
    ~KNoteFindWidget();

    void setExistingNotes(const QHash<Akonadi::Item::Id, Akonadi::Item> &notes);

Q_SIGNALS:
    void noteSelected(Akonadi::Item::Id);

private Q_SLOTS:
    void slotTextChanged(const QString &);
    void slotSearchNote();
    void slotItemDoubleClicked(QListWidgetItem *);

private:
    void customContextMenuRequested(const QPoint &);
    QHash<Akonadi::Item::Id, Akonadi::Item> mNotes;
    QLabel *mResultSearch = nullptr;
    QLineEdit *mSearchLineEdit = nullptr;
    QPushButton *mSearchButton = nullptr;
    NoteShared::NoteListWidget *mNoteList = nullptr;
};

class KNOTES_EXPORT KNoteFindDialog : public QDialog
{
    Q_OBJECT
public:
    explicit KNoteFindDialog(QWidget *parent = nullptr);
    ~KNoteFindDialog();
    void setExistingNotes(const QHash<Akonadi::Item::Id, Akonadi::Item> &notes);

Q_SIGNALS:
    void noteSelected(Akonadi::Item::Id);

private:
    void writeConfig();
    void readConfig();
    KNoteFindWidget *mNoteFindWidget = nullptr;
};

#endif // KNOTEFINDDIALOG_H
