#include <application.h>

// LED instance
bc_led_t led;


void application_init(void)
{
    // Initialize logging
    bc_log_init(BC_LOG_LEVEL_DUMP, BC_LOG_TIMESTAMP_ABS);
    
    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_set_mode(&led, BC_LED_MODE_ON);

    bc_radio_init(BC_RADIO_MODE_NODE_LISTENING);
    bc_radio_pairing_request("wireless-lock", VERSION);

}

