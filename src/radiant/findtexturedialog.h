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

#ifndef _FINDTEXTUREDIALOG_H_
#define _FINDTEXTUREDIALOG_H_

#include "dialog.h"

class FindTextureDialog : public Dialog
{
public:
static void setReplaceStr( const char* p );
static void setFindStr( const char* p );
static bool isOpen();
static void show();
static void updateTextures( const char* p );

FindTextureDialog ();
virtual ~FindTextureDialog ();
void BuildDialog();

bool m_bSelectedOnly;
Str m_strFind;
Str m_strReplace;
bool m_bForce;
bool m_bLive;

};


#endif //_FINDTEXTUREDIALOG_H_
