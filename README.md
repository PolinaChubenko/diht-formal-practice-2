# Practical task on Earley and LR(1) parsers

### Задание:
Реализовать алгоритмы Эрли и LR(1), которые проверяют, выводится ли слово в заданной контекстно-свободной грамматике.


#### Для сборки проекта:

- Выполните установку пакетов boost:
    - Ubuntu: ```sudo apt-get install libboost-all-dev```
    - MacOS: ```brew install boost```

- Выполните установку openssl:
    - Ubuntu: ```sudo apt-get install openssl libssl-dev```
    - MacOS: ```brew install openssl@1.1```

- Создайте папку build, перейдите в нее

- Выполните ```cmake -DCMAKE_BUILD_TYPE=Debug ..```

- После этого найдите версию интерпретатора python в выводе, выполните:
  ```./install_deps.sh python<your python version> (3.6, 3.8, …)```

- Далее запустите сборку проекта: ```make -j<num cores + 1>```

- В папке bin появятся исполняемый файл, библиотека .so и файл для прогона тестов

- Для запуска покрытия тестами выполните: ```make coverage_report```

- В папке build появится html-файл с покрытием кода тестами


*Задание выполнила Чубенко Полина, Б05-022*