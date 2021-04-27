# Function Reference: OFileGetHash
## Unique File Hash

**[PLACEHOLDER]**

## Declarations:
```cpp
	/**
	 */
extern OFileHash OFileGetHash(const char* filename);
	/**
	 * @return A numeric hash of the File's contents.
	 */
extern OFileHash OFileGetHash(const char* directory, const char* filename);
```
```
[Library  : OSL]
[Declared : src/include/OSL/OFile.hpp]
[Defined  : src/OSL/OFile.cpp]
```

### Single-Parameter Version
Returns the FileHash of the given file relative to the OApp's [working directory](https://en.wikipedia.org/wiki/Working_directory).
#### Parameters
`const char* filename` - The name/path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) of the File to calculate the hash of.
#### Returns
A [numeric hash](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md#comparing-filesofilehash) of the File's contents.

### Dual-Parameter Version
Returns the FileHash of the file relative to the given directory.
#### Parameters
`const char* directory` - A path [(absolute or relative)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to attempt to scan for the File in.

`const char* filename` - The name/path of the File to calculate the hash of relative to the given directory.
#### Returns
A [numeric hash](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md#comparing-filesofilehash) of the File's contents.