#include "rtc.h"

#include "io.h"

static uint8_t get_rtc_register(uint32_t reg) 
{
      outb(CMOS_ADDRESS_PORT, reg);
      return inb(CMOS_DATA_PORT);
}

static bool get_update_in_progress_flag()
{
    return (get_rtc_register(0x0A) & 0x80);
}

void read_rtc(rtc_state* state)
{
    rtc_state last_state;
    uint8_t last_century;

    while (get_update_in_progress_flag());

    state->second = get_rtc_register(0x00);
    state->minute = get_rtc_register(0x02);
    state->hour = get_rtc_register(0x04);
    state->day = get_rtc_register(0x07);
    state->month = get_rtc_register(0x08);
    state->year = get_rtc_register(0x09);
    uint8_t century = get_rtc_register(0x32);

    do
    {
        last_state = *state;
        last_century = century;

        while (get_update_in_progress_flag());

        state->second = get_rtc_register(0x00);
        state->minute = get_rtc_register(0x02);
        state->hour = get_rtc_register(0x04);
        state->day = get_rtc_register(0x07);
        state->month = get_rtc_register(0x08);
        state->year = get_rtc_register(0x09);
        century = get_rtc_register(0x32);
    } while (last_state.second != state->second 
            && last_state.minute != state->minute 
            && last_state.hour != state->hour 
            && last_state.day != state->day 
            && last_state.month != state->month 
            && last_state.year != state->year 
            && last_century != century);

    uint8_t registerB = get_rtc_register(0x0B);

    if (!(registerB & 0x04)) 
    {
        state->second = (state->second & 0x0F) + ((state->second / 16) * 10);
        state->minute = (state->minute & 0x0F) + ((state->minute / 16) * 10);
        state->hour = ((state->hour & 0x0F) + (((state->hour & 0x70) / 16) * 10) ) | (state->hour & 0x80);
        state->day = (state->day & 0x0F) + ((state->day / 16) * 10);
        state->month = (state->month & 0x0F) + ((state->month / 16) * 10);
        state->year = (state->year & 0x0F) + ((state->year / 16) * 10);
        century = (century & 0x0F) + ((century / 16) * 10);
    }

    if (!(registerB & 0x02) && (state->hour & 0x80)) state->hour = ((state->hour & 0x7F) + 12) % 24;

    state->year += century * 100;
}
