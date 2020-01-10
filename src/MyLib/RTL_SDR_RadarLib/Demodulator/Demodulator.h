/* Mode1090, a Mode S messages decoder for RTLSDR devices.
 *
 * Copyright (C) 2012 by Salvatore Sanfilippo <antirez@gmail.com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DEMODULATOR_H
#define DEMODULATOR_H

#include <QSharedPointer>
#include <QRunnable>

#include "demodulator_global.h"
#include "interface/IDemodulator.h"
#include "interface/IPoolObject.h"
#include "interface/ILogger.h"

#include "sdr_dev/include/constant.h"

/* The struct we use to store information about a decoded message. */
struct modesMessage
{
    /* Generic fields */
    unsigned char msg[MODES_LONG_MSG_BYTES]; /* Binary message. */
    int msgbits;                /* Number of bits in message */
    int msgtype;                /* Downlink format # */
    int crcok;                  /* True if CRC was valid */
    uint32_t crc;               /* Message CRC */
    int errorbit;               /* Bit corrected. -1 if no bit corrected. */
    int aa1, aa2, aa3;          /* ICAO Address bytes 1 2 and 3 */
    int phase_corrected;        /* True if phase correction was applied. */

    /* DF 11 */
    int ca;                     /* Responder capabilities. */

    /* DF 17 */
    int metype;                 /* Extended squitter message type. */
    int mesub;                  /* Extended squitter message subtype. */
    int heading_is_valid;
    int heading;
    int aircraft_type;
    int fflag;                  /* 1 = Odd, 0 = Even CPR message. */
    int tflag;                  /* UTC synchronized? */
    int raw_latitude;           /* Non decoded latitude */
    int raw_longitude;          /* Non decoded longitude */
    char flight[9];             /* 8 chars flight number. */
    int ew_dir;                 /* 0 = East, 1 = West. */
    int ew_velocity;            /* E/W velocity. */
    int ns_dir;                 /* 0 = North, 1 = South. */
    int ns_velocity;            /* N/S velocity. */
    int vert_rate_source;       /* Vertical rate source. */
    int vert_rate_sign;         /* Vertical rate sign. */
    int vert_rate;              /* Vertical rate. */
    int velocity;               /* Computed from EW and NS velocity. */

    /* DF4, DF5, DF20, DF21 */
    int fs;                     /* Flight status for DF4,5,20,21 */
    int dr;                     /* Request extraction of downlink request. */
    int um;                     /* Request extraction of downlink request. */
    int identity;               /* 13 bits identity (Squawk). */

    /* Fields used by multiple message types. */
    int altitude, unit;
};


class Aircraft;
/*!
 * \brief The Demodulator class
 * класс демодуляции. Реализация позаимствована у
 * Salvatore Sanfilippo <antirez@gmail.com>
 */
class DEMODULATORSHARED_EXPORT Demodulator : public IDemodulator
{
    QVector<uint16_t> _magnitude;
    QVector<uint32_t> icao_cache;

    QSharedPointer<IPoolObject> _pool;
    QSharedPointer<ILogger> _log;

    /* Statistics */
    uint64_t stat_valid_preamble = 0;
    uint64_t stat_demodulated = 0;
    uint64_t stat_goodcrc = 0;
    uint64_t stat_badcrc = 0;
    uint64_t stat_fixed = 0;
    uint64_t stat_single_bit_fix = 0;
    uint64_t stat_two_bits_fix = 0;
    uint64_t stat_out_of_phase = 0;

    /* Configuration */
    bool fix_errors = true;                 /* Single bit error correction if true. */
    bool check_crc = true;                  /* Only display messages with good CRC. */
    bool debug = false;                      /* Debugging mode. */

    int interactive_ttl = MODES_INTERACTIVE_TTL;            /* Interactive mode: TTL before deletion. */
    bool onlyaddr = false;                   /* Print only ICAO addresses. */
    bool metric = true;                     /* Use metric units. */
    bool aggressive = false;          /* Aggressive detection algorithm. */

    uint16_t* maglut;
public:
    Demodulator(QSharedPointer<IPoolObject> pool);
    ~Demodulator() override;
    /*!
     *  \brief внедрение зависимости модуля логгирования
     */
    void setLogger(QSharedPointer<ILogger> log) override { _log = log;}

    bool setDataForDemodulate(const QVector<uint8_t>& vector) override;
    void run() override;
    QByteArray getRawDump() override;
    bool demodulate() override;

    int32_t getCountObject() override;

private:
    /*!
     * \brief computeMagnitudeVector
     * Turn I/Q samples pointed by data into the magnitude vector
     * pointed by magnitude.
     * \param vector - исходный вестор
     * \param magnitude - вектор огибающей
     */
    void computeMagnitudeVector(const QVector<uint8_t> &vector,
                                QVector<uint16_t> &magnitude);
    /*!
     * \brief detectModeS
     * Detect a Mode S messages inside the magnitude buffer pointed by 'm' and of
     * size 'mlen' bytes. Every detected Mode S message is convert it into a
     * stream of bits and passed to the function to display it.
     * \param m - буфер даных
     * \param mlen - длинна буфера
     */
    void detectModeS(uint16_t *m, uint32_t mlen);

