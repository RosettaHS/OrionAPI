# Function Reference: OFileRename
## Quickly renaming Files

### Declarations
```cpp
bool OFileRename(const char* file, const char* newName);
bool OFileRename(const char* directory, const char* file, const char* newName);
```
### Use
OFileRename is a function that allows for renaming a given File relative to a given Directory.
By default, it will attempt to rename the given File relative to the [Current Working Directory (CWD)](https://en.wikipedia.org/wiki/Working_directory),
however this can be overriden by either passing in an [absolute path](https://www.lifewire.com/absolute-and-relative-paths-3466467)
or passing in a Directory as the first argument and the Filename as the second argument.

To rename a given File, use the following syntax:
```cpp
OFileRename("testfile.txt","renamedfile.txt");
```
This will attempt to rename `testfile.txt` in the CWD to `renamedfile.txt`, and will return `true` on success.

If however you need to rename a given File relative to a separate Directory, use the following syntax:
```cpp
OFileRename(OAPP_DATAPATH,"testfile.txt","renamedfile.txt");
```
Any absolute or relative path can be passed as the first argument, but in this case we're using `OAPP_DATAPATH`, a Directory automatically determined by OrionAPI for storing user data for your OApp.
In this case, it will rename `testfile.txt` to `renamedfile.txt` if it exists within `OAPP_DATAPATH`, returning `true` on success.

Since the second argument is relative to the first, renaming a File within a Folder relative to a given Directory is possible like this:
```cpp
OFileRename(OAPP_DATAPATH,"myFolder/testfile.txt","renamedfile.txt");
```
This will then attempt to attempt to rename `testfile.txt` within `myFolder` (which itself is inside of `OAPP_DATAPATH`) to `renamedfile.txt`, returning `true` on success.

### Other Information
`OAPP_DATAPATH` is allocated by OrionAPI, but it may sometimes be `NULL` if a user DataPath could not be determined. It is also ONLY allocated when `OAppStart()` is ran.

See Also:
[OFileExists](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileExists.md)
[OFileDelete](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileDelete.md)
[OFileGetHash](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileGetHash.md)