/* Mednafen - Multi-system Emulator
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "sexyal.h"
#include "convert.h"

namespace Mednafen
{

/* kludge.  yay. */
SexyAL_enumdevice *SexyALI_OSS_EnumerateDevices(void);
SexyAL_enumdevice *SexyALI_OpenBSD_EnumerateDevices(void);
SexyAL_device *SexyALI_OSS_Open(const char *id, SexyAL_format *format, SexyAL_buffering *buffering);
SexyAL_device *SexyALI_JACK_Open(const char *id, SexyAL_format *format, SexyAL_buffering *buffering);
SexyAL_device *SexyALI_SDL_Open(const char *id, SexyAL_format *format, SexyAL_buffering *buffering);
SexyAL_device *SexyALI_DSound_Open(const char *id, SexyAL_format *format, SexyAL_buffering *buffering);
SexyAL_device* SexyALI_OpenBSD_Open(const char* id, SexyAL_format* format, SexyAL_buffering* buffering);

#if HAVE_WASAPI
SexyAL_device *SexyALI_WASAPI_Open(const char *id, SexyAL_format *format, SexyAL_buffering *buffering);
SexyAL_device *SexyALI_WASAPISH_Open(const char *id, SexyAL_format *format, SexyAL_buffering *buffering);

bool SexyALI_WASAPI_Avail(void);
bool SexyALI_WASAPISH_Avail(void);
#endif

#ifdef HAVE_ALSA
SexyAL_enumdevice *SexyALI_ALSA_EnumerateDevices(void);
SexyAL_device *SexyALI_ALSA_Open(const char *id, SexyAL_format *format, SexyAL_buffering *buffering);
#endif

#ifdef DOS
SexyAL_device *SexyALI_DOS_SB_Open(const char *id, SexyAL_format *format, SexyAL_buffering *buffering);
SexyAL_device *SexyALI_DOS_ES1370_Open(const char *id, SexyAL_format *format, SexyAL_buffering *buffering);
SexyAL_device *SexyALI_DOS_ES1371_Open(const char *id, SexyAL_format *format, SexyAL_buffering *buffering);
SexyAL_device *SexyALI_DOS_CMI8738_Open(const char *id, SexyAL_format *format, SexyAL_buffering *buffering);

//
// Doing it this way results in questionable semantics with multiple sound cards of the same type coupled with user-configured non-default device selection,
// but it'll suffice for now.
bool SexyALI_DOS_SB_Avail(void);
bool SexyALI_DOS_ES1370_Avail(void);
bool SexyALI_DOS_ES1371_Avail(void);
bool SexyALI_DOS_CMI8738_Avail(void);
#endif

SexyAL_device *SexyALI_Dummy_Open(const char *id, SexyAL_format *format, SexyAL_buffering *buffering);


static uint32 FtoB(const SexyAL_format *format, uint32 frames)
{
 return frames * format->channels * SAMPFORMAT_BYTES(format->sampformat);
}

static uint32 BtoF(const SexyAL_format *format, uint32 bytes)
{
 return bytes / (format->channels * SAMPFORMAT_BYTES(format->sampformat));
}

static uint32 CanWrite(SexyAL_device *device)
{
 uint32 bytes;

 if(!device->RawCanWrite(device, &bytes))
  return(0);

 return(BtoF(&device->format, bytes));
}

static int Write(SexyAL_device *device, void *data, uint32 frames)
{
 assert(device->srcformat.noninterleaved == false);

 if(device->srcformat.sampformat == device->format.sampformat &&
	device->srcformat.channels == device->format.channels &&
	device->srcformat.rate == device->format.rate &&
	device->srcformat.noninterleaved == device->format.noninterleaved)
 {
  if(!device->RawWrite(device, data, FtoB(&device->format, frames)))
   return(0);
 }
 else
 {
  const uint8 *data_in = (const uint8 *)data;

  while(frames)
  {
   uint32 convert_this_iteration;

   convert_this_iteration = frames;

   if(convert_this_iteration > device->convert_buffer_fsize)
    convert_this_iteration = device->convert_buffer_fsize;

   SexyALI_Convert(&device->srcformat, &device->format, data_in, device->convert_buffer, convert_this_iteration);

   if(!device->RawWrite(device, device->convert_buffer, FtoB(&device->format, convert_this_iteration)))
    return(0);

   frames -= convert_this_iteration;
   data_in += FtoB(&device->srcformat, convert_this_iteration);
  }
 }

 return(1);
}

