# haikal هيكل
haikal uses metaprogramming code gen to generate C code as a pre-build step.  
haikal monomorphises C files and writes them to be included.  
basically search & replace "TYPE" with your type.
Code should be easy to debug & works great with LSPs.  
## Code example:  
The `i32` in this instance could be any data type:  
```c
template(Array(i32));
Array_i32 array = array_i32_create(12);
array.data[0] = 0x19;
array_i32_destroy(&array);
```  
## Code structure:
```
template_TYPE.h // public API
template_TYPE_internal.h // private API
template_TYPE.c // implementation
```
## Installation:
- Add `haikal` as a git submodule to your project and build the program.  
- Add `include_directories(extern/haikal/src/runtime)` and the generated output directory to your `CMakeLists.txt`.
## Running the tests:
`python build.py test debug`
## Examples:
- [main.c](https://github.com/IbrahimHindawi/haikal/blob/main/src/test.c)
- [project](https://github.com/IbrahimHindawi/c-init)
## Usage:
- To generate the files to `metapath`, build and invoke `haikal --entry src/main.c --meta extern/haikal/src/meta/` from your root directory.  
- To generate custom types, add `template(Vec(i32));` in your code and include the generated headers.
- `#include <Array.h>` to include all generated `hkArray` types public APIs.
- `#include <Array.c>` to include all generated `hkArray` implementations preferably after your `main()`.
## Configuration:
Haikal configuration is passed on the command line with `--entry` and `--meta`.
## Source parsing:
Haikal parses source files directly and does not run the C preprocessor. Type inference handles declarations that are plainly visible in the entry file and quoted local includes, but it does not expand macros, evaluate `#if`/`#ifdef`, or resolve compiler include paths. If a type is produced only by preprocessing, include or spell out a normal declaration before using it in `template(...)`.
## Data Structures:  
- Array: growable heap allocated array.
- List/hkNode: singly linked list.
- DList/hkBiNode: doubly linked list.
- Stack/hkNode: stack.
- Queue: queue.
- HashMap: hashtable.
## Current Limitations:
1. Cannot instantiate pointer types to templates unless `typedef`ed.  
  For example:  
  `int *` will become `typedef int *intptr` and then you can generate `Array_intptr`.  
  Possible Solution:  
  Could automate the process by detecting the pointer and auto `typedef`ing it.  
3. Type category detection is heuristic. Primitive aliases are recognized directly; enum and union declarations are detected from the scanned source; unknown types default to structs.
## Include Order Rules:
For containers that have value types eg `T`:
- the type must be included before the generated header.
- this is because the container expects to know the type in it's struct.
- Warning: cannot be recursive type

For containers that have pointer types eg `T *`:
- the type can be included before or after the generated header.
- this is because the container has `T` forward declared.
- Warning: can be recursive type

For types that include a container of themselves eg `struct T { Array_T arr; };`:
- the type must be included after the generated header.
- this is because the type needs to know the container definition.
- Warning: can be recursive type with `T *` but not `T`

Possible Solition: Automate header placement, will probably need the compiler to scan the types being instantiated.  
