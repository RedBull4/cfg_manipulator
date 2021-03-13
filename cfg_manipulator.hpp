#ifndef cfg_manipulator_hpp
#define cfg_manipulator_hpp

#define CM_ARRAY_SIZE(x) sizeof(x) / sizeof(*x)
#define CM_LOG(x) printf("CM_LOG: %s\n", x);

typedef const char CM_C_CHAR;
typedef char *CM_STRING;
typedef CM_C_CHAR *CM_C_STRING;

namespace cfg_manipulator {
    CM_C_STRING const allowed_file_types[] = {".cfg", ".conf", ".config"};

    class cfg_file {
      public:
        cfg_file();

        void open(CM_C_STRING file_path);

        bool is_open();

        void close();
    };
} // namespace cfg_manipulator

#endif // cfg_manipulator_hpp