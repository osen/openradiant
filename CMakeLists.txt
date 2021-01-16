cmake_minimum_required(VERSION 2.6)
project(OPENRADIANT)

string(TOLOWER "${CMAKE_CXX_COMPILER_ID}" COMP)
string(TOLOWER "${CMAKE_SYSTEM_NAME}" PLATFORM)
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" ARCH)
message("${COMP}-${PLATFORM}-${ARCH}")

include_directories(
  src/libs
  src/include
  ${PROJECT_SOURCE_DIR}/contrib/${COMP}-${PLATFORM}-${ARCH}/include
)

link_directories(
  ${PROJECT_SOURCE_DIR}/contrib/${COMP}-${PLATFORM}-${ARCH}/lib
)

#set(STATIC_DIST 1)

if(MSVC OR MINGW)
  if(MSVC)
    add_definitions("-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE")
    set(LIBZ zlib)
    set(LIBICONV iconv_a)
  elseif(MINGW)
    set(LIBZ z)
    set(LIBICONV iconv)
    set(LIBWIN32 Comdlg32)
  endif()

  set(SUBSYSTEM "WIN32")
  set(RC "src/radiant/win32.rc")
  set(NETNAME wins)
  set(LIBGLEW glew32)
  set(LIBGL opengl32)
  set(LIBWINSOCK ws2_32)
  set(LIBGTK glib-2.0 gobject-2.0 gtk-win32-2.0 gdk-win32-2.0 pango-1.0 gdkglext-win32-1.0 gtkglext-win32-1.0 pangoft2-1.0 gdk_pixbuf-2.0)
  set(LIBXML2 libxml2)
  set(LIBINTL intl)
  set(LIBPNG libpng)
  set(LIBJPEG jpeg)
else()
  add_definitions("-DUSE_POSIX")
  set(NETNAME berkeley)
  set(LIBGLEW GLEW)
  set(LIBGL GL)
  set(LIBM m)
  set(LIBGTK gtkglext-x11-1.0 gdkglext-x11-1.0 gtk-x11-2.0 gdk-x11-2.0 pangocairo-1.0 cairo gdk_pixbuf-2.0 gio-2.0 pangoft2-1.0 pango-1.0 gobject-2.0 glib-2.0 atk-1.0)
  set(LIBXML2 xml2)
  set(LIBPNG png)
  set(LIBJPEG jpeg)
  set(LIBX11 Xrender Xinerama Xi Xrandr Xcursor Xcomposite Xdamage Xfixes X11 Xext)
  set(LIBINTL intl)
  set(LIBFONTCONFIG fontconfig)
  set(LIBFREETYPE freetype)
  set(LIBHARFBUZZ harfbuzz)
  set(LIBZ z)

  include_directories(
    /usr/local/include/gtk-2.0
    /usr/local/lib/gtk-2.0/include
    /usr/local/include/pango-1.0
    /usr/local/include/gio-unix-2.0
    /usr/X11R6/include
    /usr/local/include/cairo
    /usr/local/include/atk-1.0
    /usr/local/include/gdk-pixbuf-2.0
    /usr/local/include
    /usr/local/include/fribidi
    /usr/X11R6/include/pixman-1
    /usr/local/include/libpng16
    /usr/local/include/harfbuzz
    /usr/local/include/glib-2.0
    /usr/local/lib/glib-2.0/include
    /usr/X11R6/include/freetype2
    /usr/local/include/libxml2
    /usr/local/include/gtkglext-1.0
    /usr/local/lib/gtkglext-1.0/include
  )

  link_directories(
    /usr/X11R6/lib
    /usr/local/lib
  )
endif()

add_executable(horde
  src/private/horde/main.cpp
)

set(EXTENSIONS_SRC
  src/private/extensions/objexport.cpp
  src/private/extensions/hordeload.c
  src/private/horde.c
)

set(PLUGINS_SRC
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

set(RADIANT_SRC
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
  src/libs/l_net/l_net_${NETNAME}.c

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

set(RBAKE_SRC
  src/private/rbake/main.cpp
  src/private/rbake/Model.cpp
  src/private/rbake/Baker.cpp
  src/private/rbake/Texture.cpp
  src/private/rbake/Light.cpp
  src/private/rbake/Part.cpp
)

set(RPREVIEW_SRC
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

if(STATIC_DIST)
  add_library(openradiant
    ${PLUGINS_SRC}
    ${RADIANT_SRC}
  )

  add_library(orextensions
    ${EXTENSIONS_SRC}
  )
  target_compile_options(orextensions PUBLIC -fPIC)

  add_library(rbake
    ${RBAKE_SRC}
  )

  add_library(rpreview
    ${RPREVIEW_SRC}
  )

else()
  add_executable(openradiant ${SUBSYSTEM}
    ${PLUGINS_SRC}
    ${RADIANT_SRC}
  )

  target_link_libraries(openradiant
    ${LIBX11}
    ${LIBGTK}
    ${LIBZ}
    ${LIBICONV}
    ${LIBWINSOCK}
    ${LIBWIN32}
    ${LIBXML2}
    ${LIBINTL}
    ${LIBPNG}
    ${LIBJPEG}
    ${LIBFONTCONFIG}
    ${LIBFREETYPE}
    ${LIBHARFBUZZ}
  )

  add_library(orextensions SHARED
    ${EXTENSIONS_SRC}
  )

  add_executable(rbake
    ${RBAKE_SRC}
  )

  add_executable(rpreview
    ${RPREVIEW_SRC}
  )

  target_link_libraries(rpreview ${LIBGLEW} ${LIBGL} ${LIBM} SDL2)
endif()

