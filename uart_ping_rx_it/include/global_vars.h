#ifndef _GLOBAL_VARS_INCLUDED
#define _GLOBAL_VARS_INCLUDED

// Built-in LED (Pin B5, Active Low)
#define LED_BUILTIN_PORT GPIOB
#define LED_BUILTIN_PIN  GPIO_PIN_5

static const char PING[] = "ping";
static const char PING_RESPONSE[] = "pong\n";

#endif // _GLOBAL_VARS_INCLUDED