/*
   Copyright (C) 2013-2016 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef KNOTEDISPLAYSETTINGS_H
#define KNOTEDISPLAYSETTINGS_H

#include <QColor>
#include <QFont>
#include <QPoint>

#include "knotes_export.h"
namespace NoteShared {
class NoteDisplayAttribute;
}

class KNOTES_EXPORT KNoteDisplaySettings
{
public:
    explicit KNoteDisplaySettings(NoteShared::NoteDisplayAttribute *attr = nullptr);
    ~KNoteDisplaySettings();

    void setDisplayAttribute(NoteShared::NoteDisplayAttribute *attr);
    NoteShared::NoteDisplayAttribute *displayAttribute() const;
    QColor backgroundColor() const;

    QColor foregroundColor() const;
    QSize size() const;
    bool rememberDesktop() const;
    int tabSize() const;
    QFont font() const;
    QFont titleFont() const;
    int desktop() const;
    bool isHidden() const;
    QPoint position() const;
    bool showInTaskbar() const;
    bool keepAbove() const;
    bool keepBelow() const;
    bool autoIndent() const;

private:
    NoteShared::NoteDisplayAttribute *mDisplayAttribute;
};

#endif // KNOTEDISPLAYSETTINGS_H
