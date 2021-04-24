# Function Reference: OClamp / OClampMax / OClampMin
## Basic Mathematical Operations

## Declarations
```cpp
	inline long OClamp(long& x, long& min, long& max) { return ( (x<min) ? (min) : ( (x>max) ? max : x) ); }
	inline long OClampMax(long& x, long& max)         { return ( (x>max) ? max : x ); }
	inline long OClampMin(long& x, long& min)         { return ( (x<min) ? min : x ); }
```
```
[Library  : OSL]
[Declared : src/include/OSL/OMath.hpp]
[Defined  : src/include/OSL/OMath.hpp] <inline>
```
### OClamp
Returns a version of the given value clamped between the two other values.
#### Parameters
```
long x   - The value to clamp.
long min - The minimum for the value.
long max - The maximum for the value.
```
#### Returns
If the given value `x` is within the range between `min` and `max`, it will be returned as normal.
Otherwise, the minimum or maximum value will be returned depending on which is the closest value to `x`.

### OClampMax
Returns a version of the given value clamped below the second value.
#### Parameters
```
long x   - The value to clamp.
long max - The maximum for the value.
```
#### Returns
If the given value `x` is less than the maximum value, the value of `x` will be returned.
Otherwise, the maximum value will be returned.

### OClampMin
Returns a version of the given value clamped above the second value.
#### Parameters
```
long x   - The value to clamp.
long min - The minimum for the value.
```
#### Returns
If the given value `x` is greater than the minimum value, the value of `x` will be returned.
Otherwise, the minimum value will be returned.

## Use
These three functions are very simple mathematical operations. They're used to keep a number within a certain range.

For example:
```cpp
int inputValue=10;
int myValue=OClamp(inputValue,5,0);
```
`myValue` will kept at `5`, since `inputValue` is greater than the max value (`5`) provided as the second parameter to `OClamp`.

If, however, `inputValue` was lower than the max value, yet greater than the min value (`0`), `myValue` will be set to `inputValue`.

The other two functions do one of these operations:
```cpp
int inputValue=10;
int myValue=0;

/*
 * myValue will be "5", since "10" is greater the maximum threshold of "5", and will be clamped. 
 * However, there is no minimum threshold, meaning the input can go as low as it wants.
 */
myValue=OClampMax(inputValue,5);

/*
 * myValue will be "10", since "10" is greater the minimum threshold of "5", and will NOT be clamped.
 * However, there is no maximum threshold, meaning the input can go as high as it wants.
 */
myValue=OClampMin(inputValue,5);
```

Since these functions are [inlined,](https://en.wikipedia.org/wiki/Inline_function) they are rather efficient,
and do not pose much of a performance hit in your Applications.