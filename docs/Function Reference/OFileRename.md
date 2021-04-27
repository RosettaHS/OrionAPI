# Function Reference: OFileRename
## File Renaming

**[PLACEHOLDER]**

## Declarations:
```cpp
extern bool OFileRename(const char* filename, const char* newName);
extern bool OFileRename(const char* directory, const char* filename, const char* newName);
```
[Library  : OSL]
[Declared : src/include/OSL/OFile.hpp]
[Defined  : src/OSL/OFile.cpp]
```

### Single-Parameter Version
Renames the File (relative to the OApp's [working directory](https://en.wikipedia.org/wiki/Working_directory)) to the given name. Returns true on success.
#### Parameters
`const char* filename` - The name/path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) of the File to rename.

`const char* newName` - The new name to rename the File as. Note that this is relative to the OApp's [working directory](https://en.wikipedia.org/wiki/Working_directory), not relative to the File's location.
#### Returns
`true` if the File could be renamed, otherwise `false`.

### Dual-Parameter Version
Renames the File (relative to the given directory) to the given name. Returns true on success.
#### Parameters
`const char* directory` - A path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to attempt to scan for the File in.

`const char* filename` - The name/path of the File to rename relative to the given directory.

`const char* newName` - The new name to rename the File as. Note that this is relative to the given directory, not relative to the File's location within subfolders of the directory.
#### Returns
`true` if the File could be renamed, otherwise `false`.
