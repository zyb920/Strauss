#ifndef HEADERCONTAINER_H
#define HEADERCONTAINER_H

//Forward declarations
class AudioEngine;

#include <QWidget>
#include <QDebug>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <src/common.h>
#include "src/qdraggingwidget.h"
class HeaderContainer : public QWidget
{
    Q_OBJECT
public:
    HeaderContainer(AudioEngine *audioEngine);

private:
    AudioEngine *_audioEngine;
    QGroupBox * groupBox;
    QCheckBox * recordStyle;
    QPushButton* keyboardButton;
    QPushButton* recordButton;
private slots:
    void play();
    void restart();
    void pause();
    void record(bool state);
    void notation();
    void keyboardMode(bool enabled);
    void masterTempoChanged(int tempo);
};

#endif // HEADERCONTAINER_H


