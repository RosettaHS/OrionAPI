# Function Reference: OFileRename
## File Renaming

## Declarations
```cpp
extern bool OFileRename(const char* filename, const char* newName);
extern bool OFileRename(const char* directory, const char* filename, const char* newName);
```
```
[Library  : OSL]
[Declared : src/include/OSL/OFile.hpp]
[Defined  : src/OSL/OFile.cpp]
```

### Dual-Parameter Version
Renames the File (relative to the OApp's [working directory](https://en.wikipedia.org/wiki/Working_directory)) to the given name. Returns true on success.
#### Parameters
`const char* filename` - The name/path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) of the File to rename.

`const char* newName` - The new name to rename the File as. Note that this is relative to the OApp's [working directory](https://en.wikipedia.org/wiki/Working_directory), not relative to the File's location.
#### Returns
`true` if the File could be renamed, otherwise `false`.

### Tri-Parameter Version
Renames the File (relative to the given directory) to the given name. Returns true on success.
#### Parameters
`const char* directory` - A path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to attempt to scan for the File in.

`const char* filename` - The name/path of the File to rename relative to the given directory.

`const char* newName` - The new name to rename the File as. Note that this is relative to the given directory, not relative to the File's location within subfolders of the directory.
#### Returns
`true` if the File could be renamed, otherwise `false`.

## Use
OFileRename is used for renaming a File to another name at a given path. The path can either be [absolute or relative](https://www.lifewire.com/absolute-and-relative-paths-3466467).

OFileExists provides two different [overloads](https://www.tutorialspoint.com/cplusplus/cpp_overloading.htm). A Dual-Parameter version, and a Tri-Parameter version.

### Dual-Parameter
By default, all paths inputted are relative to the [current working directory](https://en.wikipedia.org/wiki/Working_directory).

The following code will rename the File `testFile.txt` (if it exists) to `demoFile.txt`
```cpp
bool myResult=OFileRename("testFile.txt","demoFile.txt");
```
If the program is ran inside of a Directory with a File called `testFile.txt`, it will attempt to rename it to `demoFile.txt`, and if successful, will return `true`.
Otherise the result is `false`. 

It is also possible to pass an absolute path instead of a relative one.

### Tri-Parameter
While the Single-Parameter version only supports either an absolute path or an relative path,
the Dual-Parameter version allows for checking for a File within a scan Directory.

The first parameter is the path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to a Directory to scan for the File (second parameter) within.

The second parameter is the File (relative to the scan Directory) to rename.

The third parameter is the new name for the given File.

This is primarily to support the [Application Structure](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md)'s native [helpers](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md#utilising-helpers):
```cpp
bool myResult=OFileRename(OAPP_HOME,"testFile.txt","demoFile.txt");
```
Instead of utilising the full path to the File, it instead searches for and attempts to rename the given File within a parent scan Directory.
In this example, it will attempt to rename the File `testFile.txt` within the user's [Home Directory](https://en.wikipedia.org/wiki/Home_directory) to `demoFile.txt` if the File exists.

Multi-level Files can be renamed from scan Directories as well:
```cpp
bool myResult=OFileDelete(OAPP_HOME,"Documents/testFile.txt","Documents/demoFile.txt");
```
This will attempt to rename the File `testFile.txt` inside the `Documents` folder within the user's [Home Directory](https://en.wikipedia.org/wiki/Home_directory) to `demoFile.txt`

It is **VERY** important to note that, if you are renaming a multi-level File, you **MUST** include the Sub-Directories within the new name as well, as shown in the previous example.

The following example is **incorrect**:
```cpp
bool myResult=OFileDelete(OAPP_HOME,"Documents/testFile.txt","demoFile.txt");
```
Instead of renaming the File `testFile.txt` within the `Documents` folder, it will instead move it outside (to the Home Directory) and gives it a new name there.

## Other Information
Beware of trying to use relative path syntax `"../LikeThis"` when using the Dual-Parameter version, as this could fail very easily.

#### See Also:
[OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFile.md)

[OFileExists](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileExists.md)

[OFileDelete](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileDelete.md)

[OFileGetHash](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileGetHash.md)