#!/bin/bash

[ ! -e "UNINSTALL.sh" ] && echo "Orion | OKit : Please run inside the directory this script is located in." && exit

[ "$EUID" -ne 0 ] && echo "Orion | OKit : Please run as root." && exit

[ ! -e "/usr/lib/libOKit.so" ] && echo "Orion | OKit : OKit is not installed. Aborting." && exit

#Do eligibility checks here!

clear
# Thanks Ron Fritz!
echo ""
echo "O       o                                      o  o        .oOOOo.  \`o    O          "
echo "o       O        o                            O  O        .O     o.  o   O   o       "
echo "O       o                          O          o  o        O       o  O  O         O  "
echo "o       o                         oOo         O  O        o       O  oOo         oOo "
echo "o       O 'OoOo. O  'OoOo. .oOo    o   .oOoO' o  o        O       o  o  o    O    o  "
echo "O       O  o   O o   o   O \`Ooo.   O   O   o  O  O        o       O  O   O   o    O  "
echo "\`o     Oo  O   o O   O   o     O   o   o   O  o  o        \`o     O'  o    o  O    o  "
echo " \`OoooO'O  o   O o'  o   O \`OoO'   \`oO \`OoO'o Oo Oo        \`OoooO'   O     O o'   \`oO"
echo ""
read -p "Orion | OKit : OKit is eligible to be uninstalled. Would you like to proceed? (Y/N) : " conf
case ${conf} in
	n) echo "Orion | OKit : Aborted."; exit;;
	N) echo "Orion | OKit : Aborted."; exit;;
esac

rm "/usr/lib/libOKit.so" && echo "Orion | OKit : Uninstalled \"libOKit.so\"" || { echo "Orion | OKit : ERROR! Failed to uninstall \"libOKit.so\"! Aborting."; exit; }

if [ -d "/usr/include/Orion/_OKit" ]; then
	rm -rf "/usr/include/Orion/_OKit" && echo "Orion | OKit : Uninstalled OKit system headers." || { echo "Orion | OKit : ERROR! Failed to uninstall OKit system headers! Aborting."; exit; }
	rm "/usr/include/Orion/OKit" && echo "Orion | OKit : Uninstalled OKit master header." || { echo "Orion | OKit : ERROR! Failed to uninstall OKit master header! Aborting."; exit; }
	[ ! "$(ls -A /usr/include/Orion)" ] && rmdir "/usr/include/Orion/" && echo "Orion | OKit : Uninstalled Orion library directory. (Was empty.)"
fi

if dpkg -s "libx11-dev" &>/dev/null; then 
	read -p "Orion | OKit : XLib was installed alongside OKit and is no longer needed. Would you like to uninstall Xlib as well? (Y/N) : " conf
	case ${conf} in
		y | Y | "")
			echo "Orion | OKit : Working on uninstalling \"libx11-dev\"." & sudo apt-get purge "libx11-dev" -y && echo "Orion | OKit : \"libx11-dev\" successfully uninstalled." || { echo "Orion | OKit : ERROR! Failed to uninstall \"libx11-dev\"! Aborting."; exit; };;
	esac
fi

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
echo "Orion | OKit: OKit has been successfully uninstalled."