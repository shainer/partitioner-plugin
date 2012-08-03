/*
 * Utility class to execute actions in a different thread and avoid GUI freezes

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#ifndef EXECUTERTHREAD_H
#define EXECUTERTHREAD_H

#include <QThread>
#include <solid/partitioner/volumemanager.h>

using namespace Solid::Partitioner;

class ExecuterThread : public QThread
{
    Q_OBJECT
    
public:
    explicit ExecuterThread(QObject* parent = 0);
    void run();
    
signals:
    
    /*
     * Signals are taken from VolumeManager and resent as they are
     */
    void sendProgressChanged(int);
    void sendExecutionError(QString);
    void sendFinishedExecution();
    
private:
    VolumeManager* m_manager;
};

#endif