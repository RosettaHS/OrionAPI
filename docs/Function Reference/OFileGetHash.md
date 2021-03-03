# Function Reference: OFileGetHash
## Easily comparing the Contents of Files

### Declarations
```cpp
extern OFileHash OFileGetHash(const char* file);
extern OFileHash OFileGetHash(const char* directory, const char* file);
```
### Use
OFileGetHash is a function that allows for getting the Hash of a given File to a given Directory.
By default, it will get the Hash of a File relative to the [Current Working Directory (CWD)](https://en.wikipedia.org/wiki/Working_directory),
however this can be overriden by either passing in an [absolute path](https://www.lifewire.com/absolute-and-relative-paths-3466467)
or passing in a Directory as the first argument and the Filename as the second argument.

The function returns the type `OFileGetHash` (defined as `uint64_t`) which is a simple numeric Hash of a File's Contents,
used for comparing the Contents of two (or more) different Files.

To get the Hash of a given File relative to the CWD, use the following syntax:
```cpp
OFileHash hash=OFileGetHash("testfile.txt");
```
This will check the Application's CWD for a file called `testfile.txt`, and if it exists it will calculate a numeric Hash of the file and return it.
If the File does not exist, or the File has no contents, it will return a hash of `0`. To check if a File exists, see [OFileExists.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileExists.md)

If however you need to get the Hash of a File relative to a separate Directory, use the following syntax:
```cpp
OFileHash hash=OFileGetHash("/usr/lib","libO.so");
```
In this example, it will return the Hash for `/usr/lib/libO.so` (which is the library file for OrionAPI), while automatically [concatenating](https://www.thefreedictionary.com/concatenating) the Directory and the Filename together to produce a true, valid path.
This isn't required in this case, since if you know the initial Directory and the File relative to it, you can just pass it entirely as `/usr/lib/libO.so` as just one argument for this example.

This can also be used in conjunction with the OApp's allocated DataPath, as shown in the following example:
```cpp
OFileHash hash=OFileGetHash(OAPP_DATAPATH,"testfile.txt");
```
`OAPP_DATAPATH` is a Directory automatically determined by OrionAPI for storing user data for your OApp. In this case, it will attempt to calculate and return the Hash of `testfile.txt`.

Since the second argument is relative to the first, checking a File within a Folder within a given Directory is possible like this:
```cpp
OFileHash hash=OFileGetHash(OAPP_DATAPATH,"myFolder/testfile.txt");
```
This will now attempt to calculate the Hash of `testfile.txt`, which is inside of `myFolder` which itself is inside of the `OAPP_DATAPATH`.

### Other Information
While it isn't required, it's often useful to check if a given File exists before trying to get the Hash of it, since an empty File and a non-existant File will both return the same hash (`0`)

`OAPP_DATAPATH` is allocated by OrionAPI, but it may sometimes be `NULL` if a user DataPath could not be determined. It is also ONLY allocated when `OAppStart()` is ran.

See also:
[OFileExists](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileExists.md),
[OFileDelete](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileDelete.md),
[OFileRename](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileRename.md)