#include <stdint.h>


#define RCC_AHB1ENR    (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR    (*(volatile uint32_t*)0x40023840)


#define GPIOA_MODER    (*(volatile uint32_t*)0x40020000)
#define GPIOA_ODR      (*(volatile uint32_t*)0x40020014)


#define GPIOB_MODER    (*(volatile uint32_t*)0x40020400)
#define GPIOB_OSPEEDR  (*(volatile uint32_t*)0x40020408)
#define GPIOB_AFRH     (*(volatile uint32_t*)0x40020424)


#define GPIOC_MODER    (*(volatile uint32_t*)0x40020800)
#define GPIOC_ODR      (*(volatile uint32_t*)0x40020814)


#define SPI2_CR1       (*(volatile uint32_t*)0x40003800)
#define SPI2_SR        (*(volatile uint32_t*)0x40003808)
#define SPI2_DR_8BIT   (*(volatile uint8_t* )0x4000380C)


#define TFT_CS_LOW()    (GPIOA_ODR &= ~(1u<<0))
#define TFT_CS_HIGH()   (GPIOA_ODR |=  (1u<<0))
#define TFT_DC_LOW()    (GPIOA_ODR &= ~(1u<<1))
#define TFT_DC_HIGH()   (GPIOA_ODR |=  (1u<<1))
#define TFT_RST_LOW()   (GPIOC_ODR &= ~(1u<<2))
#define TFT_RST_HIGH()  (GPIOC_ODR |=  (1u<<2))
#define LED_ON()        (GPIOA_ODR |=  (1u<<5))
#define LED_OFF()       (GPIOA_ODR &= ~(1u<<5))

// ── SR bits ──────────────────────────────────────────────────────────
#define SR_RXNE  (1u<<0)
#define SR_TXE   (1u<<1)
#define SR_BSY   (1u<<7)

// ── Screen dimensions ────────────────────────────────────────────────
#define LCD_WIDTH   240
#define LCD_HEIGHT  320

// ── Colors RGB565 ────────────────────────────────────────────────────
#define RED      0xF800
#define GREEN    0x07E0
#define BLUE     0x001F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF
#define BLACK    0x0000
#define CYAN     0x07FF
#define MAGENTA  0xF81F


void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
        for (volatile uint32_t j = 0; j < 16000; j++);
}

void blink(int n)
{
    for (int i = 0; i < n; i++) {
        LED_ON();  delay_ms(150);
        LED_OFF(); delay_ms(150);
    }
    delay_ms(300);
}


void GPIO_INIT(void)
{
    RCC_AHB1ENR |= (1u<<0)|(1u<<1)|(1u<<2);

    GPIOA_MODER &= ~((3u<<0)|(3u<<2)|(3u<<10));
    GPIOA_MODER |=  ((1u<<0)|(1u<<2)|(1u<<10));
    TFT_CS_HIGH(); TFT_DC_HIGH(); LED_OFF();

    GPIOC_MODER &= ~(3u<<4);
    GPIOC_MODER |=  (1u<<4);
    TFT_RST_HIGH();

    GPIOB_MODER   &= ~(0x3Fu<<26);
    GPIOB_MODER   |=  (0x2Au<<26);
    GPIOB_OSPEEDR |=  (3u<<26)|(3u<<28)|(3u<<30);
    GPIOB_AFRH    &= ~(0xFFFu<<20);
    GPIOB_AFRH    |=  (0x5u<<20)|(0x5u<<24)|(0x5u<<28);
}


void SPI2_INIT(void)
{
    RCC_APB1ENR |= (1u<<14);
    SPI2_CR1 = 0;
    SPI2_CR1 |= (1u<<2);
    SPI2_CR1 |= (1u<<4);
    SPI2_CR1 |= (1u<<8);
    SPI2_CR1 |= (1u<<9);
    SPI2_CR1 |= (1u<<6);
}


void SPI2_Send(uint8_t data)
{
    while (!(SPI2_SR & SR_TXE));
    SPI2_DR_8BIT = data;
    while (!(SPI2_SR & SR_RXNE));
    volatile uint8_t dummy = SPI2_DR_8BIT;
    (void)dummy;
    while (SPI2_SR & SR_BSY);
}


void LCD_CMD(uint8_t cmd)
{
    TFT_DC_LOW();
    TFT_CS_LOW();
    SPI2_Send(cmd);
    TFT_CS_HIGH();
}

void LCD_DATA(uint8_t data)
{
    TFT_DC_HIGH();
    TFT_CS_LOW();
    SPI2_Send(data);
    TFT_CS_HIGH();
}


