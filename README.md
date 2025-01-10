# haikal هيكل
haikal uses metaprogramming code gen to generate C code as a pre-build step.  
haikal monomorphises C files and writes them to be included.  
basically search & replace "TYPE" with your type.
Code should be easy to debug & works great with LSPs.  
## Code example:  
The `i32` in this instance could be any data type:  
```c
// haikal@Array:i32:p
Array_i32 array = array_i32_create(12);
array.data[0] = 0x19;
array_i32_destroy(&array);
```  
## Code structure:
```
metatype_TYPE.h // public API
metatype_TYPE_internal.h // private API
metatype_TYPE.c // implementation
```
## Installation:
- Add `haikal` as a git submodule to your project and build the program.  
- Add `haikal.toml` to your project root.
- Add `include_directories(extern/haikal/src/meta/gen)` to your `CMakeLists.txt`.
## Examples:
- [main.c](https://github.com/IbrahimHindawi/haikal/blob/main/src/main.c)
- [project](https://github.com/IbrahimHindawi/c-init)
## Usage:
- To generate the files to `metapath`, build and invoke the `haikal` program from the your root directory.  
- To generate custom types, add a `// haikal@metatype:typename:s` in your code & simply include the headers.
- `#include <Array.h>` to include all generated `hkArray` types public APIs.
- `#include <Array.c>` to include all generated `hkArray` implementations preferably after your `main()`.
## Configuration:
Haikal uses toml for configuration. For example:  
```toml
[core]
metapath = "extern/haikal/src/meta/"
mainpath = "src/main.c"
```
## Data Structures:  
- Array: growable heap allocated array.
- List/hkNode: singly linked list.
- DList/hkBiNode: doubly linked list.
- Stack/hkNode: stack.
- Queue: queue.
- HashMap: hashtable.
## Current Limitations:
1. Cannot instantiate pointer types to `metatype`s unless `typedef`ed.  
  For example:  
  `int *` will become `typedef int *intptr` and then you can generate `Array_intptr`.  
  Possible Solution:  
  Could automate the process by detecting the pointer and auto `typedef`ing it.  
3. In the instatiation declaration, you must hint to `haikal` the type you want to instantiate:
  - `s` for struct
  - `e` for enum
  - `p` for primitives or pointers
  - `u` for union.  
  Possible Solution:  
  Detect the type being instantiated, this is non trivial unless we include the compiler to get type information.
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
