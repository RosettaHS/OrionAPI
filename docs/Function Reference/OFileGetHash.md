# Function Reference: OFileGetHash
## Unique File Hash

## Declarations
```cpp
extern OFileHash OFileGetHash(const char* filename);
extern OFileHash OFileGetHash(const char* directory, const char* filename);
```
```
[Library  : OSL]
[Declared : src/include/OSL/OFile.hpp]
[Defined  : src/OSL/OFile.cpp]
```

### Single-Parameter Version
Returns the [FileHash](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md#comparing-filesofilehash) of the given file relative to the OApp's [working directory](https://en.wikipedia.org/wiki/Working_directory).
#### Parameters
`const char* filename` - The name/path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) of the File to calculate the hash of.
#### Returns
A [numeric hash](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md#comparing-filesofilehash) of the File's contents.

### Dual-Parameter Version
Returns the [FileHash](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md#comparing-filesofilehash)  of the file relative to the given directory.
#### Parameters
`const char* directory` - A path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to attempt to scan for the File in.

`const char* filename` - The name/path of the File to calculate the hash of relative to the given directory.
#### Returns
A [numeric hash](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md#comparing-filesofilehash) of the File's contents.

## Use
OFileGetHash is used for computing a simple [hash](https://en.wikipedia.org/wiki/Hash_function) of a given File at the given path. The path can be either [absolute or relative](https://www.lifewire.com/absolute-and-relative-paths-3466467).

### OFileHash
OFileHash is an [alias](https://en.wikipedia.org/wiki/Typedef) for the type `uint64_t`.
It is a number that contains the (simple) [numerical hash](https://en.wikipedia.org/wiki/Hash_function) of a File's content.

(Note that OFileHash's [altname](https://github.com/RosettaHS/OrionAPI/blob/main/docs/API%20Configuration.md#orion_noaltnames) is `filehash_t`)

This number is unique to the Contents and only the Contents. 
The File itself can be different, but if the Contents are identical then the hash will be the same.

Comparing two FileHashes can be done using the `==` operator:
```cpp
OFileHash myHash=OFileGetHash("someFile.txt");
OFileHash myOtherHash=OFileGetHash("someOtherFile.txt);

if( myHash==myOtherHash ){
	...
}
```
If both of these Files share the identical Content, the condition will be `true` and any code within the block will be executed.
Note that an empty File and a non-existent File both have the identical hash `0`.

In order to retrieve an OFileHash, you can use the OFileGetHash function.

OFileGetHash provides two different [overloads](https://www.tutorialspoint.com/cplusplus/cpp_overloading.htm). A Single-Parameter version, and a Dual-Parameter version.

### Single-Parameter
By default, all paths inputted are relative to the [current working directory](https://en.wikipedia.org/wiki/Working_directory).

The following code will calculate the hash of a File called `testFile.txt` relative to the CWD:
```cpp
OFileHash myHash=OFileGetHash("testFile.txt"):
```
If the program is ran inside of a Directory with a File called `testFile.txt`, it will attempt to calculate its hash, and if successful, will return the hash.
Otherise the result is `0`. 

It is also possible to pass an absolute path instead of a relative one:
```cpp
OFileHash myHash=OFileHash("/usr/include/stdio.h");
```
On a system following the [Filesystem Hierarchy Standard](https://en.wikipedia.org/wiki/Filesystem_Hierarchy_Standard) (excluding Orion), the result will be the hash of `stdio.h`.

### Dual-Parameter
While the Single-Parameter version only supports either an absolute path or an relative path,
the Dual-Parameter version allows for checking for a File within a scan Directory.

The first parameter is the path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to a Directory to scan for the File (second parameter) within.

The second parameter is the File (relative to the scan Directory) to calculate the hash of.
This is primarily to support the [Application Structure](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md)'s native [helpers](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md#utilising-helpers):
```cpp
OFileHash myHash=OFileHash(OAPP_HOME,"testFile.txt");
```
Instead of utilising the full path to the File, it instead searches for and attempts to calculate the hash of the given File within a parent scan Directory.
In this example, it will attempt to calculate the hash of `testFile.txt` within the user's [Home Directory](https://en.wikipedia.org/wiki/Home_directory) if the File exists.

Multi-level Files can be deleted from scan Directories as well:
```cpp
OFileHash myHash=OFileHash(OAPP_HOME,"Documents/testFile.txt");
```
This will attempt to calculate the hash of`testFile.txt` inside the `Documents` folder within the user's [Home Directory](https://en.wikipedia.org/wiki/Home_directory).

## Other Information
Beware of trying to use relative path syntax `"../LikeThis"` when using the Dual-Parameter version, as this could fail very easily.

#### See Also:
[OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFile.md)

[OFileExists](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileExists.md)

[OFileDelete](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileDelete.md)

[OFileRename](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Function%20Reference/OFileRename.md)