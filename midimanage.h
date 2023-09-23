#ifndef MIDIMANAGE_H
#define MIDIMANAGE_H

#include "MidiManage_global.h"

#include <src/QMidiFile.h>
#include <src/QMidiOut.h>

#include <QString>
#include <QThread>

class MIDIMANAGE_EXPORT MidiManage : public QThread
{
    Q_OBJECT
private:
    QMidiFile *midiFile;
    QMidiOut *midiOut;

public:
    MidiManage();
    MidiManage(QMidiFile *file, QMidiOut *out);

    void loadFile(QString fileName);
    bool connectOut(QString outDeviceId);

    QString greetings();

    // QThread interface
protected:
    void run() override;
};


#endif // MIDIMANAGE_H
