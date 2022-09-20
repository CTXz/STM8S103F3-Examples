# Blink with delay written in assembly <!-- omit in toc -->

The following example blinks the built in LED of this [this blue STM8S103F3 devboard](https://www.aliexpress.com/item/1005004514078858.html?spm=a2g0o.productlist.main.7.5b6f20c9INeEUu&algo_pvid=e4ea4e0a-c28e-4b91-895d-2a02f8af5d90&algo_exp_id=e4ea4e0a-c28e-4b91-895d-2a02f8af5d90-3&pdp_ext_f=%7B%22sku_id%22%3A%2212000029432042609%22%7D&pdp_npi=2%40dis%21EUR%211.31%211.31%21%21%21%21%21%40211bf3f116631655842315357d071d%2112000029432042609%21sea&curPageLogUid=TCv6XDktNh7d). To achieve delays, this example provides a `delay_ms` function written in assembly which wastes the necessary amount of CPU cycles for the given delay. With this method timers are not required, however, at the expense of blocking the CPU for the duration of the delay, as well as not being as accurate (See the [Delay](#delay-includedelay_msh-srcdelay_msc) section for more details).

## Table of Contents <!-- omit in toc -->

- [Hardware Setup](#hardware-setup)
- [Software](#software)
	- [Configuration: src/stm8s_conf.h](#configuration-srcstm8s_confh)
	- [Delay: include/delay_ms.h, src/delay_ms.c](#delay-includedelay_msh-srcdelay_msc)
	- [Main: src/main.c](#main-srcmainc)

## Hardware Setup

Since this example only utilizes the built-in LED, no additional hardware besides the STM8S103F3 devboard and a programmer are required.

## Software

### Configuration: [src/stm8s_conf.h](src/stm8s_conf.h)

Since this example makes use of GPIOs, we must uncomment the GPIO module in the configuration header:

```c
#include "stm8s_gpio.h"
```

### Delay: [include/delay_ms.h](include/delay_ms.h), [src/delay_ms.c](src/delay_ms.c)

The `delay_ms` function is provided by the [`delay_ms.h`](include/delay_ms.h) header and defined in the [`delay_ms.c`](include/delay_ms.c) file. The [`delay_ms.h`](include/delay_ms.h) header will throw a compiler error if the `F_CPU` macro is not defined. This macro is used to calculate the number of CPU cycles to waste for the given delay. It must be specified in the [`platformio.ini`](platformio.ini) file. In our example we use the default clock speed of 2 MHz:

```ini
board_build.f_cpu = 2000000UL
```

In the [`delay_ms.c`](src/delay_ms.c) file we define the `delay_ms` function, along with a few constants and variables:

```c
// Number of instructions required for a single loop with a decrementing register value
// decw(1) & jrne(2) = 3 instructions
#define CYCLES_PER_LOOP 3

const uint16_t LOOPS_PER_MS = F_CPU/CYCLES_PER_LOOP/1000;  // Number of loops required for a millisecond to pass
volatile uint16_t _ms;					   // Required to pass ms parameter to asm code

void delay_ms(uint16_t ms) {
	_ms = ms;
	__asm
		ldw y, __ms 		// Load ms counter into y register: 2 cycles
	0000$:
		ldw x, _LOOPS_PER_MS 	// Load loop counter into x register: 2 cycles
	0001$: 				
		decw x 			// Decrease loop counter: 1 cycle 
		jrne 0001$		// Check if 1ms passed: 2 cycles (except for x == 0 where it's just 1 cycle)
				
		decw y			// Decrease ms counter: 1 cycle
		jrne 0000$		// Check if provided time has passed: 2 cycles (except for y == 0 where it's just 1 cycle)
	__endasm;
}
```

The `CYCLES_PER_LOOP` constant is used to make the code more readable and tells how many CPU cycles are required for a simple loop that decrements the `x` register. The number of cycles per instructions can be determined from the [STM8 CPU programming manual](https://www.st.com/resource/en/programming_manual/pm0044-stm8-cpu-programming-manual-stmicroelectronics.pdf).

The `LOOPS_PER_MS` constant is used to calculate the number of loops required for a millisecond to pass. This is calculated by dividing the CPU frequency by the number of cycles required for a single loop, and then dividing that by 1000 to get the number of loops required for a millisecond to pass.

The `_ms` variable is used to pass the `ms` parameter to the assembly code. I have yet to find a better way to parse parameters to a SDCC inline assembly block without needing to manually mess with the stack, so parsing the parameter to a global var, which then can be accessed within the inline asm code using the `_` prefix is the best solution I came up with.

The `delay_ms` function itself is pretty simple. It first loads the `ms` parameter into the `y` register, and then enters a loop that decrements the `x` register. The `x` register is decremented until it reaches 0, at which point the `y` register is decremented. This process is repeated until the `y` register reaches 0, at which point the delay is complete.

The `y` register acts as a millisecond countdown and gets decreased by 1 after each millisecond has passed. The `x` register is set so that almost exactly a millisecond has passed once it has been decremented to 0.

So what we really have here is a nested loop, with the outer loop counting the milliseconds, and the inner loop wasting just the exact ammount CPU cycles to make a millisecond pass.

While this yields quite accurate results, it will still not be perfect, as `3` extra cycles will be added after each millisecond has passed due to the `decw y` and `jrne 0000$` instructions. There's also a minimal overhead from the function call itself, and lastly, some pipeline magic (we'll not dive into this here) will also slightly effect the precision of the delay function. The extra cycles generated after each ms also means that error becomes more significant the longer the delay is.

For better precision, the [blink_delay_timer](../blink_delay_timer) example covers how a delay can be implemented using the STM8S's integrated timers.

### Main: [src/main.c](src/main.c)

At the top of the main file we first check if the `F_CPU` macro really matches the default clock speed:
```c
#if F_CPU != 2000000UL
#error F_CPU set to wrong value! This example runs on 2MHz!
#error Please set the f_cpu option the platformio.ini file to 2000000UL!
#endif
```

Unfortunately PlatformIO sets the F_CPU macro to 16000000UL by default, even if the clock speed has not been set/initialized in the firmware. To be extra sure, we check if the user has set the `F_CPU` macro to the correct value in the [`platformio.ini`](platformio.ini) file.

We then define a few constants to make the code more readable:

```c
// Built-in LED (Pin B5, Active Low)
#define LED_BUILTIN_PORT GPIOB
#define LED_BUILTIN_PIN  GPIO_PIN_5
```

Lastly we enter the `main` function where we initialize the GPIOs for the built-in LED, and then enter a infinite loop that alternates the LED every second using the `delay_ms` function:

```c
void main(void)
{
	GPIO_Init(LED_BUILTIN_PORT, LED_BUILTIN_PIN, GPIO_MODE_OUT_PP_LOW_FAST); // Built-in LED: Output, Push Pull, Low level, 10MHz

	while(TRUE)
	{
		GPIO_WriteReverse(LED_BUILTIN_PORT, LED_BUILTIN_PIN); // Toggle built-in LED
		delay_ms(1000);
	}
}

```