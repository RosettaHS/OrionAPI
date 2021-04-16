# Type Reference: OFileContent
## Small struct for the Contents of a given File.
OFileContent is a wrapper for the data stored by a given File. Used internally by [OFile.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md)

## Expansion
```cpp
struct OFileContent{
	union{
		struct{
			size_t lineCount;
			size_t charCount;
			OFileLine* list;
		}asLines;
		struct{
			size_t   byteCount;
			uint8_t* bytes;
		}asLinear;
	};
	bool modified;
	bool isLinear;
};
```
```
[Altname  : filecontent_t]
[Library  : OSL]
[Declared : src/include/OSL/OFile.hpp]
[Defined  : <none>]
```

## Use
OFileContent, much like [OFileLine,](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFileLine.md) is rarely used directly.
However, it is used internally by a few other types provided by OrionAPI.

Retrieving an OFileContent struct from an [OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md) is done using the following syntax:
```cpp
/* myFile is defined elsewhere, it is an OFile. */
OFileContent* myContent=myFile.getContents();
```

OFileContent stores the raw content of a given File in two different ways.
More information on why it does this can be found in the documentation for [OFile.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md)

### Reading as Lines
The default way [OFiles](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md)(and subsequently OFileContent) store a File's content
is by separating each Line of the File, creating a large array of [OFileLines.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFileLine.md)

Before attempting to retrieve any information from an OFileContent, you must first evaluate how it is storing said information.

The bool `isLinear` is set by the parent [OFile.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md)
If it is `true`, do **NOT** attempt to read the Content as Lines, instead jump to section [Reading Linearly.](#reading-linearly)

This is the correct way to check if the Contents are stored as individual Lines, and then read them:
```cpp
OFileLine myLine;
if(!myContent->isLinear){ /* If is NOT stored linearly, but instead as individual Lines: */
	for(size_t i=0;i<myContent->asLines.lineCount;i++){ /* Loop through the Lines. */
		myLine=myContent->asLines.list[i]; /* Do whatever you want with the Lines here. */
	}
}
```
If, however, `isLinear` is `true` instead of `false`, different operations are required.

### Reading Linearly
Storing a File's Contents as individual Lines is useful for [text documents,](https://en.wikipedia.org/wiki/Text_file)
but complicates the decoding/reading process of [binary files,](https://en.wikipedia.org/wiki/Binary_file) such as [PNG](https://en.wikipedia.org/wiki/Portable_Network_Graphics) or [JPEG.](https://en.wikipedia.org/wiki/JPEG)\

Because of this, [OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md)(and subsequently OFileContent) provides a secondary method for storing a File's contents: linearly.

Much like how a File is stored on disk (or how the C [FILE](https://www.tutorialspoint.com/cprogramming/c_file_io.htm) struct works),
OFileContent will contain the File's bytes in a large array, storing each byte sequentially; linearly, as it would be on the system.

If the bool `isLinear` is `true`, it means the Content has been stored linearly, and must be read differently.
If `false`, review [Reading as Lines.](#reading-as-lines)

This is the correct way to check if the Contents are stored as sequential bytes, and then read them:
```cpp
uint8_t myByte=0;
if(myContent->isLinear){ /* If IS stored linearly, instead of as individual bytes: */
	for(size_t i=0;i<myContent->asLinear.byteCount;i++){ /* Loop through the bytes. */
		myByte=myContent->asLinear.bytes[i]; /* Do whatever you want with the bytes here. */
	}
}
```

### Benefits to both
Both methods of storing a given File's Content are supplied for a reason. Each method is better at a specific task.

#### As Lines
**Benefits:**

* Allows for directly indexxing(reading or modifying) a specific Line of a File
* Text documents are easily editable
* Easier creation of utilities such as Text Editors, and easier reading of Configuration Files
* Repositionable Lines

**Drawbacks:**

* Binary files, such as images or video, are not (easily) readable
* Uses more memory when storing the Contents of a File compared to storing Contents linearly
* Slower store a File to memory

#### Linearly
**Benefits:**

* Less memory usage when storing a File to memory
* Direct access to individual bytes
* Preserves the structure of binary files
* Storing a File to memory is quicker, noticable with larger Files

**Drawbacks:**

* Text documents are more complicated to read and write to
* Direct indexxing of a given Line is not possible
* Files essentially become read-only due to the added complexity

## Breakdown
```cpp
union{
	...
};
```
**The following two entries are stored within this union:**
```cpp
	struct{
		size_t lineCount;
		size_t charCount;
		OFileLine* list;
	}asLines;
```
If `isLinear` is false, the Content will be stored here, and should be retrieved with this struct:

`size_t lineCount;` The total count of Lines in this File (starting at 1).

`size_t charCount;` The total count of Characters in this File.

`OFileLine* list;` An array of Lines for this File.
```cpp
	struct{
		size_t   byteCount;
		uint8_t* bytes;
	}asLinear;
```
If `isLinear` is true, the Content will be stored here, and should be retrieved with this struct:

`size_t   byteCount;` The size of the array.

`uint8_t* bytes;` The array of bytes of this File.
```cpp
bool modified;
```
Have the contents been modified from when changes were last applied, or when the File was last opened?
```cpp
bool isLinear;
```
If this is `true`, this means the File's contents are stored linearly in memory, as if it were one big Line,
and indexxing operations **MUST** be done using `data.asLinear.bytes` instead of `data.asLines.list`

If this is `false`, this means the File's contents are stored as separate lines in memory, as it would be displayed in an editor,
and indexxing operations **MUST** be done using `data.asLines.list` instead of `data.asLinear.bytes`

## Other Information
OFileContent is only a part of [OFile.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md)
It is important you see the documentation for the other associated types.

#### See Also:
[OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md)

[OFileLine](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFileLine.md)