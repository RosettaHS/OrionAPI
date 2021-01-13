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

if [ ! -e "INSTALL.sh" ]; then
	echo "Orion | OKit : Please run inside the directory this script is located in."
	exit 1
fi

if [ "$(id -u)" -ne 0 ]; then
	echo "Orion | OKit : Please run as root."
	exit 1
fi

if [ -z "$qflag" ]; then
	tput clear
	# Thanks Ron Fritz!
	echo
	echo "o          \`O        o                                                     .oOOOo.  \`o    O          "
	echo "O           o       O                                                     .O     o.  o   O   o       "
	echo "o           O       o                                     O               O       o  O  O         O  "
	echo "O           O       O                                    oOo              o       O  oOo         oOo "
	echo "o     o     o .oOo. o  .oOo  .oOo. \`oOOoOO. .oOo.         o   .oOo.       O       o  o  o    O    o  "
	echo "O     O     O OooO' O  O     O   o  O  o  o OooO'         O   O   o       o       O  O   O   o    O  "
	echo "\`o   O o   O' O     o  o     o   O  o  O  O O             o   o   O       \`o     O'  o    o  O    o  "
	echo "\`OoO' \`OoO'  \`OoO' Oo \`OoO' \`OoO'  O  o  o \`OoO'         \`oO \`OoO'        \`OoooO'   O     O o'   \`oO"
	echo
fi
                                                                                                     

[ -d "/usr/include/Orion" ] || mkdir "/usr/include/Orion"

if [ -e "/usr/lib/libOKit.so" ]; then
	printf "Orion | OKit is already installed, would you like to re-install? (y/N) : "
	read conf
	case ${conf} in
		[!Yy]) echo "Orion | OKit : Aborted."
			exit 1;;
	esac
	# There's no error checking here in the next three rm commands...?  Just checking to see if it's intentional or not.  I don't think error-checking would be 100% necessary as you'll install over these files anyway.  Still would be nice, though.
	rm -rf "/usr/include/Orion/_OKit"
	rm "/usr/include/Orion/OKit"
	rm "/usr/lib/libOKit.so"
	echo "Orion | OKit : Reinstalling OKit."
fi

# Instead of using dpkg for this test, you should see if a test like
# command -v make
# works or not.
if ! dpkg -s "make" >/dev/null 2>&1; then 
	printf 'Orion | OKit : "make" is not installed. It is required for OKit to be compiled. Would you like to install this dependency? (y/N) : '
	read confb
	case ${confb} in
		[!Yy]) echo 'Orion | OKit : Aborted.'
			exit 1;;
	esac
	echo "Orion | OKit : Working on installing Make."
	if apt-get install "make" -y; then
		echo "Orion | OKit : Make successfully installed."
	else
		echo "Orion | OKit : ERROR! Make could not be installed! Aborting."
		exit 1
	fi
fi

if ! dpkg -s "libx11-dev" >/dev/null 2>&1; then 
	printf 'Orion | OKit : "libx11-dev" is not installed. It is required for OKit to function. Would you like to install this dependency? (y/N) : '
	read confc
	case ${confc} in
		[!Yy]) echo 'Orion | OKit : Aborted.'
			exit 1;;
	esac
	echo "Orion | OKit : Working on installing XLib."
	if apt-get install "libx11-dev" -y
	then
		echo "Orion | OKit : XLib successfully installed."
	else
		echo "Orion | OKit : ERROR! XLib could not be installed! Aborting."
		exit 1
	fi
fi

printf 'Orion | OKit : OKit is ready to be installed. Would you like to proceed? (y/N) : '; read confd
case ${confd} in
	[!Yy]) echo 'Orion | OKit : Aborted.'
		exit 1;;
esac

echo "Orion | OKit : Working on installing OKit."
if ! cp -R "src/include/" "/usr/include/Orion/_OKit"; then
	echo 'Orion | OKit : ERROR! Failed to copy header files into "/usr/include/Orion/_OKit"! Aborting.'
	exit 1
fi

if cp "src/SYSHEADER" "/usr/include/Orion/OKit"; then
	echo 'Orion | OKit : Copied header files. Use "#include <Orion/OKit>" in your C++ files to use these headers.'
else
	echo 'Orion | OKit : ERROR! Failed to copy redirect header as "/usr/include/Orion/OKit"! Aborting.'
	exit 1
fi

echo "Orion | OKit : Compiling source code..."

# This cd src lasts until the end of the script.  If you wanted to back out, you'd need to specifically "cd .."
if cd src && make shared >>/dev/null; then
	echo "Orion | OKit : Successfully compiled OKit source code."
else
	echo "Orion | OKit : ERROR! Failed to compile source code! Aborting."
	exit 1
fi

if cp "libOKit.so" "/usr/lib/libOKit.so"; then
	echo "Orion | OKit : Copied OKit library to system libaries directory."
else
	echo 'Orion | OKit : ERROR! Failed to copy compiled library as "/usr/lib/libOKit.so"! Aborting.'
	exit 1
fi

rm ./*.o ./*.so >/dev/null 2>&1

if [ -z "$qflag" ]; then
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
echo 'Orion | OKit : OKit has been successfully installed. To compile your applications with OKit, simply use the library flag "-lOKit", and read the files in "docs" for more information.'