static int Close(SexyAL_device *device)
{
 if(device->convert_buffer)
 {
  free(device->convert_buffer);
  device->convert_buffer = NULL;
 }

 return(device->RawClose(device));
}

int SetConvert(struct __SexyAL_device *device, SexyAL_format *format)
{
 memcpy(&device->srcformat,format,sizeof(SexyAL_format));
 return(1);
}

struct SexyAL_driver
{
	int type;
	const char *name;
	const char *short_name;

	SexyAL_device * (*Open)(const char *id, SexyAL_format *format, SexyAL_buffering *buffering);
	SexyAL_enumdevice *(*EnumerateDevices)(void);
	bool (*Avail)(void);	// Optional.  Returns true if API/layer is available, false if not.
};

static SexyAL_driver drivers[] = 
{
        #if HAVE_ALSA
        { SEXYAL_TYPE_ALSA, "ALSA", "alsa", SexyALI_ALSA_Open, SexyALI_ALSA_EnumerateDevices },
        #endif

	#if HAVE_OPENBSD_AUDIO
	{ SEXYAL_TYPE_OPENBSD, "OpenBSD(/dev/audio*)", "openbsd", SexyALI_OpenBSD_Open, SexyALI_OpenBSD_EnumerateDevices },
	#endif

	#if HAVE_OSSDSP
	{ SEXYAL_TYPE_OSSDSP, "OSS(/dev/dsp*)", "oss", SexyALI_OSS_Open, SexyALI_OSS_EnumerateDevices },
	#endif

	//
	// WASAPISH should have higher priority(comes before in this array) than dsound.
	//
        #if HAVE_WASAPI
        { SEXYAL_TYPE_WASAPISH, "WASAPI(Shared mode)", "wasapish", SexyALI_WASAPISH_Open, NULL, SexyALI_WASAPISH_Avail },
        #endif

        #if HAVE_DIRECTSOUND
        { SEXYAL_TYPE_DIRECTSOUND, "DirectSound", "dsound", SexyALI_DSound_Open, NULL },
        #endif

        #if HAVE_WASAPI
        { SEXYAL_TYPE_WASAPI, "WASAPI(Exclusive mode)", "wasapi", SexyALI_WASAPI_Open, 	NULL, SexyALI_WASAPI_Avail },
        #endif

	#ifdef DOS
	//
	// List SB first, to try to prevent fubaring a PCI sound card's active Sound Blaster emulation(if present) by directly programming its PCI registers.
	//
	{ SEXYAL_TYPE_DOS_SB, "Sound Blaster 2.0/Pro/16", "sb", SexyALI_DOS_SB_Open, 	NULL, SexyALI_DOS_SB_Avail },
        { SEXYAL_TYPE_DOS_ES1370, "Ensoniq ES1370", "es1370", SexyALI_DOS_ES1370_Open, 	NULL, SexyALI_DOS_ES1370_Avail },
	{ SEXYAL_TYPE_DOS_ES1371, "Ensoniq ES1371", "es1371", SexyALI_DOS_ES1371_Open, 	NULL, SexyALI_DOS_ES1371_Avail },
	{ SEXYAL_TYPE_DOS_CMI8738, "CMI8738", "cmi8738", SexyALI_DOS_CMI8738_Open, 	NULL, SexyALI_DOS_CMI8738_Avail },
	#endif

	//
	// Keep SDL higher priority than JACK.
	//
        #if defined(HAVE_SDL)
        { SEXYAL_TYPE_SDL, "SDL", "sdl", SexyALI_SDL_Open, NULL },
        #endif

        #if HAVE_JACK
        { SEXYAL_TYPE_JACK, "JACK", "jack", SexyALI_JACK_Open, NULL },
        #endif

	{ SEXYAL_TYPE_DUMMY, "Dummy", "dummy", SexyALI_Dummy_Open, NULL },

	{ 0, NULL, NULL, NULL, NULL }
};

