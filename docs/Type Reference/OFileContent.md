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
			size_t byteCount;
			char*  bytes;
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
OFileLine line;
if(!myContent->isLinear){ /* If is NOT stored linearly, but instead as individual Lines: */
	for(size_t i=0;i<myContent->asLines.lineCount;i++){ /* Loop through the Lines. */
		line=myContent->asLines.list[i]; /* Do whatever you want with the Lines here. */
	}
}
```
If, however, `isLinear` is `true` instead of `false`, different operations are required.

### Reading Linearly