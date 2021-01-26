# Class Reference: OSignal
## Simplified event data.

### Definitions
```cpp
enum OSignalType{
	OSIG_NONE,
	OSIG_BOOL,
	OSIG_INT,
	OSIG_UNSIGNED_INT,
	OSIG_SHORT,
	OSIG_UNSIGNED_SHORT,
	OSIG_FLOAT,
	OSIG_DOUBLE,
	OSIG_LONG,
	OSIG_UNSIGNED_LONG,
	OSIG_CHAR,
	OSIG_UNSIGNED_CHAR,
	OSIG_STRING,
	OSIG_ARBITRARY,
	OSIG_OBJECT
};
	
struct OSignal : public CLoggable{
	CBaseUI* emitter;
	OSignalType type;

	union{
		bool asBool;
		int asInt;
		unsigned int asUnsignedInt;
		short asShort;
		unsigned short asUnsignedShort;
		float asFloat;
		double asDouble;
		long asLong;
		unsigned long asUnsignedLong;
		char asChar;
		unsigned char asUnsignedChar;
		struct{
			char* asText;
			size_t asLength;
		}string;
		void* asArbitrary;
		CBaseUI* asObject;
	}get;

	virtual void log(bool verbose=false) override;

	const char* getTypeAsString(void);
};
```
### Use
OSignals are emitted to functions linked via the `connect()` function of a CSignalDispatcher.
They serve as a wrapper to get data from a UI Element when an event occurs, such as the click of an OButton.
OSignal extends from [CLoggable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md), meaning it can be logged both verbosely and minimally.

OSignals are not to be created by developers manually, they are only sent to linked functions.
All event emissions from CSignalDispatchers emit a signal to the linked functions, but not all linked functions must take in an OSignal as an argument.
To create a function that recieves an OSignal, use the following syntax.
```cpp
void myReciever(OSignal signal);
```

OSignalType is an enumeration of the type carried by an OSignal.
For example, if an OSignal's `type` variable is of type `OSIG_BOOL`, this means the OSignal carries the data of a boolean.
To retrieve this boolean, use the `get` union followed by the type you wish to get it as, `get.asBool;`

Example:
```cpp
void myReciever(OSignal signal){
	if(signal.type==OSIG_BOOL){ /* This means the OSignal is carrying a boolean! */
		OLog(signal.get.asBool); /* This logs the boolean the OSignal is carrying out to the terminal. */
	}
}
```
It is very important to check the type of the OSignal before attempting to access the data, as the type is not always guaranteed.
There are special types too, such as the `OSIG_STRING` type.

If an OSignal has the type `OSIG_STRING`, it means that it carries an array of characters. To access this array, use `get.string.asText;`.
It also carries the length of the string in the `get.string` struct. To access this, use `get.string.asLength;`

OSignals also carry a pointer to the object that emit it, in the form of a pointer to a [CBaseUI-derived class.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CBaseUI.md)
For example, if an OButton emits an OSignal, the OSignal's `emitter` variable will be a pointer to the OButton, casted as a [CBaseUI](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CBaseUI.md)

To verify the type of the emitter, access the `type` member of the `emitter` pointer with the following syntax.
```cpp
if(signal.emitter->type==OT_BUTTON){ /* Replace OT_BUTTON with the type you wish to verify it as. */
	...
}
```

You can also log the contents of a given OSignal out to the terminal if you need to get information on how to retrieve its data.
To log the information, you can either call `log();` or `log(true);` to get even more verbose information.

