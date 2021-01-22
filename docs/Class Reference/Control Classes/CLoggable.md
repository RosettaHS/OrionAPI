# Control Class Reference: CLoggable
## Handling easy debugging

### Definition
```cpp
class CLoggable{
	public:
		virtual void log(bool verbose=false);
};
```
### Use
CLoggables are an [abstract class](https://en.cppreference.com/w/cpp/language/abstract_class) that handles printing to the terminal in a formatted manner.
Things such as [OCol](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md), [OTheme](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OTheme.md), and [OString](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OString.md) (plus most other things) extend from CLoggable and can be printed out to the terminal,
making all of their information easily readable by the developer.

To log a CLoggable-derived class to the terminal, you can either call the `log();` function on them, or pass them through `OLog();` (See More)

If you wish to get more detailed information, you can pass `true` to either the `log();` function, or pass `true` to `OLog();` after the CLoggable argument.

### Structure Breakdown
```cpp
virtual void log(bool verbose=false);
```
Logs the information of the derived class to the terminal. Pass true for more verbose information.

### Other Information
As with all Control classes, you are never to instantiate or create one manually.

All CLoggable-dervied classes can be passed through `OLog();` or `OVLog();` (See More)