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

## Other Information

#### See Also:
[ODirectory](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectory.md)