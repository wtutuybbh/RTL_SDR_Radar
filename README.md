# RTL_SDR_Radar

СПО для приема и демодуляции самолетных ответчиков на частоте 1090 МГц. 
Полученная информация выводится на картографическую проекцию основанную на openStreetMaps.

# RadarApp
АРМ "Пост контроля воздушного движения". Отвечает за приём, обработку и вывод информации о движении воздушных судов на картографическую проекцию, а так же дублирование данной информации в табличном представлении.
  Источником данных может выступать :
  - Приемник RTL-SDR v3
  - Сервер сбора данных от raspberry pi с подключенными к ним приемниками RTL-SDR v3.
![ezgif com-video-to-gif(1)](https://user-images.githubusercontent.com/34423525/74163125-f6beda00-4c32-11ea-9c2c-b9f83a039ed6.gif)

# RaspberryApp
Легковесное терминальное приложение для raspberry pi. Осуществляет приём, демодуляцию данных. 
Выполняет роль "выносного датчика". Полученная информация о самолётах упаковывается в бинарный формат (согласно внутреннему протоколу информационного сопряжения) и передается на сервер, для последующей обработки.
![Peek 2019-06-15 00-24](https://user-images.githubusercontent.com/34423525/59540975-1e28a000-8f08-11e9-9c11-43b84cbe69cf.gif)

# TechnologicSoft
Технологическое ПО, для анализа радиотехнической обстановки в эфире
  Предоставлет следующий функционал:
  - Вывод огибающей принятого сигнала
  - Вывод мгновенного спектра сигнала (используется библиотека http://www.fftw.org/ ver3)
  - Вывод огибающей в режиме накопления
  - Вывод огибающей в режиме пикового детектора
  - Перестройка частоты
  
![Видео пример](https://user-images.githubusercontent.com/34423525/106398953-f365c500-6426-11eb-8b0d-2f0889293256.mp4)

Для приема сигналов используется :
1. Приемник RTL-SDR v3 на базе Realtek RTL2832 
2. Антенна Харченко 

Инструкция по сборке:

  0. Окружение:
  
    ubuntu 20.04
    Qt  5.12.8. Для сборки требуется эта версия или же более позняя. 

  1. Установка требуемых библиотек :
  
    sudo apt install qt5-default  // или устновить нужную версию с сайта https://www.qt.io/download-qt-installer
    sudo apt install qtcreator 
    sudo apt install libfftw3-bin
    sudo apt install libfftw3-dev 
    sudo apt install libfftw3-double3
    sudo apt install libfftw3-long3
    sudo apt install libfftw3-quad3
    sudo apt install libfftw3-single3
    sudo apt install libusb-1.0-0
    sudo apt install libusb-1.0-0-dev

  2. Необходимо скопировать файл с правами на работу с устройством. Для этого перейдите в папку RTL_SDR_Radar и выполните следующую команду:
  
    sudo cp ./import/sdr_dev/rtl-sdr.rules /etc/udev/rules.d/
    
  3. Далее нужно внести в blacklist драйвер по умолчанию, который загружается для TV устройства, т.к. он не работает как SDR и будет конфликтовать с новым драйвером. 
    В директории /etc/modprobe.d создаем новый файл blacklist-rtl.conf или просто дописываем в конец файла blacklist.conf строку : blacklist dvb_usb_rtl28xxu. 
    После чего необходимо перезагрузить компьютер.
  4. Перейти в папку RTL_SDR_Radar/scripts/debug или RTL_SDR_Radar/scripts/release и выполнить скрипт builder.sh

  5. Для запуска приложений необходимо использовать скрипты startRadarApp.sh, startRaspberryApp.sh, startTechSoft.sh

  6. При запуске через qtCreator ОБЯЗАТЕЛЬНО необходимо отключить теневую сборку.

Информация для изучения:
http://sdr.osmocom.org/trac/wiki/rtl-sdr