static SexyAL_driver *FindDriver(int type)
{
 int x = 0;

 while(drivers[x].name)
 {
  if(drivers[x].type == type)
   return(&drivers[x]);

  x++;
 }
 return(0);
}

SexyAL_device *SexyAL_Open(const char *id, SexyAL_format *format, SexyAL_buffering *buffering, int type)
{
 SexyAL_device *ret;
 SexyAL_driver *driver;

 driver = FindDriver(type);
 if(!driver)
  return(0);

 if(id && id[0] == 0) // Set pointer to NULL on empty string.
  id = NULL;

 if(id)
 {
  if(!strcmp(id, "default"))
   id = NULL;
  else if(!strncmp(id, "sexyal-literal-", strlen("sexyal-literal-")))
   id += strlen("sexyal-literal-");
 }

 assert(format->rate >= 8192 && format->rate <= (1024 * 1024));
 assert(format->channels == 1 || format->channels == 2);
 assert(0 == format->noninterleaved);

 assert(0 == buffering->buffer_size);
 assert(0 == buffering->period_size);
 assert(0 == buffering->latency);
 assert(0 == buffering->bt_gran);

 if(!(ret = driver->Open(id, format, buffering)))
  return(0);

 assert(0 != buffering->buffer_size);
 //assert(0 != buffering->period_size);
 assert(0 != buffering->latency);

 buffering->ms = (uint64)buffering->buffer_size * 1000 / format->rate;
 buffering->period_us = (uint64)buffering->period_size * (1000 * 1000) / format->rate;

 ret->convert_buffer_fsize = (25 * format->rate + 999) / 1000;
 if(!(ret->convert_buffer = calloc(format->channels * SAMPFORMAT_BYTES(format->sampformat), ret->convert_buffer_fsize)))
 {
  ret->RawClose(ret);
  return(0);
 }

 ret->Write = Write;
 ret->Close = Close;
 ret->CanWrite = CanWrite;
 ret->SetConvert = SetConvert;

 return(ret);
}

std::vector<SexyAL_DriverInfo> SexyAL_GetDriverList(void)
{
 std::vector<SexyAL_DriverInfo> ret;

 for(int x = 0; drivers[x].name; x++)
 {
  SexyAL_DriverInfo di;

  di.short_name = drivers[x].short_name;
  di.name = drivers[x].name;
  di.type = drivers[x].type;

  ret.push_back(di);
 }

 return(ret);
}

bool SexyAL_FindDriver(SexyAL_DriverInfo* out_di, const char* name)
{
 bool need_default = ((name == NULL) || !MDFN_strazicmp(name, "default"));

 for(int x = 0; drivers[x].name; x++)
 {
  if(need_default)
  {
   if(drivers[x].Avail && !drivers[x].Avail())
    continue;
  }
  else if(MDFN_strazicmp(drivers[x].short_name, name))
   continue;

  out_di->short_name = drivers[x].short_name;
  out_di->name = drivers[x].name;
  out_di->type = drivers[x].type;

  return(true);
 }

 return(false);
}

SexyAL_enumdevice* SexyAL_EnumerateDevices(int type)
{
 SexyAL_driver *driver;

 driver = FindDriver(type);

 if(!driver)
  return(0);

 if(driver->EnumerateDevices)
  return(driver->EnumerateDevices());

 return(0);
}

}
