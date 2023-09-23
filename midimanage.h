#ifndef MIDIMANAGE_H
#define MIDIMANAGE_H

#include "MidiManage_global.h"

#include <src/QMidiFile.h>
#include <src/QMidiOut.h>

#include <QString>
#include <QThread>
#include <QDebug>

class MIDIMANAGE_EXPORT MidiManage : public QThread
{
    Q_OBJECT
private:
    QMidiFile *midiFile;
    QMidiOut *midiOut;
    double duration;

private:
    void updateFileDuration();

public:
    MidiManage();
    MidiManage(QString fileName, QString outDeviceId);

    void loadFile(QString fileName);
    bool connectOut(QString outDeviceId);

    QString greetings();
    QMap<QString, QString> devices();
    double getFileDuration(); /* получить длительность файла в секундах */

    // QThread interface
protected:
    void run() override;
};


#endif // MIDIMANAGE_H
