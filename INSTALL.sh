#!/bin/sh

###################################################################################
#                                                                                 #
#                               Copyright (c) 2021                                #
#                            Rosetta H&S International                            #
#                                                                                 #
#   Permission is hereby granted, free of charge, to any person obtaining a copy  #
#  of this software and associated documentation files (the "Software"), to deal  #
#   in the Software without restriction, including without limitation the right   #
#     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell   #
#        copies of the Software, and to permit persons to whom the Software is    #
#           furnished to do so, subject to the following conditions:              #
#                                                                                 #
#  The above copyright notice and this permission notice shall be included in all #
#                 copies or substantial portions of the Software.                 #
#                                                                                 #
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     #
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       #
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    #
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         #
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  #
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  #
#  SOFTWARE.                                                                      #
#                                                                                 #
###################################################################################

AUTO_Y=false
QUIET=false
PREV_INST=false

### Startup

[ ! -e "INSTALL.sh" ] && { echo "OrionAPI : Please run inside the directory this script is located in."; exit 1; }

[ "$(id -u)" -ne 0 ] && { echo "OrionAPI : Please run as root."; exit 1; }

while getopts qy flag; do
	case $flag in
	q) QUIET=true;;
	y) AUTO_Y=true;;
	*) echo "OrionAPI | ERROR! : Invalid Operand. Aborting"; exit 1;;
	esac
done

### Execution

if [ "$QUIET" = false ]; then
	clear
	echo "o          \`O        o                                                     .oOOOo.                            Oo    OooOOo.  ooOoOOo       oO"
	echo "O           o       O                                                     .O     o.        o                 o  O   O     \`O    O          OO"
	echo "o           O       o                                     O               O       o                         O    o  o      O    o          oO"
	echo "O           O       O                                    oOo              o       O                        oOooOoOo O     .o    O          Oo"
	echo "o     o     o .oOo. o  .oOo  .oOo. \`oOOoOO. .oOo.         o   .oOo.       O       o \`OoOo. O  .oOo. 'OoOo. o      O oOooOO'     o          oO"
	echo "O     O     O OooO' O  O     O   o  O  o  o OooO'         O   O   o       o       O  o     o  O   o  o   O O      o o           O            "
	echo "\`o   O o   O' O     o  o     o   O  o  O  O O             o   o   O       \`o     O'  O     O  o   O  O   o o      O O           O          Oo"
 	echo " \`OoO' \`OoO'  \`OoO' Oo \`OoO' \`OoO'  O  o  o \`OoO'         \`oO \`OoO'        \`OoooO'   o     o' \`OoO'  o   O O.     O o'       ooOOoOo       oO"
fi

echo ""

# Check if system has "make", and if not, install it.
if ! dpkg -s "make" >/dev/null 2>&1; then 
	if [ "$AUTO_Y" = true ]; then
		echo 'OrionAPI : Dependency "make" is not installed. Retrieving.'
	else
		printf 'OrionAPI  : Dependency "make" is not installed. It is required for OrionAPI to be compiled. Would you like to install this dependency? [Y/n] : '; read conf
		case ${conf} in
			[!Yy]) echo 'OrionAPI : Aborted.'; exit 1;;
		esac
	fi

	echo "OrionAPI : Working on installing Make."
	apt-get install "make" -y || { echo "OrionAPI | ERROR! : Make could not be installed! Aborting."; exit 1; }
	echo "OrionAPI : Make successfully installed."
fi

# Check if system has "libx11-dev", and if not, install it.
if ! dpkg -s "libx11-dev" >/dev/null 2>&1; then 
	if [ "$AUTO_Y" = true ]; then
		echo 'OrionAPI : Dependency "libx11-dev" is not installed. Retrieving.'
	else
		printf 'OrionAPI  : Dependency "libx11-dev" is not installed. It is required for OrionAPI to be compiled. Would you like to install this dependency? [Y/n] : '; read conf
		case ${conf} in
			[!Yy]) echo "OrionAPI : Aborted."; exit 1;;
		esac
	fi

	echo "OrionAPI : Working on installing Xlib." 
	apt-get install "libx11-dev" -y || { echo "OrionAPI | ERROR! : Xlib could not be installed! Aborting."; exit 1; }
	printf "\nOrionAPI : Xlib successfully installed.\n"
