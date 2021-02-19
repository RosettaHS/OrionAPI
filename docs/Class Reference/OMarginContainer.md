# Class Reference: OMarginContainer
## Containing UI Elements within a margin.

### Definition
```cpp
class OMarginContainer : public OContainer{
	friend void DRAW::OMarginContainer(CDrawable*);
	protected:
		unsigned short margin_left;
		unsigned short margin_right;
		unsigned short margin_top;
		unsigned short margin_bottom;
		CContext expandedBackground;
		bool expand;

		virtual void onLink(void) override;
		virtual void onUnlink(void) override;
		virtual void onPosChanged(void) override;
		virtual void onSizeChanged(void) override;
		virtual void sort(void) override;

		OMarginContainer(void);
	public:
		OMarginContainer(CContainer& parent, int x, int y, unsigned int w, unsigned int h, unsigned short margin=8, bool expand=true);

		void setEdgeExpansion(bool);
		void setMargin(unsigned short);
		void setLeftMargin(unsigned short);
		void setRightMargin(unsigned short);
		void setTopMargin(unsigned short);
		void setBottomMargin(unsigned short);

		unsigned short getLeftMargin(void);
		unsigned short getRightMargin(void);
		unsigned short getTopMargin(void);
		unsigned short getBottomMargin(void);
};
```
### Use
OMarginContainer is a clone of [OContainer,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OContainer.md) with the key difference being that OMarginContainer renders its children within a margin.

Extending from [OContainer,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OContainer.md) OMarginContainer inherents its base classes and methods. Review the documentation of the base classes for more details.

To create an OMarginContainer for use, use the following syntax:
```cpp
OMarginContainer myMarginContainer(myParentContainer,0,0,200,200); /* Replace myParentContainer with the Container you wish to link this MarginContainer to! */
```
This will create a MarginContainer with a size of (200,200) points and positioned on the top left of the given parent Container.
In this case, the parent Container would likely be an OWindow, however it could be anything that defers from the [base Container class.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CContainer.md)

By default, the MarginContainer will have a margin of `8`, meaning any children linked to it will be separated from the edges by 8 points.
In this case, a 200 by 200 MarginContainer will have an effective size of 184 by 184.

Linking a child to an OMarginContainer is done the same way as any other [Container:](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CContainer.md)
```cpp
myMarginContainer.link(myDrawable);
/* Alternatively, you could also do this: */
myDrawable.linkTo(myMarginContainer);
```
This will now append the passed [Drawable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) to the MarginContainer's list of child elements,
and will now allow the child to draw its content onto the surface of this MarginContainer, and the child's positional values are now relative to the top left of this MarginContainer, plus the margins.

The difference between this and an [OContainer](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OContainer.md) is that the children will be kept at a distance from the edges by using their given margins.
This will be expanded upon later on.

Much like with any other [Container,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CContainer.md) unlinking a child from a given MarginContainer is done using the following syntax:
```cpp
myMarginContainer.unlink(myDrawable);
/* Alternatively, you could also do this: */
myDrawable.unlinkThis();
```
This will now remove the child from the internal list of children, and remove the child's render content from this MarginContainer's surface.

