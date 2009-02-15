/*
 * Copyright 2008  Alex Merry <alex.merry@kdemail.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include "infopanel.h"

#include <Plasma/Label>
#include <Plasma/Theme>

#include <QGraphicsGridLayout>
#include <QLabel>

InfoPanel::InfoPanel(QGraphicsWidget *parent)
    : QGraphicsWidget(parent),
      m_artistLabel(new Plasma::Label(this)),
      m_titleLabel(new Plasma::Label(this)),
      m_albumLabel(new Plasma::Label(this)),
      m_timeLabel(new Plasma::Label(this)),
      m_artistText(new Plasma::Label(this)),
      m_titleText(new Plasma::Label(this)),
      m_albumText(new Plasma::Label(this)),
      m_timeText(new Plasma::Label(this)),
      m_layout(new QGraphicsGridLayout)
{
    m_layout->setColumnStretchFactor(0, 0);
    m_layout->setColumnSpacing(0, 10);
    m_layout->setColumnAlignment(0, Qt::AlignRight);

    m_artistLabel->setText(i18nc("For a song or other music", "Artist:"));
    m_artistLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_titleLabel->setText(i18nc("For a song or other music", "Title:"));
    m_titleLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_albumLabel->setText(i18nc("For a song or other music", "Album:"));
    m_albumLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_timeLabel->setText(i18nc("Position in a song", "Time:"));
    m_timeLabel->nativeWidget()->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // TODO: make this configurable
    m_layout->addItem(m_artistLabel, 0, 0);
    m_layout->addItem(m_artistText, 0, 1);
    m_layout->addItem(m_titleLabel, 1, 0);
    m_layout->addItem(m_titleText, 1, 1);
    m_layout->addItem(m_albumLabel, 2, 0);
    m_layout->addItem(m_albumText, 2, 1);
    m_layout->addItem(m_timeLabel, 3, 0);
    m_layout->addItem(m_timeText, 3, 1);

    setLayout(m_layout);
}

InfoPanel::~InfoPanel()
{
}

void InfoPanel::updateMetadata(const QMap<QString,QString>& metadata)
{
    m_metadata = metadata;
    updateLabels();
}

void InfoPanel::updateLabels()
{
    Plasma::Theme *theme = Plasma::Theme::defaultTheme();
    QFont font = theme->font(Plasma::Theme::DefaultFont);
    QFontMetricsF fm(font);

    m_artistText->setText(fm.elidedText(m_metadata["Artist"], Qt::ElideMiddle, m_artistText->size().width()));
    m_albumText->setText(fm.elidedText(m_metadata["Album"], Qt::ElideMiddle, m_artistText->size().width()));
    m_titleText->setText(fm.elidedText(m_metadata["Title"], Qt::ElideMiddle, m_artistText->size().width()));

    m_layout->invalidate();
}

void InfoPanel::setCurrentTime(const QString &time)
{
    m_timeText->setText(time);
}

