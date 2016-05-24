path=C:\devkitadv\bin

gcc  -o pacman.elf pacman.cpp  -lm

objcopy -O binary pacman.elf pacman.gba
