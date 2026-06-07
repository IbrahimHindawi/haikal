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
#   include <sys/stat.h>
#   include <dirent.h>
#else
#   error "Unknown Platform"
#endif

#include "core.h"
#include "bstring/bstring/bstrlib.h"
#include <clang-c/Index.h>
#include <ctype.h>
#include <errno.h>
#include <threads.h>
#ifndef _MSC_VER
#   include <time.h>
#endif

const char *metapath = NULL;
const char *mainpath = NULL;
const char *compile_db_path = NULL;
const char *project_root = NULL;
const char *haikal_source_root = NULL;
const char *typestr = "TYPE";
static bool haikal_cache_hit = false;
static const u64 HAIKAL_CACHE_VERSION = 4;
static u64 haikal_cache_signature = 0;

structdef(Directive) {
    bstring source_line;
    i32 column;
    bstring family;
    bstring arg;
    bstring path;
    i32 line;
    Directive *next;
};

Directive *Directive_create(bstring input, i32 column, bstring family, bstring arg, const char *path, i32 line) {
    Directive *result = malloc(sizeof(Directive));
    if (result == NULL) { exit(-1);}
    result->source_line = input;
    result->next = NULL;
    result->column = column;
    result->family = family;
    result->arg = arg;
    result->path = bfromcstr(path ? path : "");
    result->line = line;
    return result;
}

typedef enum TypeKind {
    TypeKind_unknown,
    TypeKind_prim,
    TypeKind_struct,
    TypeKind_union,
    TypeKind_enum,
} TypeKind;

structdef(TypeRecord) {
    bstring name;
    TypeKind kind;
    TypeRecord *next;
};

structdef(TypeTable) {
    TypeRecord *first;
};

structdef(MetaFieldRecord) {
    bstring name;
    bstring type;
    i64 offset;
    i64 size;
    i64 align;
    MetaFieldRecord *next;
};

structdef(MetaEnumValueRecord) {
    bstring name;
    i64 value;
    MetaEnumValueRecord *next;
};

structdef(MetaTypeRecord) {
    bstring name;
    TypeKind kind;
    i64 size;
    i64 align;
    MetaFieldRecord *fields_first;
    MetaFieldRecord *fields_last;
    MetaEnumValueRecord *enum_values_first;
    MetaEnumValueRecord *enum_values_last;
    MetaTypeRecord *next;
};

structdef(MetaTable) {
    MetaTypeRecord *first;
};

structdef(CompileCommand) {
    bstring directory;
    bstring command;
    bstring file;
    CompileCommand *next;
};

structdef(CompileDatabase) {
    CompileCommand *first;
    CompileCommand *last;
};

structdef(StringList) {
    char **items;
    i32 count;
    i32 capacity;
};

structdef(HashState) {
    u64 value;
};

structdef(ScanState) {
    CXTranslationUnit tu;
    Directive **templates;
    Directive **reflects;
    TypeTable *types;
    MetaTable *meta;
};

structdef(ProfileStats) {
    double total_seconds;
    double args_seconds;
    double parse_seconds;
    double direct_seconds;
    double include_seconds;
    double semantic_seconds;
    double dispose_seconds;
    i32 parsed_count;
    i32 skipped_count;
};

structdef(WorkerResult) {
    Directive *templates;
    Directive *reflects;
    TypeTable types;
    MetaTable meta;
    ProfileStats profile;
};

structdef(CompileWorkerState) {
    CompileCommand **commands;
    i32 command_count;
    i32 next_command;
    mtx_t mutex;
    WorkerResult *results;
};

structdef(CompileWorkerArg) {
    CompileWorkerState *state;
    i32 worker_index;
};

static bool append_directive(Directive **head, bstring line, i32 column, bstring family, bstring arg, const char *path, i32 line_number);
static bool path_is_project_owned(const char *path);
static bool path_has_source_or_header_extension(const char *path);
static const char *path_basename(const char *path);
static void collect_directives_from_file(const char *path, Directive **templates, Directive **reflects);

void Directive_destroy(Directive *node) {
    // while (node->
    bdestroy(node->source_line);
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

static double haikal_now_seconds(void) {
#ifdef _MSC_VER
    static LARGE_INTEGER frequency;
    static bool initialized = false;
    LARGE_INTEGER counter;
    if (!initialized) {
        QueryPerformanceFrequency(&frequency);
        initialized = true;
    }
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)frequency.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
#endif
}

static bool haikal_profile_enabled(void) {
    const char *enabled = getenv("HAIKAL_PROFILE");
    return enabled && enabled[0] && strcmp(enabled, "0") != 0;
}

/*
 * initialize main header:
 * genpath/family.h
 */
void metainit(char *family, char *ext) {
    // char *cwdstr = getCurrentWorkingDirectory();
    // bstring typecorepath = bfromcstr(cwdstr);
    bstring typecorepath = bfromcstr(metapath);
    bcatcstr(typecorepath, "gen/");
    bcatcstr(typecorepath, family);
    bcatcstr(typecorepath, ext);
    // printf("typecorepath: %s\n", bdata(typecorepath));
    FILE *output = NULL;
    if (NULL != (output = fopen(bdata(typecorepath), "wb"))) {
        bstring result = bfromcstr("#pragma once\n");
        fputs(bdatae(result, "NULL"), output);
        bdestroy(result);
        fclose(output);
    } else {
        printf("metainit: Unable to open type core gen file: '%s' for initiation.\n", bdata(typecorepath));
        exit(-1);
    }
}

void metareplace(bstring templatepath, const char *arg, bstring forwarddecl, const char *typestr, bstring outpath) {
    bstring barg = bfromcstr(arg);
    FILE *input = NULL;
    FILE *output = NULL;
    struct tagbstring postfix = bsStatic("\n");
    bstring btypestr = bfromcstr(typestr);
    bstring stubinclude = bfromcstr("#include \"");
    bcatcstr(stubinclude, typestr);
    bcatcstr(stubinclude, ".h\"");
    if (NULL != (input = fopen(bdata(templatepath), "r"))) {
        bstring filestringsource_line = bread((bNread) fread, input);
        struct bstrList *lines;
        fclose(input);
        if (NULL != (lines = bsplit(filestringsource_line, '\n'))) {
            for (int i = 0; i < lines->qty; ++i) {
                bfindreplace(lines->entry[i], stubinclude, forwarddecl, 0);
                bfindreplace(lines->entry[i], btypestr, barg, 0);
                binsert(lines->entry[i], blength(lines->entry[i]), &postfix, '?');
                // printf("%04d: %s\n", i, bdatae(lines->entry[i], "NULL"));
            }
            if (NULL != (output = fopen(bdata(outpath), "wb"))) {
                for (int i = 0; i < lines->qty; ++i) {
                    fputs(bdatae(lines->entry[i], "NULL"), output);
                }
                fclose(output);
            }
            else {
                printf("haikal: metagen: error: Failed to open file: %s.\n", bdata(outpath));
            }
            bstrListDestroy(lines);
        }
        bdestroy(filestringsource_line);
    } else {
        printf("haikal: metagen: error: Unable to open type core file: %s.\n", bdata(templatepath));
    }
}

/*
 * generate types and `#include "hk<family>_<arg>.h"` append to main header
 * genpath/family.h <---append--- genpath/family_arg.h
 */
