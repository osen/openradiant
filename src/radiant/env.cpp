#include "env.h"

#include "qe3.h"

#ifdef _WIN32
	#include <dirent_win32.h>
	#include <windows.h>
#else
	#include <dirent.h>
#endif

static std::string projectName;
static std::string projectRoot;
static std::vector<std::string> dirs;
static std::string exePath;
static std::string binDir;

std::string ExtractDirname(const std::string& path)
{
	std::string tmp;
	bool found = false;

	for (int i = path.length() - 1; i >= 0; i--)
	{
		char c = path.at(i);
		tmp = tmp + c;

		if (c == '/' || c == '\\')
		{
			found = true;
			break;
		}
	}

	if (found == false)
	{
		return "";
	}

	return path.substr(0, path.length() - tmp.length());
}

std::string ExtractBasename(const std::string& path)
{
	std::string tmp;

	for (int i = path.length() - 1; i >= 0; i--)
	{
		char c = path.at(i);

		if (c == '/' || c == '\\')
		{
			break;
		}

		tmp = c + tmp;
	}

	return tmp;
}

void Env_Init(int argc, char* argv[])
{
	TCHAR szFileName[MAX_PATH];
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	exePath = szFileName;

	binDir = ExtractDirname(exePath);
}

std::string Env_BinDirectory()
{
	return binDir;
}

std::string Env_ProjectRoot()
{
	return projectRoot;
}

std::string Env_ProjectName()
{
	return projectName;
}

std::vector<std::string>& Env_GetDirs()
{
	return dirs;
}

std::string Filetype(const std::string& path)
{
	std::string rtn;

	for (int i = path.size() - 1; i >= 0; i--)
	{
		char c = path.at(i);

		if (c == '.')
		{
			break;
		}

		rtn = c + rtn;
	}

	return rtn;
}

std::string StripFiletype(const std::string& path)
{
	std::string rtn;

	for (size_t i = 0; i < path.length(); i++)
	{
		char c = path.at(i);

		if (c == '.')
		{
			break;
		}

		rtn = rtn + c;
	}

	return rtn;
}

bool ScanTextures(const std::string& root, const std::string& path)
{
	DIR* dir = opendir(path.c_str());
	if (!dir) return false;

	while (true)
	{
		dirent* ent = readdir(dir);
		if (!ent) break;
		std::string filename = ent->d_name;
		if (filename == "." || filename == "..") continue;
		if(ScanTextures(root, path + "/" + filename)) continue;
		std::string type = Filetype(filename);

		if (type != "png" && type != "jpg" && type != "jpeg" && type != "tga"/* && type != "bmp"*/)
		{
			continue;
		}

		std::string full = path + "/" + filename;
		std::string leaf = full.substr(root.length());
		std::string vPath = StripFiletype(leaf);

		IShader* s = QERApp_Shader_ForName(vPath.c_str());
		//printf("%s\n", s->getShaderFileName());
		s->SetType(type.c_str());
	}

	closedir(dir);

	return true;
}

void Env_ScanTextures()
{
	if (projectRoot != "")
	{
		ScanTextures(projectRoot + "/", projectRoot);
	}

	QERApp_Shader_ForName(SHADER_CAULK);
	//QERApp_Shader_ForName(SHADER_NOT_FOUND);
	//QERApp_Shader_ForName(SHADER_NOTEX);
}

void Env_ResetMap(const std::string& path)
{
	projectRoot = ExtractDirname(path);

	dirs.clear();
	dirs.push_back(projectRoot + "/");

	projectName = StripFiletype(ExtractBasename(path));
	MainFrameSetExportEnabled(true);
}

void Env_Reset()
{
	projectRoot = "";
	dirs.clear();
	MainFrameSetExportEnabled(false);
}

typedef const unsigned char* (*HordeReadFunc)(const char* path, size_t* size);
static HordeReadFunc hordeReadFunc;

const unsigned char* Env_ReadFile(const char* path, size_t* size)
{
	if (!hordeReadFunc)
	{
		hordeReadFunc = (HordeReadFunc)Env_Extension("horde_load");
	}

	if (!hordeReadFunc)
	{
		//Error("Failed to find extension entry point");
		return NULL;
	}

	return hordeReadFunc(path, size);
}

typedef int (*HordeCopyFunc)(const char* filename, void** bufferptr);
static HordeCopyFunc hordeCopyFunc;

int Env_CopyFile(const char* filename, void** bufferptr)
{
	if (!hordeCopyFunc)
	{
		hordeCopyFunc = (HordeCopyFunc)Env_Extension("horde_copy");
	}

	if (!hordeCopyFunc)
	{
		//Error("Failed to find extension entry point");
		return -1;
	}

	return hordeCopyFunc(filename, bufferptr);
}

static bool loaded;
static HINSTANCE hinstLib;

FuncPtr Env_Extension(const std::string& entry)
{
	if (!loaded)
	{
		std::string dllPath = Env_BinDirectory() + "\\orextensions.dll";
		hinstLib = LoadLibrary(TEXT(dllPath.c_str()));
		loaded = true;
	}

	if(!hinstLib) return NULL;

	FuncPtr rtn = (FuncPtr)GetProcAddress(hinstLib, entry.c_str());

	return rtn;
}
