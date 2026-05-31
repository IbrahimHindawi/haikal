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
#else
#   error "Unknown Platform"
#endif

#include "core.h"
#include "bstring/bstring/bstrlib.h"
#include <tree_sitter/api.h>
#include <ctype.h>
#include <errno.h>

extern const TSLanguage *tree_sitter_c(void);

const char *metapath = NULL;
const char *mainpath = NULL;
const char *typestr = "TYPE";

structdef(Node_bstring) {
    bstring data;
    i32 foundat;
    bstring metaname;
    bstring metaarg;
    Node_bstring *next;
};

Node_bstring *Node_bstring_create(bstring input, i32 foundat, bstring metaname, bstring metaarg) {
    Node_bstring *result = malloc(sizeof(Node_bstring));
    if (result == NULL) { exit(-1);}
    result->data = input;
    result->next = NULL;
    result->foundat = foundat;
    result->metaname = metaname;
    result->metaarg = metaarg;
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

void Node_bstring_destroy(Node_bstring *node) {
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
void metainit(char *metaname, char *ext) {
    // char *cwdstr = getCurrentWorkingDirectory();
    // bstring typecorepath = bfromcstr(cwdstr);
    bstring typecorepath = bfromcstr(metapath);
    bcatcstr(typecorepath, "gen/");
    bcatcstr(typecorepath, metaname);
    bcatcstr(typecorepath, ext);
    // printf("typecorepath: %s\n", bdata(typecorepath));
    FILE *output = NULL;
    if (NULL != (output = fopen(bdata(typecorepath), "w"))) {
        bstring result = bfromcstr("#pragma once\n");
        fputs(bdatae(result, "NULL"), output);
        bdestroy(result);
        fclose(output);
    } else {
        printf("metainit::Unable to open type core gen file: '%s' for initiation.\n", bdata(typecorepath));
        exit(-1);
    }
}

void metareplace(bstring templatepath, const char *metaarg, bstring forwarddecl, const char *typestr, bstring outpath) {
    bstring bmetaarg = bfromcstr(metaarg);
    FILE *input = NULL;
    FILE *output = NULL;
    struct tagbstring postfix = bsStatic("\n");
    bstring btypestr = bfromcstr(typestr);
    bstring stubinclude = bfromcstr("#include \"");
    bcatcstr(stubinclude, typestr);
    bcatcstr(stubinclude, ".h\"");
    if (NULL != (input = fopen(bdata(templatepath), "r"))) {
        bstring filestringdata = bread((bNread) fread, input);
        struct bstrList *lines;
        fclose(input);
        if (NULL != (lines = bsplit(filestringdata, '\n'))) {
            for (int i = 0; i < lines->qty; ++i) {
                bfindreplace(lines->entry[i], stubinclude, forwarddecl, 0);
                bfindreplace(lines->entry[i], btypestr, bmetaarg, 0);
                binsert(lines->entry[i], blength(lines->entry[i]), &postfix, '?');
                // printf("%04d: %s\n", i, bdatae(lines->entry[i], "NULL"));
            }
            if (NULL != (output = fopen(bdata(outpath), "w"))) {
                for (int i = 0; i < lines->qty; ++i) {
                    fputs(bdatae(lines->entry[i], "NULL"), output);
                }
                fclose(output);
            }
            else {
                printf("haikal::metagen::error::Failed to open file: %s.\n", bdata(outpath));
            }
            bstrListDestroy(lines);
        }
        bdestroy(filestringdata);
    } else {
        printf("haikal::metagen::error::Unable to open type core file: %s.\n", bdata(templatepath));
    }
}

/*
 * generate types and `#include "hk<metaname>_<metaarg>.h"` append to main header
 * genpath/metaname.h <---append--- genpath/metaname_metaarg.h
 */
void metagen(char *metaname, char *metaarg, char *forwarddeclparam, char *ext, const char *typestr) {
    // FILE *input = NULL;
    FILE *output = NULL;
    bstring forwarddecl = bfromcstr(forwarddeclparam);
    bcatcstr(forwarddecl, "(");
    bcatcstr(forwarddecl, metaarg);
    bcatcstr(forwarddecl, ");");

    bstring templatepath = bfromcstr(metapath);
    bcatcstr(templatepath, metaname);
    bcatcstr(templatepath, "_");
    bcatcstr(templatepath, typestr);
    bcatcstr(templatepath, ext);
    // printf("templatepath: %s\n", bdata(templatepath));

    bstring outpath = bfromcstr(metapath);
    bcatcstr(outpath, "gen/");
    bcatcstr(outpath, metaname);
    bcatcstr(outpath, "_");
    bcatcstr(outpath, metaarg);
    bcatcstr(outpath, ext);
    // printf("outpath: %s\n", bdata(outpath));

    metareplace(templatepath, metaarg, forwarddecl, typestr, outpath);
    bdestroy(templatepath);
    bdestroy(outpath);

    bstring typecorepathtarget = bfromcstr(metapath);
    bcatcstr(typecorepathtarget, "gen/");
    bcatcstr(typecorepathtarget, metaname);
    bcatcstr(typecorepathtarget, ext);
    // printf("typecorepathtarget: %s\n", bdata(typecorepathtarget));
    if (NULL != (output = fopen(bdata(typecorepathtarget), "a"))) {
        bstring result = bfromcstr("#include \"");
        bcatcstr(result, metaname);
        bcatcstr(result, "_");
        bcatcstr(result, metaarg);
        bcatcstr(result, ext);
        bcatcstr(result, "\"\n");
        // printf("final header name: %s\n", bdata(result));
        fputs(bdatae(result, "NULL"), output);
        bdestroy(result);
        fclose(output);
    } else {
        printf("metainit::Unable to open type core file for initiation.");
    }
    bdestroy(typecorepathtarget);
    return;
}

void metageninternal(char *metaname, char *metaarg, char *forwarddeclparam, char *ext, const char *typestr) {
    bstring forwarddecl = bfromcstr(forwarddeclparam);
    bcatcstr(forwarddecl, "(");
    bcatcstr(forwarddecl, metaarg);
    bcatcstr(forwarddecl, ");");

    bstring templatepathinternal = bfromcstr(metapath);
    bcatcstr(templatepathinternal, metaname);
    bcatcstr(templatepathinternal, "_");
    bcatcstr(templatepathinternal, typestr);
    bcatcstr(templatepathinternal, "_internal");
    bcatcstr(templatepathinternal, ext);
    // printf("templatepathinternal: %s\n", bdata(templatepathinternal));

    bstring outpathinternal = bfromcstr(metapath);
    bcatcstr(outpathinternal, "gen/");
    bcatcstr(outpathinternal, metaname);
    bcatcstr(outpathinternal, "_");
    bcatcstr(outpathinternal, metaarg);
    bcatcstr(outpathinternal, "_internal");
    bcatcstr(outpathinternal, ext);
    // printf("outpathinternal: %s\n", bdata(outpathinternal));

    metareplace(templatepathinternal, metaarg, forwarddecl, typestr, outpathinternal);
    bdestroy(templatepathinternal);
    bdestroy(outpathinternal);
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
        metainit(metaname, ".h");
        metainit(metaname, ".c");
    }
    for (int i = 0; i < coretypeslen; ++i) {
        metagen(metaname, coretypes[i], "structdecl", ".h", typestr);
        metagen(metaname, coretypes[i], "structdecl", ".c", typestr);
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
    };
    usize count = sizeofarray(families);
    for (usize i = 0; i < count; i += 1) {
        metainit(families[i], ".h");
        metainit(families[i], ".c");
    }
}

static void usage(void) {
    printf("usage: haikal.exe --entry <path> --meta <path>\n");
}

static bool streq(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

static char *dup_range(const char *start, usize length) {
    char *result = malloc(length + 1);
    if (!result) {
        printf("haikal::error::malloc failed.\n");
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
        printf("haikal::error::malloc failed.\n");
        exit(1);
    }
    memcpy(result, path, length);
    result[length] = '/';
    result[length + 1] = 0;
    return result;
}

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
    printf("haikal::error::failed to create directory: %s\n", path);
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
        } else if ((streq(argv[i], "--meta") || streq(argv[i], "-m")) && i + 1 < argc) {
            metapath = argv[++i];
        } else if (streq(argv[i], "--help") || streq(argv[i], "-h")) {
            usage();
            exit(0);
        } else {
            printf("haikal::error::unknown or incomplete argument: %s\n", argv[i]);
            usage();
            exit(1);
        }
    }

    if (!mainpath || !metapath) {
        usage();
        exit(1);
    }
    metapath = with_trailing_slash(metapath);
    ensure_gen_directory();
    initialize_umbrella_files();
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

