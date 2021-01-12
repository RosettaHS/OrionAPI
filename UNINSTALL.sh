#!/bin/sh

# -q for quiet means no ASCII art
# Both -s or -q could've worked for this flag so I picked -q
while
	getopts q option
do
	case $option in
	q) qflag=1;;
	# When an unrecognised flag is seen, just exit.  We could do a real error/usage message before exit, but I didn't.
	*) exit 1;;
	esac
done

if
	[ ! -e "UNINSTALL.sh" ]
then
	echo "Orion | OKit : Please run inside the directory this script is located in."
	exit 1
fi

if
	[ "$(id -u)" -ne 0 ]
then
	echo "Orion | OKit : Please run as root."
	exit 1
fi

if
	[ ! -e "/usr/lib/libOKit.so" ]
then
	echo "Orion | OKit : OKit is not installed. Aborting."
	exit 1
fi

# Do eligibility checks here!

if [ -z "$qflag" ]
then
	tput clear
	# Thanks Ron Fritz!
	echo
	echo "O       o                                      o  o        .oOOOo.  \`o    O          "
	echo "o       O        o                            O  O        .O     o.  o   O   o       "
	echo "O       o                          O          o  o        O       o  O  O         O  "
	echo "o       o                         oOo         O  O        o       O  oOo         oOo "
	echo "o       O 'OoOo. O  'OoOo. .oOo    o   .oOoO' o  o        O       o  o  o    O    o  "
	echo "O       O  o   O o   o   O \`Ooo.   O   O   o  O  O        o       O  O   O   o    O  "
	echo "\`o     Oo  O   o O   O   o     O   o   o   O  o  o        \`o     O'  o    o  O    o  "
	echo " \`OoooO'O  o   O o'  o   O \`OoO'   \`oO \`OoO'o Oo Oo        \`OoooO'   O     O o'   \`oO"
	echo
fi

printf "Orion | OKit : OKit is eligible to be uninstalled. Would you like to proceed? (y/N) : "
read conf
case ${conf} in
	[!Yy])
		echo "Orion | OKit : Aborted."
		exit 1;;
esac

if
	rm "/usr/lib/libOKit.so"
then
	echo 'Orion | OKit : Uninstalled "libOKit.so"'
else
	echo 'Orion | OKit : ERROR! Failed to uninstall "libOKit.so"! Aborting.'
	exit 1
fi

if [ -d "/usr/include/Orion/_OKit" ]
then
	if
		rm -rf "/usr/include/Orion/_OKit"
	then
		echo "Orion | OKit : Uninstalled OKit system headers."
	else
		echo "Orion | OKit : ERROR! Failed to uninstall OKit system headers! Aborting."
		exit 1
	fi

	if
		rm "/usr/include/Orion/OKit"
	then
		echo "Orion | OKit : Uninstalled OKit master header."
	else
		echo "Orion | OKit : ERROR! Failed to uninstall OKit master header! Aborting."
		exit 1
	fi

	if
		[ ! "$(ls -A /usr/include/Orion)" ]
	then
		rmdir "/usr/include/Orion/" && echo "Orion | OKit : Uninstalled Orion library directory. (Was empty.)"
	fi
fi

if dpkg -s "libx11-dev" > /dev/null 2>&1
then 
	printf "Orion | OKit : XLib was installed alongside OKit and is no longer needed. Would you like to uninstall Xlib as well? (y/N) : "
	read confb
	case ${confb} in
		y | Y)
			echo 'Orion | OKit : Working on uninstalling "libx11-dev".'
			if
				# sudo?  Aren't we already root by now?  Also, consider moving -y in front of the package argument rather than behind it.  On Unix (in shell scripts) option flags are traditionally placed as early as possible.
				apt-get purge "libx11-dev" -y
			then
				echo 'Orion | OKit : "libx11-dev" successfully uninstalled.'
			else
				echo 'Orion | OKit : ERROR! Failed to uninstall "libx11-dev"! Aborting.'
				exit 1
			fi
			;;
	esac
fi

if [ -z "$qflag" ]
then
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

echo
echo "Orion | OKit: OKit has been successfully uninstalled."
