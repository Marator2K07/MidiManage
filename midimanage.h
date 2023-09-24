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
    double duration; /* длительность трэка в секундах */
    qint32 currentPos; /* текущая позиция в трэке */
    bool isPlaying;

private:
    void updateFileDuration();

public:
    MidiManage();
    MidiManage(QString fileName, QString outDeviceId);

    void loadFile(QString fileName);
    bool connectOut(QString outDeviceId);
    void disconnectOut();

    QString greetings();
    QMap<QString, QString> devices(); /* получить доступные девайсы вывода */
    double getFileDuration(); /* получить длительность файла в секундах */

    // QThread interface
protected:
    void run() override;

public slots:
    void play();
    void pause(); /* временная остановка на текущей позиции, следующий старт с нее */
    void stop(); /* полная остановка проигрывания, следующий старт будет сначала */

    void playSound(int voice,
                   int note,
                   int velocity);
};


#endif // MIDIMANAGE_H
