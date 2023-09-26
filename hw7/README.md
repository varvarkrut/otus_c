# Домашнее задание

Найти и исправить утечки памяти в программе из приложенного архива.
Требование - найдена хоть одна из утечек памяти.


# Решение

Приложен файл patch, исправляющий одну из утечек памяти:
```asm
==75827== 14 bytes in 2 blocks are definitely lost in loss record 12 of 285
==75827==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==75827==    by 0x14294B: strdup (strdup.c:24)
==75827==    by 0x1214DB: clib_package_install (clib-package.c:1383)
==75827==    by 0x11E3D7: install_packages (clib-package.c:374)
==75827==    by 0x121FA2: clib_package_install_dependencies (clib-package.c:1615)
==75827==    by 0x121E51: clib_package_install (clib-package.c:1579)
==75827==    by 0x11CB3F: main (package-install.c:84)
```