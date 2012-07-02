#include "devicetreemodel.h"
#include <kio/global.h>

using namespace Solid::Partitioner::Devices;
using namespace Solid::Partitioner::Utils;

DeviceTreeModel::DeviceTreeModel(const VolumeTree& disk, QObject* parent)
    : QAbstractListModel(parent)
    , m_disk(disk)
{
    QHash<int, QByteArray> roles;
    
    roles[DeviceName] = "deviceName";
    roles[DeviceType] = "deviceType";
    roles[DeviceSize] = "deviceSize";
    roles[DeviceLabel] = "deviceLabel";
    roles[DeviceMountPoint] = "deviceMountPoint";
    roles[DeviceFileSystem] = "deviceFileSystem";
    roles[PartitionType] = "partitionType";
    
    setRoleNames(roles);
}

VolumeTree DeviceTreeModel::currentDisk() const
{
    return m_disk;
}

void DeviceTreeModel::setDisk(const VolumeTree& disk)
{
    m_disk = disk;
}

int DeviceTreeModel::rowCount(const QModelIndex& parent) const
{
    return m_disk.allDevices().size();
}

QVariant DeviceTreeModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() > rowCount()) {
        return QVariant();
    }
    
    DeviceModified* device = m_disk.allDevices().at( index.row() );
    
    switch (role) {
        case DeviceName:
            return device->description();
            
        case DeviceType:
            return device->deviceType();
            
        case DeviceSize:
            return KIO::convertSize( (KIO::filesize_t)device->size() );
            
        case DeviceLabel: {
            if (device->deviceType() == DeviceModified::PartitionDevice) {
                Partition* partition = dynamic_cast< Partition* >(device);
                return partition->label();
            }
            
            return QString();
        }
        
        case DeviceMountPoint: {
            if (device->deviceType() == DeviceModified::PartitionDevice) {
                Partition* partition = dynamic_cast< Partition* >(device);
                return partition->mountFile();
            }
            
            return QString();
        }
        
        case DeviceFileSystem: {
            if (device->deviceType() == DeviceModified::PartitionDevice) {
                Partition* partition = dynamic_cast< Partition* >(device);
                
                if (partition->partitionType() == ExtendedPartition) {
                    return "extended";
                }
                
                return partition->filesystem().name();
            }
            
            return QString();
        }
        
        case PartitionType: {
            if (device->deviceType() == DeviceModified::PartitionDevice) {
                Partition* partition = dynamic_cast< Partition* >(device);
                return partition->partitionType();
            }
            
            return Utils::UnusedPartition;
        }
    }
    
    return QVariant();
}