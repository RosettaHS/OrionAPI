# Type Reference: Generic Information
## General information for all classes and types used in OrionAPI

## C- Prefix VS O- Prefix
Class documentation is only available for classes with the O- Prefix, with few exceptions.
The O- Prefix is for finalised classes that developers are meant to use in their projects. These classes are stable and simple to use.

The C- Prefix is for control/reference classes that are used internally by OrionAPI, and are not meant to be used by developers.
These classes are complicated, unstable, and lacking in functionality as they are meant to be strung together to form a larger class.

### Use
**NEVER** use C- Prefixed classes or types. Ignore them entirely and **ONLY** use O- Prefixed class.
O- Prefixed classes offer full functionality, ease of use, reliability, and full documentation.
There is never an instance to use C- Prefixed classes unless you are trying to remake a class or type, which is highly discouraged.

## Altnames
All types, functions, and methods provided by OrionAPI are named using the [camelCase](https://en.wikipedia.org/wiki/Camel_case) or the [PascalCase](https://en.wikipedia.org/wiki/Camel_case#Variations_and_synonyms) [naming conventions,](https://en.wikipedia.org/wiki/Naming_convention_(programming))
However this naming convention conflicts with the more common [snake_case](https://en.wikipedia.org/wiki/Snake_case) naming convention for functions, and the ["_t" type suffix](https://stackoverflow.com/questions/1391447/what-does-the-postfix-t-stand-for-in-c) commonly used by structs/types,
and commonly highlighted by [syntax highlighting](https://en.wikipedia.org/wiki/Syntax_highlighting) services such as the ones used in GitHub and [GNU nano](https://en.wikipedia.org/wiki/GNU_nano)

While this is purely cosmetic, it can give the appearance of messy code and cause severe visual style confliction if a given program's source code is using several libraries (that follow different naming conventions) alongside OrionAPI.
To circumvent this style clash, OrionAPI provides several [aliases](https://en.wikipedia.org/wiki/Typedef) to non-internal/control types to make their names fit in better with code following more standard naming conventions.

This is an example of a few aliases to File and Directory management classes provided by OrionAPI:
```cpp
typedef OFileType        filetype_t;
typedef OFileAction      fileaction_t;
typedef OFileHash        filehash_t;
typedef OFileLine        fileline_t;
typedef OFileContent     filecontent_t;
typedef OFile            file_t;

typedef ODirectoryAction diraction_t;
typedef ODEType          direnttype_t;
typedef ODirectoryEntry  direntry_t;
typedef ODirectory       dir_t;

```
The altname of a given class or type can be found within the documentation of said class or type.

Information on disabling altnames can be found [here](https://github.com/RosettaHS/OrionAPI/blob/main/docs/API%20Configuration.md#orion_noaltnames).