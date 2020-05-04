#include "DSPLib/DSPLib.h"

DSPLib::DSPLib()
{
    inbuf = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*N);
    outbuf = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*N);
    plan = fftw_plan_dft_1d(N, (fftw_complex*)inbuf, (fftw_complex*)outbuf,
                            FFTW_FORWARD, FFTW_MEASURE);
    setAutoDelete(false);
}

DSPLib::~DSPLib()
{
    fftw_destroy_plan(plan);
    fftw_free(inbuf);
    fftw_free(outbuf);
}

SrcDataAdc DSPLib::makeFFT(const QVector<uint8_t> &vector)
{
    Q_UNUSED(vector);
}

SrcDataAdc DSPLib::makeMagnitude(const QVector<uint8_t> &vector)
{
    Q_UNUSED(vector);
}

SrcDataAdc DSPLib::makeAll(const QVector<uint8_t> &vector)
{
    SrcDataAdc data;
    int size = vector.size();

    if( size <= 0)
        return  SrcDataAdc();

    if( (size % 2) != 0)
        --size;

    data.rawDataVector.resize(N);
    data.fftVector.resize(N);
    data.magnitudeVector.resize(N);

    for(int i = 0, j = 0; i < N*2; i +=2, ++j )
    {
        //The magic aligment happens here: we have to change the phase of each next complex sample
        //by pi - this means that even numbered samples stay the same while odd numbered samples
        //get multiplied by -1 (thus rotated by pi in complex plane).
        //This gets us output spectrum shifted by half its size - just what we need to get the output right.
        const double multiplier = (j % 2 == 0 ? 1 : -1);

        std::complex<double> bfr_val(vector[i], vector[i+1]);
        bfr_val = (bfr_val - std::complex<double>(127.0, 127.0)) * multiplier;
        inbuf[j][0] = bfr_val.real();
        inbuf[j][1] = bfr_val.imag();

        data.rawDataVector[j] = std::complex<int8_t>(static_cast<int8_t>(vector.at(i) - INT8_MAX),
                                                     static_cast<int8_t>(vector.at(i+1) - INT8_MAX));
        double rel2 = data.rawDataVector[j].real() * data.rawDataVector[j].real();
        double img2 = data.rawDataVector[j].imag() * data.rawDataVector[j].imag();
        data.magnitudeVector[j] = sqrt(rel2+img2);
    }
    fftw_execute(plan);

    for (int i = 0; i < N; i++)
    {
        double re2 = outbuf[i][0] * outbuf[i][0];
        double im2 = outbuf[i][1] * outbuf[i][1];

      data.fftVector[i] = 10 * log10(sqrt( re2 + im2) / pow(2,7));
    }


    lockDataBuffer();
    if(_dataADC.size() < 3)
        _dataADC.enqueue(data);
    unlockDataBuffer();
    return data;
}

void DSPLib::run()
{

}

SrcDataAdc DSPLib::takeDataBlock()
{
    if(_dataADC.isEmpty())
        return  SrcDataAdc();

    return  _dataADC.dequeue();
}

bool DSPLib::isEmptyDataBuffer()
{
    return  _dataADC.isEmpty();
}