void metagen(char *family, char *arg, char *forwarddeclparam, char *ext, const char *typestr) {
    // FILE *input = NULL;
    FILE *output = NULL;
    bstring forwarddecl = bfromcstr(forwarddeclparam);
    bcatcstr(forwarddecl, "(");
    bcatcstr(forwarddecl, arg);
    bcatcstr(forwarddecl, ");");

    bstring templatepath = bfromcstr(metapath);
    bcatcstr(templatepath, family);
    bcatcstr(templatepath, "_");
    bcatcstr(templatepath, typestr);
    bcatcstr(templatepath, ext);
    // printf("templatepath: %s\n", bdata(templatepath));

    bstring outpath = bfromcstr(metapath);
    bcatcstr(outpath, "gen/");
    bcatcstr(outpath, family);
    bcatcstr(outpath, "_");
    bcatcstr(outpath, arg);
    bcatcstr(outpath, ext);
    // printf("outpath: %s\n", bdata(outpath));

    metareplace(templatepath, arg, forwarddecl, typestr, outpath);
    bdestroy(templatepath);
    bdestroy(outpath);

    bstring typecorepathtarget = bfromcstr(metapath);
    bcatcstr(typecorepathtarget, "gen/");
    bcatcstr(typecorepathtarget, family);
    bcatcstr(typecorepathtarget, ext);
    // printf("typecorepathtarget: %s\n", bdata(typecorepathtarget));
    if (NULL != (output = fopen(bdata(typecorepathtarget), "ab"))) {
        bstring result = bfromcstr("#include \"");
        bcatcstr(result, family);
        bcatcstr(result, "_");
        bcatcstr(result, arg);
        bcatcstr(result, ext);
        bcatcstr(result, "\"\n");
        // printf("final header name: %s\n", bdata(result));
        fputs(bdatae(result, "NULL"), output);
        bdestroy(result);
        fclose(output);
    } else {
        printf("metainit: Unable to open type core file for initiation.");
    }
    bdestroy(typecorepathtarget);
    return;
}

void metageninternal(char *family, char *arg, char *forwarddeclparam, char *ext, const char *typestr) {
    bstring forwarddecl = bfromcstr(forwarddeclparam);
    bcatcstr(forwarddecl, "(");
    bcatcstr(forwarddecl, arg);
    bcatcstr(forwarddecl, ");");

    bstring templatepathinternal = bfromcstr(metapath);
    bcatcstr(templatepathinternal, family);
    bcatcstr(templatepathinternal, "_");
    bcatcstr(templatepathinternal, typestr);
    bcatcstr(templatepathinternal, "_internal");
    bcatcstr(templatepathinternal, ext);
    // printf("templatepathinternal: %s\n", bdata(templatepathinternal));

    bstring outpathinternal = bfromcstr(metapath);
    bcatcstr(outpathinternal, "gen/");
    bcatcstr(outpathinternal, family);
    bcatcstr(outpathinternal, "_");
    bcatcstr(outpathinternal, arg);
    bcatcstr(outpathinternal, "_internal");
    bcatcstr(outpathinternal, ext);
    // printf("outpathinternal: %s\n", bdata(outpathinternal));

    metareplace(templatepathinternal, arg, forwarddecl, typestr, outpathinternal);
    bdestroy(templatepathinternal);
    bdestroy(outpathinternal);
}

void metacore(char *family) {
    char *coretypes[] = {
        "i8", "i16", "i32", "i64",
        "u8", "u16", "u32", "u64",
        "f32", "f64",
        "str", "cstr",
    };
    i8 coretypeslen = sizeofarray(coretypes);
    for (int i = 0; i < coretypeslen; ++i) {
        metainit(family, ".h");
        metainit(family, ".c");
    }
    for (int i = 0; i < coretypeslen; ++i) {
        metagen(family, coretypes[i], "structdecl", ".h", typestr);
        metagen(family, coretypes[i], "structdecl", ".c", typestr);
    }
}

static void initialize_umbrella_files(void) {
    char *families[] = {
        "Array",
        "Vec",
        "Node",
        "List",
        "BiNode",
        "DList",
        "Queue",
        "Stack",
        "Map",
        "reflect",
    };
    usize count = sizeofarray(families);
    for (usize i = 0; i < count; i += 1) {
        metainit(families[i], ".h");
        metainit(families[i], ".c");
    }
}

static void usage(void) {
    printf("usage: haikal.exe --compile-db <path> --meta <path> [--entry <path>]\n");
}

