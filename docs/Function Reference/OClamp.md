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
[Defined  : src/include/OSL/OMath.hpp <inline>]
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
If the given value "x" is less than the maximum value, the value of "x" will be returned.
Otherwise, the maximum value will be returned.

### OClampMin
Returns a version of the given value clamped above the second value.
#### Parameters
```
long x   - The value to clamp.
long min - The minimum for the value.
```
#### Returns
If the given value "x" is greater than the minimum value, the value of "x" will be returned.
Otherwise, the minimum value will be returned.

## Use