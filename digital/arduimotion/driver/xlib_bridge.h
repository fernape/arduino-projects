#ifndef __XLIB_BRIDGE_H__
#define __XLIB_BRIDGE_H__

int init_bridge(void);
int update_mouse_state(const int delta_x, const int delta_y, const int button_status);
int move_mouse_cursor(const int delta_x, const int delta_y);
int mouse_button_pressed(void);
int mouse_button_released(void);
void shutdown_bridge(void);


#endif /* __XLIB_BRIDGE_H__ */
