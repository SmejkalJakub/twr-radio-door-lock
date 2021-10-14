#include <application.h>

// LED instance
twr_led_t led;

// Relay Instance
twr_module_relay_t relay;

// Led strip
twr_led_strip_t led_strip;
static uint32_t _dma_buffer[144 * 4 * 2]; // count * type * 2
const twr_led_strip_buffer_t _led_strip_buffer =
        {
                .type = TWR_LED_STRIP_TYPE_RGBW,
                .count = 144,
                .buffer = _dma_buffer
        };

void _twr_led_strip_fill(uint8_t r, uint8_t g, uint8_t b, uint8_t w);


void twr_radio_node_on_state_set(uint64_t *id, uint8_t state_id, bool *state)
{
    (void) id;

    if (state_id == TWR_RADIO_NODE_STATE_POWER_MODULE_RELAY)
    {
        twr_led_blink(&led, 1);
        twr_module_power_relay_set_state(*state);

        if(!(*state))
        {
            _twr_led_strip_fill(255, 0, 0, 0);
        }
        else
        {
            _twr_led_strip_fill(0, 255, 0, 0);

        }

        twr_radio_pub_state(TWR_RADIO_NODE_STATE_POWER_MODULE_RELAY, state);
    }
}

void _twr_led_strip_fill(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    for (int i = 0; i < _led_strip_buffer.count; ++i) {
        twr_led_strip_set_pixel_rgbw(&led_strip, i, r, g, b, w);
    }

    twr_led_strip_write(&led_strip);
}

void application_init(void)
{

    // Initialize Power Module
    twr_module_power_init();

    // Initialize led-strip on power module
    twr_led_strip_init(&led_strip, twr_module_power_get_led_strip_driver(), &_led_strip_buffer);
    twr_led_strip_set_brightness(&led_strip, 10);

    twr_radio_init(TWR_RADIO_MODE_NODE_LISTENING);
    twr_radio_pairing_request("wireless-lock", VERSION);
}

