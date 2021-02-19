# Class Reference: ORect
## Colourable Rectangles

### Definition
```cpp
class ORect : public CDrawable{ friend void DRAW::ORect(CDrawable*);
	protected:
		CContext rect;

		virtual void onLink(void) override;
		virtual void onUnlink(void) override;
		virtual void onPosChanged(void) override;
		virtual void onSizeChanged(void) override;

		ORect(void);
	public:
		ORect(CContainer& parent,int x, int y, unsigned int w, unsigned int h, OCol& col=OTHEME_SECONDARY);

		virtual void setCol(unsigned char r, unsigned char g, unsigned char b) override; virtual void setCol(OCol&) override;
}
```
### Use
ORect is a solid, colourable rectangle for use in giving extra detail to parts of your OApp. It doesn't handle any events or any complex calculations, it's merely a rectangle.
ORect extends from [CDrawable,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) and thus inherents its methods. Review the documentation for [CDrawable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) for more details

An ORect has to be a child of a [Container,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CContainer.md) so make a Container first such as an [OContainer](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OContainer.md) or OWindow before you create your Rectangle.

To create an ORect for use, use the following syntax.
```cpp
ORect myRect(myParentContainer,0,0,200,200); /* Replace myParentContainer with the Container you wish to link this Rectangle to! */
```
This will create a Rectangle with a size of (200,200) points and positioned on the top left of the given parent Container.
You can also pass an [OCol](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md) on the sixth argument to set the colour of the Rectangle when it's initialised.

Otherwise, you can modify the colour of an existing Rectangle with the following syntax.
```cpp
myRect.setCol(255,255,255); /* Replace this with the colour you want your Rectangle to be! */
```
This will set the Rectangle's colour to the RGB values passed to the method. By default, Rectangles will use the [default OApp theme's secondary colour,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md#other-information) so it may not be visible against [OContainers,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OContainer.md) as they also use the secondary theme colour.

Modifying other attributes of a Rectangle, such as its position, scale, and size, are done using methods in its base class, [CDrawable,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) so it is best you review documentation of that to see the full list of methods.

### Structure Breakdown
#### The following methods and variables are protected, and cannot be accessed by the developer.
```cpp
friend void DRAW::ORect(CDrawable*);
```
Allows the external draw function for Rectangles to access the internal, protected data of this Rectangle.
```cpp
CContext rect;
```
The actual Rectangle on the screen.
```cpp
virtual void onLink(void) override;
```
Calls whenever this Rectangle gets linked to a parent. Initialises some variables and creates the actual Rectangle on the screen.
```cpp
virtual void onUnlink(void) override;
```
Calls whenever this Rectangle gets unlinked from its parent. Destroys and unrenders the actual Rectangle on the screen.
```cpp
virtual void onPosChanged(void) override;
```
Calls whenever this Rectangle gets its position modified. Moves the Rectangle to the its new position relative to its parent.
```cpp
virtual void onSizeChanged(void) override;
```
Calls whenever this Rectangle gets its size modified. Resizes the Rectangle to the its new size.
```cpp
ORect(void);
```
Empty constructor. Sets all values to 0.
#### The following methods and variables are public, and can be accessed by the developer.
```cpp
ORect(CContainer& parent,int x, int y, unsigned int w, unsigned int h, OCol& col=OTHEME_SECONDARY);
```
Constructor. Creates and initialises a Rectangle with the given arguments.
```cpp
virtual void setCol(unsigned char r, unsigned char g, unsigned char b) override;
virtual void setCol(OCol&) override;
```
Sets the colour of this Rectangle to the passed R,G, and B values.
Also has an overload for an [OCol.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md)

### Other Information
By default, new Rectangles will use the secondary colour from the [OApp's theme.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md#other-information)
You may override this by passing in an [OCol](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md) as the sixth argument in the constructor.
Passing in another colour from the [OApp's theme,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md#other-information) such as `OTHEME_PRIMARY`, will cause the Rectangle to use that colour and automatically change to the new variant when the [OApp's theme](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md#other-information) gets modified.

Alternatively, you can pass in an [OCol](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md) to the `setCol()` method of the Rectangle, or pass in the RGB values directly.