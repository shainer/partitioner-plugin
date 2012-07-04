/*
 * Main class that displays and handles events from QML GUI

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#include <partitionerview.h>
#include <devicetreemodel.h>
#include <qdeclarative.h>
#include <qmlcombobox.h>
#include "pluginregister.h"

#include <solid/partitioner/utils/filesystem.h>
#include <solid/partitioner/utils/partitioningerror.h>
#include <solid/partitioner/volumemanager.h>
#include <solid/partitioner/actions/formatpartitionaction.h>
#include <solid/partitioner/devices/disk.h>

#include <QDeclarativeContext>

using namespace Solid::Partitioner;
using namespace Solid::Partitioner::Actions;
using namespace Solid::Partitioner::Utils;
using namespace Solid::Partitioner::Devices;

PartitionerView::PartitionerView(QObject* parent)
    : QObject(parent)
    , m_context( m_view.rootContext() )
    , m_manager( VolumeManager::instance() )
{
    /* Sets all the models relative to the QML views */
    setButtonBox();
    setGenericButtonsState();
    setDiskList();
    setActionList();
    setDiskTree( m_diskList.first() ); /* the disk initially displayed is the first in the list */
    
    plugin.registerTypes("ComboBox");
    m_view.setSource(QUrl::fromLocalFile("../../install/plugin/qml/main.qml")); // TODO: change this
    m_view.setResizeMode(QDeclarativeView::SizeViewToRootObject);
    
    m_rootObject = m_view.rootObject();
    m_formatDialog = m_rootObject->findChild< QObject* >("formatDialog");
    
    QObject::connect( m_manager, SIGNAL(deviceAdded(VolumeTree)), this, SLOT(doDeviceAdded(VolumeTree)) );
    QObject::connect( m_manager, SIGNAL(deviceRemoved(QString)), this, SLOT(doDeviceRemoved(QString)) );
    QObject::connect( m_manager, SIGNAL(diskChanged(QString)), this, SLOT(doDiskTreeChanged(QString)) );
    
    QObject::connect( m_rootObject, SIGNAL(selectedDiskChanged(QString)), this, SLOT(doSelectedDiskChanged(QString)) );
    QObject::connect( m_rootObject, SIGNAL(selectedDeviceChanged(QString)), this, SLOT(doSelectedDeviceChanged(QString)) );
    QObject::connect( m_formatDialog,
                      SIGNAL(closed(bool, QString, QString)),
                      this,
                      SLOT(formatDialogClosed(bool, QString, QString)) );
    
    m_view.show();
}

PartitionerView::~PartitionerView()
{
    m_context->deleteLater();
}

