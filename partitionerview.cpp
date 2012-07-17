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
#include <qmlkdeicon.h>
#include <executerthread.h>

#include <solid/partitioner/utils/filesystemutils.h>
#include <solid/partitioner/actions/resizepartitionaction.h>
#include <solid/partitioner/actions/createpartitionaction.h>
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
#include <QApplication>
#include <QDesktopWidget>
#include <unistd.h>

using namespace Solid::Partitioner;
using namespace Solid::Partitioner::Actions;
using namespace Solid::Partitioner::Utils;
using namespace Solid::Partitioner::Devices;

PartitionerView::PartitionerView(const QString& selectedDevice, QObject* parent)
    : QObject(parent)
    , m_context( m_view.rootContext() )
    , m_manager( VolumeManager::instance() )
    , isDialogOpen(false)
{
    /* Registers our custom plugin to give QML access to some particular objects and widgets we created in C++. */
    plugin.registerTypes("ApplicationWidgets");
    m_view.setSource(QUrl::fromLocalFile("/etc/qml-plugin/main.qml"));
    m_view.setResizeMode(QDeclarativeView::SizeViewToRootObject);
    
    m_rootObject = m_view.rootObject();
    QObject* dialogSet = m_rootObject->findChild< QObject* >("dialogSet");
    m_treeView = getTreeView();
    m_diskView = getDiskView();
    setWindowSize();
    
    /* Sets all the models and their initial state for the QML view */
    setButtonBox();
    setGenericButtonsState();
    setDiskList();
    setActionList();
    setInitialSelection(selectedDevice);
    setIconDatabase();

    /* These models are initially empty, but it's okay because they won't be displayed right now. */
    m_context->setContextProperty("flagsModel", &m_flagsModel);
    m_context->setContextProperty("registeredActions", QVariant::fromValue( QStringList() ));
    
    /* Add all dialog objects for later use */
    m_dialogs.insert(FORMAT_PARTITION, dialogSet->findChild< QObject* >("formatDialog"));
    m_dialogs.insert(MODIFY_PARTITION, dialogSet->findChild< QObject* >("modifyDialog"));
    m_dialogs.insert(CREATE_PARTITION_TABLE, dialogSet->findChild< QObject* >("createTableDialog"));
    m_dialogs.insert(REMOVE_PARTITION_TABLE, dialogSet->findChild< QObject* >("removeTableDialog"));
    m_dialogs.insert(CREATE_PARTITION, dialogSet->findChild< QObject* >("createPartitionDialog"));
    m_dialogs.insert(RESIZE_PARTITION, dialogSet->findChild< QObject* >("resizePartitionDialog"));
    m_dialogs.insert(APPLY, dialogSet->findChild< QObject* >("applyConfirmationDialog"));
    m_dialogs.insert("applyDialog", dialogSet->findChild< QObject* >("applyDialog"));
    m_dialogs.insert("error", dialogSet->findChild< QObject* >("errorDialog"));
    
    /* Receive changes from Solid */
    QObject::connect( m_manager, SIGNAL(deviceAdded(VolumeTree)), this, SLOT(doDeviceAdded(VolumeTree)) );
    QObject::connect( m_manager, SIGNAL(deviceRemoved(QString)), this, SLOT(doDeviceRemoved(QString)) );
    QObject::connect( m_manager, SIGNAL(diskChanged(QString)), this, SLOT(doDiskTreeChanged(QString)) );
    QObject::connect( m_manager, SIGNAL(accessibilityChanged(bool, const QString &)), this, SLOT(reportAccessibility(bool, const QString &)) );
    
    /* Receive changes from the QML interface */
    QObject::connect( m_rootObject, SIGNAL(selectedDiskChanged(QString)), this, SLOT(doSelectedDiskChanged(QString)) );
    QObject::connect( m_rootObject, SIGNAL(selectedDeviceChanged(QString)), this, SLOT(doSelectedDeviceChanged(QString)) );
    QObject::connect( m_rootObject, SIGNAL(actionButtonClicked(QString)), this, SLOT(doActionButtonClicked(QString)) );
    
    /* Operations to be performed when the dialog is closed (i.e. register the correspondent action, if necessary) */
    QObject::connect( m_dialogs[FORMAT_PARTITION],
                      SIGNAL(closed(bool, QString, QString, QString, QString, QString)),
                      SLOT(formatDialogClosed(bool, QString, QString, QString, QString, QString)) );
    QObject::connect( m_dialogs[MODIFY_PARTITION], SIGNAL(closed(bool, QString, QString)), SLOT(modifyDialogClosed(bool, QString, QString)) );
    QObject::connect( m_dialogs[CREATE_PARTITION_TABLE], SIGNAL(closed(bool, QString, QString)), SLOT(createTableDialogClosed(bool, QString, QString)));
    QObject::connect( m_dialogs[REMOVE_PARTITION_TABLE], SIGNAL(closed(bool, QString)), SLOT(removeTableDialogClosed(bool, QString)));
    QObject::connect( m_dialogs[CREATE_PARTITION],
                      SIGNAL(closed(bool, qreal, qreal, QString, QString, QString, QString, QString)),
                      SLOT(createPartitionDialogClosed(bool, qreal, qreal, QString, QString, QString, QString, QString)));
    QObject::connect( m_dialogs[RESIZE_PARTITION], SIGNAL(closed(bool, qreal, qreal, QString)), SLOT(resizeDialogClosed(bool, qreal, qreal, QString)));
    QObject::connect( m_dialogs[APPLY], SIGNAL(closed(bool)), SLOT(applyActions(bool)));
    QObject::connect( m_dialogs["error"], SIGNAL(closed()), SLOT(afterOkClicked()));
    QObject::connect( m_dialogs["applyDialog"], SIGNAL(closed()), SLOT(applyDialogClosed()));
    
    /* Finally, the GUI is opened! */
    m_view.show();
}

