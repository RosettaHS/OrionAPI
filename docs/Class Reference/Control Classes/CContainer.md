# Control Class Reference: CContainer
## Automatic handling of multiple UI elements

### Definition
```cpp
class CContainer{
	protected:
		CContext selfContext;
		CNodeArray arr;
		unsigned short childCount;
		CContext* contextToUse;
		CDrawable* drawableToUse;
		CContainer* containerToUse;
		friend class CDrawable;
		void tempUnlinkAll(void);
		void tempRelinkAll(void);
		virtual void sort(void);
	public:
		~CContainer(void);
		CContainer(void);

		bool link(CDrawable&);
		bool unlink(CDrawable&);
		int getIndexOf(CDrawable&);	
};
```
### Use
CContainer is the base class that handles containing multiple UI elements and provides utilities for derived classes to use when modifying the data of its children.
It does not do any modification or rendering on its own, it merely serves as a baseline for all derived Containers and stores data regarding any linked children.

As a result, it does not have many public methods that developers may interact with, only allowing for linking and unlinking of children and getting data of said children.

To link a UI element to a given Container, use the following syntax.
```cpp
myContainer.link(myChildElement); /* Or you can use myChildElement.linkTo(myContainer); */
```
If this succeeds (and returns `true`), the passed UI element will be added as a child to the given Container, and will begin to render its content to the Container.
At this stage it may either retain its current size and position (now relative to the Container), or it may be automaticallly resized and repositioned according to the given derived Container. Review the documentation for the derived Container for further details.

If you wish to unlink a UI element from a given Container, use the following syntax.
```cpp
myContainer.unlink(myChildElement); /* Or you can use myChildElement.unlinkThis(); */
```
This will now remove the UI element as a child from the Container as if it were never linked in the first place.
Do NOT attempt to modify any data on the (former) child element after it has been unlinked, this could cause problems and potentially a crash!

Containers retain information about their children after the Container itself has been unlinked, meaning you can move Containers between other Containers and have its children remain intact.

### Structure Breakdown
#### The following methods and variables are protected, and cannot be accessed by developers.
```cpp
CContext selfContext;
```
The usable area goverened by this Container.
```cpp
CNodeArray arr;
```
The array that contains pointers to this Container's children.
```cpp
unsigned short childCount;
```
The count of children owned by this Container.
```cpp
CContext* contextToUse;
```
A pointer to the usable area for children to render their content onto.
```cpp
CDrawable* drawableToUse;
```
A pointer to a [Drawable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) that relates to this Container.
```cpp
CContainer* containerToUse;
```
A pointer to a Container that the `link()` and `unlink()` methods will actually call to.
This is because some derived Containers consist of numerous Containers strung together to make one, and thus would need a pointer to the one that new children should actually link to.
```cpp
friend class CDrawable;
```
Allows [CDrawables](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) to access internal, protected data of this Container.
```cpp
void tempUnlinkAll(void);
```
Calls whenever this Container is unlinked from its parent. This temporarily unlinks its children to prevent errors but stores their data for when this Container gets relinked to another one.
```cpp
void tempRelinkAll(void);
```
Calls whenever this Container is linked to a new parent. This reads the data stored by `tempUnlinkAll()` and will relink its stored children to itself after it fully initialises to its new parent.
```cpp
virtual void sort(void);
```
Calls whenever this Container resizes, or gets/loses children.
#### The following methods and variables are public, and can be accessed by developers.
```cpp
~CContainer(void);
```
Destructor. Frees all memory and unlinks all children.
```cpp
CContainer(void);
```
Empty constructor. Sets all values to 0.
```cpp
bool link(CDrawable&);
```
Links a given [Drawable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) to this Container.
Returns true on success.
```cpp
bool unlink(CDrawable&);
```
Unlinks a given [Drawable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) from this Container.
Returns true on success, false if could not unlink, or was never linked in the first place.
```cpp
int getIndexOf(CDrawable&);
```
Returns the child index of the passed [Drawable.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md)
If return value is `-1`, this means the given [Drawable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) is not linked to this Container.

### Other Information
As with all Control classes, you are never to instantiate or create one manually.

Derived classes will all have different implemenations of the `sort()` method, so it is required you review the documentation for the given class.
Some derived classes will not sort their children automatically, and will respect the [positional/size flags](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) of their children.
However some derived classes will sort their children automatically, and override the position and size of their children with respects to the sorting rules of the derived class.

Another thing to note is that while most [Drawables](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) can have their `scale` modifer changed, this is not the case with any Container.
All Containers cannot have their `scale` modified in any way, and attempting to call the `setScale()` method will result in a discarded scale and a warning to the terminal, alerting you that Containers cannot be scaled.
They can, however, be resized, which is an entirely different thing all together.