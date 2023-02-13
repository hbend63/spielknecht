#ifndef BEEPER_H
#define BEEPER_H

#include <QObject>
#include <QIODevice>
#include <QAudioFormat>
#include <QMediaDevices>
#include <QAudioSink>

class Generator : public QIODevice
{
    Q_OBJECT

public:
    Generator(const QAudioFormat &format, qint64 durationUs, int sampleRate);

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;
    qint64 bytesAvailable() const override;
    qint64 size() const override { return m_buffer.size(); }

private:
    void generateData(const QAudioFormat &format, qint64 durationUs, int sampleRate);

private:
    qint64 m_pos = 0;
    QByteArray m_buffer;
};




class Beeper : public QObject
{
    Q_OBJECT
public:
    explicit Beeper(QObject *parent = nullptr);
    void beep(int freq);

signals:


private:



};

#endif // BEEPER_H
