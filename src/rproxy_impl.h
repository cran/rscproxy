/*******************************************************************************
 *  RProxy: Connector implementation between application and R language
 *  Copyright (C) 2000--2009 Thomas Baier
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 ******************************************************************************/

#ifndef _RPROXY_IMPL_H_
#define _RPROXY_IMPL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "bdx.h"

#include <Rversion.h>
#include <Rinternals.h>
  /*#include <Graphics.h>*/
  /*#include <Rdevices.h>*/
/* This misuses the NewDevDesc interface to pass its own pointers */
#undef R_USE_PROTOTYPES
#define R_USE_PROTOTYPES 0
#include <R_ext/GraphicsEngine.h>

/*
*' 08-10-11 | TB | NewDevDesc renamed to DevDesc in R 2.8.0, will be removed in
**                 2.9.0
** 08-10-17 | TB | no #define for R <= 2.8.0
*/
#if (R_VERSION >= 133120) && !defined(NewDevDesc)
#  define NewDevDesc DevDesc
#endif


/* exported functions for implementation */

/* 00-02-18 | baier | init() now receives parameter-string */
int R_Proxy_init (char const* pParameters);
int R_Proxy_evaluate (char const* pCmd,BDX_Data** pData);
int R_Proxy_evaluate_noreturn (char const* pCmd);
int R_Proxy_get_symbol (char const* pSymbol,BDX_Data** pData);
int R_Proxy_set_symbol (char const* pSymbol,BDX_Data const* pData);
int R_Proxy_term (void);

/* 06-06-18 | baier | removed vsize and nsize, added dm, reuseR */
struct _R_Proxy_init_parameters
{
  unsigned long dm;     /* data mode */
  int           reuseR; /* 1 for internal COM server */
};

int R_Proxy_parse_parameters(char const* pParameterString,
			     struct _R_Proxy_init_parameters* pParameterStruct);

struct __tag_graphics_device {
#if 0
  unsigned long version;
#endif
  union {
    SC_GraphicsDevice*  d1;
    /* SC_GraphicsDevice2* __d2; */
  } device;
};

/* 06-08-20 | baier | data structures for graphics devices ("inherits" from
 *                    NewDevDesc) */
/* 06-09-12 | baier | NewDevDesc instead of NewDevDesc* */
#define GFXDEV_ACTIVEX  1
#define GFXDEV_RECORDER 2
typedef struct __tag_graphics_device_description_cb
{
  NewDevDesc dd;
} R_Proxy_Graphics_CB;
typedef struct __tag_graphics_device_description_recorder
{
  NewDevDesc dd;
  void* recorder_data;                           /* for recording plots */
} R_Proxy_Graphics_Recorder;

extern struct __tag_graphics_device __graphics_device;

/* 06-05-17 | baier | graphics device */
/* 06-08-20 | baier | DEVDESC macro new */
#if 1
#define HASGFXDEV() (__graphics_device.device.d1 != NULL)

#define CLRGFXDEV() __graphics_device.device.d1 = NULL
#define GFXDEV() (__graphics_device.device.d1)
#define SETGFXDEV(d) __graphics_device.device.d1 = d;
#else
#define HASGFXDEV1() (__graphics_device.version == 1)
#define HASGFXDEV2() (__graphics_device.version == 2)

#define CLRGFXDEV() __graphics_device.version = 0; __graphics_device.device.d1 = NULL
#define GFXDEV1() (HASGFXDEV1() ? __graphics_device.device.d1 : NULL)
#define GFXDEV2() (HASGFXDEV2() ? __graphics_device.device.d2 : NULL)
#define SETGFXDEV1(d) __graphics_device.version = 1; __graphics_device.device.d1 = d;
#endif
#define DEVDESC(g) (&(g)->dd)

#ifdef __cplusplus
}
#endif

#endif
