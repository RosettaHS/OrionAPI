# Function Reference: ODirectoryExists
## Directory Evaluation

## Declarations
```cpp
extern bool ODirectoryExists(const char* directory);
extern bool ODirectoryExists(const char* parentDirectory, const char* subDirectory);
```
```
[Library  : OSL]
[Declared : src/include/OSL/ODirectory.hpp]
[Defined  : src/OSL/ODirectory.cpp]
```

### Single-Parameter Version
Does the given Directory exist relative to the OApp's [working directory](https://en.wikipedia.org/wiki/Working_directory)?
#### Parameters
`const char* directory` - The name/path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) of the Directory to check.
#### Returns
`true` if the Directory exists at the location, otherwise `false`.

### Dual-Parameter Version
Does the Sub-Directory exist relative to the given Directory? 
#### Parameters
`const char* parentDirectory` - A path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to attempt to search for the Sub-Directory in.

`const char* subDirectory` - The name/path of the Sub-Directory to check relative to the given Directory.
#### Returns
`true` if the Sub-Directory exists at the location, otherwise `false`.

## Use
ODirectoryExists is used for checking if a given Directory exists at the given path. The path can be either [absolute or relative](https://www.lifewire.com/absolute-and-relative-paths-3466467).

ODirectoryExists provides two different [overloads](https://www.tutorialspoint.com/cplusplus/cpp_overloading.htm). A Single-Parameter version, and a Dual-Parameter version.
### Single-Parameter
By default, all paths inputted are relative to the [current working directory](https://en.wikipedia.org/wiki/Working_directory).

The following code will check if there is a Directory labelled `Documents` relative to the CWD:
```cpp
bool myResult=ODirectoryExists("Documents");
```
If the program is ran inside of the [Home Directory](https://en.wikipedia.org/wiki/Home_directory), the result will be `true`.
If, however, the program is ran outside of the [Home Directory](https://en.wikipedia.org/wiki/Home_directory) (or any Directory that does not have a Sub-Directory labelled `Documents`),
the result will be `false`.

It is also possible to pass an absolute directory instead of a relative one.
The following code checks if the system's [include directory](https://en.wikipedia.org/wiki/Filesystem_Hierarchy_Standard#Directory_structure) exists:
```cpp
bool myResult=ODirectoryExists("/usr/include");
```
On a system following the [Filesystem Hierarchy Standard](https://en.wikipedia.org/wiki/Filesystem_Hierarchy_Standard) (excluding Orion), the result will be `true`.

### Dual-Parameter
While the Single-Parameter version only supports either an absolute path or an relative path,
the Dual-Parameter version allows for checking for a (Sub-)Directory within a parent scan Directory.

The first parameter is the path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to a Directory to scan for the sub-Directory (second parameter) within.

The second parameter is the (Sub-)Directory (relative to the scan Directory) to access.

As an example, the following code also checks if the system's [include directory](https://en.wikipedia.org/wiki/Filesystem_Hierarchy_Standard#Directory_structure) exists:
```cpp
bool myResult=ODirectoryExists("/usr","include");
```
Instead of utilising the full path to the Directory, it instead searches for the given Sub-Directory within a parent Directory.
This is primarily to support the [Application Structure](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md)'s native [helpers](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md#utilising-helpers):
```cpp
bool myResult=ODirectoryExists(OAPP_HOME,"Documents");
```
This will then check if the `Documents` folder exists within the user's [Home Directory](https://en.wikipedia.org/wiki/Home_directory).

Multi-level Sub-Directories can be accessed from parent Directories as well:
```cpp
bool myResult=ODirectoryExists("/","usr/include");
```

## Other Information
Beware of trying to use relative path syntax `"../LikeThis"` when using the Dual-Parameter version, as this could fail very easily.

#### See Also:
[ODirectory](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectory.md)