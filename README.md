# tangoOS

<a><img src="https://img.shields.io/github/languages/code-size/9xbt/tangoOS?style=for-the-badge&logo=files"/></a>
<a href="https://github.com/9xbt/tangoOS/blob/master/LICENSE"><img src="https://img.shields.io/github/license/9xbt/tangoOS?style=for-the-badge&logo=mozilla"/></a>
<img src="https://img.shields.io/badge/i_love-C_so_much-blue?style=for-the-badge&logo=c">
<img src="https://img.shields.io/badge/assembly-sucks-red?style=for-the-badge&logo=intel">

tangoOS is your average 32-bit OS written in C.

## Roadmap

### Tables
- [X] GDT
- [x] IDT

### Drivers
- [X] VGA
- [X] PIC
- [X] Serial (QEMU debugcon)
- [X] RTC
- [X] PS/2 keyboard
- [ ] PCI
- [X] PMM
- [ ] Heap
- [X] Basic ATA driver
- [ ] ext2 filesystem
- [ ] elf32 program execution

### Libraries
- [X] `printf()`, `vprintf()`, `dprintf()`
- [X] `panic()`
- [X] `memcpy()`, `memset()`, `memmove()`, `memcpy()`
- [X] `strlen()`, `strcmp()`, `strncmp()`, `strcpy()`

### Shell (built-in commands for now)
- [X] about
- [X] prompt
- [X] credits
- [X] help
- [X] uname
- [X] echo
- [X] date
- [X] clear