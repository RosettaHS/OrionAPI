#!/bin/bash

[ ! -e "INSTALL.sh" ] && echo "Orion | OKit : Please run inside the directory this script is located in." && exit

[ "$EUID" -ne 0 ] && echo "Orion | OKit : Please run as root." && exit

clear
# Thanks Ron Fritz!
echo ""
echo "o          \`O        o                                                     .oOOOo.  \`o    O          "
echo "O           o       O                                                     .O     o.  o   O   o       "
echo "o           O       o                                     O               O       o  O  O         O  "
echo "O           O       O                                    oOo              o       O  oOo         oOo "
echo "o     o     o .oOo. o  .oOo  .oOo. \`oOOoOO. .oOo.         o   .oOo.       O       o  o  o    O    o  "
echo "O     O     O OooO' O  O     O   o  O  o  o OooO'         O   O   o       o       O  O   O   o    O  "
echo "\`o   O o   O' O     o  o     o   O  o  O  O O             o   o   O       \`o     O'  o    o  O    o  "
echo "\`OoO' \`OoO'  \`OoO' Oo \`OoO' \`OoO'  O  o  o \`OoO'         \`oO \`OoO'        \`OoooO'   O     O o'   \`oO"
echo ""
                                                                                                     

[ ! -d "/usr/include/Orion" ] && mkdir "/usr/include/Orion"

if [ -e "/usr/lib/libOKit.so" ]; then
	read -p "Orion | OKit is already installed, would you like to re-install? (Y\N) : " conf
	case ${conf} in
		n) echo "Orion | OKit : Aborted."; exit;;
		N) echo "Orion | OKit : Aborted."; exit;;
	esac
	rm -rf "/usr/include/Orion/_OKit"
	rm "/usr/include/Orion/OKit"
	rm "/usr/lib/libOKit.so"
	echo "Orion | OKit : Reinstalling OKit."
fi

if ! dpkg -s "make" &>/dev/null; then 
	read -p "Orion | OKit : \"make\" is not installed. It is required for OKit to be compiled. Would you like to install this dependency? (Y\N) : " conf
	case ${conf} in
		n) echo "Orion | OKit : Aborted."; exit;;
		N) echo "Orion | OKit : Aborted."; exit;;
	esac
	echo "Orion | OKit : Working on installing Make." & sudo apt-get install "make" -y && echo "Orion | OKit : Make successfully installed." || { echo "Orion | OKit : ERROR! Make could not be installed! Aborting."; exit; }
fi

if ! dpkg -s "libx11-dev" &>/dev/null; then 
	read -p "Orion | OKit : \"libx11-dev\" is not installed. It is required for OKit to function. Would you like to install this dependency? (Y\N) : " conf
	case ${conf} in
		n) echo "Orion | OKit : Aborted."; exit;;
		N) echo "Orion | OKit : Aborted."; exit;;
	esac
	echo "Orion | OKit : Working on installing XLib." & sudo apt-get install "libx11-dev" -y && echo "Orion | OKit : XLib successfully installed." || { echo "Orion | OKit : ERROR! XLib could not be installed! Aborting."; exit; }
fi

read -p "Orion | OKit : OKit is ready to be installed. Would you like to proceed? (Y\N) : " conf
case ${conf} in
	n) echo "Orion | OKit : Aborted."; exit;;
	N) echo "Orion | OKit : Aborted."; exit;;
esac

echo "Orion | OKit : Working on installing OKit."

cp -R "src/include/" "/usr/include/Orion/_OKit" || { echo "Orion | OKit : ERROR! Failed to copy header files into \"/usr/include/Orion/_OKit\"! Aborting."; exit; }
cp "src/SYSHEADER" "/usr/include/Orion/OKit" && echo "Orion | OKit : Copied header files. Use \"#include <Orion/OKit>\" in your C++ files to use these headers. " || { echo "Orion | OKit : ERROR! Failed to copy redirect header as \"/usr/include/Orion/OKit\"! Aborting."; exit; }

echo "Orion | OKit : Compiling source code..."
make shared >>/dev/null && echo "Orion | OKit : Successfully compiled OKit source code." || { echo "Orion | OKit : ERROR! Failed to compile source code! Aborting."; exit; }
cp "libOKit.so" "/usr/lib/libOKit.so" && echo "Orion | OKit : Copied OKit library to system libaries directory." || { echo "Orion | OKit : ERROR! Failed to copy compiled library as \"/usr/lib/libOKit.so\"! Aborting."; exit; }
rm *.o *.so &>/dev/null

echo ""
echo ".oOOOo.                                             oO"
echo "o     o                                             OO"
echo "O.                                                  oO"
echo " \`OOoo.                                             Oo"
echo "      \`O O   o  .oOo  .oOo  .oOo. .oOo  .oOo        oO"
echo "       o o   O  O     O     OooO' \`Ooo. \`Ooo.         "
echo "O.    .O O   o  o     o     O         O     O       Oo"
echo " \`oooO'  \`OoO'o \`OoO' \`OoO' \`OoO' \`OoO' \`OoO'       oO"
echo ""
echo ""                                                      
echo "Orion | OKit : OKit has been successfully installed. To compile your applications with OKit, simply use the library flag \"-lOKit\", and read the files in \"docs\" for more information."