static bool streq(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

static char *dup_range(const char *start, usize length) {
    char *result = malloc(length + 1);
    if (!result) {
        printf("haikal: error: malloc failed.\n");
        exit(1);
    }
    memcpy(result, start, length);
    result[length] = 0;
    return result;
}

static char *trim_dup(const char *start, usize length) {
    while (length > 0 && isspace((unsigned char)*start)) {
        start += 1;
        length -= 1;
    }
    while (length > 0 && isspace((unsigned char)start[length - 1])) {
        length -= 1;
    }
    return dup_range(start, length);
}

static const char *with_trailing_slash(const char *path) {
    usize length = strlen(path);
    if (length > 0 && (path[length - 1] == '/' || path[length - 1] == '\\')) {
        return path;
    }

    char *result = malloc(length + 2);
    if (!result) {
        printf("haikal: error: malloc failed.\n");
        exit(1);
    }
    memcpy(result, path, length);
    result[length] = '/';
    result[length + 1] = 0;
    return result;
}

static char *infer_project_root(const char *compile_db);
static char *dirname_dup(const char *path);

static void ensure_directory(const char *path) {
#ifdef _MSC_VER
    if (_mkdir(path) == 0) {
        return;
    }
    if (errno == EEXIST) {
        return;
    }
#else
    if (mkdir(path, 0775) == 0) {
        return;
    }
    if (errno == EEXIST) {
        return;
    }
#endif
    printf("haikal: error: failed to create directory: %s\n", path);
    exit(1);
}

static void ensure_gen_directory(void) {
    bstring genpath = bfromcstr(metapath);
    bcatcstr(genpath, "gen");
    ensure_directory(bdata(genpath));
    bdestroy(genpath);
}

static void parse_args(int argc, char **argv) {
    for (int i = 1; i < argc; i += 1) {
        if ((streq(argv[i], "--entry") || streq(argv[i], "-e")) && i + 1 < argc) {
            mainpath = argv[++i];
        } else if ((streq(argv[i], "--compile-db") || streq(argv[i], "-cdb")) && i + 1 < argc) {
            compile_db_path = argv[++i];
        } else if ((streq(argv[i], "--meta") || streq(argv[i], "-m")) && i + 1 < argc) {
            metapath = argv[++i];
        } else if (streq(argv[i], "--help") || streq(argv[i], "-h")) {
            usage();
            exit(0);
        } else {
            printf("haikal: error: unknown or incomplete argument: %s\n", argv[i]);
            usage();
            exit(1);
        }
    }

    if (!compile_db_path || !metapath) {
        usage();
        exit(1);
    }
    metapath = with_trailing_slash(metapath);
    project_root = infer_project_root(compile_db_path);
    char *meta_dir = dirname_dup(metapath);
    haikal_source_root = dirname_dup(meta_dir);
    free(meta_dir);
    ensure_gen_directory();
}

static bstring read_file_bstring(const char *path) {
    FILE *input = fopen(path, "rb");
    if (!input) {
        return NULL;
    }

    bstring result = bread((bNread)fread, input);
    fclose(input);
    return result;
}

static char *dirname_dup(const char *path) {
    const char *last_slash = strrchr(path, '/');
    const char *last_backslash = strrchr(path, '\\');
    const char *last = last_slash;
    if (!last || (last_backslash && last_backslash > last)) {
        last = last_backslash;
    }
    if (!last) {
        return dup_range(".", 1);
    }
    return dup_range(path, (usize)(last - path));
}

static char *infer_project_root(const char *compile_db) {
    char *build_dir = dirname_dup(compile_db);
    char *root = dirname_dup(build_dir);
    free(build_dir);
    return root;
}

static bool path_starts_with(const char *path, const char *prefix) {
    usize prefix_len = strlen(prefix);
    for (usize i = 0; i < prefix_len; i += 1) {
        char a = path[i];
        char b = prefix[i];
        if (a == '\\') { a = '/'; }
        if (b == '\\') { b = '/'; }
#ifdef _MSC_VER
        a = (char)tolower((unsigned char)a);
        b = (char)tolower((unsigned char)b);
#endif
        if (a != b) {
            return false;
        }
    }
    return true;
}

static bool path_equals(const char *a, const char *b) {
    while (*a && *b) {
        char ca = *a++;
        char cb = *b++;
        if (ca == '\\') { ca = '/'; }
        if (cb == '\\') { cb = '/'; }
#ifdef _MSC_VER
        ca = (char)tolower((unsigned char)ca);
        cb = (char)tolower((unsigned char)cb);
#endif
        if (ca != cb) {
            return false;
        }
    }
    return *a == 0 && *b == 0;
}

static bool path_has_segment(const char *path, const char *segment) {
    usize segment_len = strlen(segment);
    const char *cursor = path;
    while (*cursor) {
        while (*cursor == '/' || *cursor == '\\') {
            cursor += 1;
        }

        const char *start = cursor;
        while (*cursor && *cursor != '/' && *cursor != '\\') {
            cursor += 1;
        }

        usize length = (usize)(cursor - start);
        if (length == segment_len) {
            bool same = true;
            for (usize i = 0; i < length; i += 1) {
                char a = start[i];
                char b = segment[i];
#ifdef _MSC_VER
                a = (char)tolower((unsigned char)a);
                b = (char)tolower((unsigned char)b);
#endif
                if (a != b) {
                    same = false;
                    break;
                }
            }
            if (same) {
                return true;
            }
        }
    }
    return false;
}

static void type_table_add(TypeTable *table, const char *name, TypeKind kind) {
    if (!name || name[0] == 0) {
        return;
    }

    for (TypeRecord *record = table->first; record; record = record->next) {
        if (strcmp(bdata(record->name), name) == 0) {
            record->kind = kind;
            return;
        }
    }

    TypeRecord *record = malloc(sizeof(TypeRecord));
    if (!record) {
        printf("haikal: error: malloc failed.\n");
        exit(1);
    }
    record->name = bfromcstr(name);
    record->kind = kind;
    record->next = table->first;
    table->first = record;
}

static void type_table_add_primitives(TypeTable *table) {
    const char *types[] = {
        "voidptr",
        "i8", "i16", "i32", "i64",
        "u8", "u16", "u32", "u64",
        "f32", "f64",
        "char", "str", "cstr",
    };
    for (usize i = 0; i < sizeofarray(types); i += 1) {
        type_table_add(table, types[i], TypeKind_prim);
    }
}

static TypeKind type_table_find(TypeTable *table, const char *name) {
    for (TypeRecord *record = table->first; record; record = record->next) {
        if (strcmp(bdata(record->name), name) == 0) {
            return record->kind;
        }
    }
    return TypeKind_unknown;
}

static MetaTypeRecord *meta_table_find(MetaTable *table, const char *name) {
    for (MetaTypeRecord *record = table->first; record; record = record->next) {
        if (strcmp(bdata(record->name), name) == 0) {
            return record;
        }
    }
    return NULL;
}

static MetaTypeRecord *meta_table_add(MetaTable *table, const char *name, TypeKind kind, i64 size, i64 align) {
    if (!name || name[0] == 0) {
        return NULL;
    }

    MetaTypeRecord *record = meta_table_find(table, name);
    if (!record) {
        record = malloc(sizeof(MetaTypeRecord));
        if (!record) {
            printf("haikal: error: malloc failed.\n");
            exit(1);
        }
        record->name = bfromcstr(name);
        record->fields_first = NULL;
        record->fields_last = NULL;
        record->enum_values_first = NULL;
        record->enum_values_last = NULL;
        record->next = table->first;
        table->first = record;
    }

    record->kind = kind;
    record->size = size;
    record->align = align;
    return record;
}

static bool meta_type_has_enum_value(MetaTypeRecord *type, const char *name) {
    for (MetaEnumValueRecord *value = type->enum_values_first; value; value = value->next) {
        if (strcmp(bdata(value->name), name) == 0) {
            return true;
        }
    }
    return false;
}

static void meta_type_add_enum_value(MetaTypeRecord *type, const char *name, i64 value) {
    if (!type || !name || name[0] == 0 || meta_type_has_enum_value(type, name)) {
        return;
    }

    MetaEnumValueRecord *record = malloc(sizeof(MetaEnumValueRecord));
    if (!record) {
        printf("haikal: error: malloc failed.\n");
        exit(1);
    }
    record->name = bfromcstr(name);
    record->value = value;
    record->next = NULL;

    if (type->enum_values_last) {
        type->enum_values_last->next = record;
    } else {
        type->enum_values_first = record;
    }
    type->enum_values_last = record;
}

static bool meta_type_has_field(MetaTypeRecord *type, const char *name) {
    for (MetaFieldRecord *field = type->fields_first; field; field = field->next) {
        if (strcmp(bdata(field->name), name) == 0) {
            return true;
        }
    }
    return false;
}

static void meta_type_add_field(MetaTypeRecord *type, const char *name, const char *field_type, i64 offset, i64 size, i64 align) {
    if (!type || !name || name[0] == 0 || meta_type_has_field(type, name)) {
        return;
    }

    MetaFieldRecord *field = malloc(sizeof(MetaFieldRecord));
    if (!field) {
        printf("haikal: error: malloc failed.\n");
        exit(1);
    }
    field->name = bfromcstr(name);
    field->type = bfromcstr(field_type ? field_type : "");
    field->offset = offset;
    field->size = size;
    field->align = align;
    field->next = NULL;

    if (type->fields_last) {
        type->fields_last->next = field;
    } else {
        type->fields_first = field;
    }
    type->fields_last = field;
}

static void merge_directives(Directive **dst, Directive *src) {
    for (Directive *iter = src; iter; iter = iter->next) {
        append_directive(dst, bstrcpy(iter->source_line), iter->column, bstrcpy(iter->family), bstrcpy(iter->arg), bdata(iter->path), iter->line);
    }
}

static void merge_types(TypeTable *dst, TypeTable *src) {
    for (TypeRecord *iter = src->first; iter; iter = iter->next) {
        type_table_add(dst, bdata(iter->name), iter->kind);
    }
}

static void merge_meta(MetaTable *dst, MetaTable *src) {
    for (MetaTypeRecord *type = src->first; type; type = type->next) {
        MetaTypeRecord *merged = meta_table_add(dst, bdata(type->name), type->kind, type->size, type->align);
        for (MetaFieldRecord *field = type->fields_first; field; field = field->next) {
            meta_type_add_field(merged, bdata(field->name), bdata(field->type), field->offset, field->size, field->align);
        }
        for (MetaEnumValueRecord *value = type->enum_values_first; value; value = value->next) {
            meta_type_add_enum_value(merged, bdata(value->name), value->value);
        }
    }
}

static char *cx_string_dup(CXString string) {
    const char *cstr = clang_getCString(string);
    char *result = NULL;
    if (cstr) {
        result = dup_range(cstr, strlen(cstr));
    }
    clang_disposeString(string);
    return result;
}

static TypeKind type_kind_from_cursor_kind(enum CXCursorKind kind) {
    switch (kind) {
        case CXCursor_StructDecl: return TypeKind_struct;
        case CXCursor_UnionDecl: return TypeKind_union;
        case CXCursor_EnumDecl: return TypeKind_enum;
        default: return TypeKind_unknown;
    }
}

static TypeKind typedef_kind_from_cursor(CXCursor cursor, const char *typedef_name) {
    CXType underlying = clang_getTypedefDeclUnderlyingType(cursor);
    CXCursor decl = clang_getTypeDeclaration(underlying);
    TypeKind kind = type_kind_from_cursor_kind(clang_getCursorKind(decl));
    if (kind == TypeKind_unknown) {
        return TypeKind_prim;
    }

    char *decl_name = cx_string_dup(clang_getCursorSpelling(decl));
    bool same_name = decl_name && strcmp(decl_name, typedef_name) == 0;
    free(decl_name);
    return same_name ? kind : TypeKind_prim;
}

static bool cursor_is_project_local(CXCursor cursor) {
    CXSourceLocation location = clang_getCursorLocation(cursor);
    if (clang_equalLocations(location, clang_getNullLocation())) {
        return true;
    }

    CXFile file = NULL;
    clang_getSpellingLocation(location, &file, NULL, NULL, NULL);
    if (!file) {
        return true;
    }

    char *path = cx_string_dup(clang_getFileName(file));
    bool result = path_is_project_owned(path);
    free(path);
    return result;
}

static enum CXChildVisitResult collect_field_cursor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    (void)parent;
    MetaTypeRecord *type = (MetaTypeRecord *)client_data;
    if (clang_getCursorKind(cursor) != CXCursor_FieldDecl) {
        return CXChildVisit_Continue;
    }

    char *name = cx_string_dup(clang_getCursorSpelling(cursor));
    char *field_type = cx_string_dup(clang_getTypeSpelling(clang_getCursorType(cursor)));
    i64 offset = clang_Cursor_getOffsetOfField(cursor);
    if (offset >= 0) {
        offset /= 8;
    }
    CXType cxtype = clang_getCursorType(cursor);
    i64 size = clang_Type_getSizeOf(cxtype);
    i64 align = clang_Type_getAlignOf(cxtype);
    meta_type_add_field(type, name, field_type, offset, size, align);
    free(name);
    free(field_type);
    return CXChildVisit_Continue;
}

