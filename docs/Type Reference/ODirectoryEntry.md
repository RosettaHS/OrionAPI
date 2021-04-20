# Type Reference: ODirectoryEntry
## Small struct for a given entry in a Directory

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

## Other Information

#### See Also:
[ODirectory](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectory.md)