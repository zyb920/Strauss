#include "headercontainer.h"
#include <src/audioengine.h>
#include <src/mainwindow.h>


const int maxWidth = 50;
HeaderContainer::HeaderContainer(AudioEngine *audioEngine) : _audioEngine(audioEngine)
{
    auto *playButton     = new QPushButton(this);
    auto *pauseButton    = new QPushButton(this);
    auto *restartButton  = new QPushButton(this);
    recordButton         = new QPushButton("Record",this);
    keyboardButton       = new QPushButton(this);
    auto *hbox           = new QHBoxLayout;
    auto *mainbox        = new QHBoxLayout;
    recordStyle          = new QCheckBox;
    groupBox             = new QGroupBox;
    auto *tempoLabel     = new QLabel("Tempo");
    auto *tempoWidget = new QDraggingWidget(10,400,120);
    hbox->addWidget(playButton);
    hbox->addWidget(pauseButton);
    hbox->addWidget(restartButton);
    hbox->addWidget(recordButton);
    hbox->addWidget(recordStyle);
    hbox->addWidget(keyboardButton);
    playButton->setMaximumWidth(maxWidth);
    pauseButton->setMaximumWidth(maxWidth);
    restartButton->setMaximumWidth(maxWidth);
    recordButton->setMaximumWidth(maxWidth);
    keyboardButton->setCheckable(true);
    tempoWidget->setFixedSize(35,35);

    hbox->setMargin(0);
    groupBox->setLayout(hbox);
    groupBox->setFlat(true);
    mainbox->addWidget(tempoLabel);
    mainbox->addWidget(tempoWidget);
    mainbox->addWidget(groupBox);
    mainbox->setAlignment(Qt::AlignTop|Qt::AlignCenter);
    setLayout(mainbox);
    recordButton ->setCheckable(true);
    QObject::connect(playButton,     &QPushButton::clicked,this,&HeaderContainer::play);
    QObject::connect(pauseButton,    &QPushButton::clicked,this,&HeaderContainer::pause);
    QObject::connect(restartButton,  &QPushButton::clicked,this,&HeaderContainer::restart);
    QObject::connect(recordButton,   &QPushButton::toggled,this,&HeaderContainer::record);
    QObject::connect(keyboardButton, &QPushButton::toggled,this,&HeaderContainer::keyboardMode);
    QObject::connect(tempoWidget,    &QDraggingWidget::valueChanged,this,&HeaderContainer::masterTempoChanged);


    groupBox->setStyleSheet("QGroupBox {  border: 1px solid rgb(20,20,20);"
                            " padding: 6px;"
                            " background: rgb(50,50,50);}");
    //  playButton->setStyleSheet("background-image: url(:/images/icons/play_pause.png);");
    QPixmap pic(":/icons/play_pause.png");
    playButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPlay));
    pauseButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaPause));
    restartButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_MediaSeekBackward));
    // playButton->setIconSize(pic.size());
    keyboardButton->setIcon(QIcon(":/icons/keyboard_icon.png"));

}

void HeaderContainer::play()
{

    if(_audioEngine->isPaused()){
        _audioEngine->setPaused(false);
    }
    else{
        _audioEngine->requestPlaybackRestart();
    }

}

void HeaderContainer::restart()
{
    _audioEngine->setPaused(false);
    MidiPlayer::canRecordInput = false;
    _audioEngine->requestPlaybackRestart();
    if(MidiPlayer::canRecordInput)
    {
        // MidiPlayer::addMidiAfterRecording();
        MidiPlayer::canRecordInput = false;
    }
}

void HeaderContainer::pause()
{
    _audioEngine->setPaused(true);
    if(MidiPlayer::canRecordInput)
    {
        //MidiPlayer::addMidiAfterRecording();
        MidiPlayer::canRecordInput = false;
    }

}

void HeaderContainer::record(bool state)
{
    recordButton->setProperty("toggled",state);
    style()->unpolish(recordButton);
    style()->polish(recordButton);
    MidiPlayer::canRecordInput = state;
    MidiPlayer::recordingOverwrites = state;
    if(state)
        play();
}

void HeaderContainer::notation()
{

}

void HeaderContainer::keyboardMode(bool enabled)
{
    keyboardButton->setProperty("toggled",enabled);
    style()->unpolish(keyboardButton);
    style()->polish(keyboardButton);
    keyboardModeEnabled = enabled;
}

void HeaderContainer::masterTempoChanged(int tempo)
{
    g_tempo = tempo;
    g_timer->setDuration((static_cast<float>(60.0/g_tempo)*g_quarterNotes*1000));
}


