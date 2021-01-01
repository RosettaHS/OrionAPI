# Class Reference: OVec & OVec4
## Handling positions, sizes, or both.


### Definitons
```cpp
struct OVec{
	int x,y;

	OVec();
	OVec(int x, int y);
	void log();
};

struct OVec4{
	int x,y;
	unsigned int w,h;

	OVec4();
	OVec4(int x, int y, unsigned int w, unsigned int h);
	void log();
};
```

### Use
OVec is a method of storing the X and Y positional data or the W and H size data of any GUI element.
It isn't meant to be used often as it is less efficient and cumbersome in most situations.
OVec4 is for storing both the positional and size data of a GUI element. Both are functionally identical.

To create an OVec for later storage, use the following syntax.
```cpp
OVec myVec(16,16);
```
The first 2 values are the X and Y respectively. The OVec can now be passed into **some** functions that would normally take an X and Y, although this is wasteful so it is advised against.

To create an OVec4, use the following syntax.
```cpp
OVec4 myVec4(16,16,32,32);
```
The first 2 values are the X and Y respectively, and the last 2 values are the W amd H respectively. 
Very few functions accept this as an argument, however some functions will return this type when getting full geometry data of a GUI element.

You can print the data of any given OVec or OVec4 to the terminal by calling the `log()` function on either type.

### Breakdown: OVec
```cpp
int x,y;
```
The X and Y positions for later storage. Used as positional data, although can be used as size data however you lose half of the capacity of an integer.
```cpp
OVec();
```
An empty constructor. Will set all values to 0.
```cpp
OVec(int x, int y);
```
Creates and initalises an OVec with the given parameters.
```cpp
void log();
```
Prints all data of a given OVec to the terminal. Useful for debugging.

### Breakdown: OVec4
```cpp
int x,y;
```
Same as OVec, this is the X and Y positions for later storage. Used as positional data only.
```cpp
unsigned int w,h
```
The width and height size for later storage. Often functions that return an OVec4 from a GUI element will give the size data stored here.
```cpp
OVec4();
```
An empty constructor. Will set all values to 0.
```cpp
OVec4(int x, int y, unsigned int w, unsigned int h);
```
Creates and initalises an OVec with the given parameters. First two values are to be used as positional data, and the last two are for size data.
```cpp
void log();
```
Prints all data of a given OVec to the terminal. Useful for debugging.

### Other Information
Usage of an OVec or an OVec4 is quite rare, and often never created by the developer or OKit.
The most common use would be from a function that returns a position and size from a given object, often referred to as "Geometry Data".

Unsigned integers are used for width and height since OKit (and X) does not support negative or inverted sizes for GUI elements. Passing a negative value would instantly crash X, which is why this is locked to being unsigned only.