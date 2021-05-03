# Generic Information: API Configuration
## Configurable properties for OrionAPI

OrionAPI is a very large, general-purpose API for making Applications for the Orion Operating System.
Due to the size of OrionAPI, there are some features that might conflict with other libraries, or features that some developers might not want.

## Preprocessor Configuration
By default, OrionAPI is configured to have all of the features enabled to give the developer the most amount of tools at their disposal as possible.
However, OrionAPI also provides a list of [preprocessor macros](https://www.tutorialspoint.com/cplusplus/cpp_preprocessor.htm) that can be defined to enable or disable features.

The following is a list of all non-internal definable macros that can enable or disable features:
```
ORION_UNUSE_NAMESPACE
ORION_NODEBUG
ORION_NOHELPERS
ORION_NOALTNAMES
```
The developer can define any one of these before including `<OrionAPI>` to enable or disable certain features.

### ORION_UNUSE_NAMESPACE
All OrionAPI types and functions are declared under the `Orion` [namespace.](https://www.tutorialspoint.com/cplusplus/cpp_namespaces.htm)
However, you may notice that you never need to prefix any OrionAPI types or functions with `Orion::`.
This is because OrionAPI automatically [uses](https://en.wikipedia.org/wiki/Namespace#Use_in_common_languages) the `Orion` namespace for convenience and ease of use.
However, this can cause conflictions with other libraries. In order to circumvent this, you may define `ORION_UNUSE_NAMESPACE` *before* including `<OrionAPI>`

A normal OrionAPI Application may look like this:
```cpp
#include <OrionAPI>

int main(void){
	if(OFileExists("Hello.txt")==false){
		OFile file("Hello.txt");
		file.setLine(0,"Hello World!");
		file.close(true);
		OLog("File has been saved!\n");
	}
}
```
However by defining `ORION_UNUSE_NAMESPACE`, it will look like this:
```cpp
#define ORION_UNUSE_NAMESPACE
#include <OrionAPI>

int main(void){
	if(Orion::OFileExists("Hello.txt")==false){
		Orion::OFile file("Hello.txt");
		file.setLine(0,"Hello World!");
		file.close(true);
		Orion::OLog("File has been saved!\n");
	}
}
```
This can help prevent [global namespace pollution,](https://en.wikipedia.org/wiki/Namespace#Name_conflicts) at the cost of making the code harder to read and write.

### ORION_NODEBUG
OrionAPI provides [several logging functions,](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OLog.md) one of which being ODLog.
However ODLog isn't like the rest of the logging functions, it is actually a preprocessor macro:
```cpp
#ifndef ORION_NODEBUG
	#define _DEBUGLOG(...) Orion::OLog("%s - LINE : %d\t",__FILE__,__LINE__), Orion::OLog(__VA_ARGS__)
#else
	#define _DEBUGLOG(...)
#endif /* !ORION_NODEBUG */

#ifndef ORION_NOHELPERS /** We'll get to this macro next! **/
	/* Logs the formatted string out to the terminal, alongside the name of the File and the Line at which this is called for debugging. */
	#define ODLog(...) _DEBUGLOG(__VA_ARGS__)
#endif /* !ORION_NOHELPERS */
```
This is to allow for having [debug code](https://en.wikipedia.org/wiki/Debug_code) (specifically operation logs) removable at [compile-time,](https://en.wikipedia.org/wiki/Compile_time)
instead of requiring the developer to go in and remove all instances of debug logs for the [release of the Application,](https://en.wikipedia.org/wiki/Software_release_life_cycle#Release_candidate)
or providing a function that checks a variable for each debug log message, which would be intensive.

The following is an example of a program that provides operation logs if `ORION_NODEBUG` is not defined:
```cpp
#include <OrionAPI>

int main(void){
	if(OFileExists("Hello.txt")==false){
		ODLog("File not found, creating\n");
		OFile file("Hello.txt");
		ODLog("File created\n");
		file.setLine(0,"Hello World!");
		ODLog("Lines written\n");
		file.close(true);
		ODLog("File has been saved!\n");
	}
}
```
This will provide an operation log for every step of the procedure. Note that this example does not do any checking on the [return values](https://en.wikipedia.org/wiki/Return_statement) for error correction.

If `ORION_NODEBUG` is defined, the compiler will *effectively* see the program as this:
```cpp
#define ORION_NODEBUG
#include <OrionAPI>

int main(void){
	if(OFileExists("Hello.txt")==false){
		//ODLog("File not found, creating\n"); 
		OFile file("Hello.txt");
		//ODLog("File created\n");
		file.setLine(0,"Hello World!");
		//ODLog("Lines written\n");
		file.close(true);
		//ODLog("File has been saved!\n");
	}
}
```
This macro could also be used for excluding certain chunks of your own code at compile-time for a full release by encasing debug-only code in a `#ifndef ORION_NODEBUG` preprocessor block.

### ORION_NOHELPERS
Alongside all of the types and functions, OrionAPI also provides a few helper macros to more easily access a select set of [global variables](https://en.wikipedia.org/wiki/Global_variable) pertaining to the state of the Application, or information regarding it.

The following is a list of all non-internal helper macros (and their expansions) defined by OrionAPI:
```cpp
#define ONATIVEONLY         if(Orion::OApp.Flags.isNativeOApp)
#define OAPP                (Orion::OApp)
#define OAPP_NAME           (Orion::OApp.name)
#define OAPP_IDENTIFIER     (Orion::OApp.identifier)
#define OAPP_USERNAME       (Orion::OApp.username)
#define OAPP_PID            (Orion::OApp.pid)
#define OAPP_INTERFACE      (Orion::OApp.Interface)
#define OAPP_SCALE          (Orion::OApp.Interface.scale)
#define OAPP_FOCUSEDELEMENT (Orion::OApp.Interface.focusedElement)
#define OAPP_THEME          (*Orion::OApp.Interface.theme)
#define OAPP_STORAGE        (Orion::OApp.Storage)
#define OAPP_STATICPATH     (Orion::OApp.Storage.staticPath)
#define OAPP_LIBPATH        (Orion::OApp.Storage.libPath)
#define OAPP_DATAPATH       (Orion::OApp.Storage.dataPath)
#define OAPP_PATH           (Orion::OApp.Path)
#define OAPP_BINPATH        (Orion::OApp.Path.toBinary)
#define OAPP_BINDIR         (Orion::OApp.Path.toBinaryFolder)
#define OAPP_HOME           (Orion::OApp.Path.toHome)
#define OAPP_CWD            (Orion::OApp.Path.toCWD)
#define OAPP_FLAGS          (Orion::OApp.Flags)
#define OAPP_NATIVE         (Orion::OApp.Flags.isNativeOApp)
#define OAPP_VERBOSE        (Orion::OApp.Flags.isVerbose)
#define OAPP_RUNNING        (Orion::OApp.Flags.isRunning)
#define ODLog(...)          _DEBUGLOG(__VA_ARGS__)
#define CHARBIT_UTF8_MB     (0x80)
#define CHARBIT_UTF8_1X     (0x40)
#define CHARBIT_UTF8_2X     (0x20)
#define CHARBIT_UTF8_3X     (0x10)
#define CHARBIT_UTF8_4X     (0x8)
#define CHARBIT_UTF8_5X     (0x4)
#define CHARBIT_UTF8_6X     (0x2)
```
This makes it incredibly easy to read the global state of the Application, and access information provided to you by OrionAPI.
However, this can pollute the global namespace even further, and thus has a higher chance of causing conflicts.

The most notable helper macros are the ones that provide easy access to the Orion-Native Data Directories provided by OrionAPI.
They reside under the `OApp` global struct, and can be accessed either directly through the struct, or (preferably) by the helper macros.

The following is a program that demonstrates the use of a helper macro by using it to list off all files and folders in the directory the Application is being ran in:
```cpp
#include <OrionAPI>

int main(void){
	OAppStart(); /* To initialise OAPP_CWD */

	if(OAPP_CWD){
		ODirectory dir(OAPP_CWD);
		dir.log();
		dir.close();
	}else{
		OLog("Could not access the current working directory!\n");
	}

	OAppEnd(); /* To free OAPP_CWD */
}
```
Now disabling all helper macros, the program would need to look like this instead:
```cpp
#define ORION_NOHELPERS
#include <OrionAPI>

int main(void){
	OAppStart(); /* To initialise OApp.Path.toCWD */

	if(OApp.Path.toCWD){
		ODirectory dir(OApp.Path.toCWD);
		dir.log();
		dir.close();
	}else{
		OLog("Could not access the current working directory!\n");
	}

	OAppEnd(); /* To free OApp.Path.toCWD */
}
```
It is discouraged to use this syntax, as there is a likelyhood of the structure changing with updates, whereas the helper macros are constant and will merely redirect to the correct variable.

### ORION_NOALTNAMES
All types, functions, and methods provided by OrionAPI are named using the [camelCase](https://en.wikipedia.org/wiki/Camel_case) or the [PascalCase](https://en.wikipedia.org/wiki/Camel_case#Variations_and_synonyms) [naming conventions,](https://en.wikipedia.org/wiki/Naming_convention_(programming))
However this naming convention conflicts with the more common [snake_case](https://en.wikipedia.org/wiki/Snake_case) naming convention for functions, and the ["_t" type suffix](https://stackoverflow.com/questions/1391447/what-does-the-postfix-t-stand-for-in-c) commonly used by structs/types,
and commonly highlighted by [syntax highlighting](https://en.wikipedia.org/wiki/Syntax_highlighting) services such as the ones used in GitHub and [GNU nano](https://en.wikipedia.org/wiki/GNU_nano)

While this is purely cosmetic, it can give the appearance of messy code and cause severe visual style confliction if a given program's source code is using several libraries (that follow different naming conventions) alongside OrionAPI.
To circumvent this style clash, OrionAPI provides several [aliases](https://en.wikipedia.org/wiki/Typedef) to non-internal/control types to make their names fit in better with code following more standard naming conventions.

This is an example of a few aliases to File and Directory management classes provided by OrionAPI:
```cpp
typedef OFileType        filetype_t;
typedef OFileAction      fileaction_t;
typedef OFileHash        filehash_t;
typedef OFileLine        fileline_t;
typedef OFileContent     filecontent_t;
typedef OFile            file_t;

typedef ODirectoryAction diraction_t;
typedef ODEType          direnttype_t;
typedef ODirectoryEntry  direntry_t;
typedef ODirectory       dir_t;
```

The following two programs list off verbose information regarding all files in the given working directory, and are entirely identical:
```cpp
#include <OrionAPI>

int main(void){
	ODirectory       dir;
	ODirectoryEntry* entry;
	OFile            file;

	dir.open(".");
	for(size_t i=0;i<dir.getEntryCount();i++){
		entry=dir.getEntry(i);
		if(entry && entry->type==ODT_FILE){
			file.open(dir.getEntryPath(i), OFILE_OPEN_READONLY);
			file.log(true);
		}
	}
	file.close(false);
	dir.close();
}
```
Now using altnames:
```cpp
#include <OrionAPI>

int main(void){
	dir_t       dir;
	direntry_t* entry;
	file_t      file;

	dir.open(".");
	for(size_t i=0;i<dir.getEntryCount();i++){
		entry=dir.getEntry(i);
		if(entry && entry->type==ODT_FILE){
			file.open(dir.getEntryPath(i), OFILE_OPEN_READONLY);
			file.log(true);
		}
	}
	file.close(false);
	dir.close();
}
```
Altnames and the original names are entirely interchangeable, the *only* difference is the visual appearance of their names.

However there is a downside to this, it severely increases the risk of conflicts if you are using multiple libraries alongside OrionAPI.
In order to prevent this, define `ORION_NOALTNAMES` before including `<OrionAPI>`
```cpp
#define ORION_NOALTNAMES
#include <OrionAPI>

/*** This no longer compiles, and will result in an error! Use the original names instead! ***/

int main(void){
	dir_t       dir;
	direntry_t* entry;
	file_t      file;

	dir.open(".");
	for(size_t i=0;i<dir.getEntryCount();i++){
		entry=dir.getEntry(i);
		if(entry && entry->type==ODT_FILE){
			file.open(dir.getEntryPath(i), OFILE_OPEN_READONLY);
			file.log(true);
		}
	}
	file.close(false);
	dir.close();
}
```

### Should I disable these features?
These features should only be disabled if they are causing conflicts. OrionAPI is carefully designed to avoid conflicts as much as possible, even with use of other libraries,
however there is always a chance of conflictions when using several libraries. In most simple Applications, OrionAPI provides all of the tools one might need, more complex Applications may require use of external libraries.
In those circumstance, first test to see if conflicts do arise, and if they do, disable features accordingly. Disabling all features can severely complicate OrionAPI, and reduce its ease of use.