/*
    SPDX-FileCopyrightText: 2020 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "markdownview.hpp"

// Qt
#include <QScrollBar>
#include <QContextMenuEvent>
#include <QWheelEvent>


MarkdownView::MarkdownView(QTextDocument* document, QWidget* parent)
    : QTextBrowser(parent)
{
    setOpenLinks(false);

    setDocument(document);
}

bool MarkdownView::hasSelection() const
{
    return textCursor().hasSelection();
}

void MarkdownView::setScrollPosition(QPoint offset)
{
    horizontalScrollBar()->setValue(offset.x());
    verticalScrollBar()->setValue(offset.y());
}

QPoint MarkdownView::scrollPosition() const
{
    return {
        horizontalScrollBar()->value(),
        verticalScrollBar()->value()
    };
}

int MarkdownView::scrollPositionX() const
{
    return horizontalScrollBar()->value();
}

int MarkdownView::scrollPositionY() const
{
    return verticalScrollBar()->value();
}

void MarkdownView::wheelEvent(QWheelEvent* event)
{
    // QTextEdit's built-in ctrl-wheel zoom only scales the default font, leaving
    // fragments with explicit sizes (code spans/blocks) untouched. Handle zoom
    // ourselves so the part can rescale every font consistently.
    if (event->modifiers() & Qt::ControlModifier) {
        // touchpads and forwarded remote-desktop scrolls deliver deltas smaller
        // than one notch (120); accumulate so they still add up to zoom steps
        m_accumulatedWheelDelta += event->angleDelta().y();
        const int steps = m_accumulatedWheelDelta / 120;
        if (steps != 0) {
            m_accumulatedWheelDelta -= steps * 120;
            Q_EMIT zoomRequested(steps);
        }
        event->accept();
        return;
    }
    m_accumulatedWheelDelta = 0;
    QTextBrowser::wheelEvent(event);
}

void MarkdownView::contextMenuEvent(QContextMenuEvent* event)
{
    // Compare KWebKitPart's WebView::contextMenuEvent & WebEnginePart's WebEngineView::contextMenuEvent
    // for the patterns used to fill the menu.
    // QTextBrowser at of Qt 5.15 provides less data though, so for now this is reduced variant.

    // trying to get linkText skipped, because not reliable to get:
    // anchorAt uses ExactHit test, but cursorAt FuzzyHit, so this might not match

    const QUrl linkUrl(anchorAt(event->pos()));

    // only report any selection if this is not a context menu for a link
    const bool hasSelection = !linkUrl.isValid() && this->hasSelection();

    Q_EMIT contextMenuRequested(event->globalPos(),
                                linkUrl,
                                hasSelection);

    event->accept();
}

#include "moc_markdownview.cpp"
