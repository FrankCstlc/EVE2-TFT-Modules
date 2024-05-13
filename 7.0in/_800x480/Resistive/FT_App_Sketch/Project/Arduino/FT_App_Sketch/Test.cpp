#include "Test.h"
#include "FT_Gpu_Hal.h"
#include "FT_CoPro_Cmds.h"
#include "DebugPrint.h"

#define BM_HA_POS               0
#define BM_HA_MASK              (0x3 << BM_HA_POS)
#define BM_HA_VALUE(value)      ((value << BM_HA_POS) & BM_HA_MASK)
#define BM_HA_LEFT              BM_HA_VALUE(0)
#define BM_HA_RIGHT             BM_HA_VALUE(1)
#define BM_HA_CENTER            BM_HA_VALUE(2)

#define BM_VA_POS               0
#define BM_VA_MASK              (0x3 << BM_VA_POS)
#define BM_VA_VALUE(value)      ((value << BM_VA_POS) & BM_VA_MASK)
#define BM_VA_UP                BM_VA_VALUE(0)
#define BM_VA_DOWN              BM_VA_VALUE(1)
#define BM_VA_CENTER            BM_VA_VALUE(2)

#define STR_BUFFER_SIZE         16          

extern Ft_Gpu_Hal_Context_t *phost;
extern ft_void_t Ft_App_WrCoCmd_Buffer(Ft_Gpu_Hal_Context_t *phost, ft_uint32_t cmd);
extern ft_void_t Ft_App_Flush_Co_Buffer(Ft_Gpu_Hal_Context_t *phost);

void Test_LoadKirbyToRAMG(void)
{
    ft_uint32_t address = RAM_G + 0L;
    Ft_Gpu_Hal_WrCmd32(phost,CMD_INFLATE);
    Ft_Gpu_Hal_WrCmd32(phost,address);
    Ft_Gpu_Hal_WrCmdBuf(phost,kirby.data,kirby.size);
    Ft_Gpu_Hal_WaitCmdfifo_empty(phost);
    *kirby.address = address;
}

ft_uint32_t Test_GetCounter(void)
{
    static ft_uint32_t counter = 0;
    return counter++ / 50;
}

void Test_DrawImage(Bitmap_st *bitmap, ft_uint8_t alignment, ft_int16_t x, ft_int16_t y)
{
    ft_int16_t _x, _y;
    if ((alignment & BM_HA_MASK) == BM_HA_RIGHT)
    {
        _x = x - bitmap->width;
    }
    else if ((alignment & BM_HA_MASK) == BM_HA_CENTER)
    {
        _x = x - bitmap->width / 2;
    }
    else
    {
        _x = x;
    }
    if ((alignment & BM_VA_MASK) == BM_VA_DOWN)
    {
        _y = y - bitmap->height;
    }
    else if ((alignment & BM_VA_MASK) == BM_VA_CENTER)
    {
        _y = y - bitmap->height / 2;
    }
    else
    {
        _y = y;
    }

    Ft_App_WrCoCmd_Buffer(phost,BITMAP_HANDLE(0));
    Ft_App_WrCoCmd_Buffer(phost,BITMAP_SOURCE(*bitmap->address));
    Ft_App_WrCoCmd_Buffer(phost,BITMAP_LAYOUT(bitmap->format, bitmap->stride & 0x3FF, bitmap->height & 0x1FF));
    Ft_App_WrCoCmd_Buffer(phost,BITMAP_LAYOUT_H(bitmap->stride >> 10, bitmap->height >> 9));
    Ft_App_WrCoCmd_Buffer(phost,BITMAP_SIZE(NEAREST, BORDER, BORDER, bitmap->width & 0x1FF, bitmap->height & 0x1FF));
    Ft_App_WrCoCmd_Buffer(phost,BITMAP_SIZE_H(bitmap->width >> 9, bitmap->height >> 9));
    Ft_App_WrCoCmd_Buffer(phost,CELL(0));

    //Ft_Gpu_CoCmd_SetBitmap(phost, *bitmap->address, bitmap->format, bitmap->width, bitmap->height);

    Ft_App_WrCoCmd_Buffer(phost, VERTEX_FORMAT(0));
    Ft_App_WrCoCmd_Buffer(phost,BEGIN(BITMAPS));
    Ft_App_WrCoCmd_Buffer(phost,VERTEX2F(_x, _y));
    Ft_App_WrCoCmd_Buffer(phost,END());
}

void Test_DrawScreen(ft_uint32_t counter, ft_uint32_t errorCounter)
{
    Ft_Gpu_CoCmd_Dlstart(phost);

    Ft_App_WrCoCmd_Buffer(phost,CLEAR_COLOR_RGB(255, 255, 255));
    Ft_App_WrCoCmd_Buffer(phost,CLEAR(1,1,1));

    Test_DrawImage(&kirby, BM_HA_CENTER | BM_VA_CENTER, 125, 240);

    char counterStr[STR_BUFFER_SIZE] = {0,};
    char errorcounterStr[STR_BUFFER_SIZE] = {0,};
    snprintf(counterStr, sizeof(counterStr), "%u\n", counter);
    snprintf(errorcounterStr, sizeof(errorcounterStr), "%u\n", errorCounter);

    Ft_Gpu_CoCmd_RomFont(phost, 1, 34);
    Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(0, 0, 0));
    Ft_Gpu_CoCmd_Text(phost, 780, 120, 1, OPT_CENTERY | OPT_RIGHTX, counterStr);
    Ft_Gpu_CoCmd_Text(phost, 780, 310, 1, OPT_CENTERY | OPT_RIGHTX, errorcounterStr);
    
    Ft_Gpu_CoCmd_RomFont(phost, 1, 32);
    Ft_App_WrCoCmd_Buffer(phost,COLOR_RGB(0, 0, 175));
    Ft_Gpu_CoCmd_Text(phost, 780, 190, 1, OPT_CENTERY | OPT_RIGHTX, "Counter");
    Ft_Gpu_CoCmd_Text(phost, 780, 380, 1, OPT_CENTERY | OPT_RIGHTX, "Error counter");

    Ft_App_WrCoCmd_Buffer(phost, DISPLAY());
    Ft_Gpu_CoCmd_Swap(phost);
    Ft_App_Flush_Co_Buffer(phost);
    Ft_Gpu_Hal_WaitCmdfifo_empty(phost);
}

void Test_Init(void)
{
	DebugInit();
}

void Test_Run(void)
{
    Test_LoadKirbyToRAMG();
    while(1)
    {
        ft_uint32_t counter = Test_GetCounter();
        ft_uint32_t errorCounter = Ft_GetErrorCounter();
        Test_DrawScreen(counter, errorCounter);
        Ft_Gpu_Hal_Sleep(10);
    }
}