Same as with an [OContainer,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OContainer.md) OMarginContainer does no automatic sorting, and respects [Drawables'](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) positional and sizing flags, such as `START`, `CENTRE`, `END`, and `FILL`.

#### Margining
By default, the margin for a MarginContainer across all sides is `8`. When constructing a new MarginContainer, you can override this by specifying the overall margin as the 6th parameter in the constructor.
```cpp
OMarginContainer myMarginContainer(myParentContainer,0,0,200,200,4);
```
Now the overall margin will be `4`.

You can manually set the overall margin after creation by using the `setMargin()` method.
```cpp
myMarginContainer.setMargin(8);
```
This will now set the overall margin back to `8`, and will automatically sort its children to fit the new margin.

However, an overall margin may not be applicable in some circumstances, and sometimes you may need a different margin per edge.
This can be achieved with the edge margin setters.
```cpp
/* You can use these methods individually, but in this example we're using all of them. */
myMarginContainer.setLeftMargin(8);
myMarginContainer.setRightMargin(8);
myMarginContainer.setTopMargin(4);
myMarginContainer.setBottomMargin(4);
```
In this example, the margin across the left and right sides will be `8`, giving the MarginContainer an effective horizontal size of 184,
but sets the margin across the top and bottom to `4`, giving the MarginContainer an effective vertical size of 192.
Setting any of these margins to `0` is valid, and will strip the margin out on that side entirely.

Another thing to note is the background of the MarginContainer will fit its actual size (200,200 in this case) by default,
however it is possible to make the MarginContainer's background shrink down by its margin, and fit against its children.

You can change this by passing a `true`(default) or `false` as the 7th argument of the constructor.
```cpp
OMarginContainer myMarginContainer(myParentContainer,0,0,200,200,4,false); /* Default for the 7th argument is "true"! */
```
Now this MarginContainer's background will shrink down by its margins along the sides, in this case it will shrink overall by `4`, giving it an effective and visual size of (192,192).

Setting this after creation can be done by using the `setEdgeExpansion()` method, although this is advised against due to the intense recalculations involved.
It is recommended to avoid this all-together and set this in the constructor alongside your margin of choice.

### Structure Breakdown
#### The following methods and variables are protected, and cannot be accessed by the developer.
```cpp
friend void DRAW::OMarginContainer(CDrawable*);
```
Allows the external draw function for MarginContainers to access the internal, protected data of this MarginContainer.
```cpp
unsigned short margin_left;
```
The margin along the left side.
```cpp
unsigned short margin_right;
```
The margin along the right side.
```cpp
unsigned short margin_top;
```
The margin along the top.
```cpp
unsigned short margin_bottom;
```
The margin along the bottom.
```cpp
CContext expandedBackground;
```
The background that fits to the actual extents of this MarginContainer if `expand` is `true`.
```cpp
bool expand;
```
Should the background fit the actual extents? `true` by default, but if `false`, the background will fit to the margin.
```cpp
virtual void onLink(void) override;
```
Calls whenever this MarginContainer gets linked to a parent. Creates the Container on the surface of its parent, and renders its children (if it has any).
```cpp
virtual void onUnlink(void) override;
```
Calls whenever this MarginContainer gets unlinked from its parent. Unrenders this MarginContainer, and temporarily unlinks its children
until this MarginContainer gets linked again.
```cpp
virtual void onPosChanged(void) override;
```
Calls whenever this MarginContainer gets its position modified. Moves the MarginContainer to the its new position relative to its parent.
```cpp
virtual void onSizeChanged(void) override;
```
Calls whenever this MarginContainer gets its size modified. Resizes the MarginContainer to the its new size, and calls `sort()`.
```cpp
virtual void sort(void) override;
```
Goes through its children, and modifies the position or size of child [Drawables](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) if they are using any positional or sizing flags.
```cpp
OMarginContainer(void);
```
Empty constructor. Sets all values to 0.
#### The following methods and variables are public, and can be accessed by the developer.
```cpp
OMarginContainer(CContainer& parent, int x, int y, unsigned int w, unsigned int h, unsigned short margin=8, bool expand=true);
```
Creates and links a new MarginContainer to the given parent with the given positional and sizing values.
Optionally, pass a number on the 6th argument to set a default, overall margin for this MarginContainer.
And also optionally, pass a boolean to the 7th argument to enable or disable edge expansion.
```cpp
void setEdgeExpansion(bool);
```
Decides whether this MarginContainer's background should fit its sizing extents, or shrink and fit its margin against its children. Default is `true`.
```cpp
void setMargin(unsigned short);
```
Sets the overall margin across all sides.
```cpp
void setLeftMargin(unsigned short);
```
Sets the margin along the left side.
```cpp
void setRightMargin(unsigned short);
```
Sets the margin along the right side.
```cpp
void setTopMargin(unsigned short);
```
Sets the margin along the top.
```cpp
void setBottomMargin(unsigned short);
```
Sets the margin along the bottom.
```cpp
unsigned short getLeftMargin(void);
```
Returns the margin along the left side.
```cpp
unsigned short getRightMargin(void);
```
Returns the margin along the right side.
```cpp
unsigned short getTopMargin(void);
```
Returns the margin along the bottom.
```cpp
unsigned short getBottomMargin(void);
```
Returns the margin along the top.

### Other Information
By default, new MarginContainers will use the secondary colour from the [OApp's theme.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md#other-information)
Passing in another colour from the [OApp's theme,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md#other-information) such as `OTHEME_PRIMARY`, will cause the Container to use that colour and automatically change to the new variant when the [OApp's theme](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md#other-information) gets modified.

Another thing to note is that no [Containers](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CContainer.md) are allowed to have their `scale` modified,
attempting to use `setScale()` on a derived Container in the same way you would an ordinary [Drawable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md) will result in a warning message in the terminal and an ignored value.

Also by default, MarginContainers' backgrounds will fit the raw sizing extents. However, this is *slightly* more intensive than having the MarginContainer fit only its margin,
as an edge expansion requires the use of another Context due to the structure of [Drawables.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CDrawable.md)