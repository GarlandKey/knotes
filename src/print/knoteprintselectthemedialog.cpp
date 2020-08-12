/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "knoteprintselectthemedialog.h"
#include "knoteprintselectthemecombobox.h"

#include <KLocalizedString>

#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

KNotePrintSelectThemeDialog::KNotePrintSelectThemeDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Select theme"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);

    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    QPushButton *user1Button = new QPushButton;
    buttonBox->addButton(user1Button, QDialogButtonBox::ActionRole);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &KNotePrintSelectThemeDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &KNotePrintSelectThemeDialog::reject);

    QWidget *w = new QWidget;
    QHBoxLayout *lay = new QHBoxLayout;
    w->setLayout(lay);

    QLabel *lab = new QLabel(i18n("Themes:"));
    lay->addWidget(lab);

    mThemes = new KNotePrintSelectThemeComboBox;
    mThemes->loadThemes();
    lay->addWidget(mThemes);
    mainLayout->addWidget(w);
    mainLayout->addWidget(buttonBox);
}

KNotePrintSelectThemeDialog::~KNotePrintSelectThemeDialog()
{
}

QString KNotePrintSelectThemeDialog::selectedTheme() const
{
    return mThemes->selectedTheme();
}
