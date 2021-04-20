# Type Reference: ODirectoryEntry
## Small struct for a given entry in a Directory
ODirectoryEntry is a small wrapper for a specific entry (file or folder) of a Directory accessed by [ODirectory.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectory.md)

## Expansion
```cpp
struct ODirectoryEntry{
	ODEType type;
	char*   name;

	inline ODirectoryEntry(void) : type{ODT_ERROR}, name{0} {}
};
```
```
[Altname  : direntry_t]
[Library  : OSL]
[Declared : src/include/OSL/ODirectory.hpp]
[Defined  : <none>]
```

## Use
ODirectoryEntry is used as the return value from the `getEntry()` method of [ODirectory.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectory.md)

To retrieve an ODirectoryEntry for use, use the following syntax:
```cpp
/* myDirectory is defined elsewhere. It is an ODirectory. */
ODirectoryEntry* myEntry=myDirectory[0];
```
If the returned value (`myEntry`) is not `NULL`, it is a valid Entry, and can be used for further operations.

### Determining Entry Type
If valid, the `type` member of the Entry is set to one of a list([enumeration](https://en.wikipedia.org/wiki/Enumerated_type)) of possible file types (as known by the file system):
```
ODT_UNKNOWN - Type could not be determined.
ODT_ERROR   - Entry is invalid. Do not use this Entry!
ODT_DIR     - Entry is a (sub)Directory.
ODT_FILE    - Entry is a File.
ODT_SYML    - Entry is a Symlink.
ODT_SOCKET  - Entry is a UNIX Socket.
ODT_PIPE    - Entry is a Named Pipe/FIFO.
```
It's very important to check the `type` memmber before doing any sort of operation using the Entry.

### Opening using [OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md)
An ODirectoryEntry by itself, **CANNOT** be opened by an [OFile.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md)
Instead, the [ODirectory](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectory.md) used to retrieve the Entry is what is used to retrieve the full path

**[PLACEHOLDER]**

```cpp
OFile myFile;
if(myEntry->type==ODT_FILE){
	myFile.open( myDirectory.getEntryPath(0) );
}

## Other Information

#### See Also:
[ODirectory](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectory.md)