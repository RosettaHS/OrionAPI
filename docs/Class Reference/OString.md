# Class Reference: OString
## Handling text natively.

### Definition
```cpp
class OString{								/* Minimal wrapper for strings. */
	private:
		char* str;							/* Internal. The character array that makes up the string. */
		size_t length;						/* The length of the string. */
		bool isMemStatic;					/* Internal. Was the memory dynamically allocated? */
		size_t calcLength(const char*);		/* Internal. Calculates the length of another string. */
	public:
		~OString();							/* Frees all memory (if dynamically allocated) and sets all values to 0. */
		OString();							/* Empty constructor. Sets all values to 0. */
		OString(const char*);				/* Creates the string with the given text. */

		void setTo(const char*);			/* Sets the string to the given text. */
		void append(const char*);			/* Appends the new text to the current string. */

		size_t getLength();					/* Gets the length of the current string. */
		void log(bool verbose=false);		/* Prints the string to the terminal. Pass true if you want all of the data. */

		operator char*() const;				/* Overload. Converts into a char*. Example: char* test=myOString; */
		operator const char*() const;		/* Overload. Same as above, but with a const char* instead. */
		OString& operator+=(const char*);	/* Overload. Same as append(const char*). */
		OString operator+(const char*);		/* Overload. Returns a new string that is the first string with the second string appended. Example: OString newString = oldString+"Appended String!"; */
		OString& operator=(const char*);	/* Overload. Same as setTo(const char*). */
};
```
### Use
OString is the Orion-Native method of handling encapsulated strings.
It allows for Python-inspired string modification, such as easy appendication.

To create an OString for use, use the following syntax.
```cpp
OString myString("This is my string!");
```

# ///UNFINISHED///