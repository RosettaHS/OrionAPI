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
		bool forceSelfOnNext;
		friend class CDrawable;
		void tempUnlinkAll(void);
		void tempRelinkAll(void);
		virtual void sort(void);
	public:
		~CContainer(void);
		CContainer(void);

		bool link(CDrawable&);
		bool unlink(CDrawable&);
		void unlinkAll(void);
		int getIndexOf(CDrawable&);	
		OChildList getChildren(void);
		unsigned short getChildCount(void);
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

There are some scenarios where you need to loop through the children owned by a given Container and access or modify the childs' memory.
In order to access the children of a given Container, use the following syntax.
```cpp
OChildList myChildren = myContainer.getChildren();
```
This will return an OChildList, which is an array of pointers to [Drawables](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) (defined as `CDrawable**`)
that you can iterate through to access all of the children owned by a give Container.

To access the amount of children of a given Container, use the following syntax.
```cpp
unsigned short myChildCount = myContainer.getChildCount();
```
This will return an `unsigned short` which contains the number of children owned by a given Container, allowing you to now iterate through the Container's children using the following syntax.
```cpp
for(unsigned short i=0; i<myChildCount; i++){
	myChildren[i]->log(); /* Replace this with something meaningful! */
}
```
This will iterate through the [Drawable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) pointers stored in `myChildren` and, in this example, log their data out to the terminal.
You can use this format to do any sort of data modification or method calling on the children owned by a given Container.
However, do *NOT* attempt to replace, remove. or change the order of any children in that array, as this has a very high likelihood of causing severe problems because the internal child array is a dynamic array that does automatic allocation and ordering.
If you want to remove all children from a given Container, use `unlinkAll()` instead.

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
bool forceSelfOnNext;
```
If this is true, then this Container will ignore `containerToUse` and just use a pointer to itself next time `link()`, `unlink()` or any other method is called.
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
void unlinkAll(void);
```
Unlinks all [Drawables](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) owned by this container. Use with caution!
```cpp
int getIndexOf(CDrawable&);
```
Returns the child index of the passed [Drawable.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md)
If return value is `-1`, this means the given [Drawable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) is not linked to this Container.
```cpp
OChildList getChildren(void);
```
Returns an array of pointers to [Drawables](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) owned by this Container.
OChildList is defined as `Orion::CDrawable**`.
```cpp
unsigned short getChildCount(void);
```
Returns the number of children owned by this Container.
### Other Information
As with all Control classes, you are never to instantiate or create one manually.

Derived classes will all have different implemenations of the `sort()` method, so it is required you review the documentation for the given class.
Some derived classes will not sort their children automatically, and will respect the [positional/size flags](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) of their children.
However some derived classes will sort their children automatically, and override the position and size of their children with respects to the sorting rules of the derived class.

Another thing to note is that while most [Drawables](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) can have their `scale` modifer changed, this is not the case with any Container.
All Containers cannot have their `scale` modified in any way, and attempting to call the `setScale()` method will result in a discarded scale and a warning to the terminal, alerting you that Containers cannot be scaled.
They can, however, be resized, which is an entirely different thing all together.

While you can access the internal array of children by using `getChildren()`, this does NOT mean you should modify anything that comes from that array.
Do NOT attempt to set the position or size of any children gathered from that array, and do NOT attempt to remove, re-order, or replace any children within that array.
This can and will cause serious problems and will not work as intended. The reason why is because the internal array of Containers is a dynamic array that reallocates new memory whenever it is resized beyond a certain point and,
it will automatically shuffle and re-order the contents of the internal array whenever an object is linked or unlinked,
meaning iterating through the children and unlinking them will either cause a crash or cause an unintuitive outcome. If you need to remove all children, please use the `unlinkAll()` method instead.