void LCD_RESET(void)
{
    TFT_RST_HIGH(); delay_ms(10);
    TFT_RST_LOW();  delay_ms(20);
    TFT_RST_HIGH(); delay_ms(150);
}
void LCD_INIT(void)
{
    LCD_RESET();

    LCD_CMD(0x01); delay_ms(150);   // software reset
    LCD_CMD(0x11); delay_ms(120);   // sleep out

    LCD_CMD(0x36);                  // memory access control
    LCD_DATA(0x00);                 // normal orientation

    LCD_CMD(0x3A);                  // pixel format
    LCD_DATA(0x05);                 // RGB565 ← ST7789 uses 0x05 not 0x55!

    LCD_CMD(0xB2);                  // porch setting
    LCD_DATA(0x0C); LCD_DATA(0x0C);
    LCD_DATA(0x00); LCD_DATA(0x33); LCD_DATA(0x33);

    LCD_CMD(0xB7);                  // gate control
    LCD_DATA(0x35);

    LCD_CMD(0xBB);                  // VCOM setting
    LCD_DATA(0x19);

    LCD_CMD(0xC0);                  // LCM control
    LCD_DATA(0x2C);

    LCD_CMD(0xC2);                  // VDV and VRH command enable
    LCD_DATA(0x01);

    LCD_CMD(0xC3);                  // VRH set
    LCD_DATA(0x12);

    LCD_CMD(0xC4);                  // VDV set
    LCD_DATA(0x20);

    LCD_CMD(0xC6);                  // frame rate — 60Hz
    LCD_DATA(0x0F);

    LCD_CMD(0xD0);                  // power control 1
    LCD_DATA(0xA4); LCD_DATA(0xA1);

    LCD_CMD(0xE0);                  // positive voltage gamma
    LCD_DATA(0xD0); LCD_DATA(0x04);
    LCD_DATA(0x0D); LCD_DATA(0x11);
    LCD_DATA(0x13); LCD_DATA(0x2B);
    LCD_DATA(0x3F); LCD_DATA(0x54);
    LCD_DATA(0x4C); LCD_DATA(0x18);
    LCD_DATA(0x0D); LCD_DATA(0x0B);
    LCD_DATA(0x1F); LCD_DATA(0x23);

    LCD_CMD(0xE1);                  // negative voltage gamma
    LCD_DATA(0xD0); LCD_DATA(0x04);
    LCD_DATA(0x0C); LCD_DATA(0x11);
    LCD_DATA(0x13); LCD_DATA(0x2C);
    LCD_DATA(0x3F); LCD_DATA(0x44);
    LCD_DATA(0x51); LCD_DATA(0x2F);
    LCD_DATA(0x1F); LCD_DATA(0x1F);
    LCD_DATA(0x20); LCD_DATA(0x23);

    LCD_CMD(0x21);                  // display inversion ON (ST7789 needs this)
    LCD_CMD(0x29); delay_ms(50);    // display ON

    // ST7789 display window — set full 240x320
    LCD_CMD(0x2A);                  // column address
    LCD_DATA(0x00); LCD_DATA(0x00);
    LCD_DATA(0x00); LCD_DATA(0xEF); // 0→239

    LCD_CMD(0x2B);                  // row address
    LCD_DATA(0x00); LCD_DATA(0x00);
    LCD_DATA(0x01); LCD_DATA(0x3F); // 0→319
}
// ─────────────────────────────────────────────────────────────────────
// 7. Draw filled rectangle — core function
//    x0,y0 = top left corner
//    x1,y1 = bottom right corner
// ─────────────────────────────────────────────────────────────────────
void LCD_RECT(uint16_t x0, uint16_t y0,
              uint16_t x1, uint16_t y1,
              uint16_t color)
{
    // boundary check
    if (x1 >= LCD_WIDTH)  x1 = LCD_WIDTH  - 1;
    if (y1 >= LCD_HEIGHT) y1 = LCD_HEIGHT - 1;

    // column address
    LCD_CMD(0x2A);
    LCD_DATA(x0 >> 8); LCD_DATA(x0 & 0xFF);
    LCD_DATA(x1 >> 8); LCD_DATA(x1 & 0xFF);

    // row address
    LCD_CMD(0x2B);
    LCD_DATA(y0 >> 8); LCD_DATA(y0 & 0xFF);
    LCD_DATA(y1 >> 8); LCD_DATA(y1 & 0xFF);

    // write pixels
    LCD_CMD(0x2C);
    TFT_DC_HIGH();
    TFT_CS_LOW();

    uint32_t total = (uint32_t)(x1 - x0 + 1) * (y1 - y0 + 1);
    for (uint32_t i = 0; i < total; i++) {
        SPI2_Send(color >> 8);
        SPI2_Send(color & 0xFF);
    }

    while (SPI2_SR & SR_BSY);
    TFT_CS_HIGH();
}

// ─────────────────────────────────────────────────────────────────────
// 8. Fill full screen
// ─────────────────────────────────────────────────────────────────────
void LCD_FILL(uint16_t color)
{
    LCD_RECT(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1, color);
}

// ─────────────────────────────────────────────────────────────────────
// 9. Draw single pixel
// ─────────────────────────────────────────────────────────────────────
void LCD_PIXEL(uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT) return;
    LCD_RECT(x, y, x, y, color);
}


void LCD_FOUR_PARTS(void)
{

    LCD_RECT(0,   0,   119, 159, RED);


    LCD_RECT(120, 0,   239, 159, GREEN);


    LCD_RECT(0,   160, 119, 319, BLUE);


    LCD_RECT(120, 160, 239, 319, YELLOW);
}


int main(void)
{
    GPIO_INIT();
    SPI2_INIT();

    blink(3);

    LCD_INIT();   // ← now ST7789
    blink(4);

    LCD_FILL(RED);   delay_ms(500);
    LCD_FILL(GREEN); delay_ms(500);
    LCD_FILL(BLUE);  delay_ms(500);

    LCD_FOUR_PARTS();   // 4 clean quadrants

    while(1){}
}
