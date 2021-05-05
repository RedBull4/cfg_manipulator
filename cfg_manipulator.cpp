#include "cm_includes.hpp"
using namespace cfg_manipulator;

namespace cfg_manipulator {
	FILE* file;

	struct file_data {
		vector<pair<size_t, string>> lines;
		map<string, vector<pair<size_t, string>>> namespaces;
		CM_C_STRING file_path;
		bool errors = false;
	} file_data;

	void print_error(const string message, size_t line_id, bool _exit) {
		if (line_id > 0)
			printf("\x1B[31mERROR\x1B[0m[cfg_manipulator]: Line %s\n",
				   string(to_string(line_id) + ", " + message).c_str());
		else
			printf("\x1B[31mERROR\x1B[0m[cfg_manipulator]: %s\n",
				   message.c_str());

		if (_exit)
			exit(EXIT_FAILURE);
		else
			file_data.errors = true;
	}

	char* standard_string() { return (char*)malloc(WCHAR_MAX); }

	void append(char* a, const char& b) {
		a[strlen(a)] = b;
		a[strlen(a) + 1] = '\0';
	}

	CM_C_STRING trim_characters(const CM_C_STRING str, const size_t begin,
								const size_t end, CM_C_CHAR character) {
		CM_STRING output = standard_string();

		memset(output, 0, CHAR_MAX);

		for (size_t i = begin; i < end; i++) {
			if (str[i] != character) output[strlen(output)] = str[i];
		}

		return output;
	}

	bool is_namespace(const size_t line_id, const CM_C_STRING line) {
		bool begin = false, end = false;
		CM_C_STRING _line = trim_characters(line, 0, strlen(line), ' ');

		for (size_t i = 0; i < strlen(_line); i++) {
			if (_line[i] == '[' && !end) begin = true;
			if (_line[i] == ']') end = true;
		}

		if (begin && end) {
			if (_line[0] != '[' || _line[strlen(_line) - 1] != ']')
				print_error("namespace contains undefined character.", line_id,
							false);
		}

		return true ? begin && end : false;
	}

	CM_C_STRING get_namespace_name(size_t line_id, const CM_C_STRING line) {
		CM_STRING output = standard_string();
		bool _begin = false, _bool = false;

		memset(output, 0, CHAR_MAX);

		for (size_t i = 0; i < strlen(line); i++) {
			if (line[i] == '[') {
				_begin = true;
				continue;
			}

			if (_begin && line[i] != ' ') _bool = true;

			if (line[i] == ']') break;

			if (_bool) append(output, line[i]);
		}

		for (size_t i = 0; i < strlen(output); i++) {
			if (output[(strlen(output) - 1) - i] != ' ') break;
			output[(strlen(output) - 1) - i] = 0;
		}

		for (size_t i = 0; i < strlen(output); i++) {
			if (output[i] == ' ') {
				print_error("spaces are not allowed in namespace names.",
							line_id, false);
				break;
			}
		}

		return output;
	}

	size_t get_characters_count(const CM_C_STRING str, const size_t begin,
								const size_t end, CM_C_CHAR character) {
		size_t output = 0;

		for (size_t i = begin; i < end; i++) {
			if (str[i] == character) output++;
		}

		return output;
	}

	CM_C_STRING get_line_name(const CM_C_STRING line) {
		CM_STRING _line = standard_string(), output = standard_string();
		bool _bool = false;

		memset(_line, 0, CHAR_MAX);
		memset(output, 0, CHAR_MAX);

		for (size_t i = 0; i < strlen(line); i++) {
			if (line[i] == '=') {
				_bool = false;
				break;
			}
			if (line[i] != ' ') _bool = true;
			if (_bool) append(_line, line[i]);
		}

		for (size_t i = 0; i < strlen(_line); i++) {
			if (_line[(strlen(_line) - 1) - i] != ' ') _bool = true;
			if (_bool)
				output[(strlen(_line) - 1) - i]
					= _line[(strlen(_line) - 1) - i];
		}

		return output;
	}

