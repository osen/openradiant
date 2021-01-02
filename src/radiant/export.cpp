#include "export.h"
//#include "qe3.h"
#include "env.h"

#include <orplugin.h>

#include <string>

HINSTANCE hinstLib;

typedef void (__cdecl *FUNC)(PluginData*, bool, bool);
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