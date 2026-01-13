from machine import Pin
import time

# Initialize LED (GPIO25, output mode)
led = Pin(25, Pin.OUT)
# Initialize button (GPIO16, input mode + pull-up resistor, default high level)
button = Pin(16, Pin.IN, Pin.PULL_UP)

# Initial state: LED off, button not pressed, LED state flag
led.value(0)
button_last_state = 1  # Record the last state of the button (initial high level)
led_state = 0          # Record current LED state (0 = off, 1 = on)
debounce_time = 20     # Debounce time (milliseconds)

while True:
    # Read current button state
    button_current_state = button.value()
    
    # Detect button level change (high → low, i.e., button pressed) and filter debounce
    if button_current_state != button_last_state:
        time.sleep_ms(debounce_time)  # Wait for debounce to settle
        # Reconfirm button state (exclude jitter)
        button_current_state = button.value()
        # Confirm valid press (low level)
        if button_current_state == 0 and button_last_state == 1:
            # Toggle LED state (0 → 1, 1 → 0)
            led_state = not led_state
            led.value(led_state)
            print("LED state toggled to:", "ON" if led_state else "OFF")
    
    # Update last button state
    button_last_state = button_current_state
    time.sleep_ms(10)  # Short delay in main loop to reduce CPU usage