static char *join_path(const char *dir, const char *name) {
    usize dir_len = strlen(dir);
    usize name_len = strlen(name);
    bool needs_sep = dir_len > 0 && dir[dir_len - 1] != '/' && dir[dir_len - 1] != '\\';
    char *result = malloc(dir_len + needs_sep + name_len + 1);
    if (!result) {
        printf("haikal::error::malloc failed.\n");
        exit(1);
    }
    memcpy(result, dir, dir_len);
    if (needs_sep) {
        result[dir_len] = '/';
    }
    memcpy(result + dir_len + needs_sep, name, name_len);
    result[dir_len + needs_sep + name_len] = 0;
    return result;
}

static void append_file_and_local_includes(bstring out, const char *path, int depth) {
    if (depth > 8) {
        return;
    }

    bstring file = read_file_bstring(path);
    if (!file) {
        return;
    }

    bcatcstr(out, "\n");
    bconcat(out, file);
    bcatcstr(out, "\n");

    char *dir = dirname_dup(path);
    struct bstrList *lines = bsplit(file, '\n');
    if (lines) {
        for (int i = 0; i < lines->qty; i += 1) {
            const char *line = bdata(lines->entry[i]);
            const char *include = strstr(line, "#include");
            if (!include) {
                continue;
            }

            const char *first_quote = strchr(include, '"');
            if (!first_quote) {
                continue;
            }
            const char *second_quote = strchr(first_quote + 1, '"');
            if (!second_quote) {
                continue;
            }

            char *include_name = dup_range(first_quote + 1, (usize)(second_quote - first_quote - 1));
            char *include_path = join_path(dir, include_name);
            append_file_and_local_includes(out, include_path, depth + 1);
            free(include_path);
            free(include_name);
        }
        bstrListDestroy(lines);
    }

    free(dir);
    bdestroy(file);
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
        printf("haikal::error::malloc failed.\n");
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

static bool node_is_type(TSNode node, const char *type) {
    return strcmp(ts_node_type(node), type) == 0;
}

static char *node_text_dup(bstring source, TSNode node) {
    uint32_t start = ts_node_start_byte(node);
    uint32_t end = ts_node_end_byte(node);
    if (end < start || end > (uint32_t)blength(source)) {
        return NULL;
    }
    return dup_range((const char *)bdata(source) + start, end - start);
}

static void record_type_identifier_descendants(TypeTable *table, bstring source, TSNode node, TypeKind kind) {
    if (node_is_type(node, "type_identifier")) {
        char *name = node_text_dup(source, node);
        type_table_add(table, name, kind);
        free(name);
    }

    uint32_t count = ts_node_named_child_count(node);
    for (uint32_t i = 0; i < count; i += 1) {
        record_type_identifier_descendants(table, source, ts_node_named_child(node, i), kind);
    }
}

static TSNode unwrap_type_specifier(TSNode node) {
    if (!node_is_type(node, "type_specifier")) {
        return node;
    }

    uint32_t count = ts_node_named_child_count(node);
    for (uint32_t i = 0; i < count; i += 1) {
        TSNode child = ts_node_named_child(node, i);
        if (node_is_type(child, "struct_specifier") ||
            node_is_type(child, "union_specifier") ||
            node_is_type(child, "enum_specifier")) {
            return child;
        }
    }
    return node;
}

static TypeKind kind_from_specifier(TSNode node) {
    node = unwrap_type_specifier(node);
    if (node_is_type(node, "struct_specifier")) {
        return TypeKind_struct;
    }
    if (node_is_type(node, "union_specifier")) {
        return TypeKind_union;
    }
    if (node_is_type(node, "enum_specifier")) {
        return TypeKind_enum;
    }
    return TypeKind_unknown;
}

static void record_specifier_name(TypeTable *table, bstring source, TSNode node, TypeKind kind) {
    TSNode name = ts_node_child_by_field_name(node, "name", 4);
    if (!ts_node_is_null(name)) {
        char *text = node_text_dup(source, name);
        type_table_add(table, text, kind);
        free(text);
    }
}

static void collect_type_table_walk(TypeTable *table, bstring source, TSNode node) {
    TypeKind spec_kind = kind_from_specifier(node);
    if (spec_kind != TypeKind_unknown) {
        record_specifier_name(table, source, unwrap_type_specifier(node), spec_kind);
    }

    if (node_is_type(node, "type_definition")) {
        TSNode type = ts_node_child_by_field_name(node, "type", 4);
        TypeKind typedef_kind = kind_from_specifier(type);
        if (typedef_kind != TypeKind_unknown) {
            record_specifier_name(table, source, unwrap_type_specifier(type), typedef_kind);

            uint32_t count = ts_node_named_child_count(node);
            for (uint32_t i = 0; i < count; i += 1) {
                TSNode child = ts_node_named_child(node, i);
                if (!node_is_type(child, "type_specifier") &&
                    !node_is_type(child, "struct_specifier") &&
                    !node_is_type(child, "union_specifier") &&
                    !node_is_type(child, "enum_specifier") &&
                    !node_is_type(child, "field_declaration_list") &&
                    !node_is_type(child, "enumerator_list") &&
                    !node_is_type(child, "type_qualifier") &&
                    !node_is_type(child, "attribute_specifier")) {
                    record_type_identifier_descendants(table, source, child, typedef_kind);
                }
            }
        }
    }

    uint32_t count = ts_node_named_child_count(node);
    for (uint32_t i = 0; i < count; i += 1) {
        collect_type_table_walk(table, source, ts_node_named_child(node, i));
    }
}

static void collect_type_table(TypeTable *table, bstring source) {
    type_table_add_primitives(table);

    TSParser *parser = ts_parser_new();
    if (!parser || !ts_parser_set_language(parser, tree_sitter_c())) {
        printf("haikal::tree-sitter::error::failed to initialize C parser.\n");
        exit(1);
    }

    TSTree *tree = ts_parser_parse_string(parser, NULL, bdata(source), (uint32_t)blength(source));
    if (!tree) {
        printf("haikal::tree-sitter::error::failed to parse source.\n");
        exit(1);
    }

    collect_type_table_walk(table, source, ts_tree_root_node(tree));

    ts_tree_delete(tree);
    ts_parser_delete(parser);
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

static void append_annotation(Node_bstring **head, bstring line, i32 foundat, bstring metaname, bstring metaarg) {
    Node_bstring *node = Node_bstring_create(line, foundat, metaname, metaarg);
    if (*head == NULL) {
        *head = node;
        return;
    }

    Node_bstring *iter = *head;
    while (iter->next != NULL) {
        iter = iter->next;
    }
    iter->next = node;
}

static void parse_template_inner(const char *inner, bstring *metaname, bstring *metaarg) {
    const char *open = strchr(inner, '(');
    const char *comma = strchr(inner, ',');

    if (open && (!comma || open < comma)) {
        const char *close = strrchr(inner, ')');
        if (!close || close < open) {
            printf("haikal::template::error::malformed directive: %s\n", inner);
            exit(1);
        }

        char *name = trim_dup(inner, (usize)(open - inner));
        char *arg = trim_dup(open + 1, (usize)(close - open - 1));
        *metaname = bfromcstr(name);
        *metaarg = bfromcstr(arg);
        free(name);
        free(arg);
        return;
    }

    if (comma) {
        char *name = trim_dup(inner, (usize)(comma - inner));
        char *arg = trim_dup(comma + 1, strlen(comma + 1));
        *metaname = bfromcstr(name);
        *metaarg = bfromcstr(arg);
        free(name);
        free(arg);
        return;
    }

    printf("haikal::template::error::expected template(Vec(i32)) or template(Vec, i32): %s\n", inner);
    exit(1);
}

static void collect_templates(bstring source, Node_bstring **head) {
    struct bstrList *lines = bsplit(source, '\n');
    if (!lines) {
        printf("haikal::template::error::line read error.\n");
        exit(1);
    }

    for (int i = 0; i < lines->qty; i += 1) {
        const char *line = bdata(lines->entry[i]);
        const char *cursor = line;
        while ((cursor = strstr(cursor, "template(")) != NULL) {
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
                printf("haikal::template::error::unbalanced directive: %s\n", line);
                exit(1);
            }

            char *inner_text = dup_range(inner, (usize)((end - 1) - inner));
            bstring metaname = NULL;
            bstring metaarg = NULL;
            parse_template_inner(inner_text, &metaname, &metaarg);
            printf("haikal::template detected: %s(%s)\n", bdata(metaname), bdata(metaarg));
            append_annotation(head, lines->entry[i], (i32)(cursor - line), metaname, metaarg);
            free(inner_text);
            cursor = end;
        }
    }

    /* Keep line bstrings alive through generation; the bstrList shell is not needed. */
    free(lines->entry);
    free(lines);
}

int main(int argc, char *argv[]) {
    printf("haikal::codegen::initialize.\n");

    char *cwdstr = getCurrentWorkingDirectory();
    printf("haikal::main::cwd::%s\n", cwdstr);
    bool verbose = false;
    parse_args(argc, argv);
    printf("haikal::core::metapath::%s\n", metapath);
    printf("haikal::core::mainpath::%s\n", mainpath);

    // TODO(ibrahim): parse files with main recursively to find hktags
    bstring cpath;
    // cpath = bfromcstr(cwdstr);
    // bconchar(cpath, '/');
    cpath = bfromcstr("");
    bconcat(cpath, cstr2bstr(mainpath));
    printf("haikal::main::cpath::%s\n", bdata(cpath));
    Node_bstring *head = NULL;

    bstring entry_source = read_file_bstring(bdata(cpath));
    if (!entry_source) {
        printf("metagen::main::error::Unable to open entry file: %s.\n", bdata(cpath));
        exit(1);
    }
    collect_templates(entry_source, &head);

    bstring source = bfromcstr("");
    append_file_and_local_includes(source, bdata(cpath), 0);
    TypeTable types = {};
    collect_type_table(&types, source);

    if (head != NULL) {
        Node_bstring *iter = head;
        while (iter != NULL) {
            if (verbose) {
                printf("haikal::metainit::%s\n", bdata(iter->metaname));
            }
            metainit(bdata(iter->metaname), ".h");
            metainit(bdata(iter->metaname), ".c");
            iter = iter->next;
        }
        if (verbose) {
            printf("haikal::metainit::complete.\n\n");
        }

        iter = head;
        while (iter != NULL) {
            const char *forwarddecl = forward_decl_for_type(&types, bdata(iter->metaarg));
            if (verbose) {
                printf("haikal::metagen::%s(%s) -> %s\n", bdata(iter->metaname), bdata(iter->metaarg), forwarddecl);
            }
            metagen(bdata(iter->metaname), bdata(iter->metaarg), (char *)forwarddecl, ".h", typestr);
            metagen(bdata(iter->metaname), bdata(iter->metaarg), (char *)forwarddecl, ".c", typestr);
            metageninternal(bdata(iter->metaname), bdata(iter->metaarg), (char *)forwarddecl, ".h", typestr);
            iter = iter->next;
        }
        if (verbose) {
            printf("haikal::metagen::complete.\n\n");
        }
    } else {
        printf("metagen::main::error::no template directives found.\n");
    }

    printf("haikal::CodeGen::Finalize.\n");
    return 0;
}
