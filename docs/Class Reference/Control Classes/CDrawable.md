# Control Class Reference: CDrawable
## Abstractive Rendering

### Definition
```cpp
class CDrawable : public CBaseUI , public CLoggable {
	protected:
		int x,y;
		int centreX, centreY;
		int offsetX, offsetY;
		unsigned int w,h;
		unsigned int minW,minH;
		float scale;
		float rotation;
		int index;
		bool fullRedraw;

		CContext* context;
		CDrawable* parentDrawable;
		CContainer* parentContainer;

		OTheme internalTheme;
		CTheme theme;
		uint8_t themeFlags;

		bool setFlag(short xFlag, short yFlag, short wFlag, short hFlag);
		void init(int, int, unsigned int, unsigned int);
		virtual void onLink(void);
		virtual void onUnlink(void);
		virtual void onPosChanged(void);
		virtual void onSizeChanged(void);

		friend class CContainer;
	public:
		struct{
			void(*drawPtr)(CDrawable*);	
			uint8_t modFlags;
		}internal;

		CDrawable(void);

		bool linkTo(CContainer&);
		bool unlinkThis(void);


		void setPos(int x, int y); void setPos(OVec&);
		void setCentre(int x, int y);
		void setSize(unsigned int w,unsigned int h, bool force=false); void setSize(OVec&, bool force=false);
		void setMinSize(unsigned int w, unsigned int h); void setMinSize(OVec&);
		virtual void setScale(float);
		void setRotation(float);

		virtual void setCol(unsigned char r, unsigned char g, unsigned char b); virtual void setCol(OCol&);
	
		void setTheme(OTheme&);
		void setThemePrimaryCol(unsigned char r, unsigned char g, unsigned char b); void setThemePrimaryCol(OCol&);
		void setThemeSecondaryCol(unsigned char r, unsigned char g, unsigned char b); void setThemeSecondaryCol(OCol&);
		void setThemeTertiaryCol(unsigned char r, unsigned char g, unsigned char b); void setThemeTertiaryCol(OCol&);
		void setThemeAccentCol(unsigned char r, unsigned char g, unsigned char b); void setThemeAccentCol(OCol&);
		void resetTheme(void);

		CDrawable* getParent(void);
		OVec getPos(bool globalToWindow=false);
		OVec getCentre(void);
		OVec4 getSize(bool useScale=false);
		OVec4 getMinSize(bool useScale=false);
		float getScale(void);
		float getRotation(bool includeParents=false);
		OVec4 getGeometry(bool globalToWindow=false);
		int getIndex(void);

		OTheme getTheme(void);

		virtual void log(bool verbose=false) override;
};
```
### Use
CDrawable is the base class for all UI. It extends from [CLoggable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md) and [CBaseUI.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CBaseUI.md)
It contains all the variables and methods that derived classes can use to handle containing, modificaion, and rendering of UI elements.
All methods contain base implementations but derived classes can modify the definitions for some to better suit the needs of that class.

Drawables must be linked to a CContainer-derived class in order to render its elements, otherwise render instructions are ignored and a warning is emitted.
Drawables contain all information of their parents and get updated when its parent changes its state.

The constructors for all CDrawable-derived classes often follows the same structure:
```cpp
(CContainer& parent, int x, int y, unsigned int w, unsigned int h, ...);
```
The first argument is always the parent container that the Drawable will link and render its content on. Things such as OWindows, OContainers, and other Containers can be passed.
Second and third arguments always refer to the X and Y position relative to the top-left(usually) of the Container.
Alongside taking in direct numeric values, it can take in flags such as `START`,`CENTRE`(alternatively, `CENTER`), and `END`. These flags govern whether the container should keep this Drawable aligned to a specific axis or not.
These are ignored in Containers that do automatic sorting, but base containers such as OContainer or OWindow that do no automatic sorting will respect these flags.
`START` will tell the Container to keep this Drawable at the start of the given axis. If the X position is set to `START`, the Drawable will be kept at the far left of the Container.
if the Y position is `START`, it will keep the Drawable at the top of that axis. `CENTRE` or `CENTER` (identical) will keep the Drawable at the centre of the axis.

