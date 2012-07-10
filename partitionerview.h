/*
 * Main class that displays and handles events from QML GUI: header

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#ifndef PARTITIONERVIEW_H
#define PARTITIONERVIEW_H

#include <QObject>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QGraphicsObject>

#include <buttonboxmodel.h>
#include <devicetreemodel.h>
#include "pluginregister.h"
#include "flagsmodel.h"

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
    void doDiskTreeChanged(QString);
    void doSelectedDiskChanged(QString);
    void doSelectedDeviceChanged(QString);
    void doActionButtonClicked(QString);
    
    /*
     * These slots are called when the correspondent dialog is closed, to gather information and request the right
     * action from the library. For better readability, a slot is added here even when the action doesn't open any dialog,
     * and is called as a normal function right after the button is pressed.
     */
    void formatDialogClosed(bool, QString, QString);
    void modifyDialogClosed(bool, QString, QString);
    void removePartitionDialogClosed(QString);
    void createTableDialogClosed(bool, QString, QString);
    void removeTableDialogClosed(bool, QString);
    void createPartitionDialogClosed(bool, qreal, qreal, QString, QString, QString, QString, QString);
    void resizeDialogClosed(bool, qreal, qreal, QString);
    void applyDialogClosed();

    void undoDialogClosed();
    void redoDialogClosed();
    void applyActions(bool);
    
    void afterCancelClicked();
    void afterOkClicked();
    
    void reportProgress(int);
    void executionError(QString);
private:
    QDeclarativeView m_view;
    QDeclarativeContext* m_context;
    QWidgetsPlugin plugin;
    
    /* Models */
    ButtonBoxModel m_boxmodel;
    DeviceTreeModel m_treeModel;
    FlagsModel m_flagsModel;
    
    /* QML component objects */
    QGraphicsObject* m_rootObject;
    QObject* m_treeView;
    QHash< QString, QObject* > m_dialogs;
    
    VolumeManager* m_manager;
    QList< Actions::Action* > m_registeredActions;
    QStringList m_diskList;
    
    QString m_currentDisk; /* the currently selected disk */
    QString m_currentDevice;
    bool isDialogOpen; /* block all selection changes when a dialog is currently open */
    
    void setButtonBox();
    void setGenericButtonsState();
    void setDiskList();
    void setActionList();
    void setDiskTree(const QString &);
    
    QObject* getTreeView();
    QStringList checkedFlags(const QString &);
    QStringList acceptedPartitionTypes(const VolumeTree &, Devices::DeviceModified *);
    void checkErrors();
};

#endif