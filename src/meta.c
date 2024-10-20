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
#   include <windows.h>
#   include <direct.h>
#elif __linux__
#   include <unistd.h>
#else
#   error "Unknown Platform"
#endif

#include "core.h"
// #include "jsmn.h"
#include "bstring/bstring/bstrlib.h"
#include <toml-c.h>

const char *metapath = NULL;

structdef(Node_bstring) {
    bstring data;
    i32 foundat;
    Node_bstring *next;
};

Node_bstring *Node_bstring_create(bstring input, i32 foundat) {
    Node_bstring *result = malloc(sizeof(Node_bstring));
    if (result == NULL) { exit(-1);}
    result->data = input;
    result->next = NULL;
    result->foundat = foundat;
    return result;
}

void Node_bstring_destroy(Node_bstring *node) {
    Node_bstring *destroyer = node;
    // while (node->
    bdestroy(node->data);
    // node->next = 
}

char *getCurrentWorkingDirectory() {
    const i32 buffersize = 256;
    char *cwdstr = malloc(buffersize);
    if (!cwdstr) {
        printf("malloc failure.");
        return NULL;
    }
#ifdef _MSC_VER
    // i32 pathstrlen = GetCurrentDirectoryA(buffersize, cwdstr);
    _getcwd(cwdstr, buffersize);
#elif __linux__
    getcwd(cwdstr, buffersize);
#endif
    return cwdstr;
}

/*
 * initialize main header:
 * genpath/metaname.h
 */
void metainit(char *metaname) {
    // char *cwdstr = getCurrentWorkingDirectory();
    // bstring typecorepath = bfromcstr(cwdstr);
    bstring typecorepath = bfromcstr(metapath);
    bcatcstr(typecorepath, "gen/");
    bcatcstr(typecorepath, metaname);
    bcatcstr(typecorepath, ".h");
    // printf("typecorepath: %s\n", bdata(typecorepath));
    FILE *output = NULL;
    if (NULL != (output = fopen(bdata(typecorepath), "w"))) {
        bstring result = bfromcstr("#pragma once\n");
        fputs(bdatae(result, "NULL"), output);
        bdestroy(result);
        fclose(output);
    } else {
        printf("metainit::Unable to open type core gen file for initiation.\n");
        exit(-1);
    }
}

/*
 * generate types and append to main header
 * genpath/metaname.h <---append--- genpath/metaname_genname.h
 */