These are interchangable between the positional values. Any configuration of `START`, `CENTRE`, and `END` can exist between the X and Y position values of the Drawable, so long as the Container respects the flags.

The fourth and fifth arguments always refer to the horizontal and vertical size of the Drawable, and much like the positional arguments this can take in either direct explicit size or can use the `FILL` flag,
which tells the Container to make this Drawable stretch to the very end of the given axis.

Any following arguments are up to the implementation of the derived class and you must look at the documentation for said class if you wish to know the proper syntax and breakdown.

`FILL` as a flag can ONLY be put on the width and height arguments, if it is put in the positional arguments, OKit will give out an error and close the program, alerting the developer of this mistake.
Conversely, `START`, `CENTRE`, and `END` positional flags can NOT be put in the size arguments of the Drawable, as they are STRICTLY POSITIONAL ONLY. This will cause an error and a crash.
Another thing of note is that the `FILL` size flag can NOT be paired with `CENTRE` or `END` flags on the same axis.
For example, the X positional flag being `START` and the W size flag being `FILL` is legal, but X positional flag being `CENTRE` or `END` and the W size flag being `FILL` is illegal, and will cause an error and a crash.

Most Containers do automatic sorting and alignment, so they will ignore these flags, but base containers such as OContainer will respect these flags and align the Drawable based on the developer's wishes.

One thing all Containers do respect is the minimum size of the Drawable. When a Container automatically sorts, it will shrink and stretch children to all properly fit within the bounds of the Container.
However, the Container will not shrink a child Drawable smaller than the minimum size. This will be explained later on in this documentation alongside method reference.

To link a Drawable to a Container, use the following syntax.
```cpp
myDrawable.linkTo(myContainer);
```
This will attempt to link the Drawable to the passed Container and add it as a child. If this succeeds, it will return `true` and your Drawable will render on that Container.
Do NOT attempt to link Drawables to non-initialised Containers, or non-initialised Drawables to Containers.

To unlink a Drawable from its Container, use the following syntax.
```cpp
myDrawable.unlinkThis();
```
This will attempt to unlink the Drawable to its parent Container and it will no longer do any computation, it will be as if it was never created in the first place.
If this succeeds, it will return `true` and your Drawable will disappear from its parent Container and the Container will resort.
If the Drawable has not been linked to a Container, or it failed to unlink, it will return `false`.

Alternatively, similar methods exist on Containers to link Drawables directly.
```cpp
myContainer.link(myDrawable); /* This is identical to myDrawable.linkTo(myContainer); */
myContainer.unlink(myDrawable); /* This is identical to myDrawable.unlinkThis(); */
```

To retrieve the parent of a given Drawable, use `getParent()` to retrieve a pointer to it, then cast it into a [CBaseUI*](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CBaseUI.md) to then get its `type`, and cast from there.
And to retrieve the index at which this Drawable is stored in the child array of its parent Container, use the `getIndex()` method to retrieve it. If it returns `-1` it either could not find the index, or it is not linked to a Container.


If the parent Container handles automatic sorting (view the documentation for the given Container first!), do NOT attempt to manually modify the position or size of the child Drawable.
The Container will handle this for you and will discard any modifications you make.

#### The following two functions only apply if the Container does not do automatic sorting.

To modify the position of a given Drawable relative to its parent, use the following syntax.
```cpp
myDrawable.setPos(100,100); /* Replace this with the values you wish to set. */
```
This will set the Drawable's position to 100 points horizontally and 100 points vertically relative to the top-left of its Parent.
Alternatively, an [OVec](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OVec.md) can be passed in as an argument.

If you wish to retrieve the position of a given Drawable, use `getPos()` to retrieve the local position of the Drawable. To get the global position, pass `true` and it will return the position of the Drawable relative to the Window.

To modify the size of a given Drawable, use the following syntax.
```cpp
myDrawable.setSize(50,50); /* Replace this with the values you wish to set. */
```
This will set the Drawable's size to 50 points horizontally and 50 points vertically. If you attempt to set the size to a number smaller than the minimum size of the given Drawable, it will clamp to the minimum size and ignore anything smaller.
If you attempt to set the size to the current size of the Drawable, it will ignore your input and return from the function early to save resources on rendering. To force a render, pass `true` as the third argument. The default is `false`.

