/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#ifndef SHOWFOLDERNOTESATTRIBUTE_H
#define SHOWFOLDERNOTESATTRIBUTE_H

#include <AkonadiCore/attribute.h>
#include "noteshared_export.h"

namespace NoteShared {
class NOTESHARED_EXPORT ShowFolderNotesAttribute : public Akonadi::Attribute
{
public:
    ShowFolderNotesAttribute();
    ~ShowFolderNotesAttribute() override;

    ShowFolderNotesAttribute *clone() const override;
    Q_REQUIRED_RESULT QByteArray type() const override;
    Q_REQUIRED_RESULT QByteArray serialized() const override;
    void deserialize(const QByteArray &data) override;
};
}

#endif // SHOWFOLDERNOTESATTRIBUTE_H
