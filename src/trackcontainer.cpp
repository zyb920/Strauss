#include "trackcontainer.h"
#include <QWidget>
#include <src/trackmidiview.h>
#include "src/plugineditorcontainer.h"
#include "src/trackmidi.h"
#include "src/mastertrack.h"
int ID = 0;
/*This class represents a container for the tracks, located above the Pianoroll.
 *
 *
 */
TrackContainer::TrackContainer(PluginEditorContainer *pluginEditorContainer, PianoRollContainer *pianoRollContainer, MasterTrack *masterTrack)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->pluginEditorContainer = pluginEditorContainer;
    this->pianoRollContainer = pianoRollContainer;
    this->masterTrack = masterTrack;
    vSplitter = new QSplitter(this);
    vSplitter->setOrientation(Qt::Vertical);
    vSplitter->setContentsMargins(0,0,0,0);
    vSplitter->setHandleWidth(0);
    vSplitter->setStyleSheet("QSplitter {background-color: rgb(170,170,170); border: 1px black; }");
    vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    hLayout = new QHBoxLayout;
    hLayout->setSpacing(0);
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->addLayout(vLayout);
    auto lt = new QVBoxLayout;
    lt->setSpacing(0);
    lt->setContentsMargins(0,0,0,0);
    lt->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    lt->addWidget(vSplitter);
    hLayout->addLayout(lt);
    setLayout(hLayout);

    QObject::connect(this,&TrackContainer::addPianoRoll,pianoRollContainer,&PianoRollContainer::addPianoRoll);

}

TrackView *TrackContainer::addTrackFromLoadProject(const MidiTrack &midi_track, TrackMidi *midiTrack)
{
    auto midiData = midiTrack->midiData();
    for (int i = 0; i < midi_track.midi_data_size(); ++i)
    {
        auto midi_data = midi_track.midi_data(i);
        midiData->noteMap[midi_data.total_dt()].push_back(midi_data.event());
    }
    midiData->totalDT = g_totalDt;
    MidiManager::recalculateNoteListDT(midiData);
    auto *midiView = new TrackMidiView;
    TrackView *trackView = new TrackView(midiTrack,midiView,this);
    midiData->instrumentName = QString::fromStdString(midi_track.name());

    trackView->id = ID++;
    vLayout->addWidget(midiView);
    vSplitter->addWidget(trackView);
    return trackView;
}

void TrackContainer::addTrackView(const mSong &song)
{
    //    foreach (const auto &track, song.tracks)
    //    {
    //        QWidget *widget = new QWidget;
    //        auto *midiView = new TrackMidiView;
    //        auto *view = new TrackView(track,midiView,this);
    //        auto *hlayout = new QHBoxLayout;
    //        hlayout->setAlignment(Qt::AlignTop);
    //        hlayout->setSpacing(0);
    //        hlayout->setContentsMargins(0,0,0,0);
    //        widget->setLayout(hlayout);
    //        hlayout->addWidget(view);

    //        view->id = ID++;

    //        emit addPianoRoll(view);
    //        QObject::connect(view,&TrackView::trackClickedOn,pianoRollContainer,&PianoRollContainer::switchPianoRoll);
    //        vLayout->addWidget(midiView);
    //        vSplitter->addWidget(view);
    //        view->pluginTrack = pluginEditorContainer->addTrack(view);
    //    }
    //    adjustSize();
}


TrackView* TrackContainer::addSingleView(TrackMidi *midiTrack)
{
    auto *midiView = new TrackMidiView;
    TrackView *trackView = new TrackView(midiTrack,midiView,this);
    trackView->id = ID++;
    vLayout->addWidget(midiView);
    vSplitter->addWidget(trackView);
    return trackView;
}

void TrackContainer::deleteTrack(TrackView *trackView, TrackMidiView *midiView)
{
    trackView->hide();
    midiView->hide();
    vLayout->removeWidget(midiView);
    trackView->setParent(NULL);
    midiView->deleteLater();
}

std::vector<TrackView *> TrackContainer::getTrackViews() const
{
    std::vector<TrackView *> tracks;
    for (int var = 0; var < vSplitter->children().size(); ++var)
    {
        auto track_view = dynamic_cast<TrackView*>(vSplitter->children().at(var));
        if (track_view)
        {
            tracks.push_back(track_view);
        }
    }
    return tracks;
}


void TrackContainer::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_C:
        switchControlChange();
        break;
    default:
        break;
    }

    QWidget::keyPressEvent(event);
}


int TrackContainer::getNumTracks() const
{
    return vSplitter->children().size();
}

void TrackContainer::deleteAllTracks()
{
    auto tracks = getTrackViews();
    for (uint var = 0; var < tracks.size(); ++var)
    {
        tracks.at(var)->deleteTrack();
    }
}

void TrackContainer::trackClicked(TrackView *trackView)
{
    masterTrack->setCurrentTrack(trackView->midiTrack());
    pianoRollContainer->switchPianoRoll(trackView);
    for(const auto &track : getTrackViews())
    {
        if (trackView != track) {
            track->deselect();
        }
    }

}
