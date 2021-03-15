#include "cm_includes.hpp"
using namespace cfg_manipulator;

namespace cfg_manipulator {
    FILE *file;

    struct file_data {
        vector<pair<size_t, string>> lines;
        map<string, vector<string>> namespaces;
    } file_data;

    void print_error(CM_C_STRING message, size_t line_id) {
        if (line_id > 0)
            printf("\x1B[31mERROR\x1B[0m[cfg_manipulator]: Line %s\n",
                   string(to_string(line_id) + ", " + message).c_str());
        else
            printf("\x1B[31mERROR\x1B[0m[cfg_manipulator]: %s\n", message);

        exit(EXIT_FAILURE);
    }

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

    size_t get_characters_count(CM_C_STRING str, size_t begin, size_t end,
                                CM_C_CHAR character) {
        size_t output = 0;

        for (size_t i = begin; i < strlen(str) - (strlen(str) - end); i++) {
            if (str[i] == character)
                output++;
        }

        return output;
    }

    CM_C_STRING get_line_name(const pair<size_t, string> line) {
        CM_STRING output = default_string();
        bool _bool = false;

        for (size_t i = 0; i < strlen(line.second.c_str()); i++) {
            if (line.second.c_str()[i] == '=')
                break;
            if (line.second.c_str()[i] != ' ')
                _bool = true;
            if (_bool)
                output[strlen(output)] = line.second.c_str()[i];
        }

        for (size_t i = 0; i < strlen(output); i++) {
            if (output[(strlen(output) - 1) - i] != ' ')
                break;
            output[(strlen(output) - 1) - i] = 0;
        }

        return output;
    }

    void scan_line_for_errors(const pair<size_t, string> line) {
        CM_C_STRING _line = trim_characters(line.second.c_str(), 0,
                                            strlen(line.second.c_str()), ' ');
        size_t characters[2] = {0, 0};

        for (size_t i = 0; i < strlen(line.second.c_str()); i++) {
            if (line.second.c_str()[i] == '=' && characters[1] == 0)
                characters[0]++;
            if (line.second.c_str()[i] == '"')
                characters[1]++;
        }

        if (characters[0] != 1 || characters[1] != 2)
            print_error(
                "the line shoud be in the following style: line = \"value\".",
                line.first);

        if (get_characters_count(get_line_name(line), 0,
                                 strlen(line.second.c_str()), ' ') != 0)
            print_error("spaces are not allowed in line names.", line.first);

        for (size_t i = 0; i < strlen(_line); i++) {
            if (_line[i] == '=' && _line[i + 1] != '"')
                print_error("line contains undefined characters.", line.first);
        }

        if (_line[strlen(_line) - 1] != '"')
            print_error("line contains undefined characters.", line.first);
    }

    void parse_lines() {
        CM_STRING line = default_string(), namespace_name = default_string();
        size_t line_id = 0;
        bool _namespace = false;

        while (fgets(line, WCHAR_MAX, file)) {
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

            scan_line_for_errors(pair<size_t, string>(line_id, line));

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
} // namespace cfg_manipulator

cfg_file::cfg_file() {}

void cfg_file::open(CM_C_STRING file_path) {
    check_file_type(file_path);

    if (access(file_path, F_OK) == 0)
        file = fopen(file_path, "r");
    else
        print_error("No such file or directory.", 0);

    parse_lines();
}

bool cfg_file::is_open() { return file != NULL; }

void cfg_file::close() {
    fclose(file);
    file = NULL;
}

CM_C_STRING cfg_file::read(CM_C_STRING line_name) {
    pair<size_t, string> output;

    if (!is_open())
        print_error("File is not open.", 0);

    for (size_t i = 0; i < file_data.lines.size(); i++) {
        if (strcmp(get_line_name(file_data.lines.at(i)), line_name) == 0)
            output = file_data.lines.at(i);
    }

    if (strcmp(get_line_name(output), "") == 0)
        print_error(string("The given name could not be found: " +
                           string(line_name) + '.')
                        .c_str(),
                    0);

    return get_line_value(output.first, output.second.c_str());
}

CM_C_STRING cfg_file::read(CM_C_STRING namespace_name, CM_C_STRING line_name) {
    if (!is_open())
        print_error("File is not open.", 0);
}