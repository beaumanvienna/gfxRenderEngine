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

#include "main.h"
#include <trio/trio.h>

#include "sound.h"

#include <mednafen/sexyal/sexyal.h>

static SexyAL_device* Output = NULL;
static SexyAL_format format;
static SexyAL_buffering buffering;

static int16* EmuModBuffer = NULL;
static int32 EmuModBufferSize = 0;	// In frames.

static double SoundRate = 0;
static bool NeedReInit = false;

bool Sound_NeedReInit(void)
{
 return NeedReInit;
}

double Sound_GetRate(void)
{
 return SoundRate;
}

uint32 Sound_CanWrite(void)
{
 if(!Output)
  return 0;

 return Output->CanWrite(Output);
}

void Sound_Write(int16 *Buffer, int Count)
{
 if(!Output)
  return;

 if(!Output->Write(Output, Buffer, Count))
 {
  //
  // TODO; We need to take assert()'s out of the wasapi and wasapish code before we can safely enable this
  //
  //NeedReInit = true;
  //printf("Output->Write failure? %d\n", Count);
 }
}

void Sound_WriteSilence(int ms)
{
    unsigned int frames = (uint64)format.rate * ms / 1000;
    #ifdef _MSC_VER
    int16* SBuffer = new int16[frames * format.channels];
    #else
    int16 SBuffer[frames * format.channels];
    #endif //_MSC_VER

    memset(SBuffer, 0, sizeof(SBuffer));
    Output->Write(Output, SBuffer, frames);
}



bool Sound_Init(MDFNGI *gi)
{
 SexyAL_DriverInfo CurDriver;

 NeedReInit = false;
 SoundRate = 0;

 memset(&format, 0, sizeof(format));
 memset(&buffering, 0, sizeof(buffering));

 format.sampformat = SEXYAL_FMT_PCMS16;
 format.channels = gi->soundchan;
 format.noninterleaved = false;
 format.rate = MDFN_GetSettingUI("sound.rate");

 buffering.ms = MDFN_GetSettingUI("sound.buffer_time");

 if(!buffering.ms)
 {
  buffering.overhead_kludge = true;
  buffering.ms = 7 + floor(0.5 + 1.5 * 1000.0 / gi->fps * (256 * 65536));
 }
 else
  buffering.overhead_kludge = false;

 buffering.period_us = MDFN_GetSettingUI("sound.period_time");

 std::string zedevice = MDFN_GetSettingS("sound.device");
 std::string zedriver = MDFN_GetSettingS("sound.driver");

 MDFNI_printf(_("\nInitializing sound...\n"));
 MDFN_indent(1);

 if(!SexyAL_FindDriver(&CurDriver, zedriver.c_str()))
 {
  std::vector<SexyAL_DriverInfo> DriverTypes = SexyAL_GetDriverList();

  MDFN_printf(_("\nUnknown sound driver \"%s\".  Compiled-in sound drivers:\n"), zedriver.c_str());

  MDFN_indent(2);
  for(unsigned x = 0; x < DriverTypes.size(); x++)
  {
   MDFN_printf("%s\n", DriverTypes[x].short_name);
  }
  MDFN_indent(-2);
  MDFN_printf("\n");

  MDFN_indent(-1);
  return false;
 }

 if(!MDFN_strazicmp(zedevice.c_str(), "default"))
  MDFNI_printf(_("Using \"%s\" audio driver with SexyAL's default device selection."), CurDriver.name);
 else
  MDFNI_printf(_("Using \"%s\" audio driver with device \"%s\":"), CurDriver.name, zedevice.c_str());
 MDFN_indent(1);

 if(!(Output = SexyAL_Open(zedevice.c_str(), &format, &buffering, CurDriver.type)))
 {
  MDFN_Notify(MDFN_NOTICE_ERROR, _("Error opening a sound device."));

  MDFN_indent(-2);
  return false;
 }

 if(format.rate < 22050 || format.rate > 192000)
 {
  MDFN_Notify(MDFN_NOTICE_ERROR, _("Set rate of %u is out of range [22050-192000]"), format.rate);
  Sound_Kill();
  MDFN_indent(-2);
  return false;
 }
 {
  const char* enc_str = _("unknown encoding");

  switch(SAMPFORMAT_ENC(format.sampformat))
  {
   case SEXYAL_ENC_PCM_UINT: enc_str = _("unsigned"); break;
   case SEXYAL_ENC_PCM_SINT: enc_str = _("signed"); break;
   case SEXYAL_ENC_PCM_FLOAT: enc_str = _("floating-point"); break;
  }

  MDFNI_printf("\n");

  MDFNI_printf(_("Format: %u bits(%s, %u bytes%s)\n"),
	SAMPFORMAT_BITS(format.sampformat),
	enc_str,
	SAMPFORMAT_BYTES(format.sampformat),
	(SAMPFORMAT_BYTES(format.sampformat) <= 1) ? "" : SAMPFORMAT_BIGENDIAN(format.sampformat) ? ", big-endian" : ", little-endian");
  MDFNI_printf(_("Rate: %u\n"), format.rate);
  MDFNI_printf(_("Channels: %u%s\n"), format.channels, format.noninterleaved ? _(" (non-interleaved) ") : "");
  MDFNI_printf(_("Buffer size: %u sample frames(%f ms)\n"), buffering.buffer_size, (double)buffering.buffer_size * 1000 / format.rate);
  MDFNI_printf(_("Latency: %u sample frames(%f ms)\n"), buffering.latency, (double)buffering.latency * 1000 / format.rate);
 }

 if(buffering.period_size)
 {
  //int64_t pt_test_result = ((int64_t)buffering.period_size * (1000 * 1000) / format.rate);
  int64_t bt_test_result = ((int64_t)(buffering.bt_gran ? buffering.bt_gran : buffering.period_size) * (1000 * 1000) / format.rate);
  MDFNI_printf(_("Period size: %u sample frames(%f ms)\n"), buffering.period_size, (double)buffering.period_size * 1000 / format.rate);

  if(bt_test_result > 5333)
  {
   MDFN_indent(1);

   if(!buffering.bt_gran)
    MDFN_printf(_("Warning: Period time is too large(it should be <= ~5.333ms).  Video will appear very jerky.\n"));
   else
    MDFN_printf(_("Warning: Buffer update timing granularity is too large(%f; it should be <= ~5.333ms).  Video will appear very jerky.\n"), (double)buffering.bt_gran * 1000 / format.rate);
   MDFN_indent(-1);
  }
 }

 format.sampformat = SEXYAL_FMT_PCMS16;
 format.channels = gi->soundchan;
 format.noninterleaved = false;
 //format.rate=gi->soundrate?gi->soundrate:soundrate;

 Output->SetConvert(Output, &format);

 EmuModBufferSize = (500 * format.rate + 999) / 1000;
 EmuModBuffer = (int16 *)calloc(sizeof(int16) * format.channels, EmuModBufferSize);

 SoundRate = format.rate;
 MDFN_indent(-2);

 return true;
}

bool Sound_Kill(void)
{
 SoundRate = 0;

 if(EmuModBuffer)
 {
  free(EmuModBuffer);
  EmuModBuffer = NULL;

  EmuModBufferSize = 0;
 }

 if(Output)
  Output->Close(Output);

 if(!Output)
  return false;

 Output = NULL;

 return true;
}


int16 *Sound_GetEmuModBuffer(int32 *max_size_bytes)
{
 *max_size_bytes = EmuModBufferSize;

 return EmuModBuffer;
}
