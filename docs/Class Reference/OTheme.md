# Class Reference: OTheme
## Dealing with many colours to build UI.

### Definition
```cpp
struct OTheme : public CLoggable {
	OCol primary;
	OCol secondary;
	OCol tertiary;
	OCol accent;

	void setPrimary(unsigned char r, unsigned char g, unsigned char b);
	void setSecondary(unsigned char r, unsigned char g, unsigned char b);
	void setTertiary(unsigned char r, unsigned char g, unsigned char b);
	void setAccent(unsigned char r, unsigned char g, unsigned char b);
	virtual void log(bool verbose=false) override;
};
```

### Use
OTheme is a container used to house multiple [OCols](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md) for use in a GUI element, such as a button or window.
It's strictly a structure that makes moving or storing mulitple [OCols](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md) easier. It provides no extra functionality to [OCol]((https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md)), and is functionally identical to storing 4 [OCols](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md) manually.
To create an OTheme for use, use the following syntax.
```cpp
OTheme myTheme();
myTheme.setPrimary(255,255,255);
myTheme.setSecondary(100,100,100);
myTheme.setTertiary(150,150,150);
myTheme.setAccent(25,25,25);
```
The constructor takes no data, as this is meant to provide no extra functionality.
To assign [OCols](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md) to the theme, either set the colours directly with the functions above, or copy an [OCol]((https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md)) directly to a given value.
```cpp
OCol myPrimary(255,255,255);
OTheme myTheme();
myTheme.primary=myPrimary;
//etc...
```

### Structure Breakdown
```cpp
OCol primary;
```
The primary colour of the OTheme. Often used for backgrounds of GUI elements, such as the background of an OWindow.
```cpp
OCol secondary;
```
The secondary colour of the OTheme. Often used as the main colour of top-level GUI elements, such as the face of an OButton.
```cpp
OCol tertiary;
```
The tertiary colour of the OTheme. Rarely used, but is used as a border colour between neighbouring accent-less GUI elements.
```cpp
OCol accent;
```
The accent colour of the OTheme. Often used as a border seen around a GUI element, such as the coloured border around an OWindow or OButton.
```cpp
void setPrimary(unsigned char r, unsigned char g, unsigned char b);
void setSecondary(unsigned char r, unsigned char g, unsigned char b);
void setTertiary(unsigned char r, unsigned char g, unsigned char b);
void setAccent(unsigned char r, unsigned char g, unsigned char b);
```
Sets the respective colour to the given parameters. Internally it calls `setTo(R,G,B)` on the [OCol](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OCol.md), making it functionally identical to setting the colour manually on a given value.
```cpp
virtual void log(bool verbose=false) override;
```
Prints all data of the OTheme to the terminal in a formatted manner. Pass `true` to get more verbose information.

### Other Information
There are a few macros and global variables that come alongside OThemes
```cpp
extern OTheme OTHEME_GLOBAL;
```
Defined with a macro as `OTHEME`. It is the global theme all GUI elements will automatically be created with.
You can change this manually, however your changes will be overriden with system theme updates if overrides are not set. Learn more further down.
```cpp
OTHEME_PRIMARY
```
This macro is a quick re-route to `Orion::OTHEME_GLOBAL.primary`. This can be modified or read, and is the primary colour that all GUI elements will be created with.
```cpp
OTHEME_SECONDARY
```
This macro is a quick re-route to `Orion::OTHEME_GLOBAL.secondary`. This can be modified or read, and is the secondary colour that all GUI elements will be created with.
```cpp
OTHEME_TERTIARY
```
This macro is a quick re-route to `Orion::OTHEME_GLOBAL.tertiary`. This can be modified or read, and is the tertiary colour that all GUI elements will be created with.
```cpp
OTHEME_ACCENT
```
This macro is a quick re-route to `Orion::OTHEME_GLOBAL.accent`. This can be modified or read, and is the accent colour that all GUI elements will be created with.

#### Overriding
Orion-based operating systems have a built-in theming system that automatically changes the theme of all OApps running on the system.
If you manually set your OApp's global theme, or the theme of individual GUI elements, they will be overridden when Orion's system theme changes.
This is where Overrides come in. There is a series of global booleans that govern whether a value from the global theme will be reset when Orion's system theme changes.
These variables are
```cpp
bool OTHEME_PRIMARY_OVERRIDE;
bool OTHEME_SECONDARY_OVERRIDE;
bool OTHEME_TERTIARY_OVERRIDE;
bool OTHEME_ACCENT_OVERRIDE;
```
Setting any one of these to `true` (as opposed to the default `false`) will tell Orion not to change the given value to the new updated system theme upon a theme change.
Note that there are no macros that point to these variables, so to access them in a global scope you must access them from their namespace.
```
Orion::OTHEME_PRIMARY_OVERRIDE=true;
```

Another thing of note about overriding your OApp's theme is it is highly discouraged. A small change such as changing the accent colour is acceptable,
however large-scale changes such as changing the primary, secondary, and/or tertiary colours are strongly discouraged, as users will see a strong clash between the rest of their system and your OApp.
More importantly, a user can veto your theme override at any time within their system settings, ordering *all* OApps to adhere to the system theme, regardless of developer overrides.
Keep this in mind when you decide to override your OApp's theme.
