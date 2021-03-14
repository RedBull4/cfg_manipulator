#include "cm_includes.hpp"
using namespace cfg_manipulator;

namespace cfg_manipulator {
    struct file_data {
        FILE *file;
        vector<pair<size_t, string>> lines;
        map<string, vector<string>> namespaces;
    } file_data;
} // namespace cfg_manipulator

void print_error(CM_C_STRING message, size_t line_id) {
    if (line_id > 0)
        printf("\x1B[31mERROR\x1B[0m[cfg_manipulator]: Line %s\n",
               string(to_string(line_id) + ", " + message).c_str());
    else
        printf("\x1B[31mERROR\x1B[0m[cfg_manipulator]: %s\n", message);

    exit(EXIT_FAILURE);
}

cfg_file::cfg_file() {}

char *default_string() { return (char *)malloc(WCHAR_MAX); }

CM_C_STRING trim_characters(CM_C_STRING str, size_t begin, size_t end,
                            CM_C_CHAR character) {
    CM_STRING output = default_string();

    for (size_t i = begin; i < end; i++) {
        if (str[i] != character)
            output[strlen(output)] = str[i];
    }

    return output;
}

bool is_namespace(CM_C_STRING line) {
    bool begin = false, end = false;

    for (size_t i = 0; i < strlen(line); i++) {
        if (line[i] == '[' && !end)
            begin = true;
        if (line[i] == ']')
            end = true;
    }

    if (begin && end)
        return true;
    else
        return false;
}

CM_C_STRING get_namespace_name(CM_C_STRING line) {
    CM_STRING output = default_string();
    bool _begin = false;

    for (size_t i = 0; i < strlen(line); i++) {
        if (line[i] == '[') {
            _begin = true;
            continue;
        }

        if (line[i] == ']')
            break;

        output[strlen(output)] = line[i];
    }

    return output;
}

void parse_lines() {
    CM_STRING line = default_string(), namespace_name = default_string();
    size_t line_id = 0;
    bool _namespace = false;

    while (fgets(line, WCHAR_MAX, file_data.file)) {
        line_id++;

        line[strcspn(line, "\n")] = 0;

        if (is_namespace(line)) {
            if (!_namespace)
                _namespace = true;

            strcpy(namespace_name, get_namespace_name(line));

            file_data.namespaces.insert(
                pair<string, vector<string>>(namespace_name, NULL));

            continue;
        }

        if (!_namespace)
            file_data.lines.push_back(pair<size_t, string>(line_id, line));
        else
            file_data.namespaces.at(namespace_name).push_back(line);
    }
}

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
            print_error("Unrecognized file type.", 0);
    }
}

void cfg_file::open(CM_C_STRING file_path) {
    check_file_type(file_path);

    if (access(file_path, F_OK) == 0)
        file_data.file = fopen(file_path, "r");
    else
        print_error("No such file or directory.", 0);

    parse_lines();
}

bool cfg_file::is_open() { return file_data.file != NULL; }

void cfg_file::close() {
    fclose(file_data.file);
    file_data.file = NULL;
}

CM_C_STRING get_line_name(size_t line_id, CM_C_STRING line) {
    CM_STRING output = default_string();
    bool _bool = false;

    for (size_t i = 0; i < strlen(line); i++) {
        if (line[i] == '=')
            break;
        if (line[i] != ' ')
            _bool = true;
        if (_bool)
            output[strlen(output)] = line[i];
    }

    for (size_t i = 0; i < strlen(output); i++) {
        if (output[(strlen(output) - 1) - i] != ' ')
            break;
        output[(strlen(output) - 1) - i] = 0;
    }

    for (size_t i = 0; i < strlen(output); i++) {
        if (output[i] == ' ') {
            print_error("spaces in line names are not allowed.", line_id);
        }
    }

    return output;
}

CM_C_STRING get_line_value(size_t line_id, CM_C_STRING line) {
    CM_STRING output = default_string();
    bool _begin = false;

    for (size_t i = 0; i < strlen(line); i++) {
        if (line[i] == '"' && !_begin) {
            _begin = true;
            continue;
        }
        if (line[i] == '"' && _begin)
            break;
        if (_begin)
            output[strlen(output)] = line[i];
    }

    return output;
}

CM_C_STRING cfg_file::read(CM_C_STRING line_name) {
    CM_STRING output = default_string();

    if (is_open()) {
        for (size_t i = 0; i < file_data.lines.size(); i++) {
            if (strcmp(get_line_name(file_data.lines.at(i).first,
                                     file_data.lines.at(i).second.c_str()),
                       line_name) == 0)
                strcpy(output,
                       get_line_value(file_data.lines.at(i).first,
                                      file_data.lines.at(i).second.c_str()));
        }
    } else
        print_error("File is not open.", 0);

    return output;
}