	void scan_line_for_errors(const size_t line_id, const CM_C_STRING line) {
		CM_C_STRING _line = trim_characters(line, 0, strlen(line), ' ');
		size_t characters[2] = {0, 0}, first_quote_id = 0;

		for (size_t i = 0; i < strlen(_line); i++) {
			if (_line[i] == '=' && characters[1] == 0) characters[0]++;
			if (_line[i] == '"') {
				characters[1]++;

				if (first_quote_id == 0) first_quote_id = i;
			}
		}

		if (strcmp(_line, "") != 0
			&& get_characters_count(_line, 0, strlen(_line), ' ')
				   != strlen(_line)) {
			if (characters[0] != 1 || characters[1] != 2)
				print_error(
					"the line shoud be in the following style: line = "
					"\"value\".",
					line_id, false);

			if (get_characters_count(get_line_name(line), 0, strlen(line), ' ')
				!= 0)
				print_error("spaces are not allowed in line names.", line_id,
							false);

			if (_line[first_quote_id - 1] != '='
				|| _line[strlen(_line) - 1] != '"') {
				print_error("line contains undefined characters.", line_id,
							false);
			}
		}
	}

	void trim_comment(CM_STRING& line) {
		CM_STRING output = standard_string();
		bool quote = false;

		memset(output, 0, CHAR_MAX);

		for (size_t i = 0; i < strlen(line); i++) {
			if (!quote) {
				if (line[i] == '#') break;
			}

			output[strlen(output)] = line[i];

			if (line[i] == '"' && !quote) {
				quote = true;
				continue;
			} else if (line[i] == '"' && quote) {
				quote = false;
				continue;
			}
		}

		strcpy(line, output);
	}

	void parse_file() {
		CM_STRING line = standard_string(), namespace_name = standard_string();
		size_t line_id = 0;
		bool _namespace = false;

		memset(line, 0, CHAR_MAX);
		memset(namespace_name, 0, CHAR_MAX);

		file_data.lines.clear();
		file_data.namespaces.clear();

		while (fgets(line, WCHAR_MAX, file)) {
			line_id++;

			line[strcspn(line, "\n")] = 0;

			trim_comment(line);

			if (is_namespace(line_id, line)) {
				if (!_namespace) _namespace = true;

				strcpy(namespace_name, get_namespace_name(line_id, line));

				file_data.namespaces.insert(
					pair<string, vector<pair<size_t, string>>>(namespace_name,
															   NULL));

				continue;
			}

			scan_line_for_errors(line_id, line);

			if (!_namespace)
				file_data.lines.push_back(pair<size_t, string>(line_id, line));
			else
				file_data.namespaces.at(namespace_name)
					.push_back(pair<size_t, string>(line_id, line));
		}
	}

	CM_C_STRING get_file_type(const CM_C_STRING file_path) {
		CM_STRING output = standard_string();

		memset(output, 0, CHAR_MAX);

		for (size_t i = 0; i < strlen(file_path) - 1; i++) {
			append(output, file_path[(strlen(file_path) - 1) - i]);

			if (file_path[(strlen(file_path) - 1) - i] == '.') break;
		}

		reverse(output, output + strlen(output));

		return output;
	}

	void check_file_type(const CM_C_STRING file_path) {
		CM_STRING file_type = standard_string();

		strcpy(file_type, get_file_type(file_path));

		for (size_t i = 0; i < CM_ARRAY_SIZE(allowed_file_types); i++) {
			if (strcmp(file_type, allowed_file_types[i]) == 0)
				break;
			else {
				CM_LOG(file_type);

				print_error("Unrecognized file type.", 0, true);
			}
		}
	}

	CM_C_STRING get_line_value(const CM_C_STRING line) {
		CM_STRING output = standard_string();
		bool _begin = false;

		memset(output, 0, CHAR_MAX);

		for (size_t i = 0; i < strlen(line); i++) {
			if (line[i] == '"' && !_begin) {
				_begin = true;
				continue;
			}
			if (line[i] == '"' && _begin) break;
			if (_begin) append(output, line[i]);
		}

		return output;
	}
}  // namespace cfg_manipulator

cfg_file::cfg_file() {}

void cfg_file::open(const CM_C_STRING file_path) {
	check_file_type(file_path);

	if (access(file_path, F_OK) == 0) {
		file = fopen(file_path, "r");
		file_data.file_path = file_path;
	} else
		print_error("No such file or directory \""s + file_path + "\".", 0,
					true);

	parse_file();

	fclose(file);

	if (file_data.errors) exit(EXIT_FAILURE);
}

bool cfg_file::is_open() { return file != NULL; }

void cfg_file::close() {
	fclose(file);

	file = NULL;

	file_data.lines.clear();
	file_data.namespaces.clear();
}

