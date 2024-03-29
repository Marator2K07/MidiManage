#include "midimanage.h"

void MidiManage::updateFileDuration()
{
    QList<QMidiEvent*> events = currentMidiFile->events();
    duration = currentMidiFile->timeFromTick(events.last()->tick());
}

MidiManage::MidiManage()
{
    loadMidiFile = new QMidiFile();
    recordMidiFile = new QMidiFile();
    recordMidiFile->createTrack();

    midiOut = new QMidiOut();    

    currentPos = 0;
    isPlaying = false;
}

MidiManage::MidiManage(QString fileName, QString outDeviceId)
{
    loadMidiFile = new QMidiFile();
    recordMidiFile = new QMidiFile();
    recordMidiFile->createTrack();

    loadMidiFile->load(fileName);
    currentMidiFile = loadMidiFile;
    updateFileDuration();    

    midiOut = new QMidiOut();
    midiOut->connect(outDeviceId);

    currentPos = 0;
    isPlaying = false;
}

void MidiManage::loadFile(QString fileName)
{
    isPlaying = false;
    currentPos = 0;

    loadMidiFile->load(fileName);
    currentMidiFile = loadMidiFile;
    updateFileDuration();    
}

bool MidiManage::connectOutDevice(QString outDeviceId)
{
    return midiOut->connect(outDeviceId);
}

void MidiManage::disconnectOutDevice()
{
    if (midiOut->isConnected()) {
        midiOut->disconnect();
    }
}

QString MidiManage::greetings()
{
    return "You are using MidiManage's lab";
}

QMap<QString, QString> MidiManage::devices()
{
    return QMidiOut::devices();
}

QList<QMidiEvent *> MidiManage::events()
{
    return currentMidiFile->events();
}

double MidiManage::getFileDuration()
{
    return duration;
}

void MidiManage::run()
{
    QElapsedTimer t;
    t.start();
    QList<QMidiEvent*> events = currentMidiFile->events();
    // участок кода для учитывания возможной паузы
    // чтобы задать сдвиг ожидания, и как следствие - корректная работа
    // после продолжения воспроизведения после паузы
    QMidiEvent *eB = events.at(currentPos);
    qint64 timeShift = currentMidiFile->timeFromTick(eB->tick()) * 1000;

    QMidiEvent *e = nullptr;
    // анализ звуковых сообщений и их проигрывание
    for (int pos = currentPos; pos < events.length() && isPlaying; pos++) {
        e = events.at(pos);
        if (e->type() != QMidiEvent::Meta) {
            currentPos = pos; // сохраняем текущую позицию

            qint64 event_time = currentMidiFile->timeFromTick(e->tick()) * 1000;
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
    if (currentMidiFile->tracks().length() > 0) {
        isPlaying = true;
        start();
    }
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

void MidiManage::playRecordSound(int voice,
                                 int note,
                                 int velocity,
                                 int tick)
{
    QMidiEvent *e = new QMidiEvent();
    e->setType(QMidiEvent::NoteOn);
    e->setVoice(voice);
    e->setNote(note);
    e->setVelocity(velocity);
    midiOut->sendEvent(*e);

    recordMidiFile->addEvent(tick, e);
}

void MidiManage::switchToLoadFile()
{
    currentMidiFile = loadMidiFile;
}

void MidiManage::switchToRecordFile()
{
    currentMidiFile = recordMidiFile;
}
