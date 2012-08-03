/*
 * Defines a SpinBox widget to be displayed in QML.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#ifndef QMLSPINBOX_H
#define QMLSPINBOX_H

#include <QGraphicsProxyWidget>
#include <QSpinBox>

class SpinBox : public QGraphicsProxyWidget
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(double actualValue READ actualValue WRITE setActualValue)
    Q_PROPERTY(double maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(double minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)
    
public:
    SpinBox(QGraphicsItem* parent = 0);
    ~SpinBox();
    
    int width() const;
    void setWidth(int);
    
    int height() const;
    void setHeight(int);
    
    double value() const;
    void setValue(double);
    
    double actualValue() const;
    void setActualValue(double);
    
    double maximum() const;
    void setMaximum(double);
    
    double minimum() const;
    void setMinimum(double);
    
    QString suffix() const;
    void setSuffix(const QString &);
    
    Q_INVOKABLE void setValueWithoutNotify(double);
    
signals:
    void valueChanged(double oldValue, double newValue);
    
    /*
     * Since these spinboxes have a very specific use in the application, emitting valueChanged means other spinboxes are
     * affected by this change. This signal is sent when we don't want other spinboxes to be influenced by this change (i.e.
     * when setValueWithoutNotify is called).
     */
    void simpleValueChanged(double newValue);
    
private:
    QDoubleSpinBox* widget;
    double m_actualValue;
    double m_currentValue;
    
private slots:
    void doValueChanged(double);
};

#endif