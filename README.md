# Welcome to OrionAPI. Luxury + Linux, for everyone.
The Application Framework for the Orion Operating System
### This toolkit is in development and currently does not work!

## What is the OrionAPI?
The OrionAPI is a bundle of different frameworks used for creating Applications native to the Orion Operating System, handling basic Orion-Native Application management for its sub-libraries.

OrionAPI contains the following frameworks:

* OSL - The Orion Standard Library
* OKit - The Window Widget Toolkit for the Orion Operating System

These frameworks make development for the Orion Operating System easier than ever, and easier than using the alternatives.
Some frameworks, such as OKit and parts of OSL, work perfectly on non-Orion systems, such as Ubuntu or Debian.
### What is OSL?
OSL is a set of functions and classes built as a replacement or wrapper to some functions and classes found in C and C++, including things like [OStrings,](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Class%20Reference/OString.md) and [logging functionality](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OLog.md)
Currently incomplete, but will serve as the basis for all of OrionAPI's sub-libraries.
### What is OKit?
OKit is OrionAPI's implementation for a graphical user interface. It allows for making easy GUI-based Applications with minimal overhead and
quick boot times.
OKit is meant to be Orion's replacement for [GTK](https://en.wikipedia.org/wiki/GTK) and [Qt.](https://en.wikipedia.org/wiki/Qt_(toolkit)) Built from the ground up using [Xlib](https://en.wikipedia.org/wiki/Xlib), it is highly minimal and designed for performance and network transparency.
Alongside performance, Applications made with OKit will look and feel just like the rest of the Orion Operating System, because the entirety of Orion's UI will be made using OKit!
#### Could I see a code sample?
Writing a graphical Application using OKit is very simple. The following code is all that is required to get a Window on the screen:
```cpp
#include <OrionAPI>

int main(void){
	OAppStart(); /* Initialises the OrionAPI Service. */

	/* Creates a Window 450px wide and 300px tall in the centre of the screen. */
	OWindow myWindow(CENTRE,CENTRE,450,300,"My Window");

	OAppEnd(); /* Runs OKit, and ends the OrionAPI Service. */
}
```
#### Why should I use OKit?
OKit is designed for extreme ease of use, but also maximum performance. OKit is a brand new [widget toolkit](https://en.wikipedia.org/wiki/Widget_toolkit) utilising the least amount of code possible internally while providing the highest abstraction for the developer.
Making useful graphical Applications will be a breeze when using OKit. Follow the documentation for examples and tutorials!
OKit isn't Orion-Exclusive either, it can be used on other Linux-based systems as well, although it will match the theme of Orion the best.
### How do I start developing with the OrionAPI?
If you're on an Orion-based operating system, your system automatically comes with all libraries required for developing with OrionAPI.
Follow the tutorials and compile your Applications with the following library flag:
```
-lO
```
### When will the OrionAPI be complete?
There is no known time at the moment, however keep an eye on this repository as updates are very frequent to OrionAPI and its sub-frameworks.
New features and elements are being added daily, progress is steady!