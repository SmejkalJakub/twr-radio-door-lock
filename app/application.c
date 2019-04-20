#include <application.h>

// LED instance
bc_led_t led;

// Relay Instance
bc_module_relay_t relay;

// Led strip
bc_led_strip_t led_strip;
static uint32_t _dma_buffer[144 * 4 * 2]; // count * type * 2
const bc_led_strip_buffer_t _led_strip_buffer =
        {
                .type = BC_LED_STRIP_TYPE_RGBW,
                .count = 144,
                .buffer = _dma_buffer
        };

void _bc_led_strip_fill(uint8_t r, uint8_t g, uint8_t b, uint8_t w);


void bc_radio_node_on_state_set(uint64_t *id, uint8_t state_id, bool *state)
{
    (void) id;

    if (state_id == BC_RADIO_NODE_STATE_POWER_MODULE_RELAY)
    {
        bc_led_blink(&led, 1);
        bc_module_power_relay_set_state(*state);
        
        if(!(*state))
        {
            _bc_led_strip_fill(255, 0, 0, 0);
        }
        else
        {
            _bc_led_strip_fill(0, 255, 0, 0);

        }

        bc_radio_pub_state(BC_RADIO_NODE_STATE_POWER_MODULE_RELAY, state);
    }
}

void _bc_led_strip_fill(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    for (int i = 0; i < _led_strip_buffer.count; ++i) {
        bc_led_strip_set_pixel_rgbw(&led_strip, i, r, g, b, w);
    }

    bc_led_strip_write(&led_strip);
}

void application_init(void)
{

    // Initialize Power Module
    bc_module_power_init();

    // Initialize led-strip on power module
    bc_led_strip_init(&led_strip, bc_module_power_get_led_strip_driver(), &_led_strip_buffer);
    bc_led_strip_set_brightness(&led_strip, 10);

    bc_radio_init(BC_RADIO_MODE_NODE_LISTENING);
    bc_radio_pairing_request("wireless-lock", VERSION);
}

