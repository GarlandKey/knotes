/*
  Copyright (c) 2014-2015 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef KNOTEDELETESELECTEDNOTESDIALOG_H
#define KNOTEDELETESELECTEDNOTESDIALOG_H

#include <QDialog>
#include <AkonadiCore/Item>
class QPushButton;
namespace NoteShared
{
class NoteListWidget;
}

class KNoteDeleteSelectedNotesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit KNoteDeleteSelectedNotesDialog(QWidget *parent = Q_NULLPTR);
    ~KNoteDeleteSelectedNotesDialog();

    void setNotes(const Akonadi::Item::List &notes);

    Akonadi::Item::List selectedNotes() const;

private Q_SLOTS:
    void slotDeleteNoteSelectionChanged();
private:
    void readConfig();
    void writeConfig();
    NoteShared::NoteListWidget *mNoteList;
    QPushButton *mOkButton;
};

#endif // KNOTEDELETESELECTEDNOTESDIALOG_H
