# Function Reference: ODirectoryExists
## Directory Evaluation

## Declarations:
```cpp
extern bool ODirectoryExists(const char* directory);
extern bool ODirectoryExists(const char* parentDirectory, const char* subDirectory);
```
```
[Library  : OSL]
[Declared : src/include/OSL/ODirectory.hpp]
[Defined  : src/OSL/ODirectory.cpp]
```

### Single-Parameter Version
Does the given Directory exist relative to the OApp's working directory?
#### Parameters
`const char* directory` - The name/path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) of the Directory to check.
#### Returns
`true` if the Directory exists at the location, otherwise `false`.

### Double-Parameter Version
Does the Sub-Directory exist relative to the given Directory? 
#### Parameters
`const char* parentDirectory` - A path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to attempt to search for the Sub-Directory in.

`const char* subDirectory` - The name/path of the Sub-Directory to check relative to the given Directory.
#### Returns
`true` if the Sub-Directory exists at the location, otherwise `false`.