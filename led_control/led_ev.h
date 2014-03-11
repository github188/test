#ifndef LED_SOCK__H__
#define LED_SOCK__H__

#define PORT (9001)
#define IP ("127.0.0.1")

extern struct ev_loop *led_loop;
extern int sts[];
void led_ev_init(void);
void led_ev_release(void);

#endif // LED_SOCK__H__
