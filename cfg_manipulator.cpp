#include "cm_includes.hpp"
using namespace cfg_manipulator;

namespace cfg_manipulator {
    struct file_data {
        FILE *file;
    } file_data;
} // namespace cfg_manipulator

void print_error(CM_C_STRING message, size_t line_id, bool _exit) {
    if (line_id > 0)
        printf(string("\x1B[31mERROR\x1B[0m[cfg_manipulator]: Line  %s\n" +
                      to_string(line_id) + ", ")
                   .c_str(),
               message);
    else
        printf("\x1B[31mERROR\x1B[0m[cfg_manipulator]: %s\n", message);

    if (_exit)
        exit(EXIT_FAILURE);
}

cfg_file::cfg_file() {}

char *default_string() { return (char *)malloc(WCHAR_MAX); }

/*CM_C_STRING trim_characters(CM_C_STRING str, size_t begin, size_t end,
                            CM_C_CHAR character) {
    CM_STRING output = default_string();

    for (size_t i = begin; i < end; i++) {
        if (str[i] != character)
            output[strlen(output)] = str[i];
    }

    return output;
}*/

CM_C_STRING get_file_type(CM_C_STRING file_path) {
    CM_STRING output = default_string();

    for (size_t i = 0; i < strlen(file_path); i++) {
        output[i] = file_path[(strlen(file_path) - 1) - i];

        if (file_path[(strlen(file_path) - 1) - i] == '.')
            break;
    }

    reverse(output, output + strlen(output));

    return output;
}

void check_file_type(CM_C_STRING file_path) {
    CM_STRING file_type = default_string();

    strcpy(file_type, get_file_type(file_path));

    for (size_t i = 0; i < CM_ARRAY_SIZE(allowed_file_types); i++) {
        if (strcmp(file_type, allowed_file_types[i]) == 0)
            break;
        else
            print_error("Unrecognized file type.", 0, true);
    }
}

void cfg_file::open(CM_C_STRING file_path) {
    check_file_type(file_path);

    if (access(file_path, F_OK) == 0)
        file_data.file = fopen(file_path, "r");
    else
        print_error("No such file or directory.", 0, true);
}

bool cfg_file::is_open() { return file_data.file != NULL; }

void cfg_file::close() {
    fclose(file_data.file);
    file_data.file = NULL;
}