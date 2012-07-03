/*
 * Entry point

   Copyright (C) 2012 Lisa Vitolo <shainer@chakra-project.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
*/

#include <QApplication>
#include <partitionerview.h>

using namespace Solid::Partitioner;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    PartitionerView pview;
    
    return app.exec();
}
