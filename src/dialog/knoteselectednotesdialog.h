/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef KNOTESELECTEDNOTESDIALOG_H
#define KNOTESELECTEDNOTESDIALOG_H

#include <QDialog>
#include <AkonadiCore/Item>
class QListWidget;
class KNote;
class QPushButton;
class KNoteSelectedNotesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit KNoteSelectedNotesDialog(QWidget *parent = nullptr);
    ~KNoteSelectedNotesDialog();

    void setNotes(const QHash<Akonadi::Item::Id, KNote *> &notes);

    QStringList selectedNotes() const;

private Q_SLOTS:
    void slotSelectionChanged();

private:
    enum listViewData {
        AkonadiId = Qt::UserRole + 1
    };

    void writeConfig();
    void readConfig();
    QListWidget *mListNotes = nullptr;
    QHash<Akonadi::Item::Id, KNote *> mNotes;
    QPushButton *mOkButton = nullptr;
};

#endif // KNOTESELECTEDNOTESDIALOG_H
