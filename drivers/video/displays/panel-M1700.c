
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/gfp.h>
#include <linux/gpio.h>

#include <video/jzpanel.h>
#include <video/panel-M1700.h>
#include "jz4770gpio.h"
#define SPEN		JZ_GPIO_PORTE(16)      /*LCD_CS*/
#define SPCK		JZ_GPIO_PORTE(15)       /*LCD_SCL*/
#define SPDA		JZ_GPIO_PORTE(17)      /*LCD_SDA*/
#define LCD_RET 	JZ_GPIO_PORTE(2)

struct M1700 {
	struct M1700_platform_data *pdata;
};
void spi_writ_bit16_NEW(unsigned short val) 
{ 
	unsigned char no;
	unsigned short value;

	value=val&0xFFFF;	

	__gpio_set_pin(SPEN);
	__gpio_clear_pin(SPCK);
	__gpio_clear_pin(SPDA);
	__gpio_clear_pin(SPEN);
	udelay(50);
	for(no=0;no<16;no++)
	{
		__gpio_clear_pin(SPCK);
		if((value&0x8000)==0x8000)
			__gpio_set_pin(SPDA);
		else
			__gpio_clear_pin(SPDA);
		udelay(50);
		__gpio_set_pin(SPCK);
		value <<= 1; 
		udelay(50);
	 }
	__gpio_set_pin(SPEN);
	udelay(50);
}
static void SPI_WriteComm(struct M1700_platform_data *pdata, u8 data)
{
  int bit;
 
  gpio_direction_output(pdata->gpio_enable, 0);
  gpio_direction_output(pdata->gpio_clock, 0);
  gpio_direction_output(pdata->gpio_data, 0);
  udelay(10);
  gpio_direction_output(pdata->gpio_clock, 1);
  udelay(10);
  for(bit=7; bit>=0; bit--){
    gpio_direction_output(pdata->gpio_clock, 0);
    gpio_direction_output(pdata->gpio_data, (data >> bit) & 1);
    udelay(10);
    gpio_direction_output(pdata->gpio_clock, 1);
    udelay(10);
  }
  gpio_direction_output(pdata->gpio_enable, 1);
}
 
static void SPI_WriteData(struct M1700_platform_data *pdata, u8 data)
{
  int bit;
 
  gpio_direction_output(pdata->gpio_enable, 0);
  gpio_direction_output(pdata->gpio_clock, 0);
  gpio_direction_output(pdata->gpio_data, 1);
  udelay(10);
  gpio_direction_output(pdata->gpio_clock, 1);
  udelay(10);
  for(bit=7; bit>=0; bit--){
    gpio_direction_output(pdata->gpio_clock, 0);
    gpio_direction_output(pdata->gpio_data, (data >> bit) & 1);
    udelay(10);
    gpio_direction_output(pdata->gpio_clock, 1);
    udelay(10);
  }
  gpio_direction_output(pdata->gpio_enable, 1);
}


static int M1700_panel_init(void **out_panel, struct device *dev,
			      void *panel_pdata)
{
	struct M1700_platform_data *pdata = panel_pdata;
	struct M1700 *panel;
	int ret;

	panel = devm_kzalloc(dev, sizeof(*panel), GFP_KERNEL);
	if (!panel) {
		dev_err(dev, "Failed to alloc panel data\n");
		return -ENOMEM;
	}

	panel->pdata = pdata;

	*out_panel = panel;

	/* Reserve GPIO pins. */

	ret = devm_gpio_request(dev, pdata->gpio_reset, "LCD panel reset");
	if (ret) {
		dev_err(dev,
			"Failed to request LCD panel reset pin: %d\n", ret);
		return ret;
	}

	ret = devm_gpio_request(dev, pdata->gpio_clock, "LCD 3-wire clock");
	if (ret) {
		dev_err(dev,
			"Failed to request LCD panel 3-wire clock pin: %d\n",
			ret);
		return ret;
	}

	ret = devm_gpio_request(dev, pdata->gpio_enable, "LCD 3-wire enable");
	if (ret) {
		dev_err(dev,
			"Failed to request LCD panel 3-wire enable pin: %d\n",
			ret);
		return ret;
	}

	ret = devm_gpio_request(dev, pdata->gpio_data, "LCD 3-wire data");
	if (ret) {
		dev_err(dev,
			"Failed to request LCD panel 3-wire data pin: %d\n",
			ret);
		return ret;
	}

	/* Set initial GPIO pin directions and value. */

	gpio_direction_output(pdata->gpio_clock,  1);
	gpio_direction_output(pdata->gpio_enable, 1);
	gpio_direction_output(pdata->gpio_data,   0);

	return 0;
}

static void M1700_panel_exit(void *panel)
{
}

static void M1700_panel_enable(void *panel)
{
	struct M1700_platform_data *pdata = ((struct M1700 *)panel)->pdata;
	int i;

	/* Reset LCD panel. */
	gpio_direction_output(pdata->gpio_reset, 1);
	mdelay(1);
	gpio_direction_output(pdata->gpio_reset, 0);
	mdelay(1);
	gpio_direction_output(pdata->gpio_reset, 1);
	mdelay(1);




#if 1
spi_writ_bit16_NEW(0xFD00); //SW reset
spi_writ_bit16_NEW(0x0200);
mdelay(10);	
	
spi_writ_bit16_NEW(0xFD00);
spi_writ_bit16_NEW(0x1800);//01
spi_writ_bit16_NEW(0x4420);
//spi_writ_bit16_NEW(0x4530);


spi_writ_bit16_NEW(0x1987);
spi_writ_bit16_NEW(0x1A80);
spi_writ_bit16_NEW(0x1C00);
spi_writ_bit16_NEW(0x1D09);


spi_writ_bit16_NEW(0xFDC5);
spi_writ_bit16_NEW(0x820C);
spi_writ_bit16_NEW(0xA2B4);//VGH=15V, VGL=-14V  BA
	

spi_writ_bit16_NEW(0xFDC4);
spi_writ_bit16_NEW(0x8245);//DCVCOM precharge	
	
spi_writ_bit16_NEW(0xFDC1);
spi_writ_bit16_NEW(0x9102);//03		
mdelay(20);


spi_writ_bit16_NEW(0xFD00);
spi_writ_bit16_NEW(0x0101);
#endif	
}

static void M1700_panel_disable(void *panel)
{
	struct M1700_platform_data *pdata = ((struct M1700 *)panel)->pdata;

}

struct panel_ops M1700_panel_ops = {
	.init		= M1700_panel_init,
	.exit		= M1700_panel_exit,
	.enable		= M1700_panel_enable,
	.disable	= M1700_panel_disable,
};