All Drawables have a minimum allowed size for use with automatic sorting in a Container.
This governs the minimum size a Drawable can be resized, and no amount of brute-forcing can cause the Drawable to shrink below this point without setting the scale override.

If you wish to retrieve the size of a given Drawable, use `getSize()` to retrieve the size of the Drawable. To get the true, scaled size, pass `true`.

To modify the minimum size of a Drawable, use the following syntax.
```cpp
myDrawable.setMinSize(25,25); /* Replace this with the values you wish to set. */
```
This now sets the minimum allowed size of the Drawable to 25 points horizontally and vertically, and causes the parent Container to resort its children.
*This can cause major stability problems so only set this if you know what you're doing!*

Creating a Drawable with a size below the default minimum size for the Drawable will set the minimum size to the given size to prevent frustration and complication. Read documentation for the given derived-class for more information.

If you wish to retrieve the minimum size of a given Drawable, use `getMinSize()` to retrieve the minimum size of the Drawable. To get the true, scaled minimum allowed size, pass `true`.

Individual Drawables have an internal scale modifier which scales the entire Drawable relative to its centre while still using the same point values as before.
A Drawable with the size of `(50,50)` and a scale of `2` will have an effective size of `(100,100)` while its `getSize()` method will still return `(50,50)` unless you pass `true`.

