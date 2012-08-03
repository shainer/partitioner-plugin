/*
 * Utility class to execute actions in a different thread and avoid GUI freezes

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#include "executerthread.h"
#include <QMetaType>

using namespace Solid::Partitioner;

ExecuterThread::ExecuterThread(QObject* parent)
    : QThread(parent)
    , m_manager( VolumeManager::instance() )
{
    QObject::connect(m_manager, SIGNAL(progressChanged(int)), SIGNAL(sendProgressChanged(int)), Qt::DirectConnection);
    QObject::connect(m_manager, SIGNAL(executionError(QString)), SIGNAL(sendExecutionError(QString)), Qt::DirectConnection);
    QObject::connect(m_manager, SIGNAL(executionFinished()), SIGNAL(sendFinishedExecution()), Qt::DirectConnection);
}

void ExecuterThread::run()
{
    m_manager->apply();
}