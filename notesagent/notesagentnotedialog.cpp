/*
   SPDX-FileCopyrightText: 2013-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "notesagentnotedialog.h"
#include "notesagent_debug.h"
#include <ItemFetchJob>
#include <ItemFetchScope>
#include "attributes/notedisplayattribute.h"
#include <KPIMTextEdit/RichTextEditorWidget>
#include <KPIMTextEdit/RichTextEditor>

#include <KSharedConfig>

#include <KMime/KMimeMessage>

#include <QIcon>

#include <QLineEdit>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <KConfigGroup>
#include <QPushButton>

NotesAgentNoteDialog::NotesAgentNoteDialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(QIcon::fromTheme(QStringLiteral("knotes")));

    mSubject = new QLineEdit(this);
    mSubject->setReadOnly(true);
    mainLayout->addWidget(mSubject);

    mNote = new KPIMTextEdit::RichTextEditorWidget(this);
    mNote->setReadOnly(true);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &NotesAgentNoteDialog::reject);

    mainLayout->addWidget(mNote);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

NotesAgentNoteDialog::~NotesAgentNoteDialog()
{
    writeConfig();
}

void NotesAgentNoteDialog::setNoteId(Akonadi::Item::Id id)
{
    Akonadi::Item item(id);
    Akonadi::ItemFetchJob *job = new Akonadi::ItemFetchJob(item, this);
    job->fetchScope().fetchFullPayload(true);
    job->fetchScope().fetchAttribute< NoteShared::NoteDisplayAttribute >();
    connect(job, &Akonadi::ItemFetchJob::result, this, &NotesAgentNoteDialog::slotFetchItem);
}

void NotesAgentNoteDialog::slotFetchItem(KJob *job)
{
    if (job->error()) {
        qCDebug(NOTESAGENT_LOG) << "fetch item failed " << job->errorString();
        return;
    }
    Akonadi::ItemFetchJob *itemFetchJob = static_cast<Akonadi::ItemFetchJob *>(job);
    const Akonadi::Item::List lstItem = itemFetchJob->items();
    if (!lstItem.isEmpty()) {
        const Akonadi::Item item = lstItem.first();
        KMime::Message::Ptr noteMessage = item.payload<KMime::Message::Ptr>();
        if (noteMessage) {
            const KMime::Headers::Subject *const subject = noteMessage->subject(false);
            if (subject) {
                mSubject->setText(subject->asUnicodeString());
            }
            if (noteMessage->contentType()->isHTMLText()) {
                mNote->setAcceptRichText(true);
                mNote->setHtml(noteMessage->mainBodyPart()->decodedText());
            } else {
                mNote->setAcceptRichText(false);
                mNote->setPlainText(noteMessage->mainBodyPart()->decodedText());
            }
        }
        if (item.hasAttribute<NoteShared::NoteDisplayAttribute>()) {
            const NoteShared::NoteDisplayAttribute *attr = item.attribute<NoteShared::NoteDisplayAttribute>();
            if (attr) {
                mNote->editor()->setTextColor(attr->backgroundColor());
                //TODO add background color.
            }
        }
    }
}

void NotesAgentNoteDialog::readConfig()
{
    KConfigGroup grp(KSharedConfig::openConfig(), "NotesAgentNoteDialog");
    const QSize size = grp.readEntry("Size", QSize(300, 200));
    if (size.isValid()) {
        resize(size);
    }
}

void NotesAgentNoteDialog::writeConfig()
{
    KConfigGroup grp(KSharedConfig::openConfig(), "NotesAgentNoteDialog");
    grp.writeEntry("Size", size());
    grp.sync();
}
