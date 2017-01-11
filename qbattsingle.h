#ifndef QBATTSINGLE_H
#define QBATTSINGLE_H

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

class QBattSingle {
public:
    QBattSingle(const QString& key);
    ~QBattSingle();

    bool isAnotherRunning();
    bool tryToRun();
    void release();

private:
    const QString key;
    const QString memLockKey;
    const QString sharedmemKey;

    QSharedMemory sharedMem;
    QSystemSemaphore memLock;

    Q_DISABLE_COPY(QBattSingle)
};

#endif // QBATTSINGLE_H
