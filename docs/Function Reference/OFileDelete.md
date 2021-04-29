# Function Reference: OFileDelete
## File Deletion

## Declarations
```cpp
extern bool OFileDelete(const char* filename);
extern bool OFileDelete(const char* directory, const char* filename);
```
```
[Library  : OSL]
[Declared : src/include/OSL/OFile.hpp]
[Defined  : src/OSL/OFile.cpp]
```

### Single-Parameter Version
Deletes the given File relative to the OApp's [working directory](https://en.wikipedia.org/wiki/Working_directory).
#### Parameters
`const char* filename` - The name/path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) of the File to delete.
#### Returns
`true` if the File could be deleted, otherwise `false`.

### Dual-Parameter Version
Deletes the File relative to the given directory.
#### Parameters
`const char* directory` - A path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to attempt to delete the File from.

`const char* filename` - The name/path of the File to delete relative to the given directory.
#### Returns
`true` if the File could be deleted, otherwise `false`.

## Use
OFileDelete is used for deleting a given File at the given path. The path can be either [absolute or relative](https://www.lifewire.com/absolute-and-relative-paths-3466467).

OFileDelete provides two different [overloads](https://www.tutorialspoint.com/cplusplus/cpp_overloading.htm). A Single-Parameter version, and a Dual-Parameter version.

### Single-Parameter
By default, all paths inputted are relative to the [current working directory](https://en.wikipedia.org/wiki/Working_directory).

The following code will delete a File called `testFile.txt` relative to the CWD:
```cpp
bool myResult=OFileDelete("testFile.txt"):
```
If the program is ran inside of a Directory with a File called `testFile.txt`, it will attempt to delete it, and if successful, will return `true`.
Otherise the result is `false`. 

It is also possible to pass an absolute path instead of a relative one.

### Dual-Parameter
While the Single-Parameter version only supports either an absolute path or an relative path,
the Dual-Parameter version allows for checking for a File within a scan Directory.

The first parameter is the path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to a Directory to scan for the File (second parameter) within.

The second parameter is the File (relative to the scan Directory) to delete.
This is primarily to support the [Application Structure](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md)'s native [helpers](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md#utilising-helpers):
```cpp
bool myResult=OFileDelete(OAPP_HOME,"testFile.txt");
```
Instead of utilising the full path to the File, it instead searches for and attempts to delete the given File within a parent scan Directory.
In this example, it will attempt to delete the File `testFile.txt` within the user's [Home Directory](https://en.wikipedia.org/wiki/Home_directory) if the File exists.

Multi-level Files can be deleted from scan Directories as well:
```cpp
bool myResult=OFileDelete(OAPP_HOME,"Documents/testFile.txt");
```
This will attempt to delete the File `testFile.txt` inside the `Documents` folder within the user's [Home Directory](https://en.wikipedia.org/wiki/Home_directory).

## Other Information
Beware of trying to use relative path syntax `"../LikeThis"` when using the Dual-Parameter version, as this could fail very easily.

#### See Also:
[OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFile.md)

[OFileExists](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileExists.md)

[OFileRename](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileRename.md)

[OFileGetHash](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileGetHash.md)