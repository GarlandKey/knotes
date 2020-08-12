/*******************************************************************
 KNotes -- Notes for the KDE project

 SPDX-FileCopyrightText: 2003 Daniel Martin <daniel.martin@pirack.com>
 SPDX-FileCopyrightText: 2004, 2006 Michael Brade <brade@kde.org>
 SPDX-FileCopyrightText: 2013 Laurent Montel <montel@kde.org>

 SPDX-License-Identifier: GPL-2.0-or-later
*******************************************************************/

#include "notesnetworkreceiver.h"

#include <QDateTime>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTimer>
#include <QTextCodec>
#include "noteshared_debug.h"
#include "noteutils.h"
#include <QLocale>

// Maximum note size in chars we are going to accept,
// to prevent "note floods".
#define MAXBUFFER 4096

// Maximum time we are going to wait between data receptions,
// to prevent memory and connection floods. In milliseconds.
#define MAXTIME 10000

// Small buffer's size
#define SBSIZE 512
using namespace NoteShared;

class NoteShared::NotesNetworkReceiverPrivate
{
public:
    NotesNetworkReceiverPrivate(QTcpSocket *s)
        : m_timer(nullptr)
        , m_buffer(new QByteArray())
        , m_sock(s)
    {
    }

    ~NotesNetworkReceiverPrivate()
    {
        delete m_buffer;
        delete m_sock;
    }

    QTimer *m_timer = nullptr;       // to avoid memory and connection floods

    QByteArray *m_buffer = nullptr;
    QTcpSocket *m_sock = nullptr;

    QString m_titleAddon;
};

NotesNetworkReceiver::NotesNetworkReceiver(QTcpSocket *s)
    : QObject()
    , d(new NoteShared::NotesNetworkReceiverPrivate(s))
{
    const QString date = QLocale().toString(QDateTime::currentDateTime(), QLocale::ShortFormat);

    // Add the remote IP or hostname and the date to the title, to help the
    // user guess who wrote it.
    d->m_titleAddon = QStringLiteral(" [%1, %2]")
                      .arg(d->m_sock->peerAddress().toString(), date);

    // Setup the communications
    connect(d->m_sock, &QTcpSocket::readyRead, this, &NotesNetworkReceiver::slotDataAvailable);
    connect(d->m_sock, &QTcpSocket::disconnected, this, &NotesNetworkReceiver::slotConnectionClosed);
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    connect(d->m_sock, qOverload<QAbstractSocket::SocketError>(&QTcpSocket::error), this, &NotesNetworkReceiver::slotError);
#else
    connect(d->m_sock, qOverload<QAbstractSocket::SocketError>(&QTcpSocket::errorOccurred), this, &NotesNetworkReceiver::slotError);
#endif
    // Setup the timer
    d->m_timer = new QTimer(this);
    d->m_timer->setSingleShot(true);
    connect(d->m_timer, &QTimer::timeout, this, &NotesNetworkReceiver::slotReceptionTimeout);
    d->m_timer->start(MAXTIME);
}

NotesNetworkReceiver::~NotesNetworkReceiver()
{
    delete d;
}

void NotesNetworkReceiver::slotDataAvailable()
{
    char smallBuffer[SBSIZE];
    int smallBufferLen;

    do {
        // Append to "big buffer" only if we have some space left.
        int curLen = d->m_buffer->count();

        smallBufferLen = d->m_sock->read(smallBuffer, SBSIZE);

        // Limit max transfer over buffer, to avoid overflow.
        smallBufferLen = qMin(smallBufferLen, MAXBUFFER - curLen);

        if (smallBufferLen > 0) {
            d->m_buffer->resize(curLen + smallBufferLen);
            memcpy(d->m_buffer->data() + curLen, smallBuffer, smallBufferLen);
        }
    } while (smallBufferLen == SBSIZE);

    // If we are overflowing, close connection.
    if (d->m_buffer->count() == MAXBUFFER) {
        d->m_sock->close();
    } else {
        d->m_timer->start(MAXTIME);
    }
}

void NotesNetworkReceiver::slotReceptionTimeout()
{
    d->m_sock->close();
}

void NotesNetworkReceiver::slotConnectionClosed()
{
    QTextCodec *codec = QTextCodec::codecForLocale();

    if (d->m_timer->isActive()) {
        const QString noteText = QString(codec->toUnicode(*d->m_buffer)).trimmed();
        NoteUtils utils;
        const NoteUtils::NoteText result = utils.extractNoteText(noteText, d->m_titleAddon);
        if (!result.noteText.isEmpty()) {
            Q_EMIT sigNoteReceived(result.noteTitle, result.noteText);
        }
    }

    deleteLater();
}

void NotesNetworkReceiver::slotError(QAbstractSocket::SocketError error)
{
    qCWarning(NOTESHARED_LOG) << "error type :" << static_cast<int>(error) << " error string : " << d->m_sock->errorString();
}
