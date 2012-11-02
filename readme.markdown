﻿# BRus MUD Engine readme.

Переработано и дополнено с инструкции за авторством *Кродо*, доступной по адресу [http://www.mud.ru/?coder][1]

- - -

## Содержание:
### I. Visual Studio C++: 
1. _Сборка под VSC++._

### II. Сygwin
1. _Сборка под Cygwin._
2. _Управление репозиторием из Cygwin._

### III. CentOS 6.2
1. _Сборка под CentOS 6.2._

- - -

## I. Visual Studio C++.
**Cборка в VSC++.**

1. Для сборки мада вам как минимум понадобятся:
     * Современный C++ компилятор
     * boost >=1.49.0
     * Python >=2.6
     * Zlib
     * Mercurial (http://mercurial.selenic.com/)

2. Устанавливаем boost, кроме заголовочных файлов понадобится библиотека boost_python, убеждаемся, что буст виден студией.

3. Клонируем репозиторий (предполагается, что меркуриал уже установлен):

	hg clone https://bitbucket.org/Posvist/mud/

4. Создаем чистый проект, идем в Project->Add existing Item и добавляем все файлы из mud\\src\\.

5. В Project->Properties->General выставляем Output Directory в mud\\bin.

6. После чего жмем Build->Build Solution и все должно собраться. 

7. Запускаем скрипт winprep.bat, чтобы создать всенужные для работы мада каталоги.

8. Переименовываем папку lib.template => lib

9. Идем в bin, запускаем circle.exe и радуемся жизни.

- - -

##II. Cygwin.
**II. 1. Cборка в Cygwin.**

Этот вариант более мутный, зато приближенный к реалиям на сервере.

1. Качаем и запускаем [http://www.cygwin.com/setup.exe][2].

2. Добираемся до списка доступных пакетов, переключаем справа вверху кнопку View на Full и отмечаем следующие пакеты (слева вверху есть поле поиска по названию):
	* gcc4-core
	* gcc4-g++
	* libboost-devel
	* libboost 1.48
	* libboost_python 1.48
	* make
	* mercurial
	* python
	* zlib
	* zlib-devel

     Плюс все, что там было отмечено по умолчанию, отметилось при выборе этих пакетов и рекомендует выбрать после, качаем, ставим.

3. Запускаем cygwin, далее в командной строке пишем дословно:

        hg clone https://bitbucket.org/Posvist/mud/
После чего у вас должна создаться директория mud в которой будет последний код (mud/src). Для обновления кода в будущем всего этого повторять не надо, достаточно будет запустить cygwin, зайти в эту директорию mud ("``cd mud``") и набрать "``hg pull -u``". Физически все это будет лежать в ..\\cygwin\\home\\ваше-имя\\mud\\.

    Если видим сообщения типа:
         
        warning: bitbucket.org certificate with fingerprint XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX not verified (check hostfingerprints or web.cacerts config setting)
        
    И эти сообщения Вас не раздражают, то переходим к пункту 4.

	Иначе:
      
	* идем в "..\cygwin\home\<Имя_пользователя_ОС>\mud\\.hg\".
	* открываем текстовым редактором файл "hgrc".
	* и приводим его в соответствующий описанию ниже вид:

            [paths]
            default = https://bitbucket.org/Posvist/mud``

            [hostfingerprints]
            bitbucket.org = XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX

4. Находясь в cygwin'е заходим в src (что-то вроде "``cd mud/src``"), пишем "``make test``" и радуемся, если все собирается без приключений.

5. Подготавливаем папку lib: "``cd ..``", "``./prepare``"

6. И наконец - "``bin/circle``" - это оно и есть, 4000 порт ждет вас.
- - -
**II. 2. Работа с репозиторием из Cygwin.**

И так. Вы решили что-либо исправить в коде или что-нибудь дописать туда новое? Ок, тогда давайте проверим все ли для этого готово:
Если: у Вас установлен Cygwin, через него клонирован репозиторий Былин, и корректно собран и запускается сервер – то все отлично, можно продолжать.

1. Откройте веб-браузер и зайдите по адресу: [https://bitbucket.org][3].

2. Зарегистрируйтесь в системе Bitbucket(Sing up).

3. Пройдите по адресу: [https://bitbucket.org/Posvist/mud/fork][4].

4. Ничего здесь не меняем, а в самом низу нажимаем “Fork repository”.

5. После того, как сформируется «вилка» Былин найдите на странице строчку нижеследующего вида:

	`hg clone https://xxxxxx@bitbucket.org/xxxxxx/mud`

	Вместо иксов там будет имя Вашего аккаунта на сайте.

6. Запускаем Cygwin и пишем: "``mkdir fork``", "``cd fork``".

	Затем вводим/копируем туда строчку с веб-страницы, которая начинается с hg clone…

7. Теперь по адресу «..\cygwin\home\<Имя_пользователя_ОС>\fork\mud\src» будет лежать Ваша рабочая версия кода.

8. Чтобы собрать код, который Вы поменяли, делаем все так же, как и при обычной сборке, тоесть:
 
	 * запускаете Сygwin.
	 * "``cd fork/mud/src``".
	 * "``make test``".
	 * ждете пока соберется движок.
	 * "``cd ..``".
	 * "``bin/circle``".

9. Далее, если Вы сделали в коде Вашей рабочей копии(..\fork\mud) какие-либо изменения и хотите, чтобы их применили на официальном сервере, то делать надо следующее:
 
	 * создаете в директори (..\fork\mud) текстовой файл commit.txt.
	 * описываете в нем изменения, которые вы сделали.
	 * сохраняете файл(commit.txt) в кодировке 65001 (UTF - 8).
	 * запускаете Cygwin.
	 * "``cd fork/mud``".
	 * "``hg commit -l commit.txt -u "UNAME <UEMAIL>"``",
		что является первым способом, с использованием текстового файла.
		
		Второй же способ - прямая передача сообщения:
		
		"``hg commit -m "UMESSAGE" -u "UNAME <UEMAIL>"``".
 
		UNAME – должно быть Вашим именем на сайте bitbucket.com.
		
		UEMAIL – должен быть Вашей почтой, указанной при регистрации на bitbucket.com.
		
		UMESSAGE - должно содержать написанное Вами сообщение об изменениях.

	 * После того, как добавили коммит вводим: "``hg push``".

10. Пройдите на Ваш репозиторий на bitbucket’е, там уже должна была появится запись с изменениями, которые Вы сделали в коде.
Если запись не появилась, то попробуйте все сделать с начала со вниманием и не забудьте выполнить перед этим команду: "``hg rollback``" – чтобы откатить запись коммита в логе.

11. И так. Запись появилась, и теперь Вы хотите применить изменения в официальном коде Былин. Делаете следующее:
	- пройдите на свой репозиторий на bitbucked.com.
	- сверху справа будет большая кнопка «Create pull request», нажимайте на неё.
	- описывайте изменения в Title и Description и нажимайте на кнопку внизу «Send pull request». После этого на официальном репозитории, во вкладке «Pull requests» должна появится Ваша заявка.Теперь остается только ждать, когда её одобрят старшие админы.

12. Чтобы перенести новые изменения из официального репозитория в свой рабочий, делаем следующее:
	
	* запускаете Cygwin.
	* cd fork/mud
	* hg pull -r default https://bitbucket.org/Posvist/mud
	* hg update
	* hg push

- - -

##III. CentOS 6.2.
**II. 1. Cборка под CentOS 6.2.**

Дистрибутив minimal является достаточным для сборки кода Былин, значительно уменьшая объем трафика, скачанного из инета.
После того как вы получили готовую систему и зашли в нее под root-ом, делаем следующее:

1. Настройка сети:
В файле /etc/sysconfig/network-scripts/ifcfg-ethN (где вместо N чаще всего 0) добавляем\изменяем строки.
Из текстовых редакторов доступен только довольно своеобразный в современном понимании редактор vi.
Минимальная последовательность действий:
# vi /etc/sysconfig/network-scripts/ifcfg-eth0
[Нажать кнопку i для входа в режим INSERT]
[Поправить конфиг в нужную сторону (см. ниже)]
[Нажать ESC для выхода из режима INSERT]
# :wq

Конфиг редактируем в зависимости от сети, в которой находится Ваш сервер...
### В случае наличия DHCP в сети:
> ONBOOT="yes"
> BOOTPROTO="dhcp"
### В остальных случаях:
> BOOTPROTO="static"
> ONBOOT="yes"
> TYPE="Ethernet"
> IPADDR="IP.адрес"			- ip-адрес сервера (4 цифры от 0 до 254)
> NETMASK="255.255.255.0"	- маска сети
> GATEWAY="IP.адрес"		- ip-адрес шлюза
Кроме того, если у Вас нет DHCP, необходимо указать адрес DNS-сервера в файле /etc/resolv.conf

После чего выполняем перезапуск службы сети.
# service network restart

2. Удаляем и ставим пакеты
# yum remove boost* (на случай если Вы все же взяли не minimal)
Ставим обязательные для сборки всего и вся пакеты
# yum install wget make mercurial gcc-c++ python-devel zlib-devel bzip2-devel python-devel
А также некоторые вещи для удобства (можно добавить что-то еще по вкусу)
# yum install mc nano ftp

3. Достаем исходники из репозитория:
# cd ~
# hg clone https://bitbucket.org/Posvist/mud
В результате чего появится папка ~/mud

4. Ставим более свежую, чем в репозиториях, версию Boost-а (>=1.49)
Достаем дистрибутив минимально подходящей для нас версии Boost-а:
# wget http://sourceforge.net/projects/boost/files/boost/1.49.0/boost_1_49_0.tar.bz2/download
Распаковываем в /usr/local/src
Заходим в появившуюся папку boost_1_49_0 и выполняем следующие команды:
# ./bootstrap.sh
# ./b2
# ./b2 install

5. Вбиваем костыль в Boost:
# yum install boost-python
# ln -s /usr/lib/libboost_python-mt.so.5 /usr/lib/libboost_python-mt.so
И два раза бьем в бубен, повернувшись спиной к закату.

6. Отключаем и убираем из автозагрузки iptables (фаервол)
# /etc/init.d/iptables stop
# chkconfig --level 0123456 iptables off

7. Заканчиваем: компилируем, готовим к запуску, запускаем
# cd ~/mud/src
# make test
# cd ..
# ./prepare
# bin/circle

- - -

[1]: http://www.mud.ru/?coder
[2]: http://www.cygwin.com/setup.exe
[3]: https://bitbucket.org.
[4]: https://bitbucket.org/Posvist/mud/fork