/*
 * Model for a flag list.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#ifndef FLAGSMODEL_H
#define FLAGSMODEL_H

#include <QAbstractListModel>

class FlagsModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    enum FlagsModelRoles {
        FlagName = Qt::UserRole + 1, /* associated name */
        FlagChecked /* whether the flag is set */
    };
    
    explicit FlagsModel(QObject* parent = 0);
    
    /* Add a new flag in the model */
    void addFlag(const QString &, bool);
    
    /* Deletes all flags in the model. This model is reused. */
    void reset();
    
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &, int role = Qt::DisplayRole) const;
    
private:
    QList< QPair<QString, bool> > m_flags;
};

#endif