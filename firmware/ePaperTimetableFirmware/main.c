/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#define DEBUG 
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "EPD_Test.h"   //Examples
#include "EPD_7in5b_V2.h"

int main() {
    stdio_init_all();
    gpio_set_function(1, GPIO_FUNC_UART);
    gpio_set_function(2, GPIO_FUNC_UART);
    printf("Starting\n");
	DEV_Delay_ms(500);
    printf("After delay...\n"); 
  //  EPD_7in5b_V2_test(); 
    printf("Test finished!\n");

    

//////////////////////////////////////////////////////////////

    DEV_Module_Init();
    EPD_7IN5B_V2_Init();
    EPD_7IN5B_V2_Clear();
    DEV_Delay_ms(500);
	
    //Create a new image cache named IMAGE_BW and fill it with white
    UBYTE *BlackImage, *RYImage;
    UWORD Imagesize = ((EPD_7IN5B_V2_WIDTH % 8 == 0)? (EPD_7IN5B_V2_WIDTH / 8 ): (EPD_7IN5B_V2_WIDTH / 8 + 1)) * EPD_7IN5B_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    if((RYImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for red memory...\r\n");
        return -1;
    }
    printf("NewImage:BlackImage and RYImage\r\n");
    Paint_NewImage(BlackImage, EPD_7IN5B_V2_WIDTH, EPD_7IN5B_V2_HEIGHT , 0, WHITE);
    Paint_NewImage(RYImage, EPD_7IN5B_V2_WIDTH, EPD_7IN5B_V2_HEIGHT , 0, WHITE);

    //Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);

    DEV_Delay_ms(2000);
    //EPD_7IN5B_V2_Display(BlackImage, RYImage);  

    DEV_Delay_ms(2000);
    // bitmap test
    printf("before");
    Paint_Clear(WHITE);
    Paint_Clear(RED);

    Paint_DrawBitMap(image_2);

    printf("after");
    EPD_7IN5B_V2_Display(BlackImage, RYImage);
    printf("after display");
    DEV_Delay_ms(10000);

   // EPD_7IN5B_V2_Clear();
    EPD_7IN5B_V2_Sleep();
    free(BlackImage);
    free(RYImage);
    BlackImage = NULL;
    RYImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    
    DEV_Module_Exit();


//////////////////////////////////////////////////////////////



    if (cyw43_arch_init()) 
    {
        printf("Wi-Fi init failed\n");
        return -1;
    }
    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);
    }
   
}