int metagen(char *metaname, char *genname, char *forwarddeclparam) {
    FILE *input = NULL;
    FILE *output = NULL;
    struct bstrList *lines;
    struct tagbstring postfix = bsStatic("\n");
    bstring stubinclude = bfromcstr("#include \"TYPE.h\"");
    // bstring forwarddecl = bfromcstr("");
    bstring forwarddecl = bfromcstr(forwarddeclparam);
    bconcat(forwarddecl, bfromcstr("("));
    bconcat(forwarddecl, bfromcstr(genname));
    bconcat(forwarddecl, bfromcstr(");"));

    // char *cwdstr = getCurrentWorkingDirectory();
    bstring typemetapath = bfromcstr(metapath);
    // bcatcstr(typemetapath, "/src/meta/");
    bstring typemeta = bfromcstr(metaname);
    bstring typestr = bfromcstr("TYPE");
    bconcat(typemetapath, typemeta);
    bcatcstr(typemetapath, "_");
    bconcat(typemetapath, typestr);
    bcatcstr(typemetapath, ".h");
    // printf("typemetapath: %s\n", bdata(typemetapath));

    bstring outpath = bfromcstr(metapath);
    bcatcstr(outpath, "gen/");
    bstring outstr = bfromcstr(genname);
    bconcat(outpath, typemeta);
    bcatcstr(outpath, "_");
    bconcat(outpath, outstr);
    bcatcstr(outpath, ".h");
    // printf("outpath: %s\n", bdata(outpath));

    if (NULL != (input = fopen(bdata(typemetapath), "r"))) {
        bstring b = bread((bNread) fread, input);
        fclose(input);
        if (NULL != (lines = bsplit(b, '\n'))) {
            for (int i = 0; i < lines->qty; ++i) {
                bfindreplace(lines->entry[i], stubinclude, forwarddecl, 0);
                bfindreplace(lines->entry[i], typestr, outstr, 0);
                binsert(lines->entry[i], blength(lines->entry[i]), &postfix, '?');
                // printf("%04d: %s\n", i, bdatae(lines->entry[i], "NULL"));
            }
            if (NULL != (output = fopen(bdata(outpath), "w"))) {
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
    bstring typecorepathtarget = bfromcstr(metapath);
    bcatcstr(typecorepathtarget, "gen/");
    bcatcstr(typecorepathtarget, metaname);
    bcatcstr(typecorepathtarget, ".h");
    // printf("typecorepathtarget: %s\n", bdata(typecorepathtarget));
    if (NULL != (output = fopen(bdata(typecorepathtarget), "a"))) {
        bstring result = bfromcstr("#include \"");
        bstring typename = bfromcstr(metaname);
        bconcat(result, typename);
        bcatcstr(result, "_");
        bcatcstr(result, genname);
        bcatcstr(result, ".h");
        bcatcstr(result, "\"\n");
        // printf("final header name: %s\n", bdata(result));
        fputs(bdatae(result, "NULL"), output);
        bdestroy(result);
        fclose(output);
    } else {
        printf("metainit::Unable to open type core file for initiation.");
    }
    bdestroy(typecorepathtarget);
    return 0;
}

void metacore(char *metaname) {
    char *coretypes[] = {
        "i8", "i16", "i32", "i64",
        "u8", "u16", "u32", "u64",
        "f32", "f64",
        "str", "cstr",
    };
    i8 coretypeslen = sizeofarray(coretypes);
    for (int i = 0; i < coretypeslen; ++i) {
        metainit(metaname);
    }
    for (int i = 0; i < coretypeslen; ++i) {
        metagen(metaname, coretypes[i], "structdecl");
    }
}

void metapayload() {
    char *coretypes[] = {
        "hkArray",
        "hkList", "hkNode",
        "hkDList", "hkBiNode",
        "hkQueue", "hkStack",
    };
    const i8 coretypeslen = sizeofarray(coretypes);
    for (int i = 0; i < coretypeslen; ++i) {
        metacore(coretypes[i]);
    }
}

int main(int argc, char *argv[]) {
    printf("haikal::CodeGen::Initialize.\n");

    char *cwdstr = getCurrentWorkingDirectory();
    printf("haikal::main::cwd::%s\n", cwdstr);
    // char* cwdstr = malloc(256);
    // cwdstr = getcwd(cwdstr, 256);
    bstring docpath = bfromcstr(cwdstr);
    bstring docname = bfromcstr("/haikal.toml");
    bconcat(docpath, docname);
    printf("haikal::main::docpath::%s\n", bdata(docpath));
    FILE *input = fopen(bdata(docpath), "r");
    if (!input) {
        printf("haikal::Failed to open config haikal.toml\n");
		exit(1);
    }
    fseek(input, 0L, SEEK_END);
    usize file_size = ftell(input);
    // printf("haikal::toml::file::size::%llu\n", file_size);
    // fseek(input, 0L, SEEK_SET);
    rewind(input);
    char *buffer = malloc(file_size);
    if (!buffer) {
        printf("failed to allocate memory for input toml.\n");
    }
    usize ret;
    ret = fread(buffer, sizeof(*buffer), file_size, input);
    buffer[ret] = '\0';
    // printf("%s\n", buffer);
    // printf("ret = %lu, sizeofarray(buffer) = %ld\n", ret, sizeofarray(buffer));
    // if (ret != sizeofarray(buffer)) { fprintf(stderr, "fread() failed: %zu\n", ret); exit(EXIT_FAILURE); }
    fclose(input);

	char errbuf[200];
	toml_table_t *tbl = toml_parse(buffer, errbuf, sizeof(errbuf));
	if (!tbl) {
		fprintf(stderr, "ERROR: %s\n", errbuf);
		exit(1);
	}

	toml_table_t *core_tbl = toml_table_table(tbl, "core");
    if (core_tbl) {
        int l = toml_table_len(core_tbl);
		for (int i = 0; i < l; i++) {
			int keylen;
			const char *key = toml_table_key(core_tbl, i, &keylen);
			printf("haikal::core::key[%d]::%s\n", i, key);
            // theres only one key so no need to check...
            toml_value_t metapath_value = toml_table_string(core_tbl, "metapath");
            metapath = metapath_value.u.s;
        }
    }
    printf("haikal::core::metapath::%s\n", metapath);
    printf("\n");

	toml_table_t *meta_tbl = toml_table_table(tbl, "meta");
	if (meta_tbl) {
		// Loop over all keys in a table.
		int l = toml_table_len(meta_tbl);
		for (int i = 0; i < l; i++) {
			int keylen;
			const char *key = toml_table_key(meta_tbl, i, &keylen);
			// printf("haikal::metainit::key[%d]: %s\n", i, key);
            // metainit(key);
            // metacore(key);

            toml_array_t *arr = toml_table_array(meta_tbl, key);
            if (arr) {
                int l = toml_array_len(arr);
                for (int i = 0; i < l; i++) {
                    // printf("  haikal::metagen::index[%d]: %s\n", i, toml_array_string(arr, i).u.s);
                    // metagen(key, toml_array_string(arr, i).u.s);
                }
                // printf("\n");
            }
        }
	}

    bstring cpath = bfromcstr(cwdstr);
    // get this main path from toml
    // TODO(ibrahim): parse files with main recursively to find hktags
    // bstring cmainpath = bfromcstr("/src/main.c");
    bstring cmainpath = bfromcstr("/src/win32_njin.c");
    bconcat(cpath, cmainpath);
    printf("haikal::main::cpath::%s\n", bdata(cpath));
    struct bstrList *lines;
    Node_bstring *head = NULL;
    bstring hktag = bfromcstr("haikal@");
    if (NULL != (input = fopen(bdata(cpath), "r"))) {
        bstring b = bread((bNread) fread, input);
        fclose(input);
        if (NULL != (lines = bsplit(b, '\n'))) {
            for (int i = 0; i < lines->qty; ++i) {
                // printf("%04d: %s\n", i, bdatae(lines->entry[i], "NULL"));
                int found = binstr(lines->entry[i], 0, hktag);
                if (found != BSTR_ERR) {
                    printf("haikal tag detected in main.c: '%s'\n", bdata(lines->entry[i]));
                    // printf("%s\n", bdata(iter->data));
                    if (head == NULL) {
                        head = Node_bstring_create(lines->entry[i], found);
                        printf("head initalized with: '%s'\n", bdata(head->data));
                    } else {
                        Node_bstring *iter = head;
                        while (iter->next != NULL) {
                            iter = iter->next;
                        }
                        iter->next = Node_bstring_create(lines->entry[i], found);
                    }
                }
            }
            Node_bstring *iter = head;
            if (head != NULL) {
                iter = head;
                while (iter != NULL) {
                    bstring result = bmidstr(iter->data, iter->foundat + hktag->slen, iter->data->slen - (iter->foundat + hktag->slen));
                    struct bstrList *hkCommand = bsplit(result, ':');
                    printf("haikal::metainit::%s\n", bdata(hkCommand->entry[0]));

                    printf("\thkCommand[0] = %s\n", bdata(hkCommand->entry[0]));
                    metainit(bdata(hkCommand->entry[0]), ".h");
                    metainit(bdata(hkCommand->entry[0]), ".c");
                    printf("linkedlist walk: {bstring: '%s', foundat: %d, next: %p}\n", bdata(iter->data), iter->foundat, iter->next);
                    iter = iter->next;
                }
                printf("haikal::metainit::complete.\n\n");
                iter = head;
                while (iter != NULL) {
                    bstring result = bmidstr(iter->data, iter->foundat + hktag->slen, iter->data->slen - (iter->foundat + hktag->slen));
                    // printf("result = %s\n", bdata(result));
                    struct bstrList *hkCommand = bsplit(result, ':');
                    printf("haikal::metagen::%s\n", bdata(hkCommand->entry[0]));
                    printf("\thkCommand[1] = %s:%s:%s\n", bdata(hkCommand->entry[0]), bdata(hkCommand->entry[1]), bdata(hkCommand->entry[2]));
                    if (strcmp(bdata(hkCommand->entry[2]), "s") == 0) {
                        metagen(bdata(hkCommand->entry[0]), bdata(hkCommand->entry[1]), "structdecl");
                    } else if (strcmp(bdata(hkCommand->entry[2]), "u") == 0) {
                        metagen(bdata(hkCommand->entry[0]), bdata(hkCommand->entry[1]), "uniondecl");
                    } else if (strcmp(bdata(hkCommand->entry[2]), "p") == 0) {
                        metagen(bdata(hkCommand->entry[0]), bdata(hkCommand->entry[1]), "primdecl");
                    }
                    printf("linkedlist walk: {bstring: '%s', foundat: %d, next: %p}\n", bdata(iter->data), iter->foundat, iter->next);
                    iter = iter->next;
                }
                printf("haikal::metagen::complete.\n\n");
            } else {
                printf("metagen::main::error::linkedlist is empty!\n");
            }
            // DANGER! don't destroy list before using the linked list!
            bstrListDestroy(lines);
        } else {
            printf("metagen::main::error::line read error!\n");
        }
        bdestroy(b);
    } else {
        printf("metagen::main::error::Unable to open main.c file.\n");
    }

    // metapayload();

    printf("haikal::CodeGen::Finalize.\n");
    return 0;
}
