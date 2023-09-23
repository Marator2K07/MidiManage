#include "midimanage.h"

void MidiManage::updateFileDuration()
{
    QList<QMidiEvent*> events = midiFile->events();
    duration = midiFile->timeFromTick(events.last()->tick());
}

MidiManage::MidiManage()
{
    midiFile = new QMidiFile();
    midiOut = new QMidiOut();
}

MidiManage::MidiManage(QString fileName, QString outDeviceId)
{
    midiFile = new QMidiFile();
    midiFile->load(fileName);
    updateFileDuration();
    midiOut = new QMidiOut();
    midiOut->connect(outDeviceId);
}

void MidiManage::loadFile(QString fileName)
{
    midiFile->load(fileName);
    updateFileDuration();
}

bool MidiManage::connectOut(QString outDeviceId)
{
    return midiOut->connect(outDeviceId);
}

void MidiManage::disconnectOut()
{
    midiOut->disconnect();
}

QString MidiManage::greetings()
{
    return "You are using MidiManage's lab";
}

QMap<QString, QString> MidiManage::devices()
{
    return QMidiOut::devices();
}

double MidiManage::getFileDuration()
{
    return duration;
}

void MidiManage::run()
{
    QElapsedTimer t;
    t.start();
    QList<QMidiEvent*> events = midiFile->events();
    for (QMidiEvent* e : events) {
        if (e->type() != QMidiEvent::Meta) {
            qint64 event_time = midiFile->timeFromTick(e->tick()) * 1000;
            qDebug() << event_time;

            qint32 waitTime = event_time - t.elapsed();
            if (waitTime > 0) {
                msleep(waitTime);                
            }
            if (e->type() == QMidiEvent::SysEx) {
                // TODO: sysex
            } else {
                qint32 message = e->message();
                midiOut->sendMsg(message);
            }
        }
    }
}
