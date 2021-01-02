#include "../radiant/qe3.h"
#include "../radiant/env.h"

struct PluginData
{
	void (*Error)(const char* error, ...);
	void (*Sys_Status)(const char* psz, int part);
	std::string (*Env_ProjectName)();
	std::string (*Env_ProjectRoot)();
	std::string (*Env_BinDirectory)();
	entity_t *entities;
	brush_t *active_brushes;
	brush_t *selected_brushes;
};