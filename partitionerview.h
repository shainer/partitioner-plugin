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
    
    void formatDialogClosed(bool, QString, QString);
    void modifyDialogClosed(bool, QString, QString);
    void removePartitionDialogClosed(QString);
    
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
    QHash< QString, QObject* > m_dialogs;
    
    VolumeManager* m_manager;
    QStringList m_diskList;
    QString m_currentDisk; /* the currently selected disk */
    QString m_currentDevice;
    
    void setButtonBox();
    void setGenericButtonsState();
    void setDiskList();
    void setActionList();
    void setDiskTree(const QString &);
};

#endif