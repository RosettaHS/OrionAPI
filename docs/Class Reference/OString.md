# Class Reference: OString
## Handling text natively.

### Definition
```cpp
class OString : public CLoggable {
	private:
		char* str;
		size_t length;
		bool isMemStatic;
		size_t calcLength(const char*);
	public:
		~OString(void);
		OString(void);
		OString(const char*);

		void setTo(const char*);
		void append(const char*);

		size_t getLength(void);
		virtual void log(bool verbose=false) override;

		operator char*(void) const;
		operator const char*(void) const;
		OString& operator+=(const char*);
		OString operator+(const char*);
		OString& operator=(const char*);
		char operator[](size_t);
};
```
### Use
OString is the Orion-Native method of handling encapsulated strings.
They are meant to be a replacement of using more bloated encapsulated strings such as `std::string` from the C++ Standard Library.
It allows for Python-inspired string modification, such as easily appending two strings together.
OString extends from [CLoggable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md), meaning it can be logged both verbosely and minimally.

To create an OString for use, use the following syntax.
```cpp
OString myString("This is my string!");
```

To append another string to an OString, you can either do
```cpp
myString.append(" Here's another string attached!"); // which results in "This is my string! Here's another string attached!"
```
Or you could do
```cpp
myString+=" Here's another string attached!"; 
// which also results in "This is my string! Here's another string attached!"
```

To override the data of an OString, you can either do
```cpp
myString.setTo("Here's my new string."); 
// which turns it from "This is my string!" to "Here's my new string."
```
Or you could do
```cpp
myString="Here's my new string."; 
// which also turns it from "This is my string!" to "Here's my new string."
```

You can print the data of a given OString to the terminal by calling the `log()` function on it.
Passing `true` to `log()` would print out extra data on the OString, such as the length and the pointer to the OString itself.


OStrings can be converted to a `const char*` or a `char*` implicitly. For example.
```cpp
void myPrintingFunction(const char* text){
	printf("%s",text);
}
```
`myPrintingFunction` could either take in `myPrintingFunction("Here's a string literal");` or `myPrintingFunction(myString);` and would still work exactly as intended.

### Structure Breakdown
```cpp
char* str;
```
Private. This is the raw string data and is ever interacted with directly. OString acts as a wrapper for this.
```cpp
size_t length;
```
Private. This is the length of the string. You can retrieve this by calling the `getLength()` function.
```cpp
bool isMemStatic;
```
Private. Was the memory dynamically allocated or does it come from a string literal? The purpose of this is to prevent OString from freeing a string literal
when the OString comes out of scope. If the OString dynamically allocated memory (such as with `append()`), it will free the newly-allocated memory upon deletion.
```cpp
size_t calcLength(const char*);
```
Private and Internal. Calculates the length of the internal string and sets `length` to that.
This is called internally whenever the internal string is modified.
```cpp
~OString(void);
```
Internal. Destructor. Frees the memory if `isMemStatic` is false (It is false if the OString has dynamically allocated memory, like when the OString has been appended.)
```cpp
OString(void);
```
Empty constructor. Sets all values to 0.
```cpp
OString(const char*);
```
Creates and initalises the OString with the string literal passed in. 
Only pass in string literals, as passing in any other pointer could cause huge problems if the pointer origin comes out of scope or is freed before the OString.
```cpp
void setTo(const char*);
```
Sets the OString to the string literal passed in.
Only pass in string literals, as passing in any other pointer could cause huge problems if the pointer origin comes out of scope or is freed before the OString.
```cpp
void append(const char*);
```
Appends the string literal to the OString. This causes the OString to dynamically allocate memory for the new string, setting `isMemStatic` to false.
```cpp
size_t getLength(void);
```
Returns the `length` of the OString.
```cpp
virtual void log(bool verbose=false) override;
```
Logs the data of the OString to the terminal. Pass in `true` if you want more verbose data, such as the pointer to the OString and the length of the OString.
```cpp
operator char*(void) const;
```
Override. Allows for implicit conversion of the OString to a `char*`.
Example: `char* myCharArray=myString;`

```cpp
operator const char*(void) const;
```
Override. Allows for implicit conversion of the OString to a `const char*`.
Example: `const char* myCharArray=myString;`

```cpp
OString& operator+=(const char*);
```
Override. Redirects to `append()`.

```cpp
OString operator+(const char*);
```
Override. Creates and returns a new OString that is a mix of the left OString appended with the right OString/String literal.
Example: `OString newString = myString+" Another appended string!";`

```cpp
OString& operator=(const char*);
```
Override. Copies the data of the right OString/String literal to the left OString.
Example: `OString newString="Assigned from a String Literal!";`
```cpp
char operator[](size_t);
```
Override. Returns the character at the given index using the `[]` operator.
Example: `char myChar=myString[4];`

### Other Information
OStrings are often used internally in UI Elements such as `OTextEntry` (coming soon).
If you are making Orion-Native applications, it is advised you use Orion-Native replacements for things from the C++ Standard Library for faster performance.
It's also advised you use OStrings in place of `const char*` due to how minimal OStrings are, and the additional features it provides out-of-the-box, and how they're interchangable with `const char*`s.
