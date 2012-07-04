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
#include <qmlcombobox.h>
#include <pluginregister.h>
#include <buttonnames.h>

#include <solid/partitioner/actions/removepartitiontableaction.h>
#include <solid/partitioner/utils/partitioner_enums.h>
#include <solid/partitioner/actions/createpartitiontableaction.h>
#include <solid/partitioner/actions/removepartitionaction.h>
#include <solid/partitioner/actions/modifypartitionaction.h>
#include <solid/partitioner/utils/partitiontableutils.h>
#include <solid/partitioner/utils/filesystem.h>
#include <solid/partitioner/utils/partitioningerror.h>
#include <solid/partitioner/volumemanager.h>
#include <solid/partitioner/actions/formatpartitionaction.h>
#include <solid/partitioner/devices/disk.h>
#include <solid/partitioner/devices/partition.h>

#include <QDeclarativeContext>
#include <qdeclarative.h>

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
    
    /* This model is initial empty, but it's okay because it won't be displayed right now. */
    m_context->setContextProperty("flagsModel", &m_flagsModel);
    
    plugin.registerTypes("ComboBox");
    m_view.setSource(QUrl::fromLocalFile("../../install/plugin/qml/main.qml")); // TODO: change this
    m_view.setResizeMode(QDeclarativeView::SizeViewToRootObject);
    
    m_rootObject = m_view.rootObject();
    QObject* dialogSet = m_rootObject->findChild< QObject* >("dialogSet");
    m_treeView = getTreeView();
    
    /* Add all dialog objects for later use */
    m_dialogs.insert(FORMAT_PARTITION, dialogSet->findChild< QObject* >("formatDialog"));
    m_dialogs.insert(MODIFY_PARTITION, dialogSet->findChild< QObject* >("modifyDialog"));
    m_dialogs.insert(CREATE_PARTITION_TABLE, dialogSet->findChild< QObject* >("createTableDialog"));
    m_dialogs.insert(REMOVE_PARTITION_TABLE, dialogSet->findChild< QObject* >("removeTableDialog"));
    
    /* Receive changes from Solid */
    QObject::connect( m_manager, SIGNAL(deviceAdded(VolumeTree)), this, SLOT(doDeviceAdded(VolumeTree)) );
    QObject::connect( m_manager, SIGNAL(deviceRemoved(QString)), this, SLOT(doDeviceRemoved(QString)) );
    QObject::connect( m_manager, SIGNAL(diskChanged(QString)), this, SLOT(doDiskTreeChanged(QString)) );
    
    /* Receive changes from the QML interface */
    QObject::connect( m_rootObject, SIGNAL(selectedDiskChanged(QString)), this, SLOT(doSelectedDiskChanged(QString)) );
    QObject::connect( m_rootObject, SIGNAL(selectedDeviceChanged(QString)), this, SLOT(doSelectedDeviceChanged(QString)) );
    QObject::connect( m_rootObject, SIGNAL(actionButtonClicked(QString)), this, SLOT(doActionButtonClicked(QString)) );
    
    /* Operations to be performed when the dialog is closed (i.e. register the correspondent action, if necessary) */
    QObject::connect( m_dialogs[FORMAT_PARTITION], SIGNAL(closed(bool, QString, QString)), SLOT(formatDialogClosed(bool, QString, QString)) );
    QObject::connect( m_dialogs[MODIFY_PARTITION], SIGNAL(closed(bool, QString, QString)), SLOT(modifyDialogClosed(bool, QString, QString)) );
    QObject::connect( m_dialogs[CREATE_PARTITION_TABLE], SIGNAL(closed(bool, QString, QString)), SLOT(createTableDialogClosed(bool, QString, QString)));
    QObject::connect( m_dialogs[REMOVE_PARTITION_TABLE], SIGNAL(closed(bool, QString)), SLOT(removeTableDialogClosed(bool, QString)));
    
    m_view.show();
}

PartitionerView::~PartitionerView()
{
    m_context->deleteLater();
}

/* I'm deeply sorry for this. */
QObject* PartitionerView::getTreeView()
{
    QObject* column = m_rootObject->findChild< QObject* >("mainColumn");
    QObject* row = column->findChild< QObject* >("mainRow");
    QObject* deviceTree = row->findChild< QObject* >("deviceTree");
    return deviceTree->findChild< QObject* >("deviceTreeView");
}

