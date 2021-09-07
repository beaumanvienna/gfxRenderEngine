#include <../base.hpp>

#define SUPERFX_CPP
namespace bSNES_v059 {

#include "serialization.cpp"
#include "core/core.cpp"
#include "memory/memory.cpp"
#include "mmio/mmio.cpp"
#include "timing/timing.cpp"
#include "disasm/disasm.cpp"

SuperFX superfx;
SuperFXBus superfxbus;

namespace memory {
    SuperFXCPUROM fxrom;
    SuperFXCPURAM fxram;
}

uint8 SuperFXBus::read(uint32 addr)
{
#ifndef WIN32
    static const void* cat_table[8] =
    {
     && case_0,
     && case_1,
     && case_2,
     && case_3,
     && case_default,&& case_default,&& case_default,&& case_default
    };

    goto* cat_table[addr >> 21];

case_0:			// 0x00-0x1F
case_1:			// 0x20-0x3F
    while (!superfx.regs.scmr.ron && scheduler.sync != Scheduler::SyncAll)
    {
        superfx.add_clocks(6);
        scheduler.sync_copcpu();
    }
    return rom_ptr[((addr & 0x3F0000) >> 1) | (addr & 0x7FFF)];

case_2:			// 0x40-0x5F
    while (!superfx.regs.scmr.ron && scheduler.sync != Scheduler::SyncAll)
    {
        superfx.add_clocks(6);
        scheduler.sync_copcpu();
    }
    return rom_ptr[addr & 0x1FFFFF];

case_3:			// 0x60-0x7F
    while (!superfx.regs.scmr.ran && scheduler.sync != Scheduler::SyncAll)
    {
        superfx.add_clocks(6);
        scheduler.sync_copcpu();
    }

    if (ram_ptr)
        return ram_ptr[addr & ram_mask];

    //puts("ZOO");

    return(0xFF);

case_default:
    //puts("MOO");
    return 0xFF;
#else
    enum
    {
        case_0 = 0,
        case_1,
        case_2,
        case_3
    };
    switch (addr >> 21)
    {

    case case_0:            // 0x00-0x1F
    case case_1:            // 0x20-0x3F
        while (!superfx.regs.scmr.ron && scheduler.sync != Scheduler::SyncAll)
        {
            superfx.add_clocks(6);
            scheduler.sync_copcpu();
        }
        return rom_ptr[((addr & 0x3F0000) >> 1) | (addr & 0x7FFF)];

    case case_2:            // 0x40-0x5F
        while (!superfx.regs.scmr.ron && scheduler.sync != Scheduler::SyncAll)
        {
            superfx.add_clocks(6);
            scheduler.sync_copcpu();
        }
        return rom_ptr[addr & 0x1FFFFF];

    case case_3:            // 0x60-0x7F
        while (!superfx.regs.scmr.ran && scheduler.sync != Scheduler::SyncAll)
        {
            superfx.add_clocks(6);
            scheduler.sync_copcpu();
        }

        if (ram_ptr)
            return ram_ptr[addr & ram_mask];

        //puts("ZOO");
        return(0xFF);

    default:
        //puts("MOO");
        return 0xFF;
    }
#endif
}

void SuperFXBus::write(uint32 addr, uint8 val)
{
    #ifndef WIN32
    static const void* cat_table[8] =
    {
     && case_0,
     && case_1,
     && case_2,
     && case_3,
     && case_default,&& case_default,&& case_default,&& case_default
    };

    goto* cat_table[addr >> 21];

case_0:			// 0x00-0x1F
case_1:			// 0x20-0x3F
    while (!superfx.regs.scmr.ron && scheduler.sync != Scheduler::SyncAll)
    {
        superfx.add_clocks(6);
        scheduler.sync_copcpu();
    }
    return;

case_2:			// 0x40-0x5F
    while (!superfx.regs.scmr.ron && scheduler.sync != Scheduler::SyncAll)
    {
        superfx.add_clocks(6);
        scheduler.sync_copcpu();
    }
    return;

case_3:			// 0x60-0x7F
    while (!superfx.regs.scmr.ran && scheduler.sync != Scheduler::SyncAll)
    {
        superfx.add_clocks(6);
        scheduler.sync_copcpu();
    }

    if (ram_ptr)
        ram_ptr[addr & ram_mask] = val;

    return;

case_default:
    //puts("MOOW");
    return;
    #else
    enum
    {
        case_0,
        case_1,
        case_2,
        case_3
    };

    switch(addr >> 21)
    {

        case case_0:            // 0x00-0x1F
        case case_1:            // 0x20-0x3F
            while (!superfx.regs.scmr.ron && scheduler.sync != Scheduler::SyncAll)
            {
                superfx.add_clocks(6);
                scheduler.sync_copcpu();
            }
            return;

        case case_2:            // 0x40-0x5F
            while (!superfx.regs.scmr.ron && scheduler.sync != Scheduler::SyncAll)
            {
                superfx.add_clocks(6);
                scheduler.sync_copcpu();
            }
            return;

        case case_3:            // 0x60-0x7F
            while (!superfx.regs.scmr.ran && scheduler.sync != Scheduler::SyncAll)
            {
                superfx.add_clocks(6);
                scheduler.sync_copcpu();
            }
        
            if (ram_ptr)
                ram_ptr[addr & ram_mask] = val;
        
            return;

        default:
            //puts("MOOW");
        return;
    }
    #endif
}

void SuperFXBus::init()
{
    rom_ptr = memory::cartrom.data();

    if ((int)memory::cartram.size() > 0)
    {
        ram_ptr = memory::cartram.data();
        ram_mask = memory::cartram.size() - 1;
    }
    else
    {
        ram_ptr = NULL;
        ram_mask = 0;
    }

#if 0
    map(MapDirect, 0x00, 0xff, 0x0000, 0xffff, memory::memory_unmapped);

    map(MapLinear, 0x00, 0x3f, 0x0000, 0x7fff, memory::gsurom);
    map(MapLinear, 0x00, 0x3f, 0x8000, 0xffff, memory::gsurom);
    map(MapLinear, 0x40, 0x5f, 0x0000, 0xffff, memory::gsurom);
    map(MapLinear, 0x60, 0x7f, 0x0000, 0xffff, memory::gsuram);
#endif

    bus.map(Bus::MapLinear, 0x00, 0x3f, 0x6000, 0x7fff, memory::fxram, 0x0000, 0x2000);
    bus.map(Bus::MapLinear, 0x00, 0x3f, 0x8000, 0xffff, memory::fxrom);
    bus.map(Bus::MapLinear, 0x40, 0x5f, 0x0000, 0xffff, memory::fxrom);
    bus.map(Bus::MapLinear, 0x60, 0x7d, 0x0000, 0xffff, memory::fxram);
    bus.map(Bus::MapLinear, 0x80, 0xbf, 0x6000, 0x7fff, memory::fxram, 0x0000, 0x2000);
    bus.map(Bus::MapLinear, 0x80, 0xbf, 0x8000, 0xffff, memory::fxrom);
    bus.map(Bus::MapLinear, 0xc0, 0xdf, 0x0000, 0xffff, memory::fxrom);
    bus.map(Bus::MapLinear, 0xe0, 0xff, 0x0000, 0xffff, memory::fxram);
}

//ROM / RAM access from the S-CPU

unsigned SuperFXCPUROM::size() const {
    return memory::cartrom.size();
}

uint8 SuperFXCPUROM::read(unsigned addr) {
    if (superfx.regs.sfr.g && superfx.regs.scmr.ron) {
        static const uint8_t data[16] = {
          0x00, 0x01, 0x00, 0x01, 0x04, 0x01, 0x00, 0x01,
          0x00, 0x01, 0x08, 0x01, 0x00, 0x01, 0x0c, 0x01,
        };
        return data[addr & 15];
    }
    return memory::cartrom.read(addr);
}

void SuperFXCPUROM::write(unsigned addr, uint8 data) {
    memory::cartrom.write(addr, data);
}

unsigned SuperFXCPURAM::size() const {
    return memory::cartram.size();
}

uint8 SuperFXCPURAM::read(unsigned addr) {
    if (superfx.regs.sfr.g && superfx.regs.scmr.ran) return cpu.regs.mdr;
    return memory::cartram.read(addr);
}

void SuperFXCPURAM::write(unsigned addr, uint8 data) {
    memory::cartram.write(addr, data);
}


void SuperFX::enter() {
  while(true) {
    while(scheduler.sync == Scheduler::SyncAll) {
      scheduler.exit(Scheduler::SynchronizeEvent);
    }

    if(regs.sfr.g == 0) {
      add_clocks(6);
      scheduler.sync_copcpu();
      continue;
    }

    do_op((regs.sfr.alt & 1023) | peekpipe());
    regs.r[15].data += r15_NOT_modified;    

    if(++instruction_counter >= 128) {
      instruction_counter = 0;
      scheduler.sync_copcpu();
    }
  }
}

void SuperFX::init() {
  regs.r[14].on_modify = SuperFX_r14_modify;
  regs.r[15].on_modify = SuperFX_r15_modify;
}

void SuperFX::enable() {
  for(unsigned i = 0x3000; i <= 0x32ff; i++) memory::mmio.map(i, *this);
}

void SuperFX::power() {
  clockmode = config.superfx.speed;
  reset();
}

void SuperFX::reset() {
  //printf("%d, %d\n", (int)sizeof(reg16_t), (int)((uint8*)&regs.r[0] - (uint8*)this));

  superfxbus.init();
  instruction_counter = 0;

  for(unsigned n = 0; n < 16; n++) regs.r[n] = 0x0000;
  regs.sfr   = 0x0000;
  regs.pbr   = 0x00;
  regs.rombr = 0x00;
  regs.rambr = 0;
  regs.cbr   = 0x0000;
  regs.scbr  = 0x00;
  regs.scmr  = 0x00;
  regs.colr  = 0x00;
  regs.por   = 0x00;
  regs.bramr = 0;
  regs.vcr   = 0x04;
  regs.cfgr  = 0x00;
  regs.clsr  = 0;
  regs.pipeline = 0x01;  //nop
  regs.ramaddr = 0x0000;
  regs.reset();

  memory_reset();
  timing_reset();
}

}