PartitionerView::~PartitionerView()
{
    m_context->deleteLater();
}

void PartitionerView::setWindowSize()
{
    int width = QApplication::desktop()->width();
    int height = QApplication::desktop()->height();

    /* The window size will be the same as the screen size */
    m_rootObject->setProperty("width", width);
    m_rootObject->setProperty("height", height);
}

/* I'm deeply sorry for this. */
QObject* PartitionerView::getTreeView()
{
    QObject* column = m_rootObject->findChild< QObject* >("mainColumn");
    QObject* row = column->findChild< QObject* >("mainRow");
    QObject* deviceTree = row->findChild< QObject* >("deviceTree");
    return deviceTree->findChild< QObject* >("deviceTreeView");
}

QObject* PartitionerView::getDiskView()
{
    QObject* column = m_rootObject->findChild< QObject* >("mainColumn");
    QObject* row = column->findChild< QObject* >("mainRow");
    QObject* diskList = row->findChild< QObject* >("diskList");
    return diskList->findChild< QObject* >("diskListView");
}

/*
 * When called from a Dolphin plugin, the selected device automatically becomes the selected device on our view.
 * This has to be done "manually" setting the currentIndex property.
 */
void PartitionerView::setInitialSelection(const QString& selectedDevice)
{
    QString diskName = findDiskWithDevice(selectedDevice);
    QList<VolumeTree> trees = m_manager->allDiskTrees().values();
    int diskIndex = 0, deviceIndex = 0;
    
    /* Finds the index of the disk the device is located into */
    foreach (VolumeTree tree, trees) {
        if (tree.disk()->name() == diskName) {
            break;
        }
        
        diskIndex++;
    }
    
    /* Finds the index of the device inside the disk */
    foreach (DeviceModified* diskDevice, m_manager->diskTree(diskName).allDevices(false)) {
        if (diskDevice->name() == selectedDevice) {
            break;
        }
        
        deviceIndex++;
    }

    setDiskTree(diskName);
    m_diskView->setProperty("currentIndex", diskIndex);
    doSelectedDeviceChanged(selectedDevice);
    m_treeView->setProperty("currentIndex", deviceIndex);
}

/* Utility function which returns the disk name containing a given device */
QString PartitionerView::findDiskWithDevice(const QString& devName)
{
    QMap< QString, VolumeTree > trees = m_manager->allDiskTrees();
    
    foreach (const QString& key, trees.keys()) {
        if (trees[key].searchDevice(devName)) {
            return key;
        }
    }
    
    return QString();
}