static enum CXChildVisitResult collect_enum_value_cursor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    (void)parent;
    MetaTypeRecord *type = (MetaTypeRecord *)client_data;
    if (clang_getCursorKind(cursor) != CXCursor_EnumConstantDecl) {
        return CXChildVisit_Continue;
    }

    char *name = cx_string_dup(clang_getCursorSpelling(cursor));
    meta_type_add_enum_value(type, name, clang_getEnumConstantDeclValue(cursor));
    free(name);
    return CXChildVisit_Continue;
}

static void collect_meta_type(ScanState *state, CXCursor cursor, TypeKind kind) {
    char *name = cx_string_dup(clang_getCursorSpelling(cursor));
    if (!name || name[0] == 0) {
        free(name);
        return;
    }

    CXType cxtype = clang_getCursorType(cursor);
    MetaTypeRecord *record = meta_table_add(state->meta, name, kind, clang_Type_getSizeOf(cxtype), clang_Type_getAlignOf(cxtype));
    if (record && (kind == TypeKind_struct || kind == TypeKind_union)) {
        clang_visitChildren(cursor, collect_field_cursor, record);
    } else if (record && kind == TypeKind_enum) {
        clang_visitChildren(cursor, collect_enum_value_cursor, record);
    }
    free(name);
}

static enum CXChildVisitResult collect_semantic_cursor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    (void)parent;
    ScanState *state = (ScanState *)client_data;
    enum CXCursorKind cursor_kind = clang_getCursorKind(cursor);

    if (cursor_kind != CXCursor_TranslationUnit && !cursor_is_project_local(cursor)) {
        return CXChildVisit_Continue;
    }

    TypeKind kind = type_kind_from_cursor_kind(cursor_kind);
    if (kind != TypeKind_unknown) {
        char *name = cx_string_dup(clang_getCursorSpelling(cursor));
        if (name && name[0] != 0) {
            type_table_add(state->types, name, kind);
            collect_meta_type(state, cursor, kind);
        }
        free(name);
    } else if (cursor_kind == CXCursor_TypedefDecl) {
        char *name = cx_string_dup(clang_getCursorSpelling(cursor));
        if (name && name[0] != 0) {
            type_table_add(state->types, name, typedef_kind_from_cursor(cursor, name));
        }
        free(name);
    }

    return CXChildVisit_Recurse;
}

static const char *forward_decl_for_type(TypeTable *types, const char *type) {
    switch (type_table_find(types, type)) {
        case TypeKind_prim: return "primdecl";
        case TypeKind_enum: return "enumdecl";
        case TypeKind_union: return "uniondecl";
        case TypeKind_struct: return "structdecl";
        case TypeKind_unknown: break;
    }
    return "primdecl";
}

static bool append_directive(Directive **head, bstring line, i32 column, bstring family, bstring arg, const char *path, i32 line_number) {
    for (Directive *iter = *head; iter != NULL; iter = iter->next) {
        if (strcmp(bdata(iter->family), bdata(family)) == 0 &&
            strcmp(bdata(iter->arg), bdata(arg)) == 0) {
            bdestroy(line);
            bdestroy(family);
            bdestroy(arg);
            return false;
        }
    }

    Directive *node = Directive_create(line, column, family, arg, path, line_number);
    if (*head == NULL) {
        *head = node;
        return true;
    }

    Directive *iter = *head;
    while (iter->next != NULL) {
        iter = iter->next;
    }
    iter->next = node;
    return true;
}

static void parse_template_inner(const char *inner, bstring *family, bstring *arg) {
    const char *open = strchr(inner, '(');
    const char *comma = strchr(inner, ',');

    if (open && (!comma || open < comma)) {
        const char *close = strrchr(inner, ')');
        if (!close || close < open) {
            printf("haikal: template: error: malformed directive: %s\n", inner);
            exit(1);
        }

        char *name = trim_dup(inner, (usize)(open - inner));
        char *arg_text = trim_dup(open + 1, (usize)(close - open - 1));
        *family = bfromcstr(name);
        *arg = bfromcstr(arg_text);
        free(name);
        free(arg_text);
        return;
    }

    if (comma) {
        char *name = trim_dup(inner, (usize)(comma - inner));
        char *arg_text = trim_dup(comma + 1, strlen(comma + 1));
        *family = bfromcstr(name);
        *arg = bfromcstr(arg_text);
        free(name);
        free(arg_text);
        return;
    }

    printf("haikal: template: error: expected template(Vec(i32)) or template(Vec, i32): %s\n", inner);
    exit(1);
}

static void collect_templates_from_line(const char *line, const char *path, i32 line_number, Directive **head) {
    if (strstr(line, "#define template")) {
        return;
    }

    const char *cursor = line;
    while ((cursor = strstr(cursor, "template(")) != NULL) {
        if (cursor > line && (isalnum((unsigned char)cursor[-1]) || cursor[-1] == '_')) {
            cursor += strlen("template(");
            continue;
        }
        const char *inner = cursor + strlen("template(");
        int depth = 1;
        const char *end = inner;
        while (*end && depth > 0) {
            if (*end == '(') {
                depth += 1;
            } else if (*end == ')') {
                depth -= 1;
            }
            end += 1;
        }
        if (depth != 0) {
            printf("haikal: template: error: unbalanced directive: %s\n", line);
            exit(1);
        }

        char *inner_text = dup_range(inner, (usize)((end - 1) - inner));
        bstring family = NULL;
        bstring arg = NULL;
        parse_template_inner(inner_text, &family, &arg);
        append_directive(head, bfromcstr(line), (i32)(cursor - line), family, arg, path, line_number);
        free(inner_text);
        cursor = end;
    }
}

static void collect_reflects_from_line(const char *line, const char *path, i32 line_number, Directive **head) {
    if (strstr(line, "#define reflect")) {
        return;
    }

    const char *cursor = line;
    while ((cursor = strstr(cursor, "reflect(")) != NULL) {
        if (cursor > line && (isalnum((unsigned char)cursor[-1]) || cursor[-1] == '_')) {
            cursor += strlen("reflect(");
            continue;
        }
        const char *inner = cursor + strlen("reflect(");
        int depth = 1;
        const char *end = inner;
        while (*end && depth > 0) {
            if (*end == '(') {
                depth += 1;
            } else if (*end == ')') {
                depth -= 1;
            }
            end += 1;
        }
        if (depth != 0) {
            printf("haikal: reflect: error: unbalanced directive: %s\n", line);
            exit(1);
        }

        char *type_name = trim_dup(inner, (usize)((end - 1) - inner));
        bstring family = bfromcstr("reflect");
        bstring arg = bfromcstr(type_name);
        append_directive(head, bfromcstr(line), (i32)(cursor - line), family, arg, path, line_number);
        free(type_name);
        cursor = end;
    }
}

