# haikal هيكل
Haikal uses metaprogramming to generate C code as a pre-build step.  
Code should be easy to debug & works great with LSPs.  
## Installation:
- Add `haikal` as a git submodule to your project and build the program.  
- Add `haikal.toml` to your project root.
- example: [c-init](https://github.com/IbrahimHindawi/c-init)
## Usage:
- To generate the files to `metapath`, build and invoke the `haikal` program from the your root directory.  
- To add custom types, edit the `[meta]` section & simply include the headers before including the data structure.
- `#include <hkArray.h>` to include all generated `hkArray` types.
## Code example:  
The `i32` in this instance could be any data type:  
```c
hkArray_i32 array = hkarray_i32_create(12);
array.data[0] = 0x19;
hkarray_i32_destroy(&array);
```  
## Configuration:
Haikal uses toml for configuration. For example:  
```toml
[core]
metapath = "extern/haikal/src/meta/" # where to generate files to
[meta]
hkArray = ["SomeType"] # generate hkArray_SomeType + functions
hkNode = ["Vector"]
hkQueue = []
hkList = ["Vector"]
```
## Data Structures:  
- hkArray: growable heap allocated array.
- hkList/hkNode: singly linked list.
- hkQueue: queue
## Limitations:
Cannot add pointer types to `[meta]` unless `typedef`ed. Could automate the process by detecting the pointer and auto `typedef`ing it.  
For example: `int *` will become `typedef int *intptr` and then you can generate `hkArray_intptr`.  
