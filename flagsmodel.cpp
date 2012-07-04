/*
 * Model for a flag list

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#include <flagsmodel.h>

FlagsModel::FlagsModel(QObject* parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    
    roles[FlagName] = "flagName";
    roles[FlagChecked] = "flagChecked";
    
    setRoleNames(roles);
}

void FlagsModel::addFlag(const QString& name, bool checked)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    QPair< QString, bool > pair = qMakePair< QString, bool >(name, checked);
    m_flags.append( pair );
    endInsertRows();
}

void FlagsModel::reset()
{
    emit beginResetModel();
    m_flags.clear();
    emit endResetModel();
}

int FlagsModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_flags.size();
}

QVariant FlagsModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() > rowCount()) {
        return QVariant();
    }
    
    QPair< QString, bool > flag = m_flags[ index.row() ];
    
    switch (role) {
        case FlagName:
            return flag.first;
            
        case FlagChecked:
            return flag.second;
    }
    
    return QVariant();
}
