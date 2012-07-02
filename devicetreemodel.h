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
        DeviceName = Qt::UserRole + 1,
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