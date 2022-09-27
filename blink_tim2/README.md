# Template Project <!-- omit in toc -->

The following directory provides a template project to quickly get started with STM8S103F3 development in platformio.

## Table of Contents <!-- omit in toc -->

- [Getting Started: src/stm8_conf.h](#getting-started-srcstm8_confh)
- [Prepping the interrupts: src/stm8s_it.c, include/stm8s_it.h](#prepping-the-interrupts-srcstm8s_itc-includestm8s_ith)
	- [A technical note on the interrupt handlers](#a-technical-note-on-the-interrupt-handlers)
- [What's next?](#whats-next)

## Getting Started: [src/stm8_conf.h](src/stm8_conf.h)

The [`stm8_conf.h`](src/stm8s_conf.h) header serves as a configuration file for the STM8S standard peripheral library. It contains a list of headers, or
as some refer to them, "modules", that include a variety of function calls to interact with the STM8S peripherals.

To name a few:

* `stm8s_gpio.h` - GPIOs (Ex. `GPIO_Init()`, `GPIO_WriteHigh()`, `GPIO_WriteLow()`)
* `stm8s_exti.h` - External interrupts (Ex. `EXTI_SetExtIntSensitivity()`)
* `stm8s_clk.h` - Clock (Ex. `CLK_HSIPrescalerConfig()`, `CLK_SYSCLKConfig()`, `CLK_PeripheralClockConfig()`)
* `stm8s_tim1.h` - Timer 1 (Ex. `TIM1_DeInit()`, `TIM1_TimeBaseInit()`, `TIM1_Cmd()`)
* ...

[This website](https://documentation.help/STM8S/) provides a somewhat messy reference for the STM8S standard peripheral library modules.
Alternatively, you can opt to browse through the headers yourself (For ex. [here](https://github.com/bschwand/STM8-SPL-SDCC/tree/master/Libraries/STM8S_StdPeriph_Driver/inc))

> **Note:** It is required to include the [`stm8_conf.h`](src/stm8s_conf.h) header file within the `src/` directory of your project, else the compiler will
> complain. Placing the file in the `include/` directory will not work!

Depending on the project, you must uncomment the necessary modules you need in [`stm8_conf.h`](src/stm8s_conf.h). For example, if you want to use the GPIO functions, you
must uncomment the line:

```c
// #include "stm8s_gpio.h"
```

Some modules may be greyed out. Those are modules that are not supported by the STM8S103F3. As an example, the `stm8s_uart2.h` module is not supported by the STM8S103F3.

> **Note:** Whether intentional or not, some modules that are not supported by the STM8S103F3 may not be greyed out in the [`stm8_conf.h`](src/stm8s_conf.h) header.
> For example, the `stm8s_timer4.h` module is not supported by the STM8S103F3 (as it only has two timers) yet it is not greyed out in [`stm8_conf.h`](src/stm8s_conf.h).
> To avoid confusion, it is highly recommended selecting the STM8S103F3 chip in the STM8CubeMX software to see which peripherals are actually supported.

## Prepping the interrupts: [src/stm8s_it.c](src/stm8s_it.c), [include/stm8s_it.h](include/stm8s_it.h)

The [`stm8s_it.c`](src/stm8s_it.c) and [`stm8s_it.h`](include/stm8s_it.h) files declare and define IRQ handlers. The [`stm8s_it.h`](include/stm8s_it.h) header contains
the necessary IRQ handler prototypes and links them to their appropriate interrupt vectors. The [`stm8s_it.c`](src/stm8s_it.c) file contains the
user defined code for the IRQ handlers.

To put it simply, the [`stm8s_it.c`](src/stm8s_it.c) file is where you will write your interrupt handlers. The [`stm8s_it.h`](include/stm8s_it.h) header remains untouched and must simply be included in your main file.

> **Note:** Not including the [`stm8s_it.h`](include/stm8s_it.h) header file in your main file will result in the interrupt handlers not being called!

> **Note:** Should you not be require interrupts throughout your projects, [`stm8s_it.h`](include/stm8s_it.h) and [`stm8s_it.c`](src/stm8s_it.c) may be omitted. 
> That being said, it does not harm to keep them as they will barely take up any program space.

As an example, suppose we have a LED connected to pin `B5` (Such as the built-in LED on the generic blue STM8S103F3P6 breakout boards). We also
have a button connected to any pin on `PORTD`, with the internal pull-up resistor enabled. For the sake of simplicity, we will reserve the full `PORTD` bank for the button so that we must not test which pin was toggled.

Now we wish to toggle the LED whenever the button is released (that is, on rising edge since we have the internal pull-up resistor enabled). 

We begin by including the `stm8s_exti.h` module in our [`stm8_conf.h`](src/stm8s_conf.h) file. Uncomment the following line inside [`stm8_conf.h`](src/stm8s_conf.h):

```c
// #include "stm8s_exti.h"
```

Within our main file, we must first include the [`stm8s_it.h`](include/stm8s_it.h) header file (This template project already does so):

```c
#include "stm8s_it.h"
```

In the main function we then initialize the GPIOs and the external interrupts.

```c
int main() {
	// Initialize the GPIOs
    	GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST); // LED
	GPIO_Init(GPIOD, GPIO_PIN_ALL, GPIO_MODE_IN_PU_IT); // Button with internal pull-up resistor and external interrupt enabled
```

We then set the external interrupt sensitivity to rising edge and enable the interrupts.

```c
	// Initialize the external interrupts
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_ONLY);
	enableInterrupts();
```

Finally, we let the MCU loop forever.

```c
	while(TRUE);
}
```

All in all our main function should look like this:

```c
int main() {
	// Initialize the GPIOs
    	GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST); // LED
	GPIO_Init(GPIOD, GPIO_PIN_ALL, GPIO_MODE_IN_PU_IT); // Button with internal pull-up resistor and external interrupt enabled

	// Initialize the external interrupts
	EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_ONLY);
	enableInterrupts();

	while(TRUE);
}
```

Now for the last part, we must write the interrupt handler. Within the [`stm8s_it.c`](src/stm8s_it.c) file, search for the following handler:

```c
/**
  * @brief External Interrupt PORTD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
```

Within the function, we simply toggle the LED pin.

```c
/**
  * @brief External Interrupt PORTD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
   	GPIO_WriteReverse(GPIOB, GPIO_PIN_5);
}
```

And that's it! Our LED should now toggle whenever the button is released.

### A technical note on the interrupt handlers

The procedures above may seem a bit convoluted. Why do we need to declare the interrupt handlers in the [`stm8s_it.h`](include/stm8s_it.h) file and then define them in the [`stm8s_it.c`](src/stm8s_it.c) file? Why can't we just define the interrupt handlers in the main file?

In theory we can ommit the `stm8s_it` files and declare the handler within the main file using the `INTERRUPT_HANDLER` macro, declared in `stm8s.h`, which for SDCC expands to:
```c
 #define INTERRUPT_HANDLER(a,b) void a() __interrupt(b)
```

with `a` being the name of the handler, which may be user defined, and `b` being the interrupt vector number. A overview of the interrupt vectors along with their IRQ number and meaning can be found in the [STM8S103F3 datasheet](https://www.st.com/resource/en/datasheet/stm8s103f2.pdf), page 43, Chapter 7 - 7 Interrupt vector mapping, Table 10.

If cross-compiler compatibility is not a concern, then you can also opt to use the SDCC specific interrupt handler declaration to which the `INTERRUPT_HANDLER` macro expands to.

That being said, while we can define the interrupt handlers in the main file, keeping them seperated in the `stm8_it.c` file not only ensures that the developer knows where to look for the interrupt handlers, but also provides a handy reference for the interrupt vectors so that one must not always refer to the datasheet.

## What's next?

Now that hopefully your project is prepared for your needs, you can go ahead and start working on the project. Perhpas as a quick reminder: Do not forget to initialize the peripherals at the beginning of the main function!

Have fun!