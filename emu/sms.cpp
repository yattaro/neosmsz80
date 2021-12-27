#include "sms.h"
#include <chrono>
#include <iostream>
#include <thread>

sms::sms(z80mem *mem, z80proc *proc)
{
    this->current_mem = mem;
    this->current_proc = proc;
    this->external_mem = true;
    this->external_proc = true;
}

sms::sms(std::string rom_file)
{
    this->current_mem = new z80mem(rom_file);
    this->current_proc = new z80proc(this->current_mem);
}

sms::~sms()
{
    if(!external_mem) delete current_mem;
    if(!external_proc) delete current_proc;
}

z80mem *sms::get_mem()
{
    return this->current_mem;
}

z80proc *sms::get_proc()
{
    return this->current_proc;
}

bool sms::isPaused()
{
    return this->paused;
}

bool sms::isStopped()
{
    return this->stopped;
}

void sms::run()
{
    if(this->stopped)
    {
        this->stopped = false;
        std::thread new_emu_thread(emulation_loop, this);
        emulation_thread = std::move(new_emu_thread);
    }
    if(this->paused)
    {
        this->paused = false;
        std::lock_guard<std::mutex> lk(this->pause_lock);
        this->pause_cv.notify_all();
    }
}

void sms::pause()
{
    if(!this->paused)
    {
        this->paused = true;
    }
}

void sms::stop()
{
    if(!this->stopped)
    {
        this->stopped = true;
        if(this->paused)
        {
            this->paused = false;
            std::lock_guard<std::mutex> lk(this->pause_lock);
            this->pause_cv.notify_all();
        }
        emulation_thread.join();
    }
}

void sms::emulation_loop(sms *instance)
{
    //target framerate in frames per millisecond
   const double frame_rate = 1000 / (MAIN_CLK/TICKS_PER_FRAME);
   double last_frame_time = 0;
   double current_time;
    while(!instance->isStopped())
    {
        if(!instance->isPaused())
        {
            current_time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
            if((last_frame_time + frame_rate) <= current_time)
            {
                last_frame_time = current_time;
                instance->update();
            }
        }
        else
        {
            std::unique_lock<std::mutex> lk(instance->pause_lock);
            std::cout << "Emulation paused." << std::endl;
            instance->pause_cv.wait(lk);
            std::cout << "Emulation resumed." << std::endl;
        }
    }
    std::cout << "Emulation stopped." << std::endl;
}

void sms::update()
{

}
