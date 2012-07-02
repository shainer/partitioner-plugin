#include <partitionerview.h>
#include <devicetreemodel.h>
#include <solid/partitioner/volumemanager.h>
#include <solid/partitioner/actions/formatpartitionaction.h>
#include <QGraphicsObject>
#include <QDeclarativeContext>

using namespace Solid::Partitioner;
using namespace Solid::Partitioner::Actions;

PartitionerView::PartitionerView(QObject* parent)
    : QObject(parent)
    , m_context( m_view.rootContext() )
    , m_manager( VolumeManager::instance() )
{    
    setButtonBox();
    setDiskList();
    setActionList();
    setDiskTree( m_diskList.last() );
    
    QObject::connect( m_manager, SIGNAL(deviceAdded(VolumeTree)), this, SLOT(doDeviceAdded(VolumeTree)) );
    QObject::connect( m_manager, SIGNAL(deviceRemoved(QString)), this, SLOT(doDeviceRemoved(QString)) );
    
    m_view.setSource(QUrl::fromLocalFile("../../install/plugin/qml/main.qml")); // TODO: change this
    m_view.show();
}

PartitionerView::~PartitionerView()
{}

void PartitionerView::setButtonBox()
{
    m_boxmodel.addTuple(ButtonBoxTuple("Create partition", "icon.gif", "createPartition"));
    m_boxmodel.addTuple(ButtonBoxTuple("Remove partition", "icon.gif", "createPartition"));
    m_boxmodel.addTuple(ButtonBoxTuple("Resize/move partition", "icon.gif", "createPartition"));
    m_boxmodel.addTuple(ButtonBoxTuple("Format partition", "icon.gif", "formatPartition"));
    m_boxmodel.addTuple(ButtonBoxTuple("Create partition table", "icon.gif", "createPartition"));
    m_boxmodel.addTuple(ButtonBoxTuple("Remove partition table", "icon.gif", "createPartition"));    
    
    m_context->setContextProperty("buttonBoxModel", &m_boxmodel);
}

void PartitionerView::setActionList()
{
    QStringList actionDescriptions;
    
    foreach (Action* action, VolumeManager::instance()->registeredActions()) {
        actionDescriptions << action->description();
    }
    
    m_context->setContextProperty("actionModel", QVariant::fromValue(actionDescriptions));
}

void PartitionerView::setDiskList()
{
    m_diskList = VolumeManager::instance()->allDiskTrees().keys();
    m_context->setContextProperty("diskModel", QVariant::fromValue(m_diskList));
}

void PartitionerView::setDiskTree(const QString& diskName)
{
    VolumeTree diskTree = m_manager->diskTree( diskName );
    m_treeModel.setDisk(diskTree);
    
    m_context->setContextProperty("deviceTreeModel", &m_treeModel);
}

void PartitionerView::doDeviceAdded(VolumeTree newTree)
{
    Q_UNUSED(newTree)
    setDiskList();
}

void PartitionerView::doDeviceRemoved(QString device)
{
    setDiskList();
    setActionList();
    
    VolumeTree currentTree = m_treeModel.currentDisk();
    Disk* currentDisk = currentTree.disk();
    
    if (currentDisk->name() == device) {
        setDiskTree( m_diskList.last() );
    }
}

void PartitionerView::doDiskTreeChanged(VolumeTree newTree)
{
    m_treeModel.setDisk(newTree);
}