/* Sets the information needed to display the button box on top. By default, all buttons are clickable. */
void PartitionerView::setButtonBox()
{
    m_boxmodel.addTuple( ButtonBoxTuple(CREATE_PARTITION, "list-add") );
    m_boxmodel.addTuple( ButtonBoxTuple(REMOVE_PARTITION, "list-remove") );
    m_boxmodel.addTuple( ButtonBoxTuple(RESIZE_PARTITION, "edit-cut") );
    m_boxmodel.addTuple( ButtonBoxTuple(FORMAT_PARTITION, "edit-paste") );
    m_boxmodel.addTuple( ButtonBoxTuple(MODIFY_PARTITION, "document-properties") );
    m_boxmodel.addTuple( ButtonBoxTuple(CREATE_PARTITION_TABLE, "document-new") );
    m_boxmodel.addTuple( ButtonBoxTuple(REMOVE_PARTITION_TABLE, "edit-delete") );
    m_boxmodel.addTuple( ButtonBoxTuple(UNDO, "edit-undo") );
    m_boxmodel.addTuple( ButtonBoxTuple(REDO, "edit-redo") );
    m_boxmodel.addTuple( ButtonBoxTuple(APPLY, "system-run") );
    
    m_context->setContextProperty("buttonBoxModel", &m_boxmodel);
}

/* Enables all the non action-related buttons: undo, redo and apply */
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

/* Sets the model for the list of registered actions */
void PartitionerView::setActionList()
{
    QStringList actionDescriptions;
    
    foreach (Action* action, m_manager->registeredActions()) {
        actionDescriptions << action->description();
    }
    
    m_context->setContextProperty("actionModel", QVariant::fromValue(actionDescriptions));
}

/* Sets the model for the view that displays the currently available disks */
void PartitionerView::setDiskList()
{
    m_diskList = VolumeManager::instance()->allDiskTrees().keys();
    m_context->setContextProperty("diskModel", QVariant::fromValue(m_diskList));
}

/* Sets the model for the TreeView, displaying the layout of the specifying disk */
void PartitionerView::setDiskTree(const QString& diskName)
{
    VolumeTree diskTree = m_manager->diskTree( diskName );
    m_treeModel.setDisk(diskTree);

    /* Sets the disk we just displayed as the selected device, as the user didn't have the chance to change it yet */
    m_currentDisk = diskTree.disk()->name();
    doSelectedDeviceChanged(m_currentDisk);
    
    m_context->setContextProperty("deviceTreeModel", &m_treeModel);
}

void PartitionerView::reportAccessibility(bool a, const QString& udi)
{
    Q_UNUSED(a)
    Q_UNUSED(udi)
    m_treeModel.readDataAgain(); /* simply resets the model */
}

/* When a new device is added to the system, refreshes the disk list in case it was a disk */
void PartitionerView::doDeviceAdded(VolumeTree newTree)
{
    Q_UNUSED(newTree)
    setDiskList();
    m_treeModel.readDataAgain(); /* in case it was another kind of device, e.g. a partition, read the tree again */
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
    
    m_treeModel.readDataAgain();
}

/*
 * This is called when a disk layout changes for whatever reason: updates the model.
 * 
 * NOTE: the correspondent signal is sent when a registered action, or a call to undo or redo, changed the disk's layout;
 * thus the modified tree is always the one currently displayed in the TreeView, and we update that one.
 */
void PartitionerView::doDiskTreeChanged(QString newTree)
{
    setDiskTree(newTree);
}

/* If the user selected a new disk for displaying, changes the TreeView data. */
void PartitionerView::doSelectedDiskChanged(QString newDisk)
{
    if (isDialogOpen) {
        return;
    }
    
    m_currentDisk = newDisk;
    setDiskTree(newDisk);
}

