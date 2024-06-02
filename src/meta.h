#include <stdio.h>
#include <string.h>
#include "../bstring/bstring/bstrlib.h"
#ifdef _MSC_VER
#   define _CRT_SECURE_NO_WARNINGS
#   include <windows.h>
#elif __linux__
#   include <unistd.h>
#else
#   error "Unknown Platform"
#endif
#include "core.h"

const i32 buffersize = 256;

int metagen(const char *metaname, const char*genname) {
    FILE *input = null;
    FILE *output = null;
    struct bstrList *lines;
    struct tagbstring postfix = bsStatic("\n");
    bstring stubinclude = bfromcstr("#include \"TYPE.h\"");
    bstring emptystr = bfromcstr("");

#ifdef _MSC_VER
    LPTSTR cwdstr = malloc(buffersize);
    i32 pathstrlen = GetCurrentDirectoryA(buffersize, cwdstr);
#elif __linux__
    char *cwdstr = malloc(buffersize);
    getcwd(cwdstr, buffersize);
#endif

    bstring typecorepath = bfromcstr(cwdstr);
    bcatcstr(typecorepath, "/src/meta/");
    bstring typecore = bfromcstr(metaname);
    bstring typestr = bfromcstr("TYPE");
    bconcat(typecorepath, typecore);
    bcatcstr(typecorepath, "_");
    bconcat(typecorepath, typestr);
    bcatcstr(typecorepath, ".h");
    // printf("typecorepath: %s\n", bdata(typecorepath));

    bstring typemetapath = bfromcstr(cwdstr);
    bcatcstr(typemetapath, "/src/meta/gen/");
    bstring typemetastr = bfromcstr(genname);
    bconcat(typemetapath, typecore);
    bcatcstr(typemetapath, "_");
    bconcat(typemetapath, typemetastr);
    bcatcstr(typemetapath, ".h");
    // printf("typemetapath: %s\n", bdata(typemetapath));

    if (null != (input = fopen(bdata(typecorepath), "r"))) {
        bstring b = bread((bNread) fread, input);
        fclose(input);
        if (null != (lines = bsplit(b, '\n'))) {
            for (int i = 0; i < lines->qty; ++i) {
                bfindreplace(lines->entry[i], stubinclude, emptystr, 0);
                bfindreplace(lines->entry[i], typestr, typemetastr, 0);
                binsert(lines->entry[i], blength(lines->entry[i]), &postfix, '?');
                // printf("%04d: %s\n", i, bdatae(lines->entry[i], "NULL"));
            }
            if (null != (output = fopen(bdata(typemetapath), "w"))) {
                for (int i = 0; i < lines->qty; ++i) {
                    fputs(bdatae(lines->entry[i], "NULL"), output);
                }
            }
            else {
                printf("Failed to open file.");
            }
            bstrListDestroy(lines);
        }
        bdestroy(b);
    } else {
        printf("metagen::Unable to open type core file.");
    }
    return 0;
}

int metacore(const char *metaname) {
    FILE *output = null;
    const char *coretypes[] = {
        "i8",
        "i16",
        "i32",
        "i64",
        "u8",
        "u16",
        "u32",
        "u64",
        "f32",
        "f64",
        "str",
        "strptr",
    };
    const i8 coretypeslen = sizeofarray(coretypes);

#ifdef _MSC_VER
    LPTSTR cwdstr = malloc(buffersize);
    i32 pathstrlen = GetCurrentDirectoryA(buffersize, cwdstr);
#elif __linux__
    char *cwdstr = malloc(buffersize);
    getcwd(cwdstr, buffersize);
#endif

    bstring typecorepath = bfromcstr(cwdstr);
    bcatcstr(typecorepath, "/src/meta/gen/");
    bstring typecore = bfromcstr(metaname);
    bconcat(typecorepath, typecore);
    bcatcstr(typecorepath, "_core.h");
    // printf("typecorepath: %s\n", bdata(typecorepath));
    if (null != (output = fopen(bdata(typecorepath), "w"))) {
        for (int i = 0; i < coretypeslen; ++i) {
            metagen(metaname, coretypes[i]);
            bstring result = bfromcstr("#pragma once\n#include \"");
            bstring typename = bfromcstr(metaname);
            bconcat(result, typename);
            bcatcstr(result, "_");
            bstring corename = bfromcstr(coretypes[i]);
            bcatcstr(result, bdata(corename));
            bcatcstr(result, ".h\"\n");
            fputs(bdatae(result, "NULL"), output);
            bdestroy(result);
        }
    } else {
        printf("metacore::Unable to open type core file.");
    }
    fclose(output);
    
    return 0;
}
