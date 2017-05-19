# dynamic-computational-nets
[![Build Status](https://travis-ci.org/goto1134/dynamic-computational-nets.svg?branch=master)](https://travis-ci.org/goto1134/dynamic-computational-nets)  [![DOI](https://zenodo.org/badge/22958/goto1134/dynamic-computational-nets.svg)](https://zenodo.org/badge/latestdoi/22958/goto1134/dynamic-computational-nets)

Программная реализация приложения, моделирующего Динамические вычислительные сети.

## Инструкция для настройки в CLion

1. Установить Qt.
2. Указать _MinGW Home_ в настройках со значением _<path-to-Qt>\Tools\mingw<version>_
3. Для каждого варианта билда на вкладке _CMake_ указать параметр _-DCMAKE_PREFIX_PATH=<path-to-Qt>/<Qt-version>/mingw53_32/lib/cmake_