/*
 * Model for the TreeView: displays information about devices.

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/
#ifndef DEVICETREEMODEL_H
#define DEVICETREEMODEL_H

#include <QAbstractListModel>
#include <solid/partitioner/volumetree.h>

using namespace Solid::Partitioner;

class DeviceTreeModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    enum DeviceTreeRoles {
        DeviceDescription = Qt::UserRole + 1,
        DeviceName,
        DeviceType,
        DeviceSize,
        DeviceLabel,
        DeviceMountPoint,
        DeviceFileSystem,
        PartitionType,
    };
    
    explicit DeviceTreeModel(const VolumeTree& disk = VolumeTree(), QObject* parent = 0);
    
    VolumeTree currentDisk() const;
    void setDisk(const VolumeTree &);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &, int role = Qt::DisplayRole) const;
    
private:
    VolumeTree m_disk;
};

#endif