/* This is called when the selected device changes, or when a new disk tree is being displayed */
void PartitionerView::doSelectedDeviceChanged(QString devName)
{
    if (isDialogOpen) {
        return;
    }
    
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

/*
 * This is called when the user selects a new filesystem in the filesystem combobox of the Format partition dialog.
 * Other text inputs the dialog displays are relative to the currently selected filesystem: for example there's a text input which
 * allows the user to insert a filesystem label, but not all filesystems support that.
 * So we enable or disable the widget according to the current selection.
 */
void PartitionerView::doSelectedFsChanged(QString filesystem)
{
    QObject* changeFilesystemDialog = m_dialogs[FORMAT_PARTITION];
    Utils::FilesystemUtils* fsUtils = Utils::FilesystemUtils::instance();
    
    bool isLabelSupported = fsUtils->supportsLabel(filesystem);
    bool isOwnershipSupported = fsUtils->filesystemProperty(filesystem, "supports_unix_owners").toBool();
    
    QMetaObject::invokeMethod(changeFilesystemDialog, "activateInputs", Q_ARG(QVariant, isLabelSupported), Q_ARG(QVariant, isOwnershipSupported));
}

/* When an action button is clicked, shows the right dialog and sets its initial information when required */
void PartitionerView::doActionButtonClicked(QString actionName)
{
    m_boxmodel.disableAllButtons();
    isDialogOpen = true;
    
    QObject* dialog = m_dialogs[actionName];
    VolumeTree diskTree = m_manager->diskTree(m_currentDisk);
    DeviceModified* device = diskTree.searchDevice(m_currentDevice);
    bool showDialog = true;
    
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
    else if (actionName == REMOVE_PARTITION) { /* this doesn't need a dialog, so we directly call the "dialog closed" slot */
        showDialog = false;
        removePartitionDialogClosed(m_currentDevice);
    }
    else if (actionName == CREATE_PARTITION_TABLE) {
        dialog->setProperty("currentScheme", diskTree.disk()->partitionTableScheme());
    }
    else if (actionName == FORMAT_PARTITION) {
        QStringList supportedFilesystems = Utils::FilesystemUtils::instance()->supportedFilesystems();
        dialog->setProperty("supportedFilesystems", supportedFilesystems);
        QObject* combobox = dialog->findChild< QObject* >("fsComboBox");

        doSelectedFsChanged( supportedFilesystems.first() );
        QObject::connect(combobox, SIGNAL(currentTextChanged(QString)), SLOT(doSelectedFsChanged(QString)));
    }
    else if (actionName == CREATE_PARTITION) {
        dialog->setProperty("disk", m_currentDisk);
        
        /* Shows filesystems you can format the new partition with (unformatted means "none") */
        QStringList supportedFilesystems = Utils::FilesystemUtils::instance()->supportedFilesystems();
        supportedFilesystems << "unformatted";
        dialog->setProperty("supportedFilesystems", supportedFilesystems);

        /* Shows flags you can set for this partition */
        Disk* disk = diskTree.disk();
        foreach (const QString& flag, PartitionTableUtils::instance()->supportedFlags( disk->partitionTableScheme() )) {
            m_flagsModel.addFlag(flag, false);
        }
        
        /* Available space in megabytes */
        double freeSpaceMega = (double)(device->size()) / 1024.0f / 1024.0f;
        dialog->setProperty("freespace", freeSpaceMega);
        
        /* Possible types (logical, primary or extended) for the new partition */
        QStringList types = acceptedPartitionTypes(diskTree, device);
        dialog->setProperty("acceptedPartitionTypes", types);
    }
    else if (actionName == RESIZE_PARTITION) {
        DeviceModified* leftDevice = diskTree.leftDevice(device);
        DeviceModified* rightDevice = diskTree.rightDevice(device);
        double beforeSize = 0.0f;
        double afterSize = 0.0f;
        
        if (leftDevice && leftDevice->deviceType() == DeviceModified::FreeSpaceDevice) {
            beforeSize = (double)(leftDevice->size()) / 1024.0f / 1024.0f;
        }
        if (rightDevice && rightDevice->deviceType() == DeviceModified::FreeSpaceDevice) {
            afterSize = (double)(rightDevice->size()) / 1024.04 / 1024.04;
        }
        
        dialog->setProperty("before", beforeSize);
        dialog->setProperty("size", (double)(device->size()) / 1024.0f / 1024.0f);
        dialog->setProperty("after", afterSize);
    }
    else if (actionName == UNDO) {
        showDialog = false;
        undoDialogClosed();
    }
    else if (actionName == REDO) {
        showDialog = false;
        redoDialogClosed();
    }
    else if (actionName == APPLY) {
        QStringList actionDescriptions;
        
        foreach (Action* action, m_manager->registeredActions()) {
            actionDescriptions << action->description();
        }
        
        m_context->setContextProperty("registeredActions", QVariant::fromValue(actionDescriptions));
    }
    
    if (showDialog) {
        QMetaObject::invokeMethod(dialog, "show", Qt::QueuedConnection, Q_ARG(QVariant, m_currentDevice));
    }
}

/* Inspects the current situation and determines whether we can create a new partition as logical, extended or primary. */
QStringList PartitionerView::acceptedPartitionTypes(const VolumeTree& diskTree, Devices::DeviceModified* freeSpace)
{
    DeviceModified* parent = diskTree.parentDevice(freeSpace);
    QStringList types;
    
    /* If the free space blocks is located inside an extended partition, we can only create a logical */
    if (parent->deviceType() == DeviceModified::PartitionDevice) {
        types.append("Logical");
    }
    else {
        types.append("Primary");
        
        /* There can be only one extended of a MS-DOS disk */
        if (!diskTree.extendedPartition() && diskTree.disk()->partitionTableScheme() == "mbr") {
            types.append("Extended");
        }
    }
    
    return types;
}

void PartitionerView::formatDialogClosed(bool accepted,
                                         QString filesystem,
                                         QString fsLabel,
                                         QString ownerUid,
                                         QString ownerGid,
                                         QString partition)
{ 
    if (!accepted) {
        afterCancelClicked();
        return;
    }
    
    int uid = (ownerUid.isEmpty()) ? -1 : ownerUid.toInt();
    int gid = (ownerGid.isEmpty()) ? -1 : ownerGid.toInt();
    Filesystem fs(filesystem, fsLabel, uid, gid);
    
    m_manager->registerAction( new Actions::FormatPartitionAction(partition, fs) );
    checkErrors();
    afterOkClicked();
}

void PartitionerView::modifyDialogClosed(bool accepted, QString label, QString partition)
{    
    if (!accepted) {
        afterCancelClicked();
        return;
    }
    
    QStringList flags = checkedFlags(MODIFY_PARTITION);
    
    m_manager->registerAction( new Actions::ModifyPartitionAction(partition, label, flags) );
    checkErrors();
    afterOkClicked();
}

void PartitionerView::removePartitionDialogClosed(QString partition)
{
    m_manager->registerAction( new Actions::RemovePartitionAction(partition) );
    checkErrors();
    afterOkClicked();
}

void PartitionerView::createTableDialogClosed(bool accepted, QString scheme, QString disk)
{   
    if (!accepted) {
        afterCancelClicked();
        return;
    }
    
    if (scheme.isEmpty()) {
        return;
    }
    
    PartitionTableScheme schemeEnum = (scheme == "gpt") ? GPTScheme : MBRScheme;
    m_manager->registerAction( new Actions::CreatePartitionTableAction(disk, schemeEnum) );
    checkErrors();
    afterOkClicked();
}

void PartitionerView::removeTableDialogClosed(bool accepted, QString disk)
{    
    if (!accepted) {
        afterCancelClicked();
        return;
    }
    
    m_manager->registerAction( new Actions::RemovePartitionTableAction(disk) );
    checkErrors();
    afterOkClicked();
}

void PartitionerView::createPartitionDialogClosed(bool accepted,
                                                  qreal size,
                                                  qreal sb,
                                                  QString type,
                                                  QString label,
                                                  QString filesystem,
                                                  QString containerName,
                                                  QString disk)
{    
    if (!accepted) {
        afterCancelClicked();
        return;
    }
    
    VolumeTree diskTree = m_manager->diskTree(disk);
    DeviceModified* container = diskTree.searchDevice(containerName);

    qulonglong byteSize = (qulonglong)size * 1024 * 1024;
    qulonglong spaceBefore = (qulonglong)sb * 1024 * 1024;
    qulonglong offset = container->offset() + spaceBefore;
    bool extended = (type == "Extended");
    Filesystem fs( filesystem );
    QStringList flags = checkedFlags(CREATE_PARTITION);
    
    CreatePartitionAction* action = new CreatePartitionAction(disk, offset, byteSize, extended, fs, label, flags);
    m_manager->registerAction(action);
    checkErrors();
    afterOkClicked();
}

void PartitionerView::resizeDialogClosed(bool accepted, qreal size, qreal sb, QString partitionName)
{    
    if (!accepted) {
        afterCancelClicked();
        return;
    }
    
    VolumeTree diskTree = m_manager->diskTree(m_currentDisk);
    DeviceModified* partition = diskTree.searchDevice(partitionName);
    qulonglong spaceBefore = (qulonglong)sb * 1024 * 1024;
    qulonglong newSize = (qulonglong)size * 1024 * 1024;
    qulonglong newOffset = partition->offset();
    
    qDebug() << partition->offset() << partition->size();
    
    if (spaceBefore > 0) {
        DeviceModified* leftDevice = diskTree.leftDevice(partition);
        
        if (leftDevice && leftDevice->deviceType() == DeviceModified::FreeSpaceDevice) {
            newOffset = leftDevice->offset() + spaceBefore;
        }
        else {
            newOffset = partition->offset() + spaceBefore;
        }
    }
    
    qDebug() << newOffset << newSize;
    
    m_manager->registerAction( new ResizePartitionAction(partitionName, newOffset, newSize) );
    checkErrors();
    afterOkClicked();
}

void PartitionerView::undoDialogClosed()
{
    m_manager->undo();
    afterOkClicked();
}

void PartitionerView::redoDialogClosed()
{
    m_manager->redo();
    afterOkClicked();
}

void PartitionerView::applyActions(bool confirmed)
{
    if (!confirmed) {
        afterCancelClicked();
        return;
    }
    
    m_registeredActions = m_manager->registeredActions();
    QObject* dialog = m_dialogs["applyDialog"];
    
    dialog->setProperty("actionCount", m_registeredActions.size());
    dialog->setProperty("currentAction", m_registeredActions.first()->description());
    QMetaObject::invokeMethod(dialog, "show");
    
    ExecuterThread* thread = new ExecuterThread;
    QObject::connect(thread, SIGNAL(sendProgressChanged(int)), SLOT(reportProgress(int)), Qt::DirectConnection);
    QObject::connect(thread, SIGNAL(sendExecutionError(QString)), SLOT(executionError(QString)), Qt::DirectConnection);
    thread->start();
}

/* Check if the latest action was successfully. Otherwise, show the error description in a dialog. */
void PartitionerView::checkErrors()
{
    Utils::PartitioningError error = m_manager->error();
    
    if (error.type() != Utils::PartitioningError::None) {
        QObject* errorDialog = m_dialogs["error"];        
        QMetaObject::invokeMethod(errorDialog, "show", Qt::QueuedConnection, Q_ARG(QVariant, error.description()));
    }
}

void PartitionerView::afterCancelClicked()
{    
    isDialogOpen = false;
    doSelectedDeviceChanged(m_currentDevice);
}

void PartitionerView::afterOkClicked()
{
    VolumeTree tree = m_manager->diskTree(m_currentDisk);
    DeviceModified* dev = tree.searchDevice(m_currentDevice);
    isDialogOpen = false;
    
    if (dev) {
        doSelectedDeviceChanged(m_currentDevice);
    }
    else {
        m_treeView->setProperty("currentIndex", 0);
    }
    
    setActionList(); /* change the list of registered actions in the GUI (if the previous method was successful) */
    setGenericButtonsState(); /* some non-action related buttons are affected by how many actions we registered */
}

void PartitionerView::applyDialogClosed()
{
    isDialogOpen = false;    
    doSelectedDiskChanged(m_currentDisk);
    
    setActionList();
    setGenericButtonsState();
}

void PartitionerView::reportProgress(int nextAction)
{
    QObject* dialog = m_dialogs["applyDialog"];
    
    if (nextAction < m_registeredActions.size()) {
        dialog->setProperty("currentAction", m_registeredActions.at(nextAction)->description());
        dialog->setProperty("currentActionIndex", nextAction);
    }
    else {
        dialog->setProperty("currentAction", "Finished successfully!");
        dialog->setProperty("currentActionIndex", m_registeredActions.size());
    }
}

void PartitionerView::executionError(QString err)
{
    qDebug() << err;
    QObject* dialog = m_dialogs["applyDialog"];
    dialog->setProperty("currentAction", "There were errors executing this action");
}

/*
 * Read which checkboxes are set in the flags list.
 * TODO: discover how to sent this information via QML directly.
 */
QStringList PartitionerView::checkedFlags(const QString& dialog)
{
    QStringList flags;
    
    QObject* flagsList = m_dialogs[dialog]->findChild< QObject* >("flagsList");
    QList< QObject* > rowLayout = flagsList->findChildren< QObject* >("row");
    
    foreach (QObject* row, rowLayout) {
        QObject* checkbox = row->findChild< QObject* >("flagCheckBox");
        QObject* flag = row->findChild< QObject* >("flagName");
        
        if (checkbox->property("checked").toBool()) {
            flags.append( flag->property("text").toString() );
        }
    }
    
    return flags;
}

#include "partitionerview.moc"
