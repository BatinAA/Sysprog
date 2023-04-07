# sysprog
Лабораторные работы по системному программированию за 10 семестр.
## 1dz
В данной папке лежит три задания.
1. Обмен данными между именнованными пайпами (pipes).
2. Перехват сигналов и демонстрация факта того, какие сигналы перехватить нельзя.
3. Работа с сокетами. Запуск сервера и клиентской части, а также обмен данными.
## 2dz
Пока отсутствует. В данном задании производится написание индивидульного linux kernel module, его установка и выгрузка.

В общем и целом, kernel modules - это часть кода, которая может быть загружена или установлена в ядро linux или же выгружена из него. Такие модули расширяют возможности использования ОС linux и позволяют конфигурировать работу системы непосредственно под себя без необходимости ее перезагрузки.

Узнать подробнее о том, как создавать linux kernel modules, можно перейдя по ссылке:
https://sysprog21.github.io/lkmpg/

To create a kernel module, you can read The Linux Kernel Module Programming Guide. A module can be configured as built-in or loadable. To dynamically load or remove a module, it has to be configured as a loadable module in the kernel configuration (the line related to the module will therefore display the letter M).

To rebuild a kernel module automatically when a new kernel is installed, see Dynamic Kernel Module Support (DKMS).
## 3dz 
Character device linux kernel.
##
```stl
solid cube_corner
  facet normal 0.0 -1.0 0.0
    outer loop
      vertex 0.0 0.0 0.0
      vertex 1.0 0.0 0.0
      vertex 0.0 0.0 1.0
    endloop
  endfacet
  facet normal 0.0 0.0 -1.0
    outer loop
      vertex 0.0 0.0 0.0
      vertex 0.0 1.0 0.0
      vertex 1.0 0.0 0.0
    endloop
  endfacet
  facet normal -1.0 0.0 0.0
    outer loop
      vertex 0.0 0.0 0.0
      vertex 0.0 0.0 1.0
      vertex 0.0 1.0 0.0
    endloop
  endfacet
  facet normal 0.577 0.577 0.577
    outer loop
      vertex 1.0 0.0 0.0
      vertex 0.0 1.0 0.0
      vertex 0.0 0.0 1.0
    endloop
  endfacet
endsolid
```
