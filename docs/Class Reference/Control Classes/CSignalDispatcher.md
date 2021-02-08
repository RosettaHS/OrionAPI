# Control Class Reference: CSignalDispatcher
## Callback handling

### Definition
```cpp
class CSignalDispatcher{
	protected:
		CSLArray SIGMGR;

		void emit(OMask, OSignal&);
	public:
		virtual bool connect(OMask&, void(*func)(void));
		virtual bool connect(OMask&, void(*func)(OSignal));

		virtual bool disconnect(OMask&, void(*func)(void));
		virtual bool disconnect(OMask&, void(*func)(OSignal));
}
```
### Use
CSignalDispatcher is a base class that handles [signal callbacks](https://www.tutorialspoint.com/callbacks-in-c) for interactable UI elements.
Some UI elements are interactive, and can be clicked, dragged, scrolled, or otherwise interacted with or modified. 
When they are interacted with or modified, they can emit an [OSignal](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OSignal.md) that contains the event data of its interaction to a [callback function](https://www.tutorialspoint.com/callbacks-in-c) that was linked to the UI element.

Derived classes determine what to do during an internal event and will send [OSignals](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OSignal.md) accordingly.
Review the documentation for the given derived class for information on what OMasks are allowed and how they're handled.

For functions you wish to link for a UI element to call during an event, the function must be in one of two eligible formats.

It can either take in no values, and thus no [OSignal](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OSignal.md)
```cpp
void myFunction(void);
```
Or it can take in an [OSignal](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OSignal.md) as a parameter for the UI element to pass any data onto.
```cpp
void myFunction(OSignal signal);
```
They must all return `void`, otherwise they will not eligible for linkage.
No other variations or formats are eligible for linkage either, meaning you cannot link a function that takes in any other data type or any other amount of data types other than the two listed here.

To link a given eligible function to a UI element, you must first look at the given element's documentation to see what OMasks are attributed to it.

An OMask is a value with 32 bits (`uint32_t`) used as an [event mask](https://youtu.be/aMAM5vL7wTs) for use when linking a function to a UI element's event system.
For example. OButton has a few valid OMasks, one of them being `OBUTTON_CLICK`. To link a function to be called when an OButton gets clicked, use the following syntax.
```cpp
myButton.connect(OBUTTON_CLICK,myFunction);
```
Now when the button gets clicked, `myFunction()` will be called.
If `myFunction` takes in an [OSignal,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OSignal.md) it will pass an OSignal carrying data regarding the button and its state.

However, there are many OMasks eligible for OButton in this example, masks like `OBUTTON_PRESSED`, and `OBUTTON_UNPRESSED`.
You can pair up a function to multiple event masks by using the [bitwise \| operator](https://en.wikipedia.org/wiki/Bitwise_operations_in_C#Bitwise_assignment_operators)
```cpp
myButton.connect(OBUTTON_PRESSED | OBUTTON_UNPRESSED, myFunction);
```
This now means `myFunction()` will be called when the mouse initally presses the button, and when the mouse releases the button.

To see what OMasks are defined for a given class, see the documentation for the given derived class for further details.

If you wish to unlink a function you've previously linked to a given event, use the following syntax.
```cpp
myButton.disconnect(OBUTTON_CLICK,myFunction); /* Replace "OBUTTON_CLICK" with the event you wish to unlink from! */
```
This will now unlink `myFunction()` from the `OBUTTON_CLICK` event on the button. 
However, this will NOT unlink a function from any other events it may be linked to, so make sure you use the [bitwise \| operator](https://en.wikipedia.org/wiki/Bitwise_operations_in_C#Bitwise_assignment_operators) to include any other events you wish to unlink the given function from as well.
A good rule of thumb is `disconnect()` acts inversely to `connect()`. If you've linked a function to an event, and later want to unlink said function from that event, copy and paste the code you used to link it and replace `connect()` with `disconnect()`.

As previously stated, the [callback functon](https://www.tutorialspoint.com/callbacks-in-c) must return `void`, and must either take in a `void` or an [OSignal.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OSignal.md)
Any other configuration is not allowed. For example:
```cpp
int myFunction(void);
void myFunction(int);
void myFunction(OSignal, int, ...);
...
```
These are all breaking either the `void`-only return or `void`/[OSignal](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OSignal.md)-only parameter rules, and are not eligible for linkage.

It isn't required for your [callback functon](https://www.tutorialspoint.com/callbacks-in-c) to take in an [OSignal,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OSignal.md) since some events don't need to have pass any extra data.
A good rule of thumb is events that occur on a click likely won't send any extra data, so it is acceptable to have your functon take in `void`; no arguments.
Plus, some compilers will [warn about unused parameters,](https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html) so unless the event has a reason to send extra data to a [callback functon,](https://www.tutorialspoint.com/callbacks-in-c) you likely don't have any reason to make your function take in an [OSignal.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OSignal.md)
It is still good practice to review the documentation for the given class to see what data some events of that class may send, and design your functions accordingly.

### Structure Breakdown
```cpp
CSLArray SIGMGR;
```
Protected. This is an array that contains linked functions alongside their OMasks.
```cpp
void emit(OMask, OSignal&);
```
Protected. Runs through the linked functions stored in `SIGMGR` and calls them if the stored mask matches any bits in the passed mask.
```cpp
virtual bool connect(OMask&, void(*func)(void));
```
Takes in an OMask and a function pointer that takes in no arguments (`void`) and links it to the internal array. Returns `true` if linkage was successful.
```cpp
virtual bool connect(OMask&, void(*func)(OSignal));
```
Takes in an OMask and a function pointer that takes in an [OSignal](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OSignal.md) as an argument and links it to the internal array. Returns `true` if linkage was successful.
```cpp
virtual bool disconnect(OMask&, void(*func)(void));
```
Takes in an OMask and a function pointer that takes in no arguments (`void`) and unlinks that specific event callback from the internal array. Returns `true` if removal was successful.
```cpp
virtual bool disconnect(OMask&, void(*func)(OSignal));
```
Takes in an OMask and a function pointer that takes in an [OSignal](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OSignal.md) as an argument and unlinks that specific event callback from the internal array. Returns `true` if removal was successful.

### Other Information
As with all Control classes, you are never to instantiate or create one manually.

It is highly recommended to review the documentation for the derived class in question you are trying to link your functions to for a list of all eligible event masks and their extra data when taking in an [OSignal.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OSignal.md)

While the `connect()` and `disconnect()` methods are [virtual methods,](https://en.cppreference.com/w/cpp/language/virtual) this does not mean they will actually be replaced by derived classes.
They will most likely use the base, CSignalDispatcher implementation unless they have a very specific case not to, and if this is such then it is highly recommended to review the documentation for the given class.

An OMask is [typedeffed](https://en.wikipedia.org/wiki/Typedef) as a [uint32_t](https://en.wikipedia.org/wiki/C_data_types#Fixed-width_integer_types), meaning it is a value of 4 bytes which contains 32 bits, all reserved for event masks which is up to the implementation of the derived class.
A single [callback functon](https://www.tutorialspoint.com/callbacks-in-c) can be linked to multiple event masks, but if this is the case then it is recommended to structure your function to take in an [OSignal](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OSignal.md) as an argument to further verify which event is actually coming through.