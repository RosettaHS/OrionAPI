# Function Reference: OFormat
## String Formatting

### Declaration
```cpp
void OFormat(char* output, const char* format, ...);
```
### Use
OFormat is a way to easily format a String to a buffer. It works identically to [sprintf,](https://www.tutorialspoint.com/c_standard_library/c_function_sprintf.htm) and can be interchangable.

You have to first allocate a buffer for the String to be formatted and outputted to, in this example we're going to use a [stack allocated array](https://www.tutorialspoint.com/cprogramming/c_arrays.htm) as our buffer.
```cpp
char buffer[100];
```
The buffer has to have enough memory allocated to support the entire length of the formatted String.
Be careful, as not allocating enough memory to your buffer might not show immediate signs of failure, however could result in horrific crashes and/or a [segfault.](https://en.wikipedia.org/wiki/Segmentation_fault)
In this example, `100` bytes is more than enough.

OFormat works identically to [OLog and its variants,](https://github.com/RosettaHS/OKit/blob/main/docs/Function%20Reference/OLog.md) the key difference being it sends the formatted String out to a buffer instead of [stdout.](https://en.wikipedia.org/wiki/Standard_streams#Standard_output_(stdout))

After we've allocated our buffer, use the following syntax to format a String to it:
```cpp
OFormat(buffer, "Hello %s", "World");
```
**REMINDER:** The buffer *MUST* be large enough to contain the formatted String.
With this example, `buffer` will now contain `"Hello World"`

More complex formatting is possible, as shown here:
```cpp
OFormat(buffer, "The price of a %s is %d dollars and %d cents.", "pizza", 5, 99);
```
Now `buffer` will contain `"The price of a pizza is 5 dollars and 99 cents."`

### Other Information
As was stated previously, the output buffer *MUST* be large enough to contain the String, otherwise fatal errors could occur, but won't occur immediately, making tracking the error incredibly difficult.

OFormat will also NOT allocate a buffer for you, attempting to pass `NULL` as the output buffer will result in an error.