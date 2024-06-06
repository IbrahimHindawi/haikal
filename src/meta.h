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

/*
 * initialize main header:
 * genpath/metaname.h
 */
void metainit(str metaname) {
#ifdef _MSC_VER
    LPTSTR cwdstr = malloc(buffersize);
    i32 pathstrlen = GetCurrentDirectoryA(buffersize, cwdstr);
#elif __linux__
    char *cwdstr = malloc(buffersize);
    getcwd(cwdstr, buffersize);
#endif
    bstring typecorepath = bfromcstr(cwdstr);
    bcatcstr(typecorepath, "/src/meta/gen/");
    bcatcstr(typecorepath, metaname);
    bcatcstr(typecorepath, ".h");
    printf("typecorepath: %s\n", bdata(typecorepath));
    FILE *output = null;
    if (null != (output = fopen(bdata(typecorepath), "w"))) {
        bstring result = bfromcstr("#pragma once\n");
        fputs(bdatae(result, "NULL"), output);
        bdestroy(result);
    } else {
        printf("metainit::Unable to open type core file for initiation.");
        exit(-1);
    }
}

/*
 * generate types and append to main header
 * genpath/metaname.h <---append--- genpath/metaname_genname.h
 */
int metagen(str metaname, str genname) {
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
    bstring typecorepathtarget = bfromcstr(cwdstr);
    bcatcstr(typecorepathtarget, "/src/meta/gen/");
    bcatcstr(typecorepathtarget, metaname);
    bcatcstr(typecorepathtarget, ".h");
    // printf("typecorepathtarget: %s\n", bdata(typecorepathtarget));
    if (null != (output = fopen(bdata(typecorepathtarget), "a"))) {
        bstring result = bfromcstr("#include \"");
        bstring typename = bfromcstr(metaname);
        bconcat(result, typename);
        bcatcstr(result, "_");
        bcatcstr(result, genname);
        bcatcstr(result, ".h\"\n");
        fputs(bdatae(result, "NULL"), output);
        bdestroy(result);
    } else {
        printf("metainit::Unable to open type core file for initiation.");
    }
    bdestroy(typecorepathtarget);
    return 0;
}

int metacore(str metaname) {
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
    for (int i = 0; i < coretypeslen; ++i) {
        metagen(metaname, coretypes[i]);
    }
    return 0;
}