### Structure Breakdown : OSignalType
```cpp
OSIG_NONE,
```
This means the OSignal isn't carrying any data. Do not try to access any members of the `get` union if `type` is equal to this.
```cpp
OSIG_BOOL,
```
This means the OSignal is carrying a boolean. If `type` is equal to this, you may access the carried boolean with `get.asBool;`
```cpp
OSIG_INT,
```
This means the OSignal is carrying an integer. If `type` is equal to this, you may access the carried integer with `get.asInt;`
```cpp
OSIG_UNSIGNED_INT,
```
This means the OSignal is carrying an unsigned integer. If `type` is equal to this, you may access the carried unsigned integer with `get.asUnsignedInt;`
```cpp
OSIG_SHORT,
```
This means the OSignal is carrying a short. If `type` is equal to this, you may access the carried short with `get.asShort;`
```cpp
OSIG_UNSIGNED_SHORT,
```
This means the OSignal is carrying an unsigned short. If `type` is equal to this, you may access the carried unsigned short with `get.asUnsignedShort;`
```cpp
OSIG_FLOAT,
```
This means the OSignal is carrying a float. If `type` is equal to this, you may access the carried float with `get.asFloat;`
```cpp
OSIG_DOUBLE,
```
This means the OSignal is carrying a double. If `type` is equal to this, you may access the carried double with `get.asDouble;`
```cpp
OSIG_LONG,
```
This means the OSignal is carrying a long. If `type` is equal to this, you may access the carried long with `get.asLong;`
```cpp
OSIG_UNSIGNED_LONG,
```
This means the OSignal is carrying an unsigned long. If `type` is equal to this, you may access the carried unsigned long with `get.asUnsignedLong;`
```cpp
OSIG_CHAR,
```
This means the OSignal is carrying a character. If `type` is equal to this, you may access the carried character with `get.asChar;`
```cpp
OSIG_UNSIGNED_CHAR,
```
This means the OSignal is carrying an unsigned character. If `type` is equal to this, you may access the carried unsigned character with `get.asUnsignedChar;`
```cpp
OSIG_STRING,
```
This means the OSignal is carrying a string. If `type` is equal to this, you may access the carried string's text with `get.string.asText;`.
The OSignal also carries the string's length. To access this, use `get.string.asLength;` 
```cpp
OSIG_ARBITRARY,
```
This means the OSignal is carrying a void pointer. If `type` is equal to this, you may access the carried pointer with `get.asArbitrary;`
```cpp
OSIG_OBJECT
```
This means the OSignal is carrying a [CBaseUI-derived Object.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CBaseUI.md) If `type` is equal to this, you may access the carried Object's pointer with `get.asObject;`
It is crucial to check the type of the carried Object. To access the type, use `get.asObject->type;`

### Structure Breakdown : OSignal
```cpp
CBaseUI* emitter;
```
A pointer to the [CBaseUI-derived Object.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CBaseUI.md) that emitted this OSignal.
```cpp
OSignalType type;
```
The type of the OSignal. See OSignalType for more information.


The following resides in the OSignal's `get` union. To access any of the following, use `get.` followed by the names of the following variables.


```cpp
bool asBool;
```
The data carried by the OSignal as a boolean.
```cpp
int asInt;
```
The data carried by the OSignal as an integer.
```cpp
unsigned int asUnsignedInt;
```
The data carried by the OSignal as an unsigned integer.
```cpp
short asShort;
```
The data carried by the OSignal as a short.
```cpp
unsigned short asUnsignedShort;
```
The data carried by the OSignal as an unsigned short.
```cpp
float asFloat;
```
The data carried by the OSignal as a float.
```cpp
double asDouble;
```
The data carried by the OSignal as a double.
```cpp
long asLong;
```
The data carried by the OSignal as a long.
```cpp
unsigned long asUnsignedLong;
```
The data carried by the OSignal as an unsigned long.
```cpp
char asChar;
```
The data carried by the OSignal as a character.
```cpp
unsigned char asUnsignedChar;
```
The data carried by the OSignal as an unsigned character.
```cpp
struct{
	char* asText;
	size_t asLength;
}string;
```
The data carried by the OSignal as a string struct, containing both the text of the string and the length.
```cpp
void* asArbitrary;
```
The data carried by the OSignal as a void pointer.
```cpp
CBaseUI* asObject;
```
The data carried by the OSignal as a pointer to a [CBaseUI-derived Object.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CBaseUI.md)

```cpp
virtual void log(bool verbose=false) override;
```
Logs the data of the OSignal out to the terminal. Pass `true` to get more verbose information (recommended)
```cpp
const char* getTypeAsString(void);
```
Returns the type of the OSignal as a string.
For example, if the `type` of the OSignal is set to `OSIG_INT`, `getTypeAsString();` will return `"OSIG_INT"`.

### Other Information
OSignals are never meant to be created by the developer, and has no constructor.
They are only created within internal event handling functions of CSignalDispatcher-derived classes.

It is always recommended to log the data of an OSignal verbosely, as this gives you more detailed information on how to handle and check the OSignal's properties before manipulation.
A more recommended tip is to use [OVLog()](https://github.com/RosettaHS/OKit/blob/main/docs/Function%20Reference/OLog.md) and load your OApp with the environment variable `O_VERBOSE=1` during testing.

This ensures that the OSignal will always have its data logged verbosely, but also that it will only log this data when your OApp is launched in Debug/Verbose mode.