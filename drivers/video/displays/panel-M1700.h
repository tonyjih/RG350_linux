#ifndef __PANEL_M1700_H
#define __PANEL_M1700_H

struct M1700_platform_data {
	int gpio_reset;
	int gpio_clock;
	int gpio_enable;
	int gpio_data;
};

extern struct panel_ops M1700_panel_ops;

#endif /* __PANEL_M1700_H */
