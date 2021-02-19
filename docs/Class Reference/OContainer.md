# Class Reference: OContainer
## Containing UI Elements.

### Definition
```cpp
class OContainer : public CDrawable, public CContainer{
	friend void DRAW::OContainer(CDrawable*);
	protected:
		virtual void onLink(void) override;
		virtual void onUnlink(void) override;
		virtual void onPosChanged(void) override;
		virtual void onSizeChanged(void) override;
		virtual void sort(void) override;

		OContainer(void);
	public:
		OContainer(CContainer& parent, int x, int y, unsigned int w, unsigned int h);

		virtual void setScale(float) override;
		virtual void setCol(unsigned char r, unsigned char g, unsigned char b) override; virtual void setCol(OCol&) override;
};
```
### Use
OContainer is a [Container](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CContainer.md) that does stores and displays child [Drawables.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md)
It does not do any automatic sorting, and lets the [Drawables](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) either use positional and sizing flags for automatic handling, or lets them use their own positional and sizing values.

OContainer extends from [CDrawable,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) and [CContainer,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CContainer.md) thus inherents its methods. Review the documentation the base classes for more details.

To create an OContainer for use, use the following syntax:
```cpp
OContainer myContainer(myParentContainer,0,0,200,200); /* Replace myParentContainer with the Container you wish to link this Container to! */
```
This will create a Container with a size of (200,200) points and positioned on the top left of the given parent Container.
In this case, the parent Container would likely be an OWindow, however it could be anything that defers from the [base Container class.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CContainer.md)

Once your Container has been initialised, you can link any child [Drawable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) to it using the following syntax:
```cpp
myContainer.link(myDrawable);
/* Alternatively, you could also do this: */
myDrawable.linkTo(myContainer);
```
This will now append the passed [Drawable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) to the Container's list of child elements,
and will now allow the child to draw its content onto the surface of this Container, and the child's positional values are now relative to the top left of this Container.

Unlinking a child from a given Container is done using the following syntax:
```cpp
myContainer.unlink(myDrawable);
/* Alternatively, you could also do this: */
myDrawable.unlinkThis();
```
This will now remove the child from the internal list of children, and remove the child's render content from this Container's surface.

As stated before, this Container does no automatic sorting, and respects [Drawables'](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) positional and sizing flags, such as `START`, `CENTRE`, `END`, and `FILL`.

Other methods relating to handling children of this Container can be be found in the [documentation for the base Container class.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CContainer.md)

You can also set the background colour of a Container. By default, it will use the [application theme's](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Class%20Reference/OTheme.md#overriding) secondary colour.
To override this, and set a manual colour for this Container, use the following syntax:
```cpp
myContainer.setCol(255,255,255);
/* Alternatively, you could also do this: */
myContainer.setCol(myOCol);
```
The Container's background will now be pure white (255,255,255).

### Structure Breakdown
#### The following methods and variables are protected, and cannot be accessed by the developer.
```cpp
friend void DRAW::OContainer(CDrawable*);
```
Allows the external draw function for Containers to access the internal, protected data of this Container.
```cpp
virtual void onLink(void) override;
```
Calls whenever this Container gets linked to a parent. Creates the Container on the surface of its parent, and renders its children (if it has any).
```cpp
virtual void onUnlink(void) override;
```
Calls whenever this Container gets unlinked from its parent. Unrenders this Container, and temporarily unlinks its children
until this Container gets linked again.
```cpp
virtual void onPosChanged(void) override;
```
Calls whenever this Container gets its position modified. Moves the Container to the its new position relative to its parent.
```cpp
virtual void onSizeChanged(void) override;
```
Calls whenever this Container gets its size modified. Resizes the Container to the its new size, and calls `sort()`.
```cpp
virtual void sort(void) override;
```
Goes through its children, and modifies the position or size of child [Drawables](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) if they are using any positional or sizing flags.
```cpp
OContainer(void);
```
Empty constructor. Sets all values to 0.
#### The following methods and variables are public, and can be accessed by the developer.
```cpp
OContainer(CContainer& parent, int x, int y, unsigned int w, unsigned int h);
```
Creates and renders this Container as a child of the given parent using the given position and size.
```cpp
virtual void setScale(float) override;
```
No [Containers](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CContainer.md) are allowed to rescale, this method will cause a warning and ignore the passed value. Do not call this!
```cpp
virtual void setCol(unsigned char r, unsigned char g, unsigned char b) override;
virtual void setCol(OCol&) override;
```
Sets the background colour of this Container to the passed R,G, and B values.
Also has an overload for an [OCol.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md)

### Other Information
By default, new Containers will use the secondary colour from the [OApp's theme.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md#other-information)
Passing in another colour from the [OApp's theme,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md#other-information) such as `OTHEME_PRIMARY`, will cause the Container to use that colour and automatically change to the new variant when the [OApp's theme](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md#other-information) gets modified.

Another thing to note is that no [Containers](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CContainer.md) are allowed to have their `scale` modified,
attempting to use `setScale()` on a derived Container in the same way you would an ordinary [Drawable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) will result in a warning message in the terminal and an ignored value.

When a Container gets unlinked from its parent, it will store its children in a temporary array, and when the Container gets added to a new parent, it will re-append its children to itself and draw them correctly, allowing for easy movement and re-parenting of Containers.