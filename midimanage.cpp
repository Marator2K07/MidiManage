#include "midimanage.h"

MidiManage::MidiManage()
{
    midiFile = new QMidiFile();
    midiOut = new QMidiOut();
}

MidiManage::MidiManage(QMidiFile *file, QMidiOut *out)
{
    midiFile = file;
    midiOut = out;
}

void MidiManage::loadFile(QString fileName)
{
    midiFile->load(fileName);
}

bool MidiManage::connectOut(QString outDeviceId)
{
    return midiOut->connect(outDeviceId);
}

QString MidiManage::greetings()
{
    return "You are using MidiManager's lab";
}

void MidiManage::run()
{
    QElapsedTimer t;
    t.start();
    QList<QMidiEvent*> events = midiFile->events();
    for (QMidiEvent* e : events) {
        if (e->type() != QMidiEvent::Meta) {
            qint64 event_time = midiFile->timeFromTick(e->tick()) * 1000;

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