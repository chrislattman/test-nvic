#include <hardware/irq.h>
#include <hardware/sync.h>
#include <hardware/timer.h>
#include <pico/stdlib.h>
#include <stdio.h>

// Define IRQ lines
#define HIGH_PRIO_TIMER_IRQ TIMER0_IRQ_0    // Hardware Timer IRQ 0
#define LOW_PRIO_SW_IRQ     SPARE_IRQ_0     // Using an available user/software IRQ slot

// Define NVIC Priorities (Lower number = higher priority)
#define PRIORITY_HIGH       0x00 
#define PRIORITY_LOW        0x80

// Global shared resource
volatile uint32_t shared_resource = 0;

// High Priority ISR: Modifies resource and triggers Low Priority ISR
void high_priority_isr(void) {
    // Clear the hardware timer interrupt flag
    timer_hw->intr = (1u << 0);

    // Schedule the next alarm event for exactly 1 second later
    timer_hw->alarm[0] = timer_hw->timerawl + 1000000;

    // Modify the shared resource
    ++shared_resource;

    // Software-trigger the low priority NVIC interrupt
    irq_set_pending(LOW_PRIO_SW_IRQ);
}

// Low Priority ISR: Reads resource inside a critical section
void low_priority_isr(void) {
    // --- CRITICAL SECTION START ---
    // Disable the high-priority IRQ at the NVIC level
    irq_set_enabled(HIGH_PRIO_TIMER_IRQ, false);
    __compiler_memory_barrier(); // Prevent compiler from reordering reads/writes

    // Safely read or manipulate the shared resource
    uint32_t local_copy = shared_resource;

    // Restore the high-priority IRQ
    __compiler_memory_barrier();
    irq_set_enabled(HIGH_PRIO_TIMER_IRQ, true);
    // --- CRITICAL SECTION END ---

    printf("[LOW ISR] Read resource safely: %lu\n", local_copy);
}

int main() {
    stdio_init_all();

    // 1. Configure Low-Priority Software Interrupt
    irq_set_exclusive_handler(LOW_PRIO_SW_IRQ, low_priority_isr);
    irq_set_priority(LOW_PRIO_SW_IRQ, PRIORITY_LOW);
    irq_set_enabled(LOW_PRIO_SW_IRQ, true);

    // 2. Configure High-Priority Hardware Timer Interrupt
    irq_set_exclusive_handler(HIGH_PRIO_TIMER_IRQ, high_priority_isr);
    irq_set_priority(HIGH_PRIO_TIMER_IRQ, PRIORITY_HIGH);
    irq_set_enabled(HIGH_PRIO_TIMER_IRQ, true);

    // 3. Alarm configuration: fire high-priority IRQ every 1 second (1,000,000 us)
    timer_hw->alarm[0] = timer_hw->timerawl + 1000000;
    timer_hw->inte |= (1u << 0); // Enable Alarm 0 interrupt source

    while (1) {
        __wfi();
    }
}
