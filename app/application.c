#include <application.h>

// LED instance
bc_led_t led;

// Relay Instance
bc_module_relay_t relay;

void bc_radio_node_on_state_set(uint64_t *id, uint8_t state_id, bool *state)
{
    (void) id;

    if (state_id == BC_RADIO_NODE_STATE_RELAY_MODULE_0)
    {
        bc_led_blink(&led, 2);
        bc_module_relay_set_state(&relay, *state);

        bc_radio_pub_state(BC_RADIO_PUB_STATE_RELAY_MODULE_0, state);
    }
}

void application_init(void)
{
    // Initialize logging
    bc_log_init(BC_LOG_LEVEL_DUMP, BC_LOG_TIMESTAMP_ABS);
    
    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_set_mode(&led, BC_LED_MODE_ON);

    // Initialize Relay Module
    bc_module_relay_init(&relay, 0x3B);

    bc_radio_init(BC_RADIO_MODE_NODE_LISTENING);
    bc_radio_pairing_request("wireless-lock", VERSION);

}

