#include <libpayload.h>
#include "doomgeneric.h"
#include "doomkeys.h"
#include <coreboot_tables.h>

#define FB ((unsigned char *)phys_to_virt(lib_sysinfo.framebuffer.physical_address))

#define KEYQUEUE_SIZE 16

static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;

static unsigned char convertToDoomKey(unsigned char scancode)
{
    unsigned char key = 0;

    switch (scancode)
    {
    case 0x9c:
    case 0x1c:
        key = KEY_ENTER;
        break;
    case 0x38:
    case 0xB8:
        key = KEY_RALT;
        break;
    case 0x01:
        key = KEY_ESCAPE;
        break;
    case 0x33:
    case 0xB3:
        key = KEY_STRAFE_L;
        break;
    case 0x34:
    case 0xB4:
        key = KEY_STRAFE_R;
        break;
    case 0xCB:
    case 0x4B:
        key = KEY_LEFTARROW;
        break;
    case 0xCD:
    case 0x4D:
        key = KEY_RIGHTARROW;
        break;
    case 0xC8:
    case 0x48:
        key = KEY_UPARROW;
        break;
    case 0xD0:
    case 0x50:
        key = KEY_DOWNARROW;
        break;
    case 0x1D:
        key = KEY_FIRE;
        break;
    case 0x39:
        key = KEY_USE;
        break;
    case 0x2A:
    case 0x36:
        key = KEY_RSHIFT;
        break;
    case 0x15:
        key = 'y';
        break;
    default:
        break;
    }

    return key;
}

static void addKeyToQueue(int pressed, unsigned int keyCode)
{
    unsigned char key = convertToDoomKey(keyCode);

    unsigned short keyData = (pressed << 8) | key;

    s_KeyQueue[s_KeyQueueWriteIndex] = keyData;
    s_KeyQueueWriteIndex++;
    s_KeyQueueWriteIndex %= KEYQUEUE_SIZE;
}

static void handleKeyInput()
{
    if (!keyboard_havechar())
    {
        return;
    }

    unsigned char scancode = keyboard_get_scancode();
    if (scancode > 0)
    {
        unsigned char keyRelease = (0x80 & scancode);

        scancode = (0x7F & scancode);

        if (0 == keyRelease)
        {
            addKeyToQueue(1, scancode);
        }
        else
        {
            addKeyToQueue(0, scancode);
        }
    }
}

void DG_Init()
{
    keyboard_init();
    return;
}
void DG_DrawFrame()
{
    for (int row = 0; row < DOOMGENERIC_RESY; row++) {
        memcpy(FB+row*lib_sysinfo.framebuffer.bytes_per_line, DG_ScreenBuffer + row*DOOMGENERIC_RESX, DOOMGENERIC_RESX*4);
    }

    handleKeyInput();
}
void DG_SleepMs(uint32_t ms)
{
    mdelay(ms);
}
uint32_t DG_GetTicksMs()
{
    struct timeval tp;

    gettimeofday(&tp, NULL);

    return (tp.tv_sec * 1000) + (tp.tv_usec / 1000); /* return milliseconds */
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
        if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex)
        {
                //key queue is empty

                return 0;
        }
        else
        {
                unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
                s_KeyQueueReadIndex++;
                s_KeyQueueReadIndex %= KEYQUEUE_SIZE;

                *pressed = keyData >> 8;
                *doomKey = keyData & 0xFF;

                return 1;
        }
}

void DG_SetWindowTitle(const char * title)
{
    return;
}
