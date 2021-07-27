#ifndef __PANEL_M1700_H
#define __PANEL_M1700_H

struct M1700_platform_data {
	int gpio_reset;
	int gpio_clock;
	int gpio_enable;
	int gpio_data;
};

extern struct panel_ops M1700_panel_ops;
#define M1700_INCLUDE
#if defined(M1700_INCLUDE)  
extern void spi_writ_bit16(unsigned short reg, unsigned short val);
extern void spi_send_value(unsigned short reg, unsigned short val);
extern void lcd_special_pin_init(void);
extern void LCD_M1700_init(void);
extern void lcd_enter_sleep(void);
extern void lcd_exit_sleep(void);
#endif
#endif /* __PANEL_NV3052C_H */