static void collect_directives_from_file(const char *path, Directive **templates, Directive **reflects) {
    if (!project_root || !path_starts_with(path, project_root)) {
        return;
    }
    if (haikal_source_root && path_starts_with(path, haikal_source_root)) {
        return;
    }

    bstring file = read_file_bstring(path);
    if (!file) {
        return;
    }

    struct bstrList *lines = bsplit(file, '\n');
    if (lines) {
        for (i32 i = 0; i < lines->qty; i += 1) {
            collect_templates_from_line(bdata(lines->entry[i]), path, i + 1, templates);
            collect_reflects_from_line(bdata(lines->entry[i]), path, i + 1, reflects);
        }
        bstrListDestroy(lines);
    }
    bdestroy(file);
}

static void string_list_push(StringList *list, char *item) {
    if (list->count == list->capacity) {
        i32 next_capacity = list->capacity == 0 ? 16 : list->capacity * 2;
        char **next_items = realloc(list->items, sizeof(char *) * next_capacity);
        if (!next_items) {
            printf("haikal: error: malloc failed.\n");
            exit(1);
        }
        list->items = next_items;
        list->capacity = next_capacity;
    }
    list->items[list->count++] = item;
}

static void string_list_destroy(StringList *list) {
    for (i32 i = 0; i < list->count; i += 1) {
        free(list->items[i]);
    }
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

static void hash_init(HashState *hash) {
    hash->value = 1469598103934665603ULL;
    hash->value ^= HAIKAL_CACHE_VERSION;
    hash->value *= 1099511628211ULL;
}

static void hash_bytes(HashState *hash, const void *source_line, usize size) {
    const unsigned char *bytes = (const unsigned char *)source_line;
    for (usize i = 0; i < size; i += 1) {
        hash->value ^= bytes[i];
        hash->value *= 1099511628211ULL;
    }
}

static void hash_cstr(HashState *hash, const char *text) {
    if (text) {
        hash_bytes(hash, text, strlen(text));
    }
    hash_bytes(hash, "\n", 1);
}

static void hash_file(HashState *hash, const char *path) {
    bstring content = read_file_bstring(path);
    hash_cstr(hash, path);
    if (content) {
        hash_bytes(hash, bdata(content), (usize)blength(content));
        bdestroy(content);
    } else {
        hash_cstr(hash, "<missing>");
    }
}

static bool should_skip_scan_dir(const char *path) {
    const char *base = path_basename(path);
    return streq(base, ".git") ||
           streq(base, ".cache") ||
           streq(base, "build") ||
           streq(base, "extern") ||
           (haikal_source_root && path_starts_with(path, haikal_source_root));
}

static void scan_project_tree(const char *root, HashState *hash, Directive **templates, Directive **reflects);

static void scan_project_file(const char *path, HashState *hash, Directive **templates, Directive **reflects) {
    if (!path_is_project_owned(path) || !path_has_source_or_header_extension(path)) {
        return;
    }
    hash_file(hash, path);
    collect_directives_from_file(path, templates, reflects);
}

#ifdef _MSC_VER
static void scan_project_tree(const char *root, HashState *hash, Directive **templates, Directive **reflects) {
    if (should_skip_scan_dir(root)) {
        return;
    }

    bstring pattern = bfromcstr(root);
    bcatcstr(pattern, "\\*");
    WIN32_FIND_DATAA find_data;
    HANDLE handle = FindFirstFileA(bdata(pattern), &find_data);
    bdestroy(pattern);
    if (handle == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        if (streq(find_data.cFileName, ".") || streq(find_data.cFileName, "..")) {
            continue;
        }
        bstring path = bfromcstr(root);
        bcatcstr(path, "\\");
        bcatcstr(path, find_data.cFileName);
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            scan_project_tree(bdata(path), hash, templates, reflects);
        } else {
            scan_project_file(bdata(path), hash, templates, reflects);
        }
        bdestroy(path);
    } while (FindNextFileA(handle, &find_data));

    FindClose(handle);
}
#else
static void scan_project_tree(const char *root, HashState *hash, Directive **templates, Directive **reflects) {
    if (should_skip_scan_dir(root)) {
        return;
    }

    DIR *dir = opendir(root);
    if (!dir) {
        return;
    }

    struct dirent *entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (streq(entry->d_name, ".") || streq(entry->d_name, "..")) {
            continue;
        }
        bstring path = bfromcstr(root);
        bcatcstr(path, "/");
        bcatcstr(path, entry->d_name);
        if (entry->d_type == DT_DIR) {
            scan_project_tree(bdata(path), hash, templates, reflects);
        } else {
            scan_project_file(bdata(path), hash, templates, reflects);
        }
        bdestroy(path);
    }

    closedir(dir);
}
#endif

static void hash_meta_templates(HashState *hash) {
    bstring root = bfromcstr(metapath);
    usize root_len = (usize)blength(root);
#ifdef _MSC_VER
    bcatcstr(root, "*");
    WIN32_FIND_DATAA find_data;
    HANDLE handle = FindFirstFileA(bdata(root), &find_data);
    root->slen = (int)root_len;
    bdata(root)[root_len] = 0;
    if (handle == INVALID_HANDLE_VALUE) {
        bdestroy(root);
        return;
    }
    do {
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue;
        }
        bstring path = bstrcpy(root);
        bcatcstr(path, find_data.cFileName);
        hash_file(hash, bdata(path));
        bdestroy(path);
    } while (FindNextFileA(handle, &find_data));
    FindClose(handle);
#else
    DIR *dir = opendir(bdata(root));
    if (dir) {
        struct dirent *entry = NULL;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR) {
                continue;
            }
            bstring path = bstrcpy(root);
            bcatcstr(path, entry->d_name);
            hash_file(hash, bdata(path));
            bdestroy(path);
        }
        closedir(dir);
    }
#endif
    bdestroy(root);
}

static bool file_exists(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        return false;
    }
    fclose(file);
    return true;
}

static bool generated_template_outputs_exist(Directive *templates) {
    for (Directive *iter = templates; iter; iter = iter->next) {
        const char *name = bdata(iter->family);
        const char *arg = bdata(iter->arg);
        const char *exts[] = { ".h", ".c", "_internal.h" };
        for (usize i = 0; i < sizeofarray(exts); i += 1) {
            bstring path = bfromcstr(metapath);
            bcatcstr(path, "gen/");
            bcatcstr(path, name);
            bcatcstr(path, "_");
            bcatcstr(path, arg);
            bcatcstr(path, exts[i]);
            bool exists = file_exists(bdata(path));
            bdestroy(path);
            if (!exists) {
                return false;
            }
        }
    }
    return true;
}

static bool generated_reflect_outputs_exist(Directive *reflects) {
    if (reflects) {
        bstring umbrella = bfromcstr(metapath);
        bcatcstr(umbrella, "gen/reflect.h");
        bool exists = file_exists(bdata(umbrella));
        bdestroy(umbrella);
        if (!exists) {
            return false;
        }
    }

    for (Directive *iter = reflects; iter; iter = iter->next) {
        bstring path = bfromcstr(metapath);
        bcatcstr(path, "gen/reflect_");
        bconcat(path, iter->arg);
        bcatcstr(path, ".h");
        bool exists = file_exists(bdata(path));
        bdestroy(path);
        if (!exists) {
            return false;
        }
    }
    return true;
}

static bool generated_outputs_exist(Directive *templates, Directive *reflects) {
    return generated_template_outputs_exist(templates) && generated_reflect_outputs_exist(reflects);
}

static void cache_path(bstring path) {
    bcatcstr(path, metapath);
    bcatcstr(path, "gen/.haikal_cache");
}

static void hash_inputs(HashState *hash, Directive **templates, Directive **reflects) {
    hash_init(hash);
    hash_file(hash, compile_db_path);
    hash_meta_templates(hash);
    scan_project_tree(project_root, hash, templates, reflects);
}

static bool cache_matches(u64 signature, Directive *templates, Directive *reflects) {
    if (!generated_outputs_exist(templates, reflects)) {
        return false;
    }

    bstring path = bfromcstr("");
    cache_path(path);
    bstring cache = read_file_bstring(bdata(path));
    bdestroy(path);
    if (!cache) {
        return false;
    }

    char expected[32];
    snprintf(expected, sizeof(expected), "%016llx", (unsigned long long)signature);
    bool matches = strstr(bdata(cache), expected) != NULL;
    bdestroy(cache);
    return matches;
}

