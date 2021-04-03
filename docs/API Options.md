# Generic Information: API Options
## Configurable properties for OrionAPI

OrionAPI is a very large general-purpose API for making Applications for the Orion Operating System.
Due to the size of OrionAPI, there are some features that might conflict with other libraries, or features that some developers might not want.

## Preprocessor Configuration
By default, OrionAPI is configured to have all of the features enabled to give the developer the most amount of tools at their disposal as possible.
However, OrionAPI also provides a list of [preprocessor macros](https://www.tutorialspoint.com/cplusplus/cpp_preprocessor.htm) that can be defined to enable or disable features.

The following is a list of all definable macros that can enable or disable features:
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

## ORION_NOHELPERS