/* This sets the information needed to display the button box on top. By default, all buttons are clickable. */
void PartitionerView::setButtonBox()
{
    m_boxmodel.addTuple( ButtonBoxTuple("Create partition", "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple("Remove partition", "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple("Resize/move partition", "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple("Format partition", "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple("Modify partition", "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple("Create partition table", "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple("Remove partition table", "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple("Undo", "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple("Redo", "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple("Apply actions", "icon.gif") );
    
    m_context->setContextProperty("buttonBoxModel", &m_boxmodel);
}

/* This sets the enabled state of all non action-related buttons: undo, redo and apply */
void PartitionerView::setGenericButtonsState()
{
    QStringList disabled, enabled;
    
    if (m_manager->isUndoPossible()) {
        enabled << "Undo";
    } else {
        disabled << "Undo";
    }
    
    if (m_manager->isRedoPossible()) {
        enabled << "Redo";
    } else {
        disabled << "Redo";
    }
    
    /* Enable the button if there is at least one registered action to execute */
    if (!m_manager->registeredActions().isEmpty()) {
        enabled << "Apply actions";
    } else {
        disabled << "Apply actions";
    }
    
    m_boxmodel.setButtonsEnabled(enabled, true);
    m_boxmodel.setButtonsEnabled(disabled, false);
}

/* This sets the model for the list of registered actions */
void PartitionerView::setActionList()
{
    QStringList actionDescriptions;
    
    foreach (Action* action, VolumeManager::instance()->registeredActions()) {
        actionDescriptions << action->description();
    }
    
    m_context->setContextProperty("actionModel", QVariant::fromValue(actionDescriptions));
}

/* This sets the model for the view that displays the currently available disks */
void PartitionerView::setDiskList()
{
    m_diskList = VolumeManager::instance()->allDiskTrees().keys();
    m_context->setContextProperty("diskModel", QVariant::fromValue(m_diskList));
}

/* This sets the model for the TreeView, displaying the layout of the specifying disk */
void PartitionerView::setDiskTree(const QString& diskName)
{
    VolumeTree diskTree = m_manager->diskTree( diskName );
    m_treeModel.setDisk(diskTree);

    /* Sets the disk we just displayed as the selected device, as the user didn't have the chance to change it yet */
    m_currentDisk = diskTree.disk()->name();
    doSelectedDeviceChanged(m_currentDisk);
    
    m_context->setContextProperty("deviceTreeModel", &m_treeModel);
}

/* When a new device is added to the system, refreshes the disk list in case it was a disk */
void PartitionerView::doDeviceAdded(VolumeTree newTree)
{
    Q_UNUSED(newTree)
    setDiskList();
}

/*
 * When a device is removed from the system, refreshes the disk list in case it was a disk; furthermore, registered actions
 * that were relative to the removed device are deleted, so we update the action list too.
 * 
 * If the user removed the disk that was currently displayed in the TreeView, sets a new tree for displaying.
 */
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

/*
 * This is called when a disk layout changes for whatever reason: updates the model.
 * 
 * NOTE: the correspondent signal is sent when a registered action changed the disk's layout;
 * given that to register an action on a device you must select the device, the modified tree is always
 * the one currently displayed in the TreeView. So we update that one.
 */
void PartitionerView::doDiskTreeChanged(QString newTree)
{
    setDiskTree(newTree);
}

/* If the user selected a new disk for displaying, changes the TreeView data. */
void PartitionerView::doSelectedDiskChanged(QString newDisk)
{
    m_currentDisk = newDisk;
    setDiskTree(newDisk);
}

/* This is called when the selected device changes, or when a new disk tree is being displayed */
void PartitionerView::doSelectedDeviceChanged(QString devName)
{
    VolumeTree diskTree = m_manager->diskTree(m_currentDisk);
    DeviceModified* device = diskTree.searchDevice(devName);
    
    QStringList enabled, disabled;
    
    /* Disable/enable some action buttons according to the device type (and what you can do on it) */
    switch (device->deviceType()) {
        case DeviceModified::DiskDevice: {
            disabled << "Create partition" << "Remove partition" << "Resize/move partition" << "Format partition" << "Modify partition";
            enabled << "Create partition table" << "Remove partition table";
            break;
        }
        
        case DeviceModified::PartitionDevice: {
            disabled << "Create partition" << "Create partition table" << "Remove partition table";
            enabled << "Remove partition" << "Resize/move partition" << "Format partition" << "Modify partition";
            break;
        }
        
        case DeviceModified::FreeSpaceDevice: {
            disabled << "Remove partition" << "Resize/move partition" << "Format partition" << "Create partition table"
                     << "Remove partition table" << "Modify partition";
            enabled << "Create partition";
            break;
        }
    }
    
    m_boxmodel.setButtonsEnabled(enabled, true);
    m_boxmodel.setButtonsEnabled(disabled, false);
}

void PartitionerView::formatDialogClosed(bool accepted, QString filesystem, QString partition)
{
    if (!accepted) {
        return;
    }
    
    QStringList flags;
    QObject* flagsList = m_formatDialog->findChild< QObject* >("flagsList");
    QList< QObject* > rowLayout = flagsList->findChildren< QObject* >("row");
    
    foreach (QObject* row, rowLayout) {
        QObject* checkbox = row->findChild< QObject* >("flagCheckBox");
        QObject* flag = row->findChild< QObject* >("flagName");
        
        if (checkbox->property("checked").toBool()) {
            flags.append( flag->property("text").toString() );
        }
    }
    
    Filesystem fs(filesystem, flags);
    m_manager->registerAction( new Actions::FormatPartitionAction(partition, fs) );
    
    setActionList(); /* change the list of registered actions in the GUI (if the previous method was successful) */
    setGenericButtonsState(); /* some non-action related buttons are affected by how many actions we registered */
}

#include "partitionerview.moc"
