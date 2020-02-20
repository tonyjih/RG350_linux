
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/gfp.h>
#include <linux/gpio.h>

#include <video/jzpanel.h>
#include <video/panel-nv3052c.h>


struct nv3052c {
	struct nv3052c_platform_data *pdata;
};

static void SPI_WriteComm(struct nv3052c_platform_data *pdata, u8 data)
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
 
static void SPI_WriteData(struct nv3052c_platform_data *pdata, u8 data)
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


static int nv3052c_panel_init(void **out_panel, struct device *dev,
			      void *panel_pdata)
{
	struct nv3052c_platform_data *pdata = panel_pdata;
	struct nv3052c *panel;
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

static void nv3052c_panel_exit(void *panel)
{
}

static void nv3052c_panel_enable(void *panel)
{
	struct nv3052c_platform_data *pdata = ((struct nv3052c *)panel)->pdata;
	int i;

	/* Reset LCD panel. */
	gpio_direction_output(pdata->gpio_reset, 0);
	udelay(1000);
	gpio_direction_output(pdata->gpio_reset, 1);
	udelay(1000);

	/* Init panel registers. */

SPI_WriteComm(pdata, 0xFF);SPI_WriteData(pdata, 0x30);
SPI_WriteComm(pdata, 0xFF);SPI_WriteData(pdata, 0x52);
SPI_WriteComm(pdata, 0xFF);SPI_WriteData(pdata, 0x01);  
SPI_WriteComm(pdata, 0xE3);SPI_WriteData(pdata, 0x00);  
SPI_WriteComm(pdata, 0x40);SPI_WriteData(pdata, 0x00);
SPI_WriteComm(pdata, 0x03);SPI_WriteData(pdata, 0x40);
SPI_WriteComm(pdata, 0x04);SPI_WriteData(pdata, 0x00);
SPI_WriteComm(pdata, 0x05);SPI_WriteData(pdata, 0x03);
SPI_WriteComm(pdata, 0x08);SPI_WriteData(pdata, 0x00);
SPI_WriteComm(pdata, 0x09);SPI_WriteData(pdata, 0x07);
SPI_WriteComm(pdata, 0x0A);SPI_WriteData(pdata, 0x01);
SPI_WriteComm(pdata, 0x0B);SPI_WriteData(pdata, 0x32);
SPI_WriteComm(pdata, 0x0C);SPI_WriteData(pdata, 0x32);
SPI_WriteComm(pdata, 0x0D);SPI_WriteData(pdata, 0x0B);
SPI_WriteComm(pdata, 0x0E);SPI_WriteData(pdata, 0x00);
SPI_WriteComm(pdata, 0x23);SPI_WriteData(pdata, 0xA0);

SPI_WriteComm(pdata, 0x24);SPI_WriteData(pdata, 0x0c);
SPI_WriteComm(pdata, 0x25);SPI_WriteData(pdata, 0x06);
SPI_WriteComm(pdata, 0x26);SPI_WriteData(pdata, 0x14);
SPI_WriteComm(pdata, 0x27);SPI_WriteData(pdata, 0x14);

SPI_WriteComm(pdata, 0x38);SPI_WriteData(pdata, 0xcC); 
SPI_WriteComm(pdata, 0x39);SPI_WriteData(pdata, 0xd7); 
SPI_WriteComm(pdata, 0x3A);SPI_WriteData(pdata, 0x4a); 

SPI_WriteComm(pdata, 0x28);SPI_WriteData(pdata, 0x40);
SPI_WriteComm(pdata, 0x29);SPI_WriteData(pdata, 0x01);
SPI_WriteComm(pdata, 0x2A);SPI_WriteData(pdata, 0xdf);
SPI_WriteComm(pdata, 0x49);SPI_WriteData(pdata, 0x3C);   
SPI_WriteComm(pdata, 0x91);SPI_WriteData(pdata, 0x77); 
SPI_WriteComm(pdata, 0x92);SPI_WriteData(pdata, 0x77); 
SPI_WriteComm(pdata, 0xA0);SPI_WriteData(pdata, 0x55);
SPI_WriteComm(pdata, 0xA1);SPI_WriteData(pdata, 0x50);
SPI_WriteComm(pdata, 0xA4);SPI_WriteData(pdata, 0x9C);
SPI_WriteComm(pdata, 0xA7);SPI_WriteData(pdata, 0x02);  
SPI_WriteComm(pdata, 0xA8);SPI_WriteData(pdata, 0x01);  
SPI_WriteComm(pdata, 0xA9);SPI_WriteData(pdata, 0x01);  
SPI_WriteComm(pdata, 0xAA);SPI_WriteData(pdata, 0xFC);  
SPI_WriteComm(pdata, 0xAB);SPI_WriteData(pdata, 0x28);  
SPI_WriteComm(pdata, 0xAC);SPI_WriteData(pdata, 0x06);  
SPI_WriteComm(pdata, 0xAD);SPI_WriteData(pdata, 0x06);  
SPI_WriteComm(pdata, 0xAE);SPI_WriteData(pdata, 0x06);  
SPI_WriteComm(pdata, 0xAF);SPI_WriteData(pdata, 0x03);  
SPI_WriteComm(pdata, 0xB0);SPI_WriteData(pdata, 0x08);  
SPI_WriteComm(pdata, 0xB1);SPI_WriteData(pdata, 0x26);  
SPI_WriteComm(pdata, 0xB2);SPI_WriteData(pdata, 0x28);  
SPI_WriteComm(pdata, 0xB3);SPI_WriteData(pdata, 0x28);  
SPI_WriteComm(pdata, 0xB4);SPI_WriteData(pdata, 0x33);  
SPI_WriteComm(pdata, 0xB5);SPI_WriteData(pdata, 0x08);  
SPI_WriteComm(pdata, 0xB6);SPI_WriteData(pdata, 0x26);  
SPI_WriteComm(pdata, 0xB7);SPI_WriteData(pdata, 0x08);  
SPI_WriteComm(pdata, 0xB8);SPI_WriteData(pdata, 0x26); 
SPI_WriteComm(pdata, 0xF0);SPI_WriteData(pdata, 0x00); 
SPI_WriteComm(pdata, 0xF6);SPI_WriteData(pdata, 0xC0);


SPI_WriteComm(pdata, 0xFF);SPI_WriteData(pdata, 0x30);
SPI_WriteComm(pdata, 0xFF);SPI_WriteData(pdata, 0x52);
SPI_WriteComm(pdata, 0xFF);SPI_WriteData(pdata, 0x02);
SPI_WriteComm(pdata, 0xB0);SPI_WriteData(pdata, 0x0B);
SPI_WriteComm(pdata, 0xB1);SPI_WriteData(pdata, 0x16);
SPI_WriteComm(pdata, 0xB2);SPI_WriteData(pdata, 0x17); 
SPI_WriteComm(pdata, 0xB3);SPI_WriteData(pdata, 0x2C); 
SPI_WriteComm(pdata, 0xB4);SPI_WriteData(pdata, 0x32);  
SPI_WriteComm(pdata, 0xB5);SPI_WriteData(pdata, 0x3B);  
SPI_WriteComm(pdata, 0xB6);SPI_WriteData(pdata, 0x29); 
SPI_WriteComm(pdata, 0xB7);SPI_WriteData(pdata, 0x40);   
SPI_WriteComm(pdata, 0xB8);SPI_WriteData(pdata, 0x0d);
SPI_WriteComm(pdata, 0xB9);SPI_WriteData(pdata, 0x05);
SPI_WriteComm(pdata, 0xBA);SPI_WriteData(pdata, 0x12);
SPI_WriteComm(pdata, 0xBB);SPI_WriteData(pdata, 0x10);
SPI_WriteComm(pdata, 0xBC);SPI_WriteData(pdata, 0x12);
SPI_WriteComm(pdata, 0xBD);SPI_WriteData(pdata, 0x15);
SPI_WriteComm(pdata, 0xBE);SPI_WriteData(pdata, 0x19);              
SPI_WriteComm(pdata, 0xBF);SPI_WriteData(pdata, 0x0E);
SPI_WriteComm(pdata, 0xC0);SPI_WriteData(pdata, 0x16);  
SPI_WriteComm(pdata, 0xC1);SPI_WriteData(pdata, 0x0A);
SPI_WriteComm(pdata, 0xD0);SPI_WriteData(pdata, 0x0C);
SPI_WriteComm(pdata, 0xD1);SPI_WriteData(pdata, 0x17);
SPI_WriteComm(pdata, 0xD2);SPI_WriteData(pdata, 0x14);
SPI_WriteComm(pdata, 0xD3);SPI_WriteData(pdata, 0x2E);   
SPI_WriteComm(pdata, 0xD4);SPI_WriteData(pdata, 0x32);   
SPI_WriteComm(pdata, 0xD5);SPI_WriteData(pdata, 0x3C);  
SPI_WriteComm(pdata, 0xD6);SPI_WriteData(pdata, 0x22);
SPI_WriteComm(pdata, 0xD7);SPI_WriteData(pdata, 0x3D);
SPI_WriteComm(pdata, 0xD8);SPI_WriteData(pdata, 0x0D);
SPI_WriteComm(pdata, 0xD9);SPI_WriteData(pdata, 0x07);
SPI_WriteComm(pdata, 0xDA);SPI_WriteData(pdata, 0x13);
SPI_WriteComm(pdata, 0xDB);SPI_WriteData(pdata, 0x13);
SPI_WriteComm(pdata, 0xDC);SPI_WriteData(pdata, 0x11);
SPI_WriteComm(pdata, 0xDD);SPI_WriteData(pdata, 0x15);
SPI_WriteComm(pdata, 0xDE);SPI_WriteData(pdata, 0x19);                   
SPI_WriteComm(pdata, 0xDF);SPI_WriteData(pdata, 0x10);
SPI_WriteComm(pdata, 0xE0);SPI_WriteData(pdata, 0x17);    
SPI_WriteComm(pdata, 0xE1);SPI_WriteData(pdata, 0x0A);

SPI_WriteComm(pdata, 0xFF);SPI_WriteData(pdata, 0x30);
SPI_WriteComm(pdata, 0xFF);SPI_WriteData(pdata, 0x52);
SPI_WriteComm(pdata, 0xFF);SPI_WriteData(pdata, 0x03);   
SPI_WriteComm(pdata, 0x00);SPI_WriteData(pdata, 0x2A);
SPI_WriteComm(pdata, 0x01);SPI_WriteData(pdata, 0x2A);
SPI_WriteComm(pdata, 0x02);SPI_WriteData(pdata, 0x2A);
SPI_WriteComm(pdata, 0x03);SPI_WriteData(pdata, 0x2A);
SPI_WriteComm(pdata, 0x04);SPI_WriteData(pdata, 0x61);  
SPI_WriteComm(pdata, 0x05);SPI_WriteData(pdata, 0x80);   
SPI_WriteComm(pdata, 0x06);SPI_WriteData(pdata, 0xc7);   
SPI_WriteComm(pdata, 0x07);SPI_WriteData(pdata, 0x01);  
SPI_WriteComm(pdata, 0x08);SPI_WriteData(pdata, 0x03); 
SPI_WriteComm(pdata, 0x09);SPI_WriteData(pdata, 0x04);
SPI_WriteComm(pdata, 0x70);SPI_WriteData(pdata, 0x22);
SPI_WriteComm(pdata, 0x71);SPI_WriteData(pdata, 0x80);
SPI_WriteComm(pdata, 0x30);SPI_WriteData(pdata, 0x2A);
SPI_WriteComm(pdata, 0x31);SPI_WriteData(pdata, 0x2A);
SPI_WriteComm(pdata, 0x32);SPI_WriteData(pdata, 0x2A);
SPI_WriteComm(pdata, 0x33);SPI_WriteData(pdata, 0x2A);
SPI_WriteComm(pdata, 0x34);SPI_WriteData(pdata, 0x61);
SPI_WriteComm(pdata, 0x35);SPI_WriteData(pdata, 0xc5);
SPI_WriteComm(pdata, 0x36);SPI_WriteData(pdata, 0x80);
SPI_WriteComm(pdata, 0x37);SPI_WriteData(pdata, 0x23);
SPI_WriteComm(pdata, 0x40);SPI_WriteData(pdata, 0x03); 
SPI_WriteComm(pdata, 0x41);SPI_WriteData(pdata, 0x04); 
SPI_WriteComm(pdata, 0x42);SPI_WriteData(pdata, 0x05); 
SPI_WriteComm(pdata, 0x43);SPI_WriteData(pdata, 0x06); 
SPI_WriteComm(pdata, 0x44);SPI_WriteData(pdata, 0x11); 
SPI_WriteComm(pdata, 0x45);SPI_WriteData(pdata, 0xe8); 
SPI_WriteComm(pdata, 0x46);SPI_WriteData(pdata, 0xe9); 
SPI_WriteComm(pdata, 0x47);SPI_WriteData(pdata, 0x11);
SPI_WriteComm(pdata, 0x48);SPI_WriteData(pdata, 0xea); 
SPI_WriteComm(pdata, 0x49);SPI_WriteData(pdata, 0xeb);
SPI_WriteComm(pdata, 0x50);SPI_WriteData(pdata, 0x07); 
SPI_WriteComm(pdata, 0x51);SPI_WriteData(pdata, 0x08); 
SPI_WriteComm(pdata, 0x52);SPI_WriteData(pdata, 0x09); 
SPI_WriteComm(pdata, 0x53);SPI_WriteData(pdata, 0x0a); 
SPI_WriteComm(pdata, 0x54);SPI_WriteData(pdata, 0x11); 
SPI_WriteComm(pdata, 0x55);SPI_WriteData(pdata, 0xec); 
SPI_WriteComm(pdata, 0x56);SPI_WriteData(pdata, 0xed); 
SPI_WriteComm(pdata, 0x57);SPI_WriteData(pdata, 0x11); 
SPI_WriteComm(pdata, 0x58);SPI_WriteData(pdata, 0xef); 
SPI_WriteComm(pdata, 0x59);SPI_WriteData(pdata, 0xf0); 
SPI_WriteComm(pdata, 0xB1);SPI_WriteData(pdata, 0x01); 
SPI_WriteComm(pdata, 0xB4);SPI_WriteData(pdata, 0x15); 
SPI_WriteComm(pdata, 0xB5);SPI_WriteData(pdata, 0x16); 
SPI_WriteComm(pdata, 0xB6);SPI_WriteData(pdata, 0x09); 
SPI_WriteComm(pdata, 0xB7);SPI_WriteData(pdata, 0x0f); 
SPI_WriteComm(pdata, 0xB8);SPI_WriteData(pdata, 0x0d); 
SPI_WriteComm(pdata, 0xB9);SPI_WriteData(pdata, 0x0b); 
SPI_WriteComm(pdata, 0xBA);SPI_WriteData(pdata, 0x00); 
SPI_WriteComm(pdata, 0xC7);SPI_WriteData(pdata, 0x02); 
SPI_WriteComm(pdata, 0xCA);SPI_WriteData(pdata, 0x17); 
SPI_WriteComm(pdata, 0xCB);SPI_WriteData(pdata, 0x18); 
SPI_WriteComm(pdata, 0xCC);SPI_WriteData(pdata, 0x0a); 
SPI_WriteComm(pdata, 0xCD);SPI_WriteData(pdata, 0x10); 
SPI_WriteComm(pdata, 0xCE);SPI_WriteData(pdata, 0x0e); 
SPI_WriteComm(pdata, 0xCF);SPI_WriteData(pdata, 0x0c); 
SPI_WriteComm(pdata, 0xD0);SPI_WriteData(pdata, 0x00); 
SPI_WriteComm(pdata, 0x81);SPI_WriteData(pdata, 0x00); 
SPI_WriteComm(pdata, 0x84);SPI_WriteData(pdata, 0x15); 
SPI_WriteComm(pdata, 0x85);SPI_WriteData(pdata, 0x16); 
SPI_WriteComm(pdata, 0x86);SPI_WriteData(pdata, 0x10); 
SPI_WriteComm(pdata, 0x87);SPI_WriteData(pdata, 0x0a); 
SPI_WriteComm(pdata, 0x88);SPI_WriteData(pdata, 0x0c); 
SPI_WriteComm(pdata, 0x89);SPI_WriteData(pdata, 0x0e);
SPI_WriteComm(pdata, 0x8A);SPI_WriteData(pdata, 0x02); 
SPI_WriteComm(pdata, 0x97);SPI_WriteData(pdata, 0x00); 
SPI_WriteComm(pdata, 0x9A);SPI_WriteData(pdata, 0x17); 
SPI_WriteComm(pdata, 0x9B);SPI_WriteData(pdata, 0x18);
SPI_WriteComm(pdata, 0x9C);SPI_WriteData(pdata, 0x0f);
SPI_WriteComm(pdata, 0x9D);SPI_WriteData(pdata, 0x09); 
SPI_WriteComm(pdata, 0x9E);SPI_WriteData(pdata, 0x0b); 
SPI_WriteComm(pdata, 0x9F);SPI_WriteData(pdata, 0x0d); 
SPI_WriteComm(pdata, 0xA0);SPI_WriteData(pdata, 0x01); 
SPI_WriteComm(pdata, 0xFF);SPI_WriteData(pdata, 0x30);
SPI_WriteComm(pdata, 0xFF);SPI_WriteData(pdata, 0x52);
SPI_WriteComm(pdata, 0xFF);SPI_WriteData(pdata, 0x02);  
SPI_WriteComm(pdata, 0x01);SPI_WriteData(pdata, 0x01);
SPI_WriteComm(pdata, 0x02);SPI_WriteData(pdata, 0xDA);
SPI_WriteComm(pdata, 0x03);SPI_WriteData(pdata, 0xBA);
SPI_WriteComm(pdata, 0x04);SPI_WriteData(pdata, 0xA8);
SPI_WriteComm(pdata, 0x05);SPI_WriteData(pdata, 0x9A);
SPI_WriteComm(pdata, 0x06);SPI_WriteData(pdata, 0x70);
SPI_WriteComm(pdata, 0x07);SPI_WriteData(pdata, 0xFF);
SPI_WriteComm(pdata, 0x08);SPI_WriteData(pdata, 0x91);
SPI_WriteComm(pdata, 0x09);SPI_WriteData(pdata, 0x90);
SPI_WriteComm(pdata, 0x0A);SPI_WriteData(pdata, 0xFF);
SPI_WriteComm(pdata, 0x0B);SPI_WriteData(pdata, 0x8F);
SPI_WriteComm(pdata, 0x0C);SPI_WriteData(pdata, 0x60);
SPI_WriteComm(pdata, 0x0D);SPI_WriteData(pdata, 0x58);
SPI_WriteComm(pdata, 0x0E);SPI_WriteData(pdata, 0x48);
SPI_WriteComm(pdata, 0x0F);SPI_WriteData(pdata, 0x38);
SPI_WriteComm(pdata, 0x10);SPI_WriteData(pdata, 0x2B);
SPI_WriteComm(pdata, 0xFF);SPI_WriteData(pdata, 0x30);
SPI_WriteComm(pdata, 0xFF);SPI_WriteData(pdata, 0x52);
SPI_WriteComm(pdata, 0xFF);SPI_WriteData(pdata, 0x00);   
SPI_WriteComm(pdata, 0x36);SPI_WriteData(pdata, 0x0a);

SPI_WriteComm(pdata, 0x11);SPI_WriteData(pdata, 0x00);	 //sleep out
mdelay( 100 );

SPI_WriteComm(pdata, 0x29);SPI_WriteData(pdata, 0x00);	  //display on
mdelay(10);  
}

static void nv3052c_panel_disable(void *panel)
{
	struct nv3052c_platform_data *pdata = ((struct nv3052c *)panel)->pdata;

}

struct panel_ops nv3052c_panel_ops = {
	.init		= nv3052c_panel_init,
	.exit		= nv3052c_panel_exit,
	.enable		= nv3052c_panel_enable,
	.disable	= nv3052c_panel_disable,
};
