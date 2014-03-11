#ifndef LED_SOCK__H__
#define LED_SOCK__H__

extern struct ev_loop *led_loop;
void led_ev_init(void);
void led_ev_release(void);

#endif // LED_SOCK__H__
