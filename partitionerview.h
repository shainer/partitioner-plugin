#ifndef PARTITIONERVIEW_H
#define PARTITIONERVIEW_H

#include <QObject>
#include <QDeclarativeView>
#include <QDeclarativeContext>

#include <buttonboxmodel.h>
#include <devicetreemodel.h>

#include <solid/partitioner/volumetree.h>
#include <solid/partitioner/volumemanager.h>

using namespace Solid::Partitioner;

class PartitionerView : public QObject
{
    Q_OBJECT
    
public:
    explicit PartitionerView(QObject* parent = 0);
    virtual ~PartitionerView();
    
public slots:
    void doDeviceAdded(VolumeTree);
    void doDeviceRemoved(QString);
    void doDiskTreeChanged(VolumeTree);
    
    void doSelectedDiskChanged(QString);
    void doActionButtonClicked(QString);
    
private:
    QDeclarativeView m_view;
    QDeclarativeContext* m_context;
    QObject* m_rootObject;
    
    ButtonBoxModel m_boxmodel;
    DeviceTreeModel m_treeModel;
    
    VolumeManager* m_manager;
    QStringList m_diskList;
    
    void setButtonBox();
    void setDiskList();
    void setActionList();
    void setDiskTree(const QString &);
};

#endif