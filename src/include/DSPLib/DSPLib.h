#ifndef DSPLIB_H
#define DSPLIB_H

#include <QRunnable>
#include <fftw3.h>
#include <QQueue>
#include <QMutex>

#include "DSPLib_global.h"
#include "interface/IDSP.h"

/*libfftw3-3*/
class DSPLIB_EXPORT DSPLib : public IDSP, public QRunnable
{
    fftw_complex *inbuf, *outbuf;
    fftw_plan plan;
    const int N = 8192;
    const int DEPTH = 5;
    QMutex  _mutex;
    QQueue <SrcDataAdc> _dataADC;
    SrcDataAdc data;
public:
    DSPLib();
    ~DSPLib() override;

    SrcDataAdc makeFFT(const QVector<uint8_t>& vector) override;
    SrcDataAdc makeMagnitude(const QVector<uint8_t>& vector) override;
    void makeAll(const QVector<uint8_t>& vector) override;
    void run() override;

    SrcDataAdc takeDataBlock() override;

    void lockDataBuffer() override { _mutex.lock(); }
    bool tryLockDataBuffer() override { return _mutex.tryLock(2);}
    void unlockDataBuffer() override { _mutex.unlock(); }

    bool isEmptyDataBuffer() override;
};

#endif // DSPLIB_H
