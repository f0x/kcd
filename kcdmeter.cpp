/*
 * Copyright (C) 2008-2009 Giulio Camuffo from playwolf
 * Copyright 2009  Francesco Grieco <fgrieco@gmail.com>
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

#include "kcdmeter.h"

#include <QGraphicsSceneMouseEvent>

KcdMeter::KcdMeter(QGraphicsWidget *parent)
    : Plasma::Meter(parent)
{
    //label = new WolfLabel(this);
    //label->setResizeFont(true,1);
    //resize(100,20);
    setMaximumHeight(25);
}

KcdMeter::~KcdMeter() {
}

void KcdMeter::setPosition(const int pos, const int total)
{
   setMaximum(total);
   setValue(pos);
   if (total != 0) {
       qreal t = total;
       int n = (pos/t)*100;
       //label->setText(QVariant(n).toString() + "%");
       //updateLabel(size());
   }
}

void KcdMeter::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   qreal margin = (0.001338*contentsRect().height() + 0.0002475)*contentsRect().width();
   if (event->buttons() == Qt::LeftButton) {
       int value = 0;
       if (meterType() == BarMeterHorizontal) {
           qreal x = event->pos().x();
           qreal width = contentsRect().width();
           qreal num = ( x*2*margin - width*margin )/(width - 2*margin);
           value = (((x + num)/(width))*maximum());
       }
       else if (meterType() == BarMeterVertical) {
           qreal y = event->pos().y();
           qreal height = contentsRect().height();
           qreal num = ( y*2*margin - height*margin )/(height - 2*margin);
           value = maximum() - (((y + num)/height)*maximum());
       }
       setValue(value);
       emit valueChanged(value);
   }
}

void KcdMeter::resizeEvent(QGraphicsSceneResizeEvent *event) {
    Plasma::Meter::resizeEvent(event);

    //updateLabel(event->newSize());
}

void KcdMeter::updateLabel(QSizeF size) 
{
//    if (meterType() == BarMeterHorizontal) {
//        if (size.height() > 7) {
//            label->show();
//            label->resize(size.width(),label->labelSize().height());
//            qreal y = (size.height() - label->size().height())/2.0;
//            label->setPos(0,y);
//         }
//         else
//         label->hide();
//    }
//    else if (meterType() == BarMeterVertical) {
//        if (size.width() > 7) {
//            label->show();
//            label->resize(size.width(),label->labelSize().height());
//            qreal y = (size.height() - label->size().height())/2.0;
//            label->setPos(0,y);
//    }
//    else 
//        label->hide();
//    }
}

#include "kcdmeter.moc"
