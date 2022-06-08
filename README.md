A library of string utility functions for C that I wrote for fun.
The implementation might not be very good as I'm still learning C.

Documentation for the functions are in the source `stringutils.c` file.<br>
It is the user's reponsibility to free the memory allocated for the string returned
from the functions

## TODO
- Implement a more robust iterator using macros

## NOTE
Some functions do not work in Windows. It is reccomended to use the library in a linux/macOS environment.

## Included functions
- `firstWord`
- `touppers`
- `tolowers`
- `split`
- `split_iter`
- `replace`
- `replaceChar`
- `substring`
- `strip`
- `read_word`
- `read_line`
- `contains`
- `long_to_string`
