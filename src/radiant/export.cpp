#include "export.h"
#include "env.h"

#include <orplugin.h>

#include <string>

#ifdef _WIN32
	#define CDECL __cdecl
#else
	#define CDECL
#endif

typedef void (CDECL *FUNC)(PluginData*, bool, bool);
FUNC func;

void export_obj(bool bake, bool preview)
{
	if(!func)
	{
		func = (FUNC)Env_Extension("export_obj");
	}

	if (!func)
	{
		Error("Failed to find extension entry point");
	}

	PluginData pd;
	pd.active_brushes = &active_brushes;
	pd.entities = &entities;
	pd.selected_brushes = &selected_brushes;
	pd.Env_BinDirectory = Env_BinDirectory;
	pd.Env_ProjectName = Env_ProjectName;
	pd.Env_ProjectRoot = Env_ProjectRoot;
	pd.Error = Error;
	pd.Sys_Status = Sys_Status;

	func(&pd, bake, preview);
}

