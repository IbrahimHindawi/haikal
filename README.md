# haikal هيكل
Haikal uses metaprogramming to generate C code as a pre-build step.  
Haikal monomorphises C files and writes them to be included.  
Code should be easy to debug & works great with LSPs.  
## Installation:
- Add `haikal` as a git submodule to your project and build the program.  
- Add `haikal.toml` to your project root.
- Add `include_directories(extern/haikal/src/meta/gen)` to your `CMakeLists.txt`.
- example: [c-init](https://github.com/IbrahimHindawi/c-init)
## Usage:
- To generate the files to `metapath`, build and invoke the `haikal` program from the your root directory.  
- To generate custom types, add a `//haikal@container:typename:s` in your code & simply include the headers.
- `#include <hkArray.h>` to include all generated `hkArray` types.
## Code example:  
The `i32` in this instance could be any data type:  
```c
//haikal@hkArray:i32:p
hkArray_i32 array = hkarray_i32_create(12);
array.data[0] = 0x19;
hkarray_i32_destroy(&array);
```  
## Configuration:
Haikal uses toml for configuration. For example:  
```toml
[core]
metapath = "extern/haikal/src/meta/"
mainpath = "src/main.c"
```
## Data Structures:  
- hkArray: growable heap allocated array.
- hkList/hkNode: singly linked list.
- hkDList/hkBiNode: doubly linked list.
- hkStack/hkNode: stack.
- hkQueue: queue.
- hkHashMap: hashtable.
## Syntactic Limitations:
Cannot add pointer types to `[meta]` unless `typedef`ed. Could automate the process by detecting the pointer and auto `typedef`ing it.  
For example: `int *` will become `typedef int *intptr` and then you can generate `hkArray_intptr`.  
## Monomorphization Codegen Include Order Rules:
TODO: Automate header placement:  

For containers that have value types eg `T`:
- the type must be included before the generated header.
- this is because the container expects to know the type in it's struct.
- Warning: cannot be recursive type

For containers that have pointer types eg `T *`:
- the type can be included before or after the generated header.
- this is because the container has `T` forward declared.
- Warning: can be recursive type

For types that include a container of themselves eg `struct T { hkArray_T arr; };`:
- the type must be included after the generated header.
- this is because the type needs to know the container definition.
- Warning: can be recursive type with `T *` but not `T`
