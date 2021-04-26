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
`const char* filename` - The name/path (absolute or relative) of the File to check.
#### Returns
`true` if the File exists at the location, otherwise `false`.

### Dual-Parameter Version
Does the File exist relative to the given directory? 
#### Parameters
`const char* directory` - A path (absolute or relative) to attempt to search for the File in.

`const char* filename` - The name/path of the File to check relative to the given directory.
#### Returns
`true` if the File exists at the location, otherwise `false`.

## Use
**[PLACEHOLDER]**