    /*!
     * \brief dumpRawMessage
     * This is a wrapper for dumpMagnitudeVector() that also show the message
     * in hex format with an additional description.
     *
     * \param descr  is the additional message to show to describe the dump.
     * \param msg    points to the decoded message
     * \param m      is the original magnitude vector
     * \param offset is the offset where the message starts
     */
    void dumpRawMessage(const QString &descr,
                        unsigned char *msg,
                        uint16_t *m,
                        uint32_t offset);
    /*!
     * \brief fixTwoBitsErrors
     * Similar to fixSingleBitErrors() but try every possible two bit combination.
     * This is very slow and should be tried only against DF17 messages that
     * don't pass the checksum, and only in Aggressive Mode.
     */
    int fixTwoBitsErrors(unsigned char *msg, int bits);
    /*!
     * \brief fixSingleBitErrors
     *  Try to fix single bit errors using the checksum. On success modifies
     * the original buffer with the fixed version, and returns the position
     * of the error bit. Otherwise if fixing failed -1 is returned.
     */
    int fixSingleBitErrors(unsigned char *msg, int bits);

    /*!
     * \brief Helper function for dumpMagnitudeVector().
     * It prints a single bar used to display raw signals.
     */
    void dumpMagnitudeBar(int index, int magnitude);

    /*!
     * \brief dumpMagnitudeVector
     * Display an ASCII-art alike graphical representation of the undecoded
     * message as a magnitude signal.
     *
     * The message starts at the specified offset in the "m" buffer.
     * The function will display enough data to cover a short 56 bit message.
     */
    void dumpMagnitudeVector(uint16_t *m, uint32_t offset);

    /*!
     * \brief detectOutOfPhase
     * Return -1 if the message is out of fase left-side
     * Return  1 if the message is out of fase right-size
     * Return  0 if the message is not particularly out of phase.
     */
    int detectOutOfPhase(uint16_t *m);

    /*!
     * \brief applyPhaseCorrection коррекция фазы
     * \param m буфер
     */
    void applyPhaseCorrection(uint16_t *m);

    /*!
     * \brief modesMessageLenByType
     * Given the Downlink Format (DF) of the message, return the message length
     * in bits
     */
    int modesMessageLenByType(int type);

    /*!
     * \brief decodeModesMessage
     * Decode a raw Mode S message demodulated as a stream of bytes by
     * detectModeS(), and split it into fields populating a modesMessage
     * structure.
     */
    void decodeModesMessage(modesMessage *mm, unsigned char *msg);

    /*!
     * \brief useModesMessage
     * Basically this function passes a raw message to the upper layers for
     * further processing and visualization.
     */
    void useModesMessage(modesMessage *mm);

    /*!
     * \brief modesChecksum - рассчет контрольной суммы
     */
    uint32_t modesChecksum(unsigned char *msg, int bits);

    /*!
     * \brief bruteForceAP
     * If the message type has the checksum xored with the ICAO address, try to
     * brute force it using a list of recently seen ICAO addresses.
     */
    int bruteForceAP(unsigned char *msg, modesMessage *mm);

    /*!
     * \brief addRecentlySeenICAOAddr
     *  Add the specified entry to the cache of recently seen ICAO addresses.
     * Note that we also add a timestamp so that we can make sure that the
     * entry is only valid for MODES_ICAO_CACHE_TTL seconds.
     */
    void addRecentlySeenICAOAddr(uint32_t addr);

    /*!
     * \brief ICAOCacheHashAddress
     * Hash the ICAO address to index our cache of MODES_ICAO_CACHE_LEN
     * elements, that is assumed to be a power of two.
     */
    uint32_t ICAOCacheHashAddress(uint32_t a);

    /*!
     * \brief decodeAC13Field
     * Decode the 13 bit AC altitude field (in DF 20 and others).
     * Returns the altitude, and set 'unit' to either MODES_UNIT_METERS
     * or MDOES_UNIT_FEETS
     */
    int decodeAC13Field(unsigned char *msg, int *unit);

    /*!
     * \brief decodeAC12Field
     * Decode the 12 bit AC altitude field (in DF 17 and others).
     * Returns the altitude or 0 if it can't be decoded.
     */
    int decodeAC12Field(unsigned char *msg, int *unit);

    /*!
     * \brief interactiveReceiveData
     * Receive new messages and populate the interactive mode with more info
     */
    void interactiveReceiveData(modesMessage *mm);

    /*!
     * \brief displayModesMessage
     * This function gets a decoded Mode S Message and prints it on the screen
     * in a human readable format.
     */
    void displayModesMessage(modesMessage *mm);

    /*!
     * \brief ICAOAddressWasRecentlySeen
     * Returns 1 if the specified ICAO address was seen in a DF format with
     * proper checksum (not xored with address) no more than * MODES_ICAO_CACHE_TTL
     * seconds ago. Otherwise returns 0.
     */
    int ICAOAddressWasRecentlySeen(uint32_t addr);

    /*!
     * \brief decodeCPR Decoding ADS-B position
     */
    void decodeCPR(QSharedPointer<Aircraft> a);

    /*!
     * \brief cprModFunction
     */
    int cprModFunction(int a, int b);

    /*!
     * \brief cprNLFunction
     * The NL function uses the precomputed table from 1090-WP-9-14
     */
    int cprNLFunction(double lat);

    /*!
     * \brief cprNFunction
     * The NL function uses the precomputed table from 1090-WP-9-14
     * whith odd
     */
    int cprNFunction(double lat, int isodd);

    /*!
     * \brief cprDlonFunction
     */
    double cprDlonFunction(double lat, int isodd);

    /*!
     * \brief getMEDescription get ME text
     */
    QString getMEDescription(int metype, int mesub);

    /*!
     * \brief interactiveRemoveStaleAircrafts
     * When in interactive mode If we don't receive new nessages within
     * MODES_INTERACTIVE_TTL seconds we remove the aircraft from the list.
     */
    void interactiveRemoveStaleAircrafts();

    /*!
     * \brief addDebugMsg add debug message in log
     */
    void addDebugMsg(const QString &str);
};

#endif // DEMODULATOR_H
