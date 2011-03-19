/* ============================================================
* QupZilla - WebKit based browser
* Copyright (C) 2010-2011  nowrep
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#include "sourceviewersearch.h"
#include "ui_sourceviewersearch.h"
#include "sourceviewer.h"

SourceViewerSearch::SourceViewerSearch(SourceViewer* parent) :
    Notification((QWidget*)parent)
   ,m_sourceViewer(parent)
   ,ui(new Ui::SourceViewerSearch)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->closeButton->setIcon(
#ifdef Q_WS_X11
    style()->standardIcon(QStyle::SP_DialogCloseButton)
#else
    QIcon(":/icons/faenza/close.png")
#endif
    );

    ui->next->setIcon(
#ifdef Q_WS_X11
    style()->standardIcon(QStyle::SP_ArrowForward)
#else
    QIcon(":/icons/faenza/forward.png")
#endif
    );

    ui->previous->setIcon(
#ifdef Q_WS_X11
    style()->standardIcon(QStyle::SP_ArrowBack)
#else
    QIcon(":/icons/faenza/back.png")
#endif
    );
    ui->lineEdit->setFocus();
    startAnimation();
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(ui->lineEdit, SIGNAL(textEdited(QString)), this, SLOT(next()));
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(next()));
    connect(ui->next, SIGNAL(clicked()), this, SLOT(next()));
    connect(ui->previous, SIGNAL(clicked()), this, SLOT(previous()));
}

void SourceViewerSearch::activateLineEdit()
{
    ui->lineEdit->setFocus();
}

void SourceViewerSearch::next()
{
    if (!find(0)) {
        ui->lineEdit->setStyleSheet("QLineEdit {background:#ff6666;; }");
        m_sourceViewer->sourceEdit()->moveCursor(QTextCursor::Start);
    } else {
        ui->lineEdit->setStyleSheet("");
    }
}

void SourceViewerSearch::previous()
{
    if (!find(QTextDocument::FindBackward)) {
        ui->lineEdit->setStyleSheet("QLineEdit {background:#ff6666;; }");
        m_sourceViewer->sourceEdit()->moveCursor(QTextCursor::Start);
    } else {
        ui->lineEdit->setStyleSheet("");
    }
}

bool SourceViewerSearch::find(QTextDocument::FindFlags flags)
{
    QString string = ui->lineEdit->text();
    if (string.isEmpty())
        return true;
    if (string != m_lastSearchedString) {
        QTextCursor cursor = m_sourceViewer->sourceEdit()->textCursor();
        cursor.setPosition(cursor.selectionStart());
        cursor.clearSelection();
        m_sourceViewer->sourceEdit()->setTextCursor(cursor);
        m_lastSearchedString = string;
    }

    if (!m_sourceViewer->sourceEdit()->find(string, flags)) {
        QTextCursor cursor = m_sourceViewer->sourceEdit()->textCursor();
        m_sourceViewer->sourceEdit()->moveCursor( (flags == QTextDocument::FindBackward ) ? QTextCursor::End : QTextCursor::Start );
        if (!m_sourceViewer->sourceEdit()->find(string,flags)) {
            cursor.clearSelection();
            m_sourceViewer->sourceEdit()->setTextCursor(cursor);
            return false;
        }
    }
    return true;
}

