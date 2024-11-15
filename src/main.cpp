#include <Arduino.h>

/* LEDC (LED Controller) basic example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO1         (4) // PIN 1
#define LEDC_OUTPUT_IO2         (5) // PIN 2
#define LEDC_CHANNEL1           LEDC_CHANNEL_0
#define LEDC_CHANNEL2           LEDC_CHANNEL_1
#define LEDC_DUTY_RES           LEDC_TIMER_10_BIT // Set duty resolution to 10 bits
#define LEDC_DUTY_VAL           1023 //2^10-1
#define LEDC_FREQUENCY          (50000) // Frequency in Hertz. Set frequency at 50 kHz


/* Warning:
 * For ESP32, ESP32S2, ESP32S3, ESP32C3, ESP32C2, ESP32C6, ESP32H2, ESP32P4 targets,
 * when LEDC_DUTY_RES selects the maximum duty resolution (i.e. value equal to SOC_LEDC_TIMER_BIT_WIDTH),
 * 100% duty cycle is not reachable (duty cannot be set to (2 ** SOC_LEDC_TIMER_BIT_WIDTH)).
 */

static void ledc_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_TIMER_10_BIT,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 4 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = LEDC_OUTPUT_IO1,
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL1,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    ledc_channel_config_t ledc_channe2 = {
        .gpio_num       = LEDC_OUTPUT_IO2,
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL2,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channe2));
    GPIO.func_out_sel_cfg[LEDC_OUTPUT_IO2].inv_sel = 1;//invierte la señal


}

void setup()
{
    ledc_init();
    // inicializo ambos en 50%
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL1, int(LEDC_DUTY_VAL*0.5))); // seteo valor del duty
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL1)); // actualizo para aplicar el cambio
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL2, int(LEDC_DUTY_VAL*0.5))); // idem otro pin
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL2));
}



void loop()
{
    static int i = 0, estado=0;
    delay(10); // para ajustar velocidad
    switch (estado)
    {
        case 0:
            i++;
            if( i==int(LEDC_DUTY_VAL*0.9) )
                estado=1;
            break;
        case 1:
            i--;
            if ( i==int(LEDC_DUTY_VAL*0.1) )
                estado=0;
        default:
            break;
    }

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL1, i);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL1);
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL2, i);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL2);
    GPIO.func_out_sel_cfg[LEDC_OUTPUT_IO2].inv_sel = 1; // invierte la señal
}
