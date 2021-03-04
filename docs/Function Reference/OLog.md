# Function Reference: OLog / OVLog / ODLog
## Orion-Native Logging

### Declarations
```cpp
void OLog(const char* string, ...);
void OVLog(const char* string, ...);
#define ODLog(...) _DEBUGLOG(__VA_ARGS__)


void OLog(CLoggable&);
void OLog(CLoggable*);
void OVLog(CLoggable&,bool passVerbose=true);
void OVLog(CLoggable*,bool passVerbose=true);
```
### Use
OLog and OVLog are OSL's solution for logging formatted Strings easily to the terminal.
Both functions allow for logging [CLoggables](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md) to the terminal.

OLog and OVLog act identically to [printf,](https://www.tutorialspoint.com/c_standard_library/c_function_printf.htm) and is interchangable with it.
They are meant to reduce the need to include `<stdio.h>` or `<iostream>` in your projects soley for the ability to log information out to the terminal.

To log "Hello World" out to the terminal, use the following syntax:
```cpp
OLog("Hello World!\n");
```
This will print `"Hello World!"` out to the terminal and then creates a new line, identically to how `printf` would.

More complex formatting is possible, as shown here:
```cpp
OLog("The price of a %s is %d dollars and %d cents.\n", "pizza", 5, 99);
```
This will print `"The price of a pizza is 5 dollars and 99 cents."` out to the terminal and creates a new line as well.

Sometimes you need to print debugging information, but you don't want it to clutter the terminal and obscure more useful information.
This is where OVLog comes in.

OVLog will ONLY print the formatted string to the terminal if `OAPP_VERBOSE` has been set to true.
`OAPP_VERBOSE` is set to true whenever the application is started with the environment variable `O_VERBOSE=1` in the command line.

To log conditionally, use the following syntax:
```cpp
OVLog("This will only print if OAPP_VERBOSE is true!\n");
```
Upon running this program normally, this line would not print to the terminal.
But when running the program in the command line you can set the environment variable `O_VERBOSE=1`, and the line would print alongside other OKit diagnostic information.

`O_VERBOSE=1 ./MyOApp` is how you set the environment variable from the terminal. Replace `MyOApp` with the name of your program's binary.

You can also pass [CLoggables](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md) through OLog and OVLog.

Creating an [OCol](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md)
```cpp
OCol myCol(255,255,255);
```
You may pass it through either function.
```cpp
OLog(myCol);
```
Would produce `(255, 255, 255)` in the terminal.

Passing a [CLoggable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md) through OVLog would call the log(); function on the [CLoggable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md) with `true`.
This would result in a different, more verbose output. To prevent this from happening, you may pass `false` after passing the [CLoggable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md) through OVLog.

Passing `myCol` through OVLog would appear as `OCol <Memory Address> : R 255 | G 255 | B 255 | XCOL 16777215`, if `OAPP_VERBOSE` is `true`. (With `<Memory Address>` being the actual memory address of the [OCol.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md))

In some cases, you might need to log debug information about the state of your Application out to the termimal during development,
information such as the given File and Line during execution to see where an error might be occuring. ODLog is meant for this purpose.

To log a debug message out to the terminal, use the following syntax:
```cpp
ODLog("This will print this message, alongside the Line and File in which this is being called at!\n");
```
This is very useful for finding bugs, or tracking the execution of your program. It functions identically to OLog and OVLog, but this is not a function, rather it is a macro.
If you define `ORION_NODEBUG` before including `<OrionAPI>`, it will replace all instances of ODLog with nothing,
meaning it's incredibly useful for packaging your Application for release without having to remove all instances of ODLog manually, or having it waste execution time checking a variable like OVLog.

### Breakdown
```cpp
void OLog(const char* string, ...);
```
Logs the formatted string out to the terminal.
```cpp
void OVLog(const char* string, ...);
```
Logs the formatted string out to the terminal ONLY if `OAPP_VERBOSE` is true.
```cpp
void OLog(CLoggable&);
```
Calls the `log()` function on the passed [CLoggable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md) reference.
```cpp
void OLog(CLoggable*);
```
Calls the `log()` function on the passed [CLoggable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md) pointer.
```cpp
void OVLog(CLoggable&,bool passVerbose=true);
```
Calls `log(true)` on the passed [CLoggable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md) reference ONLY if `OAPP_VERBOSE` is true.
Pass `false` as the second argument if you wish to show more simplified information.
```cpp
void OVLog(CLoggable*,bool passVerbose=true);
```
Calls `log(true)` on the passed [CLoggable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md) pointer ONLY if `OAPP_VERBOSE` is true.
Pass `false` as the second argument if you wish to show more simplified information.

### Other Information
OLog and OVLog both call the `log()` function if called with a [CLoggable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md).

Both functions are often used internally in newer modules, however older modules currently use `printf()` and conditionals for debug output.

Primitive types, such as `int`, `double`, `float` can also be passed through both OLog and OVLog, however this is bloated and potentially wasteful.