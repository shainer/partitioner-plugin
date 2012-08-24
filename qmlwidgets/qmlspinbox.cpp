/*
 * Defines a SpinBox widget to be displayed in QML.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#include <qmlwidgets/qmlspinbox.h>
#include <QDebug>

SpinBox::SpinBox(QGraphicsItem* parent)
    : QGraphicsProxyWidget(parent)
{
    widget = new QDoubleSpinBox;
    widget->setDecimals(3);
    setWidget(widget);
    
    m_currentValue = widget->value();
    m_actualValue = widget->value();
    
    QObject::connect(widget, SIGNAL(valueChanged(double)), this, SLOT(doValueChanged(double)));
}

SpinBox::~SpinBox()
{
    widget->deleteLater();
}

int SpinBox::width() const
{
    return widget->width();
}

void SpinBox::setWidth(int width)
{
    widget->setMinimumWidth(width);
}

int SpinBox::height() const
{
    return widget->height();
}

void SpinBox::setHeight(int height)
{
    widget->setMinimumHeight(height);
}

double SpinBox::value() const
{
    return widget->value();
}

void SpinBox::setValue(double value)
{
    widget->setValue(value);
    m_actualValue += (value - m_currentValue);
    m_currentValue = value;
}

double SpinBox::actualValue() const
{
    return m_actualValue;
}

void SpinBox::setActualValue(double value)
{
    m_actualValue = value;
}

double SpinBox::maximum() const
{
    return widget->maximum();
}

void SpinBox::setMaximum(double max)
{
    widget->setMaximum(max);
}

double SpinBox::minimum() const
{
    return widget->minimum();
}

void SpinBox::setMinimum(double min)
{
    widget->setMinimum(min);
}

QString SpinBox::suffix() const
{
    return widget->suffix();
}

void SpinBox::setSuffix(const QString& suffix)
{
    widget->setSuffix(suffix);
}

void SpinBox::setValueWithoutNotify(double value)
{
    QObject::disconnect(widget, SIGNAL(valueChanged(double)), this, SLOT(doValueChanged(double)));
    
    widget->setValue(value);
    m_actualValue += (value - m_currentValue);    
    m_currentValue = value;
    emit simpleValueChanged(m_currentValue);
    
    QObject::connect(widget, SIGNAL(valueChanged(double)), this, SLOT(doValueChanged(double)));
}

void SpinBox::doValueChanged(double newValue)
{
    m_actualValue += (newValue - m_currentValue);
    
    emit valueChanged(m_currentValue, newValue);
    m_currentValue = newValue;
}

#include "qmlspinbox.moc"