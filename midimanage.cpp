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

    currentPos = 0;
    isPlaying = false;
}

MidiManage::MidiManage(QString fileName, QString outDeviceId)
{
    midiFile = new QMidiFile();
    midiFile->load(fileName);
    updateFileDuration();

    midiOut = new QMidiOut();
    midiOut->connect(outDeviceId);

    currentPos = 0;
    isPlaying = false;
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
    // участок кода для учитывания возможной паузы
    // чтобы задать сдвиг ожидания, и как следствие - корректная работа
    // после продолжения воспроизведения после паузы
    QMidiEvent *eB = events.at(currentPos);
    qint64 timeShift = midiFile->timeFromTick(eB->tick()) * 1000;

    QMidiEvent *e = nullptr;
    // анализ звуковых сообщений и их проигрывание
    for (int pos = currentPos; pos < events.length() && isPlaying; pos++) {
        e = events.at(pos);
        if (e->type() != QMidiEvent::Meta) {
            currentPos = pos; // сохраняем текущую позицию

            qint64 event_time = midiFile->timeFromTick(e->tick()) * 1000;
            qint32 waitTime = event_time - t.elapsed() - timeShift;

            if (waitTime > 0) {
                msleep(waitTime);
            }
            if (e->type() == QMidiEvent::SysEx) {
                // TODO: sysex
            } else {
                qint32 message = e->message();
                midiOut->sendMsg(message);
            }
            // если поставили паузу или полностью прекратили проигрывать
            if(!isPlaying) {
                midiOut->stopAll();
                return;
            }
        }
    }
    // если дошли до конца трэка, то тоже нужно принять меры
    midiOut->stopAll();
    currentPos = 0;
}

void MidiManage::play()
{
    isPlaying = true;
    start();
}

void MidiManage::pause()
{
    isPlaying = false;
}

void MidiManage::stop()
{
    isPlaying = false;
    currentPos = 0;
}
