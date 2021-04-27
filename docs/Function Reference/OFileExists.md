# Function Reference: OFileExists
## File Checking

## Declarations:
```cpp
extern bool OFileExists(const char* filename);
extern bool OFileExists(const char* directory, const char* filename);
```
```
[Library  : OSL]
[Declared : src/include/OSL/OFile.hpp]
[Defined  : src/OSL/OFile.cpp]
```

### Single-Parameter Version
Does the given File exist relative to the OApp's [working directory](https://en.wikipedia.org/wiki/Working_directory)?
#### Parameters
`const char* filename` - The name/path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) of the File to check.
#### Returns
`true` if the File exists at the location, otherwise `false`.

### Dual-Parameter Version
Does the File exist relative to the given directory? 
#### Parameters
`const char* directory` - A path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to attempt to search for the File in.

`const char* filename` - The name/path of the File to check relative to the given directory.
#### Returns
`true` if the File exists at the location, otherwise `false`.

## Use
OFileExists is used for checking if a given File exists at the given path. The path can either be [absolute or relative](https://www.lifewire.com/absolute-and-relative-paths-3466467).

OFileExists provides two different [overloads](https://www.tutorialspoint.com/cplusplus/cpp_overloading.htm). A Single-Parameter version, and a Dual-Parameter version.

### Single-Parameter
By default, all paths inputted are relative to the [current working directory](https://en.wikipedia.org/wiki/Working_directory).

The following code will check if there is a File labelled `stdio.h` relative to the CWD:
```cpp
bool myResult=OFileExists("stdio.h");
```
If the program is ran inside of the system's [include directory](https://en.wikipedia.org/wiki/Filesystem_Hierarchy_Standard#Directory_structure), the result will be `true`.
If, however, the program is ran outside of there (or any Directory that doesn't have a file named `stdio.h`), the result will be `false`.

It is also possible to pass an absolute path instead of a relative one. The following code checks if the File `stdio.h` exists within the system's [include directory](https://en.wikipedia.org/wiki/Filesystem_Hierarchy_Standard#Directory_structure):
```cpp
bool myResult=OFileExists("/usr/include/stdio.h");
```
On a system following the [Filesystem Hierarchy Standard](https://en.wikipedia.org/wiki/Filesystem_Hierarchy_Standard) (excluding Orion), the result will be true.

### Dual-Parameter
While the Single-Parameter version only supports either an absolute path or an relative path,
the Dual-Parameter version allows for checking for a File within a scan Directory.

The first parameter is the path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to a Directory to scan for the File (second parameter) within.

The second parameter is the File (relative to the scan Directory) to access.

As an example, the following code also checks if the File `stdio.h` exists within the system's [include directory](https://en.wikipedia.org/wiki/Filesystem_Hierarchy_Standard#Directory_structure):
```cpp
bool myResult=OFileExists("/usr/include","stdio.h");
```
Instead of utilising the full path to the File, it instead searches for the given File within a parent scan Directory.
This is primarily to support the [Application Structure](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md)'s native [helpers](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md#utilising-helpers):
```cpp
bool myResult=OFileExists(OAPP_HOME,"stdio.h");
```
This will check if the File `stdio.h` exists within the user's [Home Directory](https://en.wikipedia.org/wiki/Home_directory).

Multi-level Files can be accessed from scan Directories as well:
```cpp
bool myResult=OFileExists("/usr","include/stdio.h");
```

## Other Information
Beware of trying to use relative path syntax `"../LikeThis"` when using the Dual-Parameter version, as this could fail very easily.

#### See Also:
[OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFile.md)

[OFileDelete](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileDelete.md)

[OFileRename](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileRename.md)

[OFileGetHash](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileGetHash.md)