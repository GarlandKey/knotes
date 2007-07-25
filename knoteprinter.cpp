#include "knoteprinter.h"

#include <kcal/journal.h>

#include <klocale.h>
#include <kprinter.h>

#include <qfont.h>
#include <q3paintdevicemetrics.h>
#include <qpainter.h>
#include <qrect.h>
#include <q3simplerichtext.h>
#include <qstring.h>

KNotePrinter::KNotePrinter() : m_styleSheet( 0 ), m_mimeSourceFactory( 0 )
{
}

void KNotePrinter::setContext( const QString& context )
{
    m_context = context;
}

QString KNotePrinter::context() const
{
    return m_context;
}

void KNotePrinter::setMimeSourceFactory( Q3MimeSourceFactory* factory )
{
    m_mimeSourceFactory = factory;
}

Q3MimeSourceFactory* KNotePrinter::mimeSourceFactory() const
{
    return m_mimeSourceFactory;
}

void KNotePrinter::setFont( const QFont& font )
{
    m_font = font;
}

QFont KNotePrinter::font() const
{
    return m_font;
}

void KNotePrinter::setColorGroup( const QColorGroup& colorGroup )
{
    m_colorGroup = colorGroup;
}

QColorGroup KNotePrinter::colorGroup() const
{
    return m_colorGroup;
}

void KNotePrinter::setStyleSheet( Q3StyleSheet* styleSheet )
{
    m_styleSheet = styleSheet;
}

Q3StyleSheet* KNotePrinter::styleSheet() const
{
    return m_styleSheet;
}

void KNotePrinter::doPrint( KPrinter& printer, QPainter& painter,
                            const QString& content ) const
{
    const int margin = 40;  // pt

    Q3PaintDeviceMetrics metrics( painter.device() );
    int marginX = margin * metrics.logicalDpiX() / 72;
    int marginY = margin * metrics.logicalDpiY() / 72;

    QRect body( marginX, marginY,
            metrics.width() - marginX * 2,
            metrics.height() - marginY * 2 );

    Q3SimpleRichText text( content, m_font, m_context,
            m_styleSheet, m_mimeSourceFactory,
            body.height() /*, linkColor, linkUnderline? */ );

    text.setWidth( &painter, body.width() );
    QRect view( body );

    int page = 1;

    for (;;)
    {
        text.draw( &painter, body.left(), body.top(), view, m_colorGroup );
        view.moveBy( 0, body.height() );
        painter.translate( 0, -body.height() );

        // page numbers
        painter.setFont( m_font );
        painter.drawText(
                view.right() - painter.fontMetrics().width( QString::number( page ) ),
                view.bottom() + painter.fontMetrics().ascent() + 5, QString::number( page )
                );

        if ( view.top() >= text.height() )
            break;

        printer.newPage();
        page++;
    }
}

void KNotePrinter::printNote( const QString& name, const QString& content ) const
{
    KPrinter printer;
    printer.setFullPage( true );

    if ( !printer.setup( 0, i18n("Print %1", name ) ) )
        return;
    QPainter painter;
    painter.begin( &printer );
    doPrint( printer, painter, content );
    painter.end();
}

void KNotePrinter::printNotes( const QList<KCal::Journal*>& journals ) const
{
    if ( journals.isEmpty() )
        return;

    KPrinter printer;
    printer.setFullPage( true );

    if ( !printer.setup( 0, i18np("Print Note", "Print %n notes", journals.count() ) ) )
        return;

    QPainter painter;
    painter.begin( &printer );
    QString content;
    QListIterator<KCal::Journal*> it( journals );
    while ( it.hasNext() ) {
        KCal::Journal *j = it.next();
        content += "<h2>" + j->summary() + "</h2>";
        content += j->description();
        if ( it.hasNext() )
            content += "<hr>";
    }
    doPrint( printer, painter, content );
    painter.end();
}


