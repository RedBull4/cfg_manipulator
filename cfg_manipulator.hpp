#ifndef cfg_manipulator_hpp
#define cfg_manipulator_hpp

#include <stdio.h>

#define CM_ARRAY_SIZE(x) sizeof(x) / sizeof(*x)
#define CM_LOG(x) printf("CM_LOG: %s\n", x);

#if defined(WIN32) || defined(_WIN32) \
	|| defined(__WIN32) && !defined(__CYGWIN__)
	#define F_OK 0
#endif

typedef const char CM_C_CHAR;
typedef char *CM_STRING;
typedef CM_C_CHAR *CM_C_STRING;

namespace cfg_manipulator {
	CM_C_STRING const allowed_file_types[] = {".cfg", ".conf", ".config"};

	class cfg_file {
	  public:
		cfg_file();

		void open(const CM_C_STRING &file_path);

		bool is_open();

		void close();

		CM_C_STRING read(const CM_C_STRING &line_name);

		CM_C_STRING read(const CM_C_STRING &namespace_name,
						 const CM_C_STRING &line_name);

		void change_value(const CM_C_STRING &line_name,
						  const CM_C_STRING &value);

		void change_value(const CM_C_STRING &namespace_name,
						  const CM_C_STRING &line_name,
						  const CM_C_STRING &value);
	};
}  // namespace cfg_manipulator

#endif	// cfg_manipulator_hpp