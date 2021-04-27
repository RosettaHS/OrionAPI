# Function Reference: OFileDelete
## File Deletion

**[PLACEHOLDER]**

## Declarations:
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
