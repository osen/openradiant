cmake_minimum_required(VERSION 2.8)
project(RADIANT)

string(TOLOWER "${CMAKE_CXX_COMPILER_ID}" COMP)
string(TOLOWER "${CMAKE_SYSTEM_NAME}" PLATFORM)
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" ARCH)
message("${COMP}-${PLATFORM}-${ARCH}")

if(MSVC)
  add_definitions("-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE")
  set(SUBSYSTEM "WIN32")
  set(RC "src/radiant/win32.rc")
else()
  set(RC "src/radiant/win32.rc")
endif()

include_directories(
  src/libs
  src/include
  ${PROJECT_SOURCE_DIR}/contrib/${COMP}-${PLATFORM}-${ARCH}/include
)

link_directories(
  ${PROJECT_SOURCE_DIR}/contrib/${COMP}-${PLATFORM}-${ARCH}/lib
)

add_executable(horde
  src/private/horde/main.cpp
)

add_library(orextensions SHARED
  src/private/extensions/objexport.cpp
  src/private/extensions/hordeload.c
  src/private/horde.c
)

add_library(plugins
  src/plugins/imagepng/plugin.cpp

  src/plugins/map/plugin.cpp
  src/plugins/map/parse.cpp
  src/plugins/map/write.cpp

  src/plugins/entity/plugin.cpp
  src/plugins/entity/miscmodel.cpp
  src/plugins/entity/light.cpp
  src/plugins/entity/entity_entitymodel.cpp
  src/plugins/entity/entity.cpp
  src/plugins/entity/eclassmodel.cpp

  src/plugins/image/image.cpp
  src/plugins/image/bmp.cpp
  src/plugins/image/jpeg.cpp
  src/plugins/image/lbmlib.cpp

  src/plugins/mapxml/plugin.cpp
  src/plugins/mapxml/xmlparse.cpp
  src/plugins/mapxml/xmlwrite.cpp

  src/plugins/shaders/plugin.cpp
  src/plugins/shaders/shaders.cpp

  src/plugins/surface/surfacedialog.cpp
  src/plugins/surface/surfdlg_plugin.cpp

  src/plugins/vfslocal/vfs.cpp
  src/plugins/vfslocal/vfslocal.cpp
)

add_executable(radiant ${SUBSYSTEM}
  src/radiant/bp_dlg.cpp
  src/radiant/brush.cpp
  src/radiant/brushscript.cpp
  src/radiant/brush_primit.cpp
  src/radiant/camwindow.cpp
  src/radiant/csg.cpp
  src/radiant/dialog.cpp
  src/radiant/dialoginfo.cpp
  src/radiant/drag.cpp
  src/radiant/eclass.cpp
  src/radiant/eclass_def.cpp
  src/radiant/error.cpp
  src/radiant/feedback.cpp
  src/radiant/file.cpp
  src/radiant/filters.cpp
  src/radiant/findtexturedialog.cpp
  src/radiant/glinterface.cpp
  src/radiant/glwidget.cpp
  src/radiant/glwindow.cpp
  src/radiant/groupdialog.cpp
  src/radiant/gtkdlgs.cpp
  src/radiant/gtkmisc.cpp
  src/radiant/main.cpp
  src/radiant/mainframe.cpp
  src/radiant/map.cpp
  src/radiant/missing.cpp
  src/radiant/parse.cpp
  src/radiant/patchdialog.cpp
  src/radiant/pluginentities.cpp
  src/radiant/pluginmanager.cpp
  src/radiant/pmesh.cpp
  src/radiant/points.cpp
  src/radiant/preferences.cpp
  src/radiant/profile.cpp
  src/radiant/qe3.cpp
  src/radiant/qgl_ext.cpp
  src/radiant/queuedraw.cpp
  src/radiant/select.cpp
  src/radiant/selectedface.cpp
  src/radiant/stdafx.cpp
  src/radiant/surfacedialog.cpp
  src/radiant/surfaceplugin.cpp
  src/radiant/targetname.cpp
  src/radiant/texmanip.cpp
  src/radiant/texwindow.cpp
  src/radiant/ui.cpp
  src/radiant/undo.cpp
  src/radiant/vertsel.cpp
  src/radiant/watchbsp.cpp
  src/radiant/winding.cpp
  src/radiant/xywindow.cpp
  src/radiant/z.cpp
  src/radiant/zwindow.cpp
  src/radiant/qgl.c

  src/radiant/export.cpp
  src/radiant/env.cpp

  src/libs/synapse/synapse.cpp
  src/libs/mathlib/bbox.c
  src/libs/mathlib/m4x4.c
  src/libs/mathlib/mathlib.c
  src/libs/mathlib/ray.c
  src/libs/cmdlib/cmdlib.cpp
  src/libs/l_net/l_net.c
  #src/libs/l_net/l_net_berkeley.c
  src/libs/l_net/l_net_wins.c

  src/libs/picomodel/picointernal.c
  src/libs/picomodel/picomodel.c
  src/libs/picomodel/picomodules.c
  src/libs/picomodel/pm_3ds.c
  src/libs/picomodel/pm_ase.c
  src/libs/picomodel/pm_fm.c
  src/libs/picomodel/pm_fm.h
  src/libs/picomodel/pm_lwo.c
  src/libs/picomodel/pm_md2.c
  src/libs/picomodel/pm_md3.c
  src/libs/picomodel/pm_mdc.c
  src/libs/picomodel/pm_ms3d.c
  src/libs/picomodel/pm_obj.c
  src/libs/picomodel/pm_terrain.c
  src/libs/picomodel/lwo/clip.c
  src/libs/picomodel/lwo/envelope.c
  src/libs/picomodel/lwo/list.c
  src/libs/picomodel/lwo/lwio.c
  src/libs/picomodel/lwo/lwo2.c
  src/libs/picomodel/lwo/lwob.c
  src/libs/picomodel/lwo/pntspols.c
  src/libs/picomodel/lwo/surface.c
  src/libs/picomodel/lwo/vecmath.c
  src/libs/picomodel/lwo/vmap.c

  ${RC}
)

target_link_libraries(radiant plugins
  glib-2.0
  gobject-2.0
  gtk-win32-2.0
  gdk-win32-2.0
  libxml2
  pango-1.0
  gdkglext-win32-1.0
  gtkglext-win32-1.0
  intl
  pangoft2-1.0
  ws2_32
  gdk_pixbuf-2.0
  libpng
  jpeg

  zlib
  iconv_a

  #z
  #iconv
  #Comdlg32
)

add_executable(rbake
  src/private/rbake/main.cpp
  src/private/rbake/Model.cpp
  src/private/rbake/Baker.cpp
  src/private/rbake/Texture.cpp
  src/private/rbake/Light.cpp
  src/private/rbake/Part.cpp
)

target_link_libraries(rbake)

add_executable(rpreview
  src/private/rpreview/Model.c
  src/private/rpreview/main.c
  src/private/rpreview/re/Buffer.c
  src/private/rpreview/re/Context.c
  src/private/rpreview/re/Mesh.c
  src/private/rpreview/re/Renderer.c
  src/private/rpreview/re/Shader.c
  src/private/rpreview/re/Texture.c
  src/private/rpreview/re/internal.c
  src/private/rpreview/re/mathutil.c
  src/private/rpreview/re/util.c
)

target_link_libraries(rpreview glew32 opengl32 SDL2)
