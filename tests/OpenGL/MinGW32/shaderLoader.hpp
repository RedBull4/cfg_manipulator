#ifndef shaderLoader_hpp
#define shaderLoader_hpp

class shaderLoader {
  public:
	static unsigned int loadShaders(const char *vertexShaderPath,
									const char *fragmentShaderPath);
};

#endif	// shaderLoader_hpp