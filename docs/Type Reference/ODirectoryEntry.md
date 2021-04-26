# Type Reference: ODirectoryEntry
## Small struct for a given entry in a Directory
ODirectoryEntry is a small wrapper for a specific entry (file or folder) of a Directory accessed by [ODirectory.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectory.md)

## Expansion
```cpp
struct ODirectoryEntry{
	ODEType type;
	char*   name;
	char*   path;

	inline ODirectoryEntry(void) : type{ODT_ERROR}, name{0}, path{0} {}
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
If an ODirectoryEntry is valid, and was properly initialised, it stores both the name and full, [absolute path](https://www.lifewire.com/absolute-and-relative-paths-3466467) to
the given Entry.

The following is an example of the code required to check if an Entry is valid, check if it is a File, and if so, open it:
```cpp
/* myDirectory is defined elsewhere. It is an ODirectory. */
OFile            myFile;
ODirectoryEntry* myEntry=myDirectory[0];

/* Remember, ODT_FILE means the Entry is a File, not a Directory, which needs ODirectory to open. */
if(myEntry && myEntry->type==ODT_FILE){
	myFile.open(myEntry->path);
	/* Do File operations here... */
}
```
Symlinks must first be [dereferenced](https://www.man7.org/linux/man-pages/man2/readlink.2.html) before attempting to open them using OFile.

## Breakdown
```cpp
ODEType type;
```
The type of this Entry.
```cpp
char* name;
```
The name of this Entry.
```cpp
char* path;
```
The full, real path to this Entry.
```cpp
inline ODirectoryEntry(void) : type{ODT_ERROR}, name{0},path{0} {}
```
Empty constructor. Sets all values to 0.

## Other Information
Note that you must **NEVER** free an ODirectoryEntry, or any of its contents manually. This is always handled by its parent [ODirectory](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectory.md).

#### See Also:
[ODirectory](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectory.md)