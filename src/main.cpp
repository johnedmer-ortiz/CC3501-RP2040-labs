

#include <stdio.h>
#include "pico/stdlib.h"
#include "accel_task.h"
#include "microphone_task.h"

#define SW1 15

volatile bool stop_task = false;
volatile int task_index = 0;
static int number_of_tasks = 4;

void increment_task_number(int number_tasks)
{
    task_index = (task_index + 1) % number_tasks;
}

void switch_task_interrupt(uint gpio, uint32_t events)
{
    stop_task = true;
    increment_task_number(number_of_tasks);
}

int main()
{
    stdio_init_all();
    gpio_init(SW1);

    gpio_set_irq_enabled_with_callback(SW1, GPIO_IRQ_EDGE_FALL, true, &switch_task_interrupt);
    task_index = 0;
    while (true)
    {
        stop_task = false;
        switch (task_index)
        {
        case 0:
            accel_task_run();
            break;
        case 1:
            microphone_task_run();
            break;
        default:
            break;
        }
    }

    return 0;
}