#pragma once

#define LGFX_USE_V1
#include "Arduino.h"
#include <LovyanGFX.hpp>

#include "pins.h"


class LGFX : public lgfx::LGFX_Device
{

  lgfx::Panel_GC9A01 _panel_instance;
  lgfx::Light_PWM _light_instance;
  lgfx::Bus_SPI _bus_instance;
  lgfx::Touch_CST816S _touch_instance;

public:
  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();

      // SPI bus settings for DMA - use only properties supported in LovyanGFX 1.1.16
      cfg.spi_host = SPI2_HOST;  // For ESP32-S3
      cfg.spi_mode = 0;
      cfg.freq_write = 80000000;  // Maximum frequency for good stability
      cfg.freq_read = 20000000;
      cfg.use_lock = true;
      cfg.dma_channel = 1;       // Use explicit DMA channel
      cfg.pin_sclk = SCLK;
      cfg.pin_mosi = MOSI;
      cfg.pin_miso = MISO;
      cfg.pin_dc = DC;

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance); // Set the bus to the panel.
    }

    {                                      // Set the display panel control.
      auto cfg = _panel_instance.config(); // Get the structure for display panel settings.

      cfg.pin_cs = CS;   // Pin number to which CS is connected (-1 = disable)
      cfg.pin_rst = RST; // Pin number to which RST is connected (-1 = disable)
      cfg.pin_busy = -1; // Pin number to which BUSY is connected (-1 = disable)

      /* The following settings are set to general initial values ​​for each panel, so try commenting out any items you are unsure of. */

      cfg.memory_width = SCREEN_WIDTH;   // Maximum width supported by driver IC
      cfg.memory_height = SCREEN_HEIGHT; // Maximum height supported by driver IC
      cfg.panel_width = SCREEN_WIDTH;    // Actual displayable width
      cfg.panel_height = SCREEN_HEIGHT;  // Actual displayable height
      cfg.offset_x = OFFSET_X;    // Panel offset in X direction
      cfg.offset_y = OFFSET_Y;    // Panel offset in Y direction
      cfg.offset_rotation = 0;    // Value 0~7 in rotation direction (4~7 is inverted)
      cfg.dummy_read_pixel = 8;   // Virtual number of positions read before reading image
      cfg.dummy_read_bits = 1;    // The number of imaginary words other than the image element
      cfg.readable = false;       // As long as the number of acquisitions is as high as possible, the setting is true
      cfg.invert = true;          // As a result, the brightness and darkness of the face plate is reversed, and the setting is true
      cfg.rgb_order = RGB_ORDER;  // As a result, the red color and the blue color are replaced on the face plate, and the setting is true
      cfg.dlen_16bit = false;     // From 16th position to 16th position, the length of the number of transfers is set to true
      cfg.bus_shared = false;     // How to use drawJpgFile (e.g. summary control)

      _panel_instance.config(cfg);
    }

    { // Set backlight control. (delete if not necessary)

      auto cfg = _light_instance.config(); // Get the structure for backlight configuration.

      cfg.pin_bl = BL;     // pin number to which the backlight is connected
      cfg.invert = false;  // true to invert backlight brightness
      cfg.freq = 44100;    // backlight PWM frequency
      cfg.pwm_channel = 1; // PWM channel number to use

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance); // Sets the backlight to the panel.
    }

    { // Sets touchscreen control. (Delete if not needed)

      auto cfg = _touch_instance.config();
      cfg.x_min = 0;        // Minimum X value obtained from touch screen (raw value)
      cfg.x_max = SCREEN_WIDTH;    // Maximum X value obtained from touch screen (raw value)
      cfg.y_min = 0;        // Minimum Y value obtained from touch screen (raw value)
      cfg.y_max = SCREEN_HEIGHT;   // Maximum Y value obtained from touch screen (raw value)
      cfg.pin_int = TP_INT; // Pin number to which INT is connected
      cfg.pin_rst = TP_RST;
      cfg.bus_shared = true;   // Set true if using a common bus with the screen
      cfg.offset_rotation = 0; // Adjust if display and touch orientation do not match. Set to a value between 0 and 7

      cfg.i2c_port = 0;      // Select the I2C to use (0 or 1)
      cfg.i2c_addr = 0x15;   // I2C device address number
      cfg.pin_sda = I2C_SDA; // Pin number to which SDA is connected
      cfg.pin_scl = I2C_SCL; // Pin number to which SCL is connected
      cfg.freq = 400000;     // Set the I2C clock
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance); // Set the touch screen to the panel.
    }

    setPanel(&_panel_instance); // Set the panel to use.
  }
};

LGFX tft;
