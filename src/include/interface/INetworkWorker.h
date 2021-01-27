#ifndef INETWORKWORKER
#define INETWORKWORKER

#include <QObject>

class IPackageController;
class ILogger;
/*!
 * \brief The INetworkWorker class
 * Интерфейс класса для реализации сетевого взаимодействия
 * \author Данильченко Артём
 */
class INetworkWorker: public QObject
{
    Q_OBJECT

public:
    virtual ~INetworkWorker(){}
    /*!
     *  \brief внедрение зависимости модуля логгирования
     */
    virtual void setLogger(ILogger* log) = 0;
    /*!
     *  \brief внедрение зависимости модуля контроллера пакетов
     */
    virtual void setController(IPackageController*) = 0;

    /*!
     *   \brief Подключение к физическому устройству.
     *   \return результат операции
     */
    virtual bool connect(const QString&,
                         uint16_t ,
                         uint16_t timeout = 1000) = 0;

    /*!
     * \brief Проверка состояние подключения
     * \return есть подключение или нет
     */
    virtual bool isConnected() = 0;

    /*!
     * \brief отключение
     */
    virtual void disconnect() = 0;

public slots:
    /*!
     *  \brief Запись массива данных,
     *  используя ранее установленное подключение
     *  \param [in] byteArray - массив данных
     *  \return количество отправленных байт, -1 в случае ошибки
    */
    virtual int64_t writeDatagramm(const QByteArray &byteArray) = 0;
    /*!
     *  \brief Запись массива данных,
     *  без использования установленного ранее подключения.
     *  Либо использование udp рассылки
     *  \param [in] ip - IP адрес получателя
     *  \param [in] port - порт получателя
     *  \param [in] byteArray - массив данных
     *  \return количество отправленных байт, -1 в случае ошибки
    */
    virtual int64_t writeDatagramm(QString& ip,
                                   uint16_t port,
                                   const QByteArray &byteArray) = 0;

    /*!
     *  \brief Чтение массива данных
     *  \return количество прочтённых байт, -1 в случае ошибки
    */
    virtual int64_t readDatagramm() = 0;


};
#endif // INETWORKWORKER

