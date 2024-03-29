#ifndef MIDIMANAGE_H
#define MIDIMANAGE_H

#include "MidiManage_global.h"

#include <src/QMidiFile.h>
#include <src/QMidiOut.h>

#include <QThread>

class MIDIMANAGE_EXPORT MidiManage : public QThread
{
    Q_OBJECT
private:
    QMidiFile *currentMidiFile;
    QMidiFile *loadMidiFile;
    QMidiFile *recordMidiFile;

    QMidiOut *midiOut;
    double duration; /* длительность трэка в секундах */
    qint32 currentPos; /* текущая позиция в трэке */
    bool isPlaying;

private:
    void updateFileDuration();

public:
    MidiManage();
    MidiManage(QString fileName, QString outDeviceId);

    void loadFile(QString fileName);
    bool connectOutDevice(QString outDeviceId);
    void disconnectOutDevice();

    QString greetings();
    QMap<QString, QString> devices(); /* получить доступные девайсы вывода */
    QList<QMidiEvent*> events(); /* получить список музыкальных событий текущего файла! */
    double getFileDuration(); /* получить длительность файла в секундах */

    void switchToLoadFile();
    void switchToRecordFile();

    // QThread interface
protected:
    void run() override;

public slots:
    void play();
    void pause(); /* временная остановка на текущей позиции, следующий старт с нее */
    void stop(); /* полная остановка проигрывания, следующий старт будет сначала */

    void playRecordSound(int voice,
                         int note,
                         int velocity,
                         int tick);
};


#endif // MIDIMANAGE_H