/* This sets the information needed to display the button box on top. By default, all buttons are clickable. */
void PartitionerView::setButtonBox()
{
    m_boxmodel.addTuple( ButtonBoxTuple(CREATE_PARTITION, "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple(REMOVE_PARTITION, "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple(RESIZE_PARTITION, "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple(FORMAT_PARTITION, "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple(MODIFY_PARTITION, "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple(CREATE_PARTITION_TABLE, "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple(REMOVE_PARTITION_TABLE, "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple(UNDO, "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple(REDO, "icon.gif") );
    m_boxmodel.addTuple( ButtonBoxTuple(APPLY, "icon.gif") );
    
    m_context->setContextProperty("buttonBoxModel", &m_boxmodel);
}

/* This sets the enabled state of all non action-related buttons: undo, redo and apply */
void PartitionerView::setGenericButtonsState()
{
    QStringList disabled, enabled;
    
    if (m_manager->isUndoPossible()) {
        enabled << UNDO;
    } else {
        disabled << UNDO;
    }
    
    if (m_manager->isRedoPossible()) {
        enabled << REDO;
    } else {
        disabled << REDO;
    }
    
    /* Enable the button if there is at least one registered action to execute */
    if (!m_manager->registeredActions().isEmpty()) {
        enabled << APPLY;
    } else {
        disabled << APPLY;
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
    m_currentDevice = devName;
    
    QStringList enabled, disabled;
    
    /* Disable/enable some action buttons according to the device type (and what you can do on it) */
    switch (device->deviceType()) {
        case DeviceModified::DiskDevice: {
            disabled << CREATE_PARTITION << REMOVE_PARTITION << RESIZE_PARTITION << FORMAT_PARTITION << MODIFY_PARTITION;
            enabled << CREATE_PARTITION_TABLE << REMOVE_PARTITION_TABLE;
            break;
        }
        
        case DeviceModified::PartitionDevice: {
            disabled << CREATE_PARTITION << CREATE_PARTITION_TABLE << REMOVE_PARTITION_TABLE;
            enabled << REMOVE_PARTITION << RESIZE_PARTITION << FORMAT_PARTITION << MODIFY_PARTITION;
            break;
        }
        
        case DeviceModified::FreeSpaceDevice: {
            disabled << REMOVE_PARTITION << RESIZE_PARTITION << FORMAT_PARTITION << CREATE_PARTITION_TABLE
                     << REMOVE_PARTITION_TABLE << MODIFY_PARTITION;
            enabled << CREATE_PARTITION;
            break;
        }
    }
    
    m_boxmodel.setButtonsEnabled(enabled, true);
    m_boxmodel.setButtonsEnabled(disabled, false);
}

/* When an action button is clicked, shows the right dialog and sets its initial information when required */
void PartitionerView::doActionButtonClicked(QString actionName)
{
    m_boxmodel.disableAllButtons();
    
    QObject* dialog = m_dialogs[actionName];
    VolumeTree diskTree = m_manager->diskTree(m_currentDisk);
    DeviceModified* device = diskTree.searchDevice(m_currentDevice);
    
    m_flagsModel.reset();

    if (actionName == MODIFY_PARTITION) {
        Partition* partition = dynamic_cast< Partition* >(device);
        dialog->setProperty("currentLabel", partition->label());
        
        QStringList partitionFlags = partition->flags();
        QStringList schemeFlags = PartitionTableUtils::instance()->supportedFlags( partition->partitionTableScheme() );
        
        foreach (const QString& schemeFlag, schemeFlags) {
            m_flagsModel.addFlag( schemeFlag, partitionFlags.contains(schemeFlag) );
        }
    }
    else if (actionName == REMOVE_PARTITION) { /* this doesn't need a dialog, so we directly call the "dialog closed slot" */
        removePartitionDialogClosed(m_currentDevice);
        return;
    }
    else if (actionName == CREATE_PARTITION_TABLE) {
        dialog->setProperty("currentScheme", diskTree.disk()->partitionTableScheme());
    }
    else if (actionName == FORMAT_PARTITION) { /* FIXME: do this properly */
        dialog->setProperty("supportedFilesystems", QStringList() << "ntfs" << "vfat" << "unformatted");
    }
    
    QMetaObject::invokeMethod(dialog, "show", Qt::QueuedConnection, Q_ARG(QVariant, m_currentDevice));
}

void PartitionerView::formatDialogClosed(bool accepted, QString filesystem, QString partition)
{
    if (!accepted) {
        return;
    }
    
    QStringList flags;
    QObject* flagsList = m_dialogs[FORMAT_PARTITION]->findChild< QObject* >("flagsList");
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
    afterClosedDialog();
}

void PartitionerView::modifyDialogClosed(bool accepted, QString label, QString partition)
{
    if (!accepted) {
        return;
    }
    
    QStringList flags;
    QObject* flagsList = m_dialogs[MODIFY_PARTITION]->findChild< QObject* >("flagsList");
    QList< QObject* > rowLayout = flagsList->findChildren< QObject* >("row");
    
    foreach (QObject* row, rowLayout) {
        QObject* checkbox = row->findChild< QObject* >("flagCheckBox");
        QObject* flag = row->findChild< QObject* >("flagName");
        
        if (checkbox->property("checked").toBool()) {
            flags.append( flag->property("text").toString() );
        }
    }
    
    m_manager->registerAction( new Actions::ModifyPartitionAction(partition, label, flags) );
    afterClosedDialog();
}

void PartitionerView::removePartitionDialogClosed(QString partition)
{
    m_manager->registerAction( new Actions::RemovePartitionAction(partition) );
    afterClosedDialog();
}

void PartitionerView::createTableDialogClosed(bool accepted, QString scheme, QString disk)
{
    if (!accepted || scheme.isEmpty()) {
        return;
    }
    
    PartitionTableScheme schemeEnum = (scheme == "gpt") ? GPTScheme : MBRScheme;
    m_manager->registerAction( new Actions::CreatePartitionTableAction(disk, schemeEnum) );
    afterClosedDialog();
}

void PartitionerView::removeTableDialogClosed(bool accepted, QString disk)
{
    if (!accepted) {
        return;
    }
    
    m_manager->registerAction( new Actions::RemovePartitionTableAction(disk) );
    afterClosedDialog();
}

void PartitionerView::afterClosedDialog()
{
    m_treeView->setProperty("currentIndex", 0);
    
    setActionList(); /* change the list of registered actions in the GUI (if the previous method was successful) */
    setGenericButtonsState(); /* some non-action related buttons are affected by how many actions we registered */
}

#include "partitionerview.moc"
