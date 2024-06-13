/*
 * Metaprogramming Data Structures & Algorithms in C by
 * creating an external program `meta` that parses C source
 * and replaces the `TYPE` token with a desired type.
 * for core types -> `metacore()` -> `#include "hkType_core.h"
 * for new types -> `metagen()` -> 
 *      `#include "custom.h"`
 *      `#include "hkType_custom.h"`
 */
#ifdef _MSC_VER
#   define _CRT_SECURE_NO_WARNINGS
#endif
#include "meta.h"

int main() {
    printf("Haikal::CodeGen::Initialize.\n");

    metainit("hkNode");
    metacore("hkNode");

    metainit("hkList");
    metacore("hkList");

    metainit("hkQueue");
    metacore("hkQueue");

    metainit("hkArray");
    metacore("hkArray");
    // metagen("hkArray", "CustomType");

    printf("Haikal::CodeGen::Finalize.\n");
    return 0;
}
