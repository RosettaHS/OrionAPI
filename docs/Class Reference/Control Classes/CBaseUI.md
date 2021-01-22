# Control Class Reference: CBaseUI
## The Core for everything

### Definition
```cpp
enum OType{
	OT_ERROR,
	OT_CDRAWABLE,
	OT_CCONTAINER,

	OT_OWINDOW,
};

class CBaseUI{
	public:
		OType type;
		bool ready;

		CBaseUI(void);

		const char* getTypeAsString(void);
};
```
### Use
CBaseUI is the base class for all UI elements in OKit. It contains the type of object it is, and it contains a boolean to govern whether the object is safe to modify (initialised).

OType is an enumeration of all deferrable class types used in OKit. Prefixed with `OT_`, all members of the enumeration share exact names with the classes they are representing.
`OT_OWINDOW` is the type only for the `OWindow` class.
OType is often useful when getting a `CBaseUI*` from an OSignal's `get.asObject` and `emitter` members, as they allow you to
easily cast the `CBaseUI*` into a useable pointer for the derived-class.

The `ready` boolean is always `false` until the deferred class initialises. Do NOT call any functions, or attempt to link to, or modify data on a class that has not initialised yet!
It is not required to check the `ready` variable, but it is recommended.

If you wish to get the type of `CBaseUI` as a string, you can call `getTypeAsString();`.
If it has the type `OT_OWINDOW` for example, `getTypeAsString();` will return the string `"OT_OWINDOW"`.

If a `CBaseUI` deriviative has the type `OT_ERROR`, do NOT mess with this object! Something went terribly wrong, or it has failed to initialise.

### Structure Breakdown
```cpp
OType type;
```
The type of the derived class.
```cpp
bool ready;
```
Is the Object initialised? If this is true, it is safe to call functions and modify data on the derived class.
```cpp
CBaseUI(void);
```
Empty constructor. Sets all values to 0, and sets `type` to `OT_ERROR` until the derived class initialises.
```cpp
const char* getTypeAsString(void);
```
Returns the type of this Object as a string.

### Other Information
As with all Control classes, you are never to instantiate or create one manually.

The only CBaseUI-derived class is [CDrawable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md), other control classes such as CContainer and CSignalDispatcher are entirely separate from CBaseUI.