#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

class MainWindow;

#include <iostream>
#include <fstream>
#include <QObject>
#include <string>
#include "src/midiinter.pb.h"
#include "trackcontainer.h"
#include "src/trackview.h"
#include "src/plugintrackview.h"

class ProjectManager
{
public:
    ProjectManager();
    void saveProject();
    void saveAsProject(QString path, MasterTrack *masterTrack);
    void loadProject(QString path, MainWindow *main_window, TrackContainer &track_container);

};

#endif // PROJECTMANAGER_H
