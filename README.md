Pre-requisites to running the program -- <br>
**SFML 2.6.1(IT WILL NOT WORK WITH SFML 3) -- <br>


FOR WINDOWS:
--> Unless you have set up the SFML library before, use WSL. Don't bother with path shenanigans on Windows. <br>


--> For compiling the project, use the following command: <br>

-----> g++ *.cpp -o main -lsfml-graphics -lsfml-window -lsfml-system (inside the root folder, /Raycasting) <br>


---> For running the project, use: <br>

-----> ./main <br>

FOR LINUX: <br>

--> Remove your current sfml build: <br>

sudo pacman -R sfml (OR EQUIVALENT FOR YOUR DISTRO) <br>

--> then to download 2.6.1 specifically: <br>

sudo pacman -U https://archive.archlinux.org/packages/s/sfml/sfml-2.6.1-1-x86_64.pkg.tar.zst (OR EQUIVALENT IN YOUR DISTRO LOOK IT UP DUDE) <br>

then in /etc/pacman.conf, add sfml to IGNOREPKG line. <br>


>>> For compiling the project: <br>

g++ *.cpp -o main -lsfml-graphics -lsfml-window -lsfml-system (inside the root folder, /Raycasting) <br>


>>> For running the project: <br>

 ./main <br>


AND THERE YOU HAVE IT, DOOM64 BUT SUCKS VERISON (IT IS NOT LIKE DOOM64 AT ALL, THAT GAME DOESNT EVEN USE RAYCASTING) <br>



CREDITS -> RAUF, HASSAN <br>
