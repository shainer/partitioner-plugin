/*
 * Model for the ButtonBox displaying available actions

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#ifndef BUTTONBOXMODEL_H
#define BUTTONBOXMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class ButtonBoxTuple
{
public:
    ButtonBoxTuple(const QString &, const QString &);
    
    QString buttonName() const;
    QString iconName() const;
    bool clickEnabled() const; /* whether the button is clickable */
    QString textColor() const; /* this depends on the previous property */
    
    void setClickEnabled(bool);
    
private:
    QString m_buttonName;
    QString m_iconName;
    QString m_textcolor;
    bool m_enabled;
};

class ButtonBoxModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    enum ButtonBoxRoles {
        ButtonName = Qt::UserRole + 1,
        IconName,
        ClickEnabled,
        ButtonTextColor
    };
    
    explicit ButtonBoxModel(QObject* parent = 0);
    
    void addTuple(const ButtonBoxTuple &);
    void setButtonsEnabled(const QStringList &, bool); /* change the enabled property of some buttons */
    void disableAllButtons();
    
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &, int role = Qt::DisplayRole) const;
    
private:
    QList<ButtonBoxTuple> tuples;
};

#endif