To set the scale of the Drawable, use the following syntax.
```cpp
myDrawable.setScale(2); /* Replace this with the value you wish to set. */
```
It is recommended to use [whole numbers](https://stackoverflow.com/a/13530287) instead of floating points, but there are times where this is necessary. Use floating points with caution.
This scales the Drawable by the given amount relative to its centre, meaning the centre of the given Drawable will stay fixed in place but the edges will expand by the given value.
This is merely visual and does not affect the actual position and size values of the Drawable, meaning scaling a Drawable child of a parent Container will not cause the Container to sort its children any differently than if the Drawable had an unmodified scale.
The minimum allowed size is `0.5f`, if the scale is set below this it will clamp to `0.5f`.

Another thing to note is only individual elements (such as OButton, and ORect) are allowed to scale, but Containers are not allowed to scale due to the intensive and complex mathematics required.
Attempting to call `setScale()` on a Container will produce a warning in the terminal and ignore the passed value.

If you wish to retrieve the scale of a given Drawable, use `getScale()` to retrieve the internal scale of the Drawable.

Since setting the scale causes the Drawable to expand by the centre, the centre can be manually modified by using the following syntax.
```cpp
myDrawable.setCentre(0,0); /* Replace this with the value you wish to set. */
```
This causes the effective centre of the Drawable to be at `(0,0)` (the top left), and scaling the Drawable after this call will result in the scale being from the top left, keeping the origin fixed in place.
It is not recommended to manually set this if the Drawable is in an automatically sorting Container, or the Drawable's size is modified frequently,
because setting the size of the Drawable automatically resets the centre to the correct objective centre of the Drawable and will override any modifications.

The effective centre can be placed out of the Drawable's bounds but this will be incredibly buggy and unstable and could cause potential problems with the rendering system.

If you wish to retrieve the centre of a given Drawable, use `getCentre()` to retrieve the centre of the Drawable.

Since calling both `getPos()` and `getSize()` is quite common to get the complete position and size values of the Drawable, there is a method that does exactly this.
```cpp
OVec4 myVec4 = myDrawable.getGeometry();
```
This returns an [OVec4](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OVec.md) that contains the local position and size of the given Drawable in its `x`, `y`, `w`, and `h` members.
To retrieve the global position of the Drawable, pass `true`.

Some Drawables have the ability for their colour to be modified, if the given derived class allows this, use the following syntax to modify the colour.
```cpp
myDrawable.setCol(255,255,255); /* Replace this with the colour you wish to set. */
```
This would set the Drawable's colour to pure white. This is a [virtual method,](https://en.cppreference.com/w/cpp/language/virtual) so you must read the documentation for the given derived class to see how this specific method is used, if it is allowed to be used at all.

Alternatively, all Drawables also contain an internal theme system that can be used to give colour overrides. *Do NOT call any of the following functions unless you KNOW EXACTLY WHAT YOU'RE DOING!*
```cpp
myDrawable.setTheme(myTheme); /* Replace this with the OTheme you wish to set as an override. */
```
This is HIGHLY discouraged, but is still given as an option for the rare circumstance where you need to override a given Drawable's theme.

You can also directly modify specific theme values of the given Drawable with any of the following methods.
```cpp
myDrawable.setThemePrimaryCol(myCol);
myDrawable.setThemeSecondaryCol(myCol);
myDrawable.setThemeTertiaryCol(myCol);
myDrawable.setThemeAccentCol(myCol);
```
These are entirely up to the derived class for implmentation and often these are very confusing to modify directly.
For example, to modify the colour of an ORect, you would have to call `setThemeSecondaryCol()` instead of `setThemePrimaryCol()` as intuition would dictate.
This is why `setCol()` is a [virtual method,](https://en.cppreference.com/w/cpp/language/virtual) since it allows to derived classes to offer custom implementation and redirects.

If you have caused serious damage to your Drawable by modifiying its internal theme, you can call the `resetTheme()` method to entirely reset the theme data of the Drawable.
This can still cause issues in some derived classes so it is better to never modfify the theme data in the first place.

Since CDrawable-derived classes extend from [CLoggable,](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md) they can have their data logged to the terminal both minimally and verbosely.
By default, base Drawables will only log their position and size out to the terminal by calling the `log()` method.
But by passing `true`, base Drawables will print an incredible amount of diagnostic information about the exact state and internal variables of the Drawable.

This is entirely handled by the implementation of the derived class, review the documentation of the given derived class for further information.

### Structure Breakdown

#### The following are protected members and cannot be accessed by the developers. 
```cpp
int x,y;
```
The local X and Y positional data of this Drawable relative to its parent Container.
```cpp
int centreX, centreY;
```
The local positional data pertaining to the centre of this Drawable relative to the Drawable's top left.
```cpp
int offsetX, offsetY;
```
The actual X and Y positional data of this Drawable taking into account the centre scaled by the internal scale. Used in rendering.
```cpp
unsigned int w,h;
```
The width and height of this Drawable.
```cpp
unsigned int minW,minH;
```
The minimum allowed width and height of this Drawable when being resized. Cannot shrink below this point.
```cpp
float scale;
```
The internal scale modifier used by this Drawable during rendering. Does not affect the position or size values and thus does not cause its parent Container to sort any differently.
```cpp
float rotation;
```
Unused and reserved for potential future implementation.
```cpp
int index;
```
The child index of this Drawable pertaining to its parent Container. Is -1 if not linked.
```cpp
bool fullRedraw;
```
Should the Drawable do a full wipe and render when its draw function is called? Is always false unless size is modified, or specified otherwise by derived implementations.
```cpp
CContext* context;
```
A pointer to the Context that this Drawable will render its content onto when linked.
```cpp
Drawable* parentDrawable;
```
A pointer to this Drawable's parent Drawable if it is linked.
```cpp
Container* parentContainer;
```
A pointer to this Drawable's parent Container if it is linked.
```cpp
OTheme internalTheme;
```
A collection of [OCols](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md) that this Drawable will point to when its theme is overridden.
```cpp
CTheme theme;
```
A collection of pointers to [OCols](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md) that this Drawable's render function will actually use to draw onto the screen.
```cpp
uint8_t themeFlags;
```
4(out of 8) bits to store whether the internal theme has been modified by any setTheme- methods.
```cpp
bool setFlag(short xFlag, short yFlag, short wFlag, short hFlag);
```
Checks the passed paremeter to see if it matches any allowed governing flags for the given axis. Takes in one at a time and closes the program if invalid parameter is found.
```cpp
void init(int, int, unsigned int, unsigned int);
```
Initialises the position and size values or flags (if they are flags) for the derived class.
```cpp
virtual void onLink(void);
```
Calls whenever this Drawable gets linked to a Container. Entirely up to the derived class for implementation.
```cpp
virtual void onUnlink(void);
```
Calls whenever this Drawable gets unlinked from a Container. Entirely up to the derived class for implementation.
```cpp
virtual void onPosChanged(void);
```
Calls whenever this Drawable gets its position modified. Entirely up to the derived class for implementation.
```cpp
virtual void onSizeChanged(void);
```
Calls whenever this Drawable gets its size modified. Entirely up to the derived class for implementation.
```cpp
friend class CContainer;
```
Allows CContainer to access base CDrawable protected member variables.
```cpp
struct{
	void(*drawPtr)(CDrawable*);	
	uint8_t modFlags;
}internal;
```
`drawPtr` is a pointer to a function that takes in a pointer to this Drawable and handles the actual rendering. [This is more efficient than using a virtual method](https://stackoverflow.com/a/453001)

`modFlags` store the Container governing flags set by `setFlag()`, and is read by non-automatically sorting Containers.

#### The following methods are public and can be accessed by developers.

```cpp
CDrawable(void);
```
Empty constructor. Sets all values to 0.
```cpp
bool linkTo(CContainer&);
```
Links this Drawable to a Container. Returns `true` if linkage was successful.
```cpp
bool unlinkThis(void);
```
Unlinks this Drawable from its parent Container. Returns `true` if removal was successful.
```cpp
void setPos(int x, int y);
void setPos(OVec&);
```
Sets the local position of this Drawable relative to its parent Container. This is ignored if the Container handles automatic sorting.
Also has overload for passing in an [OVec.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OVec.md)
```cpp
void setCentre(int x, int y);
```
Sets the centre of this Drawable relative to the top left of the Drawable. Use with caution, as this is unstable and only meant for debug purposes. Overridden on position or size modification.
```cpp
void setSize(unsigned int w,unsigned int h, bool force=false);
void setSize(OVec&, bool force=false);
```
Sets the size of this Drawable. Ignores values if values are identical to current values, or values are smaller than the minimum allowed size for this Drawable. Pass true to force a render after setting size.
Also has overload for passing in an [OVec.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OVec.md)
```cpp
void setMinSize(unsigned int w, unsigned int h);
void setMinSize(OVec&);
```
Sets the minimum allowed size of the Drawable and causes its parent Container to resort.
Also has overload for passing in an [OVec.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OVec.md)
```cpp
virtual void setScale(float);
```
Sets the visual scale of this Drawable and cause it to expand or shrink from its centre by the given value. Minimum allowed value is `0.5f`.
This is NOT allowed on Containers, and will log a warning message to the terminal! Review the documentation for the derived class for further information.
```cpp
void setRotation(float);
```
Unused and reserved for potential future implementation.
```cpp
virtual void setCol(unsigned char r, unsigned char g, unsigned char b);
virtual void setCol(OCol&);
```
Sets the colour of this Drawable to the given values. Some derived classes do not allow this and wil print a warning message to the terminal, please see documentation for the given derived class.
Also has overload for passing in an [OCol.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md)
```cpp
void setTheme(OTheme&);
```
Highly discouraged. Replaces the internal [theme](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OTheme.md) with the passed one. Use with caution! 
```cpp
void setThemePrimaryCol(unsigned char r, unsigned char g, unsigned char b);
void setThemePrimaryCol(OCol&);
```
Highly discouraged. Sets the primary colour of the internal [theme](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OTheme.md) with the passed one. Use with caution! 
Also has overload for passing in an [OCol.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md)
```cpp
void setThemeSecondaryCol(unsigned char r, unsigned char g, unsigned char b);
void setThemeSecondaryCol(OCol&);
```
Highly discouraged. Sets the secondary colour of the internal [theme](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OTheme.md) with the passed one. Use with caution! 
Also has overload for passing in an [OCol.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md)
```cpp
void setThemeTertiaryCol(unsigned char r, unsigned char g, unsigned char b);
void setThemeTertiaryCol(OCol&);
```
Highly discouraged. Sets the tertiary colour of the internal [theme](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OTheme.md) with the passed one. Use with caution! 
Also has overload for passing in an [OCol.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md)
```cpp
void setThemeAccentCol(unsigned char r, unsigned char g, unsigned char b);
void setThemeAccentCol(OCol&);
```
Highly discouraged. Sets the accent colour of the internal [theme](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OTheme.md) with the passed one. Use with caution! 
Also has overload for passing in an [OCol.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md)
```cpp
void resetTheme(void);
```
Resets the internal theme back to its default values after the inevitable damage you will cause by messing with the theme overrides.
```cpp
CDrawable* getParent(void);
```
Returns a pointer to the parent Container (in the form of a Drawable) if this Drawable is linked.
If pointer is not `0`, you can cast it back into its original type by casting it based on its `type` member to an [enumeration of possible object types](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CBaseUI.md) to cast it back into an appropriate pointer.
```cpp
OVec getPos(bool globalToWindow=false);
```
Returns an [OVec](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OVec.md) with the `x` and `y` members set to the positional data of this Drawable.
Pass `true` to get the global position relative to the top level Window the Drawable is on.
```cpp
OVec getCentre(void);
```
Returns an [OVec](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OVec.md) with the `x` and `y` members set to the position of the centre of this Drawable relative to the top left of the Drawable.
```cpp
OVec4 getSize(bool useScale=false);
```
Returns an [OVec4](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OVec.md) with the `w` and `h` members set to the size of the Drawable.
Pass `true` to retrieve the actual, scaled size of the Drawable.
```cpp
OVec4 getMinSize(bool useScale=false);
```
Returns an [OVec4](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OVec.md) with the `w` and `h` members set to the minimum allowed size of the Drawable.
Pass `true` to retrieve the actual, scaled minimum size of the Drawable.
```cpp
float getScale(void);
```
Returns the internal visual scale for this Drawable.
```cpp
float getRotation(bool includeParents=false);
```
Unused and reserved for potential future implementation.
```cpp
OVec4 getGeometry(bool globalToWindow=false);
```
Returns an [OVec4](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OVec.md) with the `x` and `y` members set to the Drawable's position, and the `w` and `h` members set to the size of the Drawable.
Pass `true` to retrieve the global position of the Drawable in the `x` and `y` members of the [OVec4.](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OVec.md)

```cpp
int getIndex(void);
```
Returns the child index of this Drawable pertaining to its parent Container. Returns `-1` if not linked, or could not find index.
```cpp
OTheme getTheme(void);
```
Returns the internal [theme](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OTheme.md) of this Drawable.
```cpp
virtual void log(bool verbose=false) override;
```
Logs the information of this Drawable to the terminal. Pass true for more verbose diagnostic information.

### Other Information
As with all Control classes, you are never to instantiate or create one manually.

Many of the functions that CDrawable has are [virtual methods,](https://en.cppreference.com/w/cpp/language/virtual)
meaning you are required to look at the documentaion for the derived class for further information on how the function may operate on that class, since there is no guaranteed behaviour.

As is stated numerous times throughout this documentation, while most Containers derive from CDrawable, they are NOT allowed to use the `setScale()` method, and will cause a warning message in the terminal.
This is due to the huge computational overhead required to achieve scaling with children and child Containers and the potential hierarchies that will be created.
Individual Drawable classes that are not Containers (such as OButton and ORect) are allowed to use `setScale()` due to them not being able to have children/

A Drawable's scale is merely visual, and does not affect the actual position and size data, meaning Containers ignore the custom scale and treat all children as if they had a scale of `1`.

Drawables do not use [virtual methods](https://en.cppreference.com/w/cpp/language/virtual) for their draw functions.
Instead, they have [pointers to external functions](https://www.cprogramming.com/tutorial/function-pointers.html) that handle drawing of that specific deferred class.
This complicates the rendering process, however it provides [much higher performance](https://stackoverflow.com/a/453001) compared to a [virtual method,](https://en.cppreference.com/w/cpp/language/virtual) due to the lack of a [vTable lookup.](https://en.wikipedia.org/wiki/Virtual_method_table)

These external functions are stored within the `DRAW::` namespace, and have the same name as the derived class that utilises them.
For example, ORect's draw function is stored as `Orion::DRAW::ORect(Orion::CDrawable*)`, and takes in a pointer to a CDrawable, where it later casts it into the deferred class (in this case, an ORect.)