static void write_cache(u64 signature) {
    bstring path = bfromcstr("");
    cache_path(path);
    FILE *output = fopen(bdata(path), "wb");
    if (output) {
        fprintf(output, "version=%llu\nsignature=%016llx\n",
                (unsigned long long)HAIKAL_CACHE_VERSION,
                (unsigned long long)signature);
        fclose(output);
    }
    bdestroy(path);
}

static char *json_parse_string(const char **cursor) {
    if (**cursor != '"') {
        return NULL;
    }
    *cursor += 1;

    bstring out = bfromcstr("");
    while (**cursor && **cursor != '"') {
        char c = **cursor;
        if (c == '\\') {
            *cursor += 1;
            c = **cursor;
            switch (c) {
                case '"': bconchar(out, '"'); break;
                case '\\': bconchar(out, '\\'); break;
                case '/': bconchar(out, '/'); break;
                case 'b': bconchar(out, '\b'); break;
                case 'f': bconchar(out, '\f'); break;
                case 'n': bconchar(out, '\n'); break;
                case 'r': bconchar(out, '\r'); break;
                case 't': bconchar(out, '\t'); break;
                default: bconchar(out, c); break;
            }
        } else {
            bconchar(out, c);
        }
        *cursor += 1;
    }
    if (**cursor == '"') {
        *cursor += 1;
    }

    char *result = dup_range(bdata(out), (usize)blength(out));
    bdestroy(out);
    return result;
}

static char *json_next_string_value(const char **cursor, const char *key) {
    bstring pattern = bfromcstr("\"");
    bcatcstr(pattern, key);
    bcatcstr(pattern, "\"");

    const char *found = strstr(*cursor, bdata(pattern));
    bdestroy(pattern);
    if (!found) {
        return NULL;
    }

    found = strchr(found, ':');
    if (!found) {
        return NULL;
    }
    found += 1;
    while (*found && isspace((unsigned char)*found)) {
        found += 1;
    }
    if (*found != '"') {
        return NULL;
    }

    const char *value_cursor = found;
    char *value = json_parse_string(&value_cursor);
    *cursor = value_cursor;
    return value;
}

static void compile_database_append(CompileDatabase *db, char *directory, char *command, char *file) {
    CompileCommand *entry = malloc(sizeof(CompileCommand));
    if (!entry) {
        printf("haikal: error: malloc failed.\n");
        exit(1);
    }
    entry->directory = bfromcstr(directory);
    entry->command = bfromcstr(command);
    entry->file = bfromcstr(file);
    entry->next = NULL;

    if (db->last) {
        db->last->next = entry;
    } else {
        db->first = entry;
    }
    db->last = entry;
}

static CompileDatabase read_compile_database(const char *path) {
    bstring json = read_file_bstring(path);
    if (!json) {
        printf("haikal: compile-db: error: unable to read: %s\n", path);
        exit(1);
    }

    CompileDatabase db = {};
    const char *cursor = bdata(json);
    while (true) {
        char *directory = json_next_string_value(&cursor, "directory");
        char *command = json_next_string_value(&cursor, "command");
        char *file = json_next_string_value(&cursor, "file");
        if (!directory || !command || !file) {
            free(directory);
            free(command);
            free(file);
            break;
        }

        if (directory && command && file) {
            compile_database_append(&db, directory, command, file);
        }
        free(directory);
        free(command);
        free(file);
    }

    bdestroy(json);
    return db;
}

static bool path_has_c_extension(const char *path) {
    const char *dot = strrchr(path, '.');
    return dot && (streq(dot, ".c") || streq(dot, ".C"));
}

static bool path_has_header_extension(const char *path) {
    const char *dot = strrchr(path, '.');
    return dot && (streq(dot, ".h") || streq(dot, ".H"));
}

static bool path_has_source_or_header_extension(const char *path) {
    return path_has_c_extension(path) || path_has_header_extension(path);
}

static const char *path_basename(const char *path) {
    const char *slash = strrchr(path, '/');
    const char *backslash = strrchr(path, '\\');
    const char *base = slash;
    if (!base || (backslash && backslash > base)) {
        base = backslash;
    }
    return base ? base + 1 : path;
}

static bool path_is_project_owned(const char *path) {
    return path &&
           project_root &&
           path_starts_with(path, project_root) &&
           !path_has_segment(path, "extern") &&
           !path_has_segment(path, "build") &&
           !(haikal_source_root && path_starts_with(path, haikal_source_root));
}

static StringList split_command_line(const char *command) {
    StringList result = {};
    const char *cursor = command;
    while (*cursor) {
        while (*cursor && isspace((unsigned char)*cursor)) {
            cursor += 1;
        }
        if (!*cursor) {
            break;
        }

        bstring arg = bfromcstr("");
        bool quoted = false;
        while (*cursor) {
            char c = *cursor;
            if (c == '"') {
                quoted = !quoted;
                cursor += 1;
                continue;
            }
            if (!quoted && isspace((unsigned char)c)) {
                break;
            }
            bconchar(arg, c);
            cursor += 1;
        }
        string_list_push(&result, dup_range(bdata(arg), (usize)blength(arg)));
        bdestroy(arg);
    }
    return result;
}

static bool is_source_arg(const char *arg, const char *file) {
    return streq(arg, file) || path_has_c_extension(arg);
}

static bool should_skip_clang_arg(const char *arg) {
    return streq(arg, "/nologo") ||
           streq(arg, "-c") ||
           streq(arg, "/c") ||
           strncmp(arg, "/Fo", 3) == 0 ||
           strncmp(arg, "/Fd", 3) == 0 ||
           strncmp(arg, "/Fe", 3) == 0 ||
           strncmp(arg, "/Fp", 3) == 0 ||
           strncmp(arg, "/FI", 3) == 0 ||
           strncmp(arg, "/Yu", 3) == 0 ||
           strncmp(arg, "/Yc", 3) == 0 ||
           strncmp(arg, "-o", 2) == 0;
}

static StringList clang_args_from_command(const char *command, const char *file) {
    StringList split = split_command_line(command);
    StringList args = {};
    string_list_push(&args, dup_range("--driver-mode=cl", strlen("--driver-mode=cl")));

    bool after_dashdash = false;
    for (i32 i = 1; i < split.count; i += 1) {
        const char *arg = split.items[i];
        if (streq(arg, "--")) {
            after_dashdash = true;
            continue;
        }
        if (after_dashdash && is_source_arg(arg, file)) {
            continue;
        }
        if (is_source_arg(arg, file) || should_skip_clang_arg(arg)) {
            continue;
        }
        string_list_push(&args, dup_range(arg, strlen(arg)));
    }

    string_list_destroy(&split);
    return args;
}

static void scan_compile_command(CXIndex index, CompileCommand *command, Directive **templates, Directive **reflects, TypeTable *types, MetaTable *meta, ProfileStats *profile) {
    double total_start = haikal_now_seconds();
    if (!path_has_c_extension(bdata(command->file))) {
        profile->skipped_count += 1;
        return;
    }
    if (strstr(bdata(command->file), "cmake_pch.c")) {
        profile->skipped_count += 1;
        return;
    }
    if (path_has_segment(bdata(command->file), "extern") || path_has_segment(bdata(command->file), "build")) {
        profile->skipped_count += 1;
        return;
    }
    if (haikal_source_root && path_starts_with(bdata(command->file), haikal_source_root)) {
        profile->skipped_count += 1;
        return;
    }

    double start = haikal_now_seconds();
    StringList args = clang_args_from_command(bdata(command->command), bdata(command->file));
    profile->args_seconds += haikal_now_seconds() - start;

    start = haikal_now_seconds();
    CXTranslationUnit tu = clang_parseTranslationUnit(
        index,
        bdata(command->file),
        (const char * const *)args.items,
        args.count,
        NULL,
        0,
        CXTranslationUnit_SkipFunctionBodies);
    profile->parse_seconds += haikal_now_seconds() - start;

    if (!tu) {
        printf("haikal: clang: error: failed to parse translation unit: %s\n", bdata(command->file));
        string_list_destroy(&args);
        return;
    }
    profile->parsed_count += 1;

    (void)templates;
    (void)reflects;

    ScanState state = {
        .tu = tu,
        .templates = templates,
        .reflects = reflects,
        .types = types,
        .meta = meta,
    };
    start = haikal_now_seconds();
    clang_visitChildren(clang_getTranslationUnitCursor(tu), collect_semantic_cursor, &state);
    profile->semantic_seconds += haikal_now_seconds() - start;

    start = haikal_now_seconds();
    clang_disposeTranslationUnit(tu);
    profile->dispose_seconds += haikal_now_seconds() - start;
    string_list_destroy(&args);
    profile->total_seconds += haikal_now_seconds() - total_start;
}

