#ifndef PIANOROLLHELPERVIEW_H
#define PIANOROLLHELPERVIEW_H

class ControlChangeContainer;

#include <QFrame>
#include <QObject>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QComboBox>

class PianoRollHelperView : public QFrame
{
    Q_OBJECT
public:
    PianoRollHelperView(ControlChangeContainer *controlChangeContainer);
    QComboBox *comboBox;
public slots:
    void comboBoxIdChanged(int index);
protected:
    void paintEvent(QPaintEvent *event);
private:
    void initializeComboBox();
    QVBoxLayout *vLayout;
    ControlChangeContainer *control_change_container;


};

#endif // PIANOROLLHELPERVIEW_H