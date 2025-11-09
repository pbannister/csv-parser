# csv-parser

Ran across an article in 2008 that claimed parsing of CSV data was necessarily CPU-bound. 
Proof is better than opinion, so took code from a prior exercise, and adapted to parse CSV files.

The test program does a full CSV parse as described in RFC 4180 
(including handling quoted fields with embedded line breaks) 
and a bit more, but does nothing with the parsed data.

## Usage
Build with:
```bash
make
```
Executable is in:
```
build/bin/csv-parser
```
The `csv-parser` executable demonstrates the parser.
Run it without arguments, or pass CSV filename(s) to parse. 

## Notes
- The code includes a small utility string class in `sources/base/string.h`.
  That file defines `base::string_o`, a minimal resizable C-style string
  wrapper used around the parser code.
- This repository uses CMake for configuration and standard Make for building.

## See also
Article written about this example:<br>
https://bannister.us/weblog/2008/performance-parsing-csv-data
