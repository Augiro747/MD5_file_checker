# MD5_file_checker

Данная программа выполняет проверку целостности файлов с помощью хеш функции MD5

##Использование

Программа поддерживает как запуск без параметров, так и с параметром пути файла, который необходимо проверить. 
С параметром она срабатывает 1 раз, без параметра входит в цикл, спрашивая после каждой итерациио продолжении.

Для начала необходимо обозначить файл, целостность которого будет отслеживаться, программа заносит его в хеш-таблицу и выводит "File added to hash table". 
Далее если вводить путь уже зарегестрированный в хеш-таблице, то при изменении файла программа уведомит об этом пользователя и предложит переписать
md5 хеш файла в хеш-таблице. Если же файл не изменялся, то программа выведет "File not changed".

Пример запуска из командной строки с параметром:

![WithParam](https://user-images.githubusercontent.com/39097804/228301930-1d926c15-c4a1-4c01-83b1-df05c12be0c7.png)

Пример запуска из командной строки без параметра:

![NoParam](https://user-images.githubusercontent.com/39097804/228302923-aeacb236-2af9-42a2-a3dc-498a0c755330.png)
