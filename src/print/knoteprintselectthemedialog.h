/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "knotes_export.h"
#include <QDialog>

class KNotePrintSelectThemeComboBox;
class KNOTES_EXPORT KNotePrintSelectThemeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit KNotePrintSelectThemeDialog(QWidget *parent = nullptr);
    ~KNotePrintSelectThemeDialog() override;

    Q_REQUIRED_RESULT QString selectedTheme() const;

private:
    KNotePrintSelectThemeComboBox *const mThemes;
};
