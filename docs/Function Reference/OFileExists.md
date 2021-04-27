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
### Dual-Parameter
placeholder