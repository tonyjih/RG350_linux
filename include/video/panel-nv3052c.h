#ifndef __PANEL_NV3052C_H
#define __PANEL_NV3052C_H

struct nv3052c_platform_data {
	int gpio_reset;
	int gpio_clock;
	int gpio_enable;
	int gpio_data;
};

extern struct panel_ops nv3052c_panel_ops;
#define NV3052C
#if defined(NV3052C)  
extern void spi_writ_bit16(unsigned short reg, unsigned short val);
extern void spi_send_value(unsigned short reg, unsigned short val);
extern void lcd_special_pin_init(void);
extern void LCD_NV3052C_init(void);
extern void lcd_enter_sleep(void);
extern void lcd_exit_sleep(void);
#endif
#endif /* __PANEL_NV3052C_H */