static i32 processor_count(void) {
    const char *jobs = getenv("HAIKAL_JOBS");
    if (jobs && jobs[0]) {
        i32 requested = atoi(jobs);
        if (requested > 0) {
            return requested;
        }
    }

#ifdef _MSC_VER
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    i32 count = info.dwNumberOfProcessors > 0 ? (i32)info.dwNumberOfProcessors : 1;
#else
    long count = sysconf(_SC_NPROCESSORS_ONLN);
    if (count < 1) {
        count = 1;
    }
#endif
    return count > 8 ? 8 : count;
}

static bool directives_include_header(Directive *templates, Directive *reflects) {
    for (Directive *iter = templates; iter; iter = iter->next) {
        if (path_has_header_extension(bdata(iter->path))) {
            return true;
        }
    }
    for (Directive *iter = reflects; iter; iter = iter->next) {
        if (path_has_header_extension(bdata(iter->path))) {
            return true;
        }
    }
    return false;
}

static bool command_file_has_directive(CompileCommand *command, Directive *templates, Directive *reflects) {
    for (Directive *iter = templates; iter; iter = iter->next) {
        if (path_equals(bdata(command->file), bdata(iter->path))) {
            return true;
        }
    }
    for (Directive *iter = reflects; iter; iter = iter->next) {
        if (path_equals(bdata(command->file), bdata(iter->path))) {
            return true;
        }
    }
    return false;
}

static bool compile_command_is_project_source(CompileCommand *command) {
    const char *file = bdata(command->file);
    return path_has_c_extension(file) &&
           !strstr(file, "cmake_pch.c") &&
           path_is_project_owned(file);
}

static CompileCommand **select_compile_commands(CompileDatabase *db, Directive *templates, Directive *reflects, i32 *out_count) {
    CompileCommand **commands = NULL;
    i32 count = 0;
    i32 capacity = 0;
    bool header_fallback = directives_include_header(templates, reflects);

    for (CompileCommand *command = db->first; command; command = command->next) {
        if (mainpath && !path_equals(mainpath, bdata(command->file))) {
            continue;
        }
        if (!compile_command_is_project_source(command)) {
            continue;
        }
        if (!mainpath && !header_fallback && !command_file_has_directive(command, templates, reflects)) {
            continue;
        }

        if (count == capacity) {
            capacity = capacity == 0 ? 16 : capacity * 2;
            CompileCommand **next = realloc(commands, sizeof(CompileCommand *) * (usize)capacity);
            if (!next) {
                printf("haikal: error: malloc failed.\n");
                exit(1);
            }
            commands = next;
        }
        commands[count++] = command;
    }

    *out_count = count;
    return commands;
}

static void profile_stats_add(ProfileStats *dst, const ProfileStats *src) {
    dst->total_seconds += src->total_seconds;
    dst->args_seconds += src->args_seconds;
    dst->parse_seconds += src->parse_seconds;
    dst->direct_seconds += src->direct_seconds;
    dst->include_seconds += src->include_seconds;
    dst->semantic_seconds += src->semantic_seconds;
    dst->dispose_seconds += src->dispose_seconds;
    dst->parsed_count += src->parsed_count;
    dst->skipped_count += src->skipped_count;
}

static void print_profile_stats(const ProfileStats *stats, double wall_seconds, i32 thread_count) {
    if (!haikal_profile_enabled()) {
        return;
    }

    printf("haikal: profile: threads=%d parsed=%d skipped=%d wall=%.3fs worker_total=%.3fs\n",
           thread_count,
           stats->parsed_count,
           stats->skipped_count,
           wall_seconds,
           stats->total_seconds);
    printf("haikal: profile: args=%.3fs parse=%.3fs directives=%.3fs includes=%.3fs semantic=%.3fs dispose=%.3fs\n",
           stats->args_seconds,
           stats->parse_seconds,
           stats->direct_seconds,
           stats->include_seconds,
           stats->semantic_seconds,
           stats->dispose_seconds);
}

static int compile_worker(void *userdata) {
    CompileWorkerArg *arg = (CompileWorkerArg *)userdata;
    CompileWorkerState *state = arg->state;
    WorkerResult *result = &state->results[arg->worker_index];
    CXIndex clang_index = clang_createIndex(0, 0);
    if (!clang_index) {
        printf("haikal: clang: error: failed to create clang index.\n");
        return 1;
    }

    for (;;) {
        mtx_lock(&state->mutex);
        i32 command_index = state->next_command;
        state->next_command += 1;
        mtx_unlock(&state->mutex);

        if (command_index >= state->command_count) {
            break;
        }

        CompileCommand *command = state->commands[command_index];
        if (mainpath && !streq(mainpath, bdata(command->file))) {
            continue;
        }
        scan_compile_command(clang_index, command, &result->templates, &result->reflects, &result->types, &result->meta, &result->profile);
    }

    clang_disposeIndex(clang_index);
    return 0;
}

static void collect_from_compile_database(const char *path, Directive **templates, Directive **reflects, TypeTable *types, MetaTable *meta) {
    CompileDatabase db = read_compile_database(path);
    HashState hash = {};
    double scan_start = haikal_now_seconds();
    hash_inputs(&hash, templates, reflects);
    haikal_cache_signature = hash.value;
    if (cache_matches(haikal_cache_signature, *templates, *reflects)) {
        haikal_cache_hit = true;
        printf("haikal: cache: hit.\n");
        return;
    }
    if (haikal_profile_enabled()) {
        printf("haikal: profile: prescan=%.3fs\n", haikal_now_seconds() - scan_start);
    }

    i32 command_count = 0;
    CompileCommand **commands = select_compile_commands(&db, *templates, *reflects, &command_count);
    type_table_add_primitives(types);

    if (command_count == 0) {
        free(commands);
        return;
    }

    i32 thread_count = processor_count();
    if (thread_count > command_count) {
        thread_count = command_count;
    }
    if (thread_count < 1) {
        thread_count = 1;
    }

    WorkerResult *results = calloc((usize)thread_count, sizeof(WorkerResult));
    thrd_t *threads = malloc(sizeof(thrd_t) * (usize)thread_count);
    CompileWorkerArg *args = malloc(sizeof(CompileWorkerArg) * (usize)thread_count);
    if (!results || !threads || !args) {
        printf("haikal: error: malloc failed.\n");
        exit(1);
    }

    CompileWorkerState worker_state = {
        .commands = commands,
        .command_count = command_count,
        .next_command = 0,
        .results = results,
    };
    if (mtx_init(&worker_state.mutex, mtx_plain) != thrd_success) {
        printf("haikal: threads: error: failed to initialize mutex.\n");
        exit(1);
    }

    double wall_start = haikal_now_seconds();
    for (i32 i = 0; i < thread_count; i += 1) {
        args[i].state = &worker_state;
        args[i].worker_index = i;
        if (thrd_create(&threads[i], compile_worker, &args[i]) != thrd_success) {
            printf("haikal: threads: error: failed to create worker.\n");
            exit(1);
        }
    }

    for (i32 i = 0; i < thread_count; i += 1) {
        int worker_status = 0;
        thrd_join(threads[i], &worker_status);
        if (worker_status != 0) {
            printf("haikal: threads: error: worker failed.\n");
            exit(1);
        }
    }
    double wall_seconds = haikal_now_seconds() - wall_start;

    ProfileStats profile = {};
    for (i32 i = 0; i < thread_count; i += 1) {
        merge_directives(templates, results[i].templates);
        merge_directives(reflects, results[i].reflects);
        merge_types(types, &results[i].types);
        merge_meta(meta, &results[i].meta);
        profile_stats_add(&profile, &results[i].profile);
    }
    print_profile_stats(&profile, wall_seconds, thread_count);

    mtx_destroy(&worker_state.mutex);
    free(args);
    free(threads);
    free(results);
    free(commands);
}

