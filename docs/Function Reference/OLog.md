# Function Reference: OLog / OVLog / ODLog
## Orion-Native Logging
OLog is the Orion-Native implementation and replacement for [printf](https://www.tutorialspoint.com/c_standard_library/c_function_printf.htm).
It is used internally by all of OrionAPI.

## Declarations
```cpp
extern void OLog(const char* string, ...);
extern void OVLog(const char* string, ...);
#define ODLog(...) _DEBUGLOG(__VA_ARGS__)
```
```
[Library  : OSL]
[Declared : src/include/OSL/OLog.hpp]
[Defined  : src/OSL/OLog.cpp]
```

### Parameters
`const char* string` - The String to be formatted and logged. The rest of the arguments can be of any type as long as they're capable of being [formatted](https://en.wikipedia.org/wiki/Printf_format_string) into the String.

`...` [Variadic](https://en.wikipedia.org/wiki/Variadic_function) arguments. Can be of any type as long as long as they're capable of being [formatted](https://en.wikipedia.org/wiki/Printf_format_string) into the String.

## Use
OLog (alongside its variants) are OrionAPI's solution for logging formatted Strings easily to the terminal.
It acts identically to [printf](https://www.tutorialspoint.com/c_standard_library/c_function_printf.htm), and is entirely interchangable with it.

It is meant to reduce the need to include `<stdio.h>` or `<iostream>` in your projects solely for the ability to log information out to the terminal.

To log `"Hello World!"` out to the terminal, use the following syntax:
```cpp
OLog("Hello World!\n");
```
This will print `"Hello World!"` out to the terminal and then creates a new line, identically to how `printf` would.

More complex formatting is possible, as shown here:
```cpp
OLog("The price of a %s is %d dollars and %d cents.\n", "pizza", 5, 99);
```
This will print `"The price of a pizza is 5 dollars and 99 cents."` out to the terminal and on a new line as well.

### Verbose Logging
Sometimes you need to print information regarding the [runtime](https://en.wikipedia.org/wiki/Runtime_(program_lifecycle_phase)) state of your Application, but you don't want it to clutter the terminal and obscure more useful information.
This is where OVLog comes in.

OVLog will ONLY print the formatted string to the terminal if `OAPP_VERBOSE` has been set to true.
`OAPP_VERBOSE` is set to true whenever the Application is started with the environment variable `O_VERBOSE=1` in the command line.

To log conditionally, use the following syntax:
```cpp
OVLog("This will only print if OAPP_VERBOSE is true!\n");
```
Upon running this program normally, this message would not be logged to the terminal.
However, when setting the [environment variable](https://en.wikipedia.org/wiki/Environment_variable) `O_VERBOSE=1`, the message would be logged (alongside other OrionAPI diagnostic information).

As an example: `O_VERBOSE=1 ./MyOApp` is how the environment variable is set from the terminal. Replace `MyOApp` with the name of your program's binary.

### Development Debug Logging
OVLog is meant for logging verbose information regarding procedures undergoing during the operation of your Application.
Even if `OAPP_VERBOSE` is false, OVLog will still be called but won't display a message. The internal check of the `OAPP_VERBOSE` variable can be [expensive](https://www.quora.com/What-does-it-mean-if-something-is-computationally-expensive)
if OVLog is used very often. This is where ODLog comes in.

ODLog isn't like the rest of the logging functions, it is actually a preprocessor macro:
```cpp
#ifndef ORION_NODEBUG
	#define _DEBUGLOG(...) Orion::OLog("%s - LINE : %d\t",__FILE__,__LINE__), Orion::OLog(__VA_ARGS__)
#else
	#define _DEBUGLOG(...)
#endif /* !ORION_NODEBUG */

#ifndef ORION_NOHELPERS
	/* Logs the formatted string out to the terminal, alongside the name of the File and the Line at which this is called for debugging. */
	#define ODLog(...) _DEBUGLOG(__VA_ARGS__)
#endif /* !ORION_NOHELPERS */
```
This is to allow for having [debug code](https://en.wikipedia.org/wiki/Debug_code) (specifically operation logs) removable at [compile-time](https://en.wikipedia.org/wiki/Compile_time),
instead of requiring the developer to go in and remove all instances of debug logs for the [release of the Application](https://en.wikipedia.org/wiki/Software_release_life_cycle#Release_candidate),
or providing a function(OVLog) that checks a variable for each debug log message, which would be intensive.

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


For more information, please review the documentation on [API Configuration](https://github.com/RosettaHS/OrionAPI/blob/main/docs/API%20Configuration.md).

### Loggable Types
Alongside being able to log formatted Strings, you can also pass many other types provided by OrionAPI through OLog and OVLog to log their information.

For example, [OCol](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md) is one of many types provided by OrionAPI that can be logged:
```cpp
OCol myCol(255,255,255);

OLog(myCol);
```
This would log `(255,255,255)` out to the terminal.

Optionally, you can pass flags to tell OLog whether it should log verbose information instead, and whether the logged message should appear on a newline, or extends the current line:
```cpp
/*
 * The first flag decides whether it should log verbose information about this Type instead of the traditional information. Default is false.
 * The second flag whether the output should be placed on a newline or append to the current one if applicable. Default is true.
 */
OLog(myCol,false,false);
```
This would log `(255,255,255)` out to the terminal on the current line and does not create a newline.

## Other Information
As stated before, OLog (and its variants) are identical to [printf](https://www.tutorialspoint.com/c_standard_library/c_function_printf.htm),
to the point of being able to do a find-and-replace of `printf` with `OLog` and no errors would arise.

#### See Also:
[OLogBits](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OLog.md)