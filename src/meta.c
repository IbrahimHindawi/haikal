/*
 * Metaprogramming Data Structures & Algorithms in C by
 * creating an external program `meta` that parses C source
 * and replaces the `TYPE` token with a desired type.
 * for core types -> `metacore()` -> `#include "hkType_core.h"
 * for new types -> `metagen()` -> 
 *      `#include "custom.h"`
 *      `#include "hkType_custom.h"`
 */
#include "meta.h"

int main() {
    metacore("hkNode");
    metacore("hkList");
    metacore("hkArray");
    metacore("hkQueue");
    return 0;
}