CM_C_STRING cfg_file::read(const CM_C_STRING line_name) {
	CM_STRING output = standard_string();

	if (!is_open()) print_error("File is not open.", 0, true);

	for (size_t i = 0; i < file_data.lines.size(); i++) {
		if (strcmp(get_line_name(file_data.lines.at(i).second.c_str()),
				   line_name)
			== 0)
			strcpy(output, file_data.lines.at(i).second.c_str());
	}

	if (strcmp(get_line_name(output), "") == 0)
		print_error("Unable to find a line named \""s + line_name + "\".", 0,
					true);

	return get_line_value(output);
}

CM_C_STRING cfg_file::read(const CM_C_STRING namespace_name,
						   const CM_C_STRING line_name) {
	CM_STRING output = standard_string();

	if (!is_open()) print_error("File is not open.", 0, true);

	if (file_data.namespaces.count(namespace_name) != 0) {
		for (pair<size_t, string> line :
			 file_data.namespaces.at(namespace_name)) {
			if (strcmp(get_line_name(line.second.c_str()), line_name) == 0)
				strcpy(output, line.second.c_str());
		}

		if (strcmp(get_line_name(output), "") == 0)
			print_error("The line named \""s + line_name
							+ "\" couldn't be found in the namespace \""
							+ namespace_name + "\".",
						0, true);
	} else
		print_error("Unable to find namespace \""s + namespace_name + "\".", 0,
					true);

	return get_line_value(output);
}

void _void(const size_t& line_id, const CM_C_STRING& line) {
	vector<string> lines;
	CM_STRING buffer = (char*)malloc(WCHAR_MAX);

	file = fopen(file_data.file_path, "r");

	while (fgets(buffer, WCHAR_MAX, file)) {
		if (buffer[strlen(buffer) - 1] == '\n') buffer[strlen(buffer) - 1] = 0;

		lines.push_back(buffer);
	}

	file = fopen(file_data.file_path, "w");

	lines.at(line_id - 1) = line;

	for (size_t i = 0; i < lines.size(); i++)
		fprintf(file, "%s\n", lines.at(i).c_str());

	fclose(file);
}

CM_C_STRING change_line_value(size_t line_id, const CM_C_STRING& line,
							  const CM_C_STRING& value) {
	CM_STRING output = standard_string();
	char first[WCHAR_MAX], second[WCHAR_MAX];
	size_t begin_quote_id = 0;
	bool _bool = false;

	for (size_t i = 0; i < strlen(line); i++) {
		append(first, line[i]);
		if (line[i] == '"') {
			begin_quote_id = i + 1;
			break;
		}
	}

	for (size_t i = begin_quote_id; i < strlen(line); i++) {
		if (line[i] == '"') _bool = true;
		if (_bool) append(second, line[i]);
	}

	strcpy(output,
		   string(string(first) + string(value) + string(second)).c_str());

	memset(first, 0, WCHAR_MAX);
	memset(second, 0, WCHAR_MAX);

#if defined(WIN32) || defined(_WIN32) \
	|| defined(__WIN32) && !defined(__CYGWIN__)
	_void(line_id, output);
#else
	system(string("sed -i '" + to_string(line_id) + " s/" + line + "/" + output
				  + "/g' "s + file_data.file_path)
			   .c_str());
#endif

	return output;
}

void cfg_file::change_value(const CM_C_STRING line_name,
							const CM_C_STRING value) {
	bool _bool = false;

	if (!is_open()) print_error("File is not open.", 0, true);

	for (pair<size_t, string>& line : file_data.lines) {
		if (strcmp(line_name, get_line_name(line.second.c_str())) == 0) {
			line.second
				= change_line_value(line.first, line.second.c_str(), value);
			_bool = true;
		}
	}

	if (!_bool)
		print_error(
			"Unable to change the value of the line, because the line \""s
				+ line_name + "\" was not found in the file. ",
			0, true);
}

void cfg_file::change_value(const CM_C_STRING namespace_name,
							const CM_C_STRING line_name,
							const CM_C_STRING value) {
	bool _bool = false;

	if (!is_open()) print_error("File is not open.", 0, true);

	if (file_data.namespaces.count(namespace_name) != 0) {
		for (pair<size_t, string>& line :
			 file_data.namespaces.at(namespace_name)) {
			if (strcmp(line_name, get_line_name(line.second.c_str())) == 0) {
				line.second
					= change_line_value(line.first, line.second.c_str(), value);
				_bool = true;
			}
		}
	} else
		print_error(
			"Unable to change the value of the line, because "
			"the namespace \""s
				+ namespace_name + "\" was not found in the file.",
			0, true);

	if (!_bool)
		print_error(
			"Unable to change the value of the line, because a line named \""s
				+ line_name + "\" could not be found in the namespace \""
				+ namespace_name + "\".",
			0, true);
}