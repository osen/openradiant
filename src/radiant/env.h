#include <string>
#include <vector>

void Env_Init(int argc, char* argv[]);
void Env_Reset();

std::string Env_ProjectRoot();
std::string Env_ProjectName();
std::vector<std::string>& Env_GetDirs();
void Env_ResetMap(const std::string& path);
void Env_ScanTextures();
std::string Env_BinDirectory();

const unsigned char* Env_ReadFile(const char* path, size_t* size);
int Env_CopyFile(const char* filename, void** bufferptr);

typedef void (*FuncPtr)();
FuncPtr Env_Extension(const std::string& entry);
