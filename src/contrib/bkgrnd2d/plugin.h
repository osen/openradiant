/*
   Copyright (C) 1999-2007 id Software, Inc. and contributors.
   For a list of contributors, see the accompanying CONTRIBUTORS file.

   This file is part of GtkRadiant.

   GtkRadiant is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   GtkRadiant is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GtkRadiant; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

//
// bkgrnd2d Plugin
//
// Code by reyalP aka Reed Mideke
//
// Based on spritemodel source code by hydra
//

#ifndef _PLUGIN_H_
#define _PLUGIN_H_

/*!
   \todo need general notice about lib purpose etc.
   and the external dependencies (such as GLib, STL, mathlib etc.)
 */

#include <stdio.h>
// for CPtrArray for idata.h
#include "missing.h"

#include "synapse.h"
#include "iplugin.h"
#include "itoolbar.h"
#define USE_QERTABLE_DEFINE
#include "qerplugin.h"
#include "igl.h"
#include "ifilesystem.h"
#include "ientity.h"
#include "idata.h"

// verbose messages
#define BKGRND2D_DEBUG

extern _QERFuncTable_1 g_FuncTable;
extern _QERQglTable g_QglTable;
extern _QERFileSystemTable g_FileSystemTable;
extern _QEREntityTable g_EntityTable;
extern _QERAppDataTable g_DataTable;
extern void *g_pMainWidget;

extern CSynapseServer* g_pSynapseServer;

class CSynapseClientBkgrnd2d : public CSynapseClient
{
public:
// CSynapseClient API
bool RequestAPI( APIDescriptor_t *pAPI );
const char* GetInfo();
const char* GetName();

CSynapseClientBkgrnd2d() { }
virtual ~CSynapseClientBkgrnd2d() { }
};
#define MSG_PREFIX "bkgrnd2d: "
#define MSG_WARN "bkgrnd2d WARNING: "
#define BKGRND2D_MINOR "bkgrnd2d"
#define FILETYPE_KEY "bkgrnd2d"

#endif // _PLUGIN_H_