static usize meta_field_count(MetaTypeRecord *type) {
    usize count = 0;
    for (MetaFieldRecord *field = type->fields_first; field; field = field->next) {
        count += 1;
    }
    return count;
}

static usize meta_enum_value_count(MetaTypeRecord *type) {
    usize count = 0;
    for (MetaEnumValueRecord *value = type->enum_values_first; value; value = value->next) {
        count += 1;
    }
    return count;
}

static void append_reflect_include(const char *type_name) {
    bstring path = bfromcstr(metapath);
    bcatcstr(path, "gen/reflect.h");
    FILE *output = fopen(bdata(path), "ab");
    if (!output) {
        printf("haikal: reflect: error: failed to open reflect.h.\n");
        exit(1);
    }

    fprintf(output, "#include \"reflect_%s.h\"\n", type_name);
    fclose(output);
    bdestroy(path);
}

static void generate_reflect_header(MetaTypeRecord *type) {
    bstring path = bfromcstr(metapath);
    bcatcstr(path, "gen/reflect_");
    bconcat(path, type->name);
    bcatcstr(path, ".h");

    FILE *output = fopen(bdata(path), "wb");
    if (!output) {
        printf("haikal: reflect: error: failed to open %s.\n", bdata(path));
        exit(1);
    }

    const char *type_name = bdata(type->name);
    fprintf(output, "#pragma once\n\n");
    fprintf(output, "static haikal_meta_field %s_reflect_fields[] = {\n", type_name);
    for (MetaFieldRecord *field = type->fields_first; field; field = field->next) {
        fprintf(output,
                "    { \"%s\", \"%s\", (usize)%lld, (usize)%lld, (usize)%lld },\n",
                bdata(field->name),
                bdata(field->type),
                (long long)field->offset,
                (long long)field->size,
                (long long)field->align);
    }
    fprintf(output, "};\n\n");
    fprintf(output, "static haikal_meta_enum_value %s_reflect_enum_values[] = {\n", type_name);
    for (MetaEnumValueRecord *value = type->enum_values_first; value; value = value->next) {
        fprintf(output,
                "    { \"%s\", (i64)%lld },\n",
                bdata(value->name),
                (long long)value->value);
    }
    fprintf(output, "};\n\n");
    fprintf(output,
            "static haikal_meta_type %s_reflect = { \"%s\", (usize)%lld, (usize)%lld, (usize)%llu, %s_reflect_fields, (usize)%llu, %s_reflect_enum_values };\n",
            type_name,
            type_name,
            (long long)type->size,
            (long long)type->align,
            (unsigned long long)meta_field_count(type),
            type_name,
            (unsigned long long)meta_enum_value_count(type),
            type_name);

    fclose(output);
    bdestroy(path);
    append_reflect_include(type_name);
}

static void generate_reflections(Directive *reflects, MetaTable *meta) {
    for (Directive *iter = reflects; iter; iter = iter->next) {
        MetaTypeRecord *type = meta_table_find(meta, bdata(iter->arg));
        if (!type) {
            printf("haikal: reflect: error: type not found: %s\n", bdata(iter->arg));
            printf("  directive: %s:%d:%d\n", bdata(iter->path), iter->line, iter->column + 1);
            printf("    %s\n", bdata(iter->source_line));
            printf("  note: type was not visible from selected translation units.\n");
            exit(1);
        }
        if (type->kind != TypeKind_struct && type->kind != TypeKind_union && type->kind != TypeKind_enum) {
            printf("haikal: reflect: error: only struct/union/enum reflection is supported for now: %s\n", bdata(iter->arg));
            printf("  directive: %s:%d:%d\n", bdata(iter->path), iter->line, iter->column + 1);
            printf("    %s\n", bdata(iter->source_line));
            exit(1);
        }
        generate_reflect_header(type);
    }
}

int main(int argc, char *argv[]) {
    double main_start = haikal_now_seconds();
    printf("haikal: codegen: initialize.\n");

    char *cwdstr = getCurrentWorkingDirectory();
    printf("haikal: main: cwd: %s\n", cwdstr);
    bool verbose = false;
    parse_args(argc, argv);
    printf("haikal: core: metapath: %s\n", metapath);
    printf("haikal: core: compile_db: %s\n", compile_db_path);
    if (mainpath) {
        printf("haikal: core: mainpath: %s\n", mainpath);
    }
    Directive *head = NULL;
    Directive *reflects = NULL;
    TypeTable types = {};
    MetaTable meta = {};
    double start = haikal_now_seconds();
    collect_from_compile_database(compile_db_path, &head, &reflects, &types, &meta);
    double collect_seconds = haikal_now_seconds() - start;
    if (haikal_cache_hit) {
        printf("haikal: CodeGen: Finalize.\n");
        return 0;
    }

    initialize_umbrella_files();

    double metainit_seconds = 0.0;
    double metagen_seconds = 0.0;
    if (head != NULL) {
        Directive *iter = head;
        start = haikal_now_seconds();
        while (iter != NULL) {
            printf("haikal: template detected: %s(%s)\n", bdata(iter->family), bdata(iter->arg));
            if (verbose) {
                printf("haikal: metainit: %s\n", bdata(iter->family));
            }
            metainit(bdata(iter->family), ".h");
            metainit(bdata(iter->family), ".c");
            iter = iter->next;
        }
        metainit_seconds = haikal_now_seconds() - start;
        if (verbose) {
            printf("haikal: metainit: complete.\n\n");
        }

        iter = head;
        start = haikal_now_seconds();
        while (iter != NULL) {
            const char *forwarddecl = forward_decl_for_type(&types, bdata(iter->arg));
            if (verbose) {
                printf("haikal: metagen: %s(%s) -> %s\n", bdata(iter->family), bdata(iter->arg), forwarddecl);
            }
            metagen(bdata(iter->family), bdata(iter->arg), (char *)forwarddecl, ".h", typestr);
            metagen(bdata(iter->family), bdata(iter->arg), (char *)forwarddecl, ".c", typestr);
            metageninternal(bdata(iter->family), bdata(iter->arg), (char *)forwarddecl, ".h", typestr);
            iter = iter->next;
        }
        metagen_seconds = haikal_now_seconds() - start;
        if (verbose) {
            printf("haikal: metagen: complete.\n\n");
        }
    }

    double reflect_seconds = 0.0;
    if (reflects != NULL) {
        for (Directive *iter = reflects; iter; iter = iter->next) {
            printf("haikal: reflect detected: %s\n", bdata(iter->arg));
        }
        start = haikal_now_seconds();
        generate_reflections(reflects, &meta);
        reflect_seconds = haikal_now_seconds() - start;
    }

    if (head == NULL && reflects == NULL) {
        printf("metagen: main: error: no template or reflect directives found.\n");
    }

    write_cache(haikal_cache_signature);
    if (haikal_profile_enabled()) {
        printf("haikal: profile: main collect=%.3fs metainit=%.3fs metagen=%.3fs reflect=%.3fs total=%.3fs\n",
               collect_seconds,
               metainit_seconds,
               metagen_seconds,
               reflect_seconds,
               haikal_now_seconds() - main_start);
    }
    printf("haikal: CodeGen: Finalize.\n");
    return 0;
}
