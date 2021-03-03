# Function Reference: OFileExists
## Simple File Checking

### Declarations
```cpp
bool OFileExists(const char* file);
bool OFileExists(const char* directory, const char* file);
```
### Use
OFileExists is a function that allows for checking if a given File exists relative to a given Directory.
By default, it will check if the given File exists relative to the [Current Working Directory (CWD)](https://en.wikipedia.org/wiki/Working_directory),
however this can be overriden by either passing in an [absolute path](https://www.lifewire.com/absolute-and-relative-paths-3466467)
or passing in a Directory as the first argument and the Filename as the second argument.

To check a File exists relative to the CWD, use the following syntax:
```cpp
bool result=OFileExists("testfile.txt");
```
If `testfile.txt` exists relative to the CWD, `result` will be `true`, otherwise it will return `false`.
If however you need to check if a File exists relative to a separate Directory, use the following syntax:
```cpp
bool result=OFileExists("/usr/lib","libO.so");
```
In this example, it will check the Filesystem for `/usr/lib/libO.so` (which is the library file for OrionAPI) and return `true` if it can be found,
automatically [concatenating](https://www.thefreedictionary.com/concatenating) the Directory and the Filename together to produce a true, valid path.
This isn't required in this case, since if you know the initial Directory and the File relative to it, you can just pass it entirely as `/usr/lib/libO.so` as just one argument for this example.

This can also be used in conjunction with the OApp's allocated DataPath, as shown in the following example:
```cpp
bool result=OFileExists(OAPP_DATAPATH,"testfile.txt");
```
`OAPP_DATAPATH` is a Directory automatically determined by OrionAPI for storing user data for your OApp. In this case, it will check if `testfile.txt` exists in this user's DataPath.

Since the second argument is relative to the first, checking a File within a Folder within a given Directory is possible like this:
```cpp
bool result=OFileExists(OAPP_DATAPATH,"myFolder/testfile.txt");
```
This will now check within the `OAPP_DATAPATH` for a Folder named `myFolder`, and then check inside of that for `testfile.txt`, then returns the result if it exists or not.

### Other Information
While it isn't required, it's often useful to check if a given File exists before trying to create or open it using OFile.
`OAPP_DATAPATH` is allocated by OrionAPI, but it may sometimes be `NULL` if a user DataPath could not be determined. It is also ONLY allocated when `OAppStart()` is ran.

See Also:
[OFileDelete](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileDelete.md)