fi

# Check if OrionAPI currently exists on this system, and if it does, remove the current version. 
if [ -e "/usr/lib/libO.so" ]; then
	PREV_INST=true
	if [ "$AUTO_Y" = true ]; then
		echo "OrionAPI : OrionAPI is already installed, removing currently installed version for reinstall."
	else
		printf "OrionAPI : OrionAPI is already installed, would you like to reinstall? [Y/n] : "; read conf
		case ${conf} in
			[!Yy]) echo "OrionAPI : Aborted."; exit 1;;
		esac
	fi
		rm -rf "/usr/include/.OrionAPI" >/dev/null 2>&1 || echo 'OrionAPI | WARNING! : Current version include folder "/usr/include/.OrionAPI" not found, failed to remove.'
		rm "/usr/include/OrionAPI" >/dev/null 2>&1		|| echo 'OrionAPI | WARNING! : Current version header file "/usr/include/OrionAPI" not found, failed to remove.'
		rm "/usr/lib/libO.so" >/dev/null 2>&1			|| echo 'OrionAPI | WARNING! : Current version source library "/usr/lib/libO.so" not found, failed to remove.'
	echo "OrionAPI : Current version successfully removed."
fi

# Ready to be installed. If flag "-y" is not set, ask for permisson to proceed.
if [ "$AUTO_Y" = false -a "$PREV_INST" = false ]; then
	printf 'OrionAPI : OrionAPI is ready to be installed. Would you like to proceed? [Y/n] : '; read conf
	case ${conf} in
		[!Yy]) echo 'OrionAPI : Aborted.'; exit 1;;
	esac
fi

# Compiles the source code.
echo "OrionAPI : Compiling source code..."
make shared >/dev/null 2>&1 || { echo "OrionAPI : ERROR! Failed to compile source code! Aborting."; exit 1; }
echo "OrionAPI : Successfully compiled OKit source code."

# Moves it to the "/usr/lib" directory.
cp "libO.so" "/usr/lib/libO.so" || { echo 'OrionAPI : ERROR! Failed to copy compiled library as "/usr/lib/libO.so"! Aborting.'; exit 1; }
echo "OrionAPI : Copied OrionAPI (libO.so) to system libaries directory."

# Removes the compiled files from the current directory.
rm ./*.o ./*.so >/dev/null 2>&1

# Installs the header files from "src/include" into "/usr/include/.OrionAPI".
echo "OrionAPI : Working on installing header files..."
cp -R "src/include/" "/usr/include/.OrionAPI" || { echo 'OrionAPI : ERROR! Failed to copy header files into "/usr/include/.OrionAPI"! Aborting.'; exit 1; }

# Installs the redirect header "src/SYSHEADER" as "usr/include/OrionAPI".
cp "src/SYSHEADER" "/usr/include/OrionAPI" || { echo 'OrionAPI : ERROR! Failed to copy redirect header as "/usr/include/OrionAPI"! Aborting.'; exit 1; }
echo 'OrionAPI : Copied header files. Use "#include <OrionAPI>" in your C++ files to use these headers.'

# End ASCII
if [ "$QUIET" = false ]; then
	echo
	echo ".oOOOo.                                             oO"
	echo "o     o                                             OO"
	echo "O.                                                  oO"
	echo " \`OOoo.                                             Oo"
	echo "      \`O O   o  .oOo  .oOo  .oOo. .oOo  .oOo        oO"
	echo "       o o   O  O     O     OooO' \`Ooo. \`Ooo.         "
	echo "O.    .O O   o  o     o     O         O     O       Oo"
	echo " \`oooO'  \`OoO'o \`OoO' \`OoO' \`OoO' \`OoO' \`OoO'       oO"
	echo
fi

printf "\nOrionAPI : OrionAPI has been successfully installed. To compile your applications with the OrionAPI, simply use the library flag \"-lO\", and read the files in \"docs\" for more information.\n\n"