Preequisites to running the program --
**SFML 2.6.1(IT WILL NOT WORK WITH SFML 3) --
If testing on LINUX: Remove your current sfml build: sudo pacman -R sfml (OR EQUIVALENT FOR YOUR DISTRO)
then to download 2.6.1 specifically, sudo pacman -U https://archive.archlinux.org/packages/s/sfml/sfml-2.6.1-1-x86_64.pkg.tar.zst (OR EQUIVALENT IN YOUR DISTRO LOOK IT UP DUDE)
in /etc/pacman.conf, add sfml to IGNOREPKG line.

FOR WINDOWS: I HAVE NO IDEA HOW YOU CAN MAKE IT RUN, SO GOOD LUCK THERE

TO RUN THE PROGRAM:
g++ *.cpp -o raycast -lsfml-graphics -lsfml-window -lsfml-system (inside the root folder, /Raycasting)
then, ./raycast.exe (INSIDE THE ROOT FOLDER /Raycasting)

AND THERE YOU HAVE IT DOOM64 BUT THE SUCKS VERSION.

THANKS FOR READING MAN.

CREDITS(RAUF, HASSAN)
