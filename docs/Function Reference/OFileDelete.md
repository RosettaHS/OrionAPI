# Function Reference: OFileDelete
## Deleting Files easily

### Declarations
```cpp
bool OFileDelete(const char* file);
bool OFileDelete(const char* directory, const char* file);
```
### Use
OFileDelete is a function used for deleting a given File exists relative to a given Directory.
By default, it will delete the given File relative to the [Current Working Directory (CWD)](https://en.wikipedia.org/wiki/Working_directory),
however this can be overriden by either passing in an [absolute path](https://www.lifewire.com/absolute-and-relative-paths-3466467)
or passing in a Directory as the first argument and the Filename as the second argument.

To delete a File relative to the CWD, use the following syntax:
```cpp
OFileDelete("testfile.txt");
```
OFileDelete will attempt to delete `testfile.txt`, and if it could be deleted, will return `true`.
If the File does not exist, or could not be deleted, it will return `false`.
However this is deleting a File relative to the CWD, which could be dangerous.

If you want to delete a File relative to another Directory, use the following syntax:
```cpp
OFileDelete(OAPP_DATAPATH,"testfile.txt");
```
Any absolute or relative path can be passed as the first argument, but in this case we're using `OAPP_DATAPATH`, a Directory automatically determined by OrionAPI for storing user data for your OApp.
In this case, it will delete `testfile.txt` if it exists within `OAPP_DATAPATH`, returning `true` on success.

Since the second argument is relative to the first, deleting a File within a Folder relative to a given Directory is possible like this:
```cpp
OFileDelete(OAPP_DATAPATH,"myFolder/testfile.txt");
```
This will then attempt to delete `testfile.txt`, which is inside of `myFolder`, which itself is inside of `OAPP_DATAPATH`, returning `true` on success.

### Other Information
`OAPP_DATAPATH` is allocated by OrionAPI, but it may sometimes be `NULL` if a user DataPath could not be determined. It is also ONLY allocated when `OAppStart()` is ran.

See Also:
[OFileExists](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileExists.md)
[OFileRename](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileRename.md)
[OFileGetHash](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileGetHash.md)