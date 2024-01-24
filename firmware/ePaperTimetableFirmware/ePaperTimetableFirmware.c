#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/dma.h"
#include "hardware/clocks.h"
#include "pico/cyw43_arch.h"
#include "EPD_Test.h"   //Examples
#define DEBUG 
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "EPD_Test.h"   //Examples

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19



int main()
{
    stdio_init_all();

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    


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

   // EPD_7IN5B_V2_Display(gImage_7in5_V2_b, gImage_7in5_V2_ry);
   // DEV_Delay_ms(2000);

    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawString_EN(10, 0, "dupa", &Font16, BLACK, WHITE);    

    
    //2.Draw red image
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);
    Paint_DrawString_EN(10, 20, "duza dupa dupa dupa", &Font12, WHITE, BLACK);
    
    EPD_7IN5B_V2_Display(BlackImage, RYImage);
    DEV_Delay_ms(2000);

    EPD_7IN5B_V2_Clear();
    EPD_7IN5B_V2_Sleep();
    free(BlackImage);
    free(RYImage);
    BlackImage = NULL;
    RYImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    
    DEV_Module_Exit();


//////////////////////////////////////////////////////////////
    puts("Hello, world!");

    return 0;
}
