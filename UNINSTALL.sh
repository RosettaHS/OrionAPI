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

### Startup

[ ! -e "UNINSTALL.sh" ] && { echo "OrionAPI : Please run inside the directory this script is located in."; exit 1; }

[ "$(id -u)" -ne 0 ] && { echo "OrionAPI : Please run as root."; exit 1; }

while getopts qy flag; do
	case $flag in
	q) QUIET=true;;
	y) AUTO_Y=true;;
	*) echo "OrionAPI | ERROR! : Invalid Operand. Aborting"; exit 1;;
	esac
done

### Execution

# Checks if OrionAPI is installed on this system. If false, quits the installer.
[ -e "/usr/lib/libO.so" ] || { echo "OrionAPI | ERROR! : OrionAPI is not installed on this system, cannot proceed!"; exit 1; }


if [ "$QUIET" = false ]; then
	clear
	echo "O       o                                      o  o        .oOOOo.                            Oo    OooOOo.  ooOoOOo"
	echo "o       O        o                            O  O        .O     o.        o                 o  O   O     \`O    O   "
	echo "O       o                          O          o  o        O       o                         O    o  o      O    o   "
	echo "o       o                         oOo         O  O        o       O                        oOooOoOo O     .o    O   "
	echo "o       O 'OoOo. O  'OoOo. .oOo    o   .oOoO' o  o        O       o \`OoOo. O  .oOo. 'OoOo. o      O oOooOO'     o   "
	echo "O       O  o   O o   o   O \`Ooo.   O   O   o  O  O        o       O  o     o  O   o  o   O O      o o           O   "
	echo "\`o     Oo  O   o O   O   o     O   o   o   O  o  o        \`o     O'  O     O  o   O  O   o o      O O           O   "
 	echo "\`OoooO'O  o   O o'  o   O \`OoO'   \`oO \`OoO'o Oo Oo        \`OoooO'   o     o' \`OoO'  o   O O.     O o'       ooOOoOo "
fi

echo ""

# Asks for permission to continue the removal process.
if [ "$AUTO_Y" = false ]; then
	printf "OrionAPI : OrionAPI is eligible to be uninstalled. Would you like to proceed? [Y/n] : "; read conf
	case ${conf} in
		[!Yy])
			echo "OrionAPI : Aborted."; exit 1;;
	esac
else
	echo "OrionAPI : OrionAPI is eligible to be uninstalled. Proceeding."
fi

# Removes the library file itself.
rm "/usr/lib/libO.so" || { echo 'OrionAPI | ERROR! : Failed to uninstall "libO.so"! Aborting.'; exit 1; }
echo 'OrionAPI : Uninstalled "libO.so"'

# Checks if the header directory exists, and if it does, removes it and its content.
if [ -d "/usr/include/.OrionAPI" ]; then
	rm -rf "/usr/include/.OrionAPI" || { echo "OrionAPI | ERROR! : Failed to uninstall OrionAPI system headers! Aborting."; exit 1; }
	echo "OrionAPI : Uninstalled OrionAPI system headers."

	rm "/usr/include/OrionAPI" || { echo "OrionAPI | ERROR! : Failed to uninstall OrionAPI master header! Aborting."; exit 1; }
	echo "OrionAPI : Uninstalled OrionAPI master header."
fi

# Checks if Xlib is still installed, and if it is, goes ahead and uninstalls it for you.
if dpkg -s "libx11-dev" > /dev/null 2>&1; then
	if [ "$AUTO_Y" = false ]; then
		printf "OrionAPI : Xlib was installed alongside OrionAPI and is no longer needed. Would you like to uninstall Xlib as well? [Y/n] : "; read conf
		case ${conf} in
			[Yy])
				echo 'OrionAPI : Working on uninstalling "libx11-dev".'
				apt-get purge "libx11-dev" -y || { echo 'OrionAPI | ERROR! : Failed to uninstall "libx11-dev"! Aborting.'; exit 1; }
				echo 'OrionAPI : "libx11-dev" successfully uninstalled.';;
		esac
	else
		echo "OrionAPI : Xlib was installed alongside OrionAPI and is no longer needed. Uninstalling."

		echo 'OrionAPI : Working on uninstalling "libx11-dev".'
		apt-get purge "libx11-dev" -y || { echo 'OrionAPI | ERROR! : Failed to uninstall "libx11-dev"! Aborting.'; exit 1; }
		printf "\nOrionAPI : \"libx11-dev\" successfully uninstalled.\n"
	fi
fi

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

printf "\nOrionAPI : OrionAPI has been successfully uninstalled.\n\n"