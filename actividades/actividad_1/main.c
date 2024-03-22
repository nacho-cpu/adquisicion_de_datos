// Inclusion de bibliotecas
#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lcd_i2c.h"

/*
 * @brief Programa principal
 */ 
int main() {
    // Variable para almacenar el resultado del ADC
    uint16_t adc_value = 0;
    // Variable para guardar el valor de temperatura
    float temperatura = 0.0;
    // Constante de proporcionalidad del termistor
    const uint16_t beta = 3950;
    // Habilito USB
    stdio_init_all();
    // Configuro el I2C0 a 100 KHz de clock
    i2c_init(i2c0, 100 * 1000);
    // Elijo GPIO4 como linea de SDA
    gpio_set_function(4, GPIO_FUNC_I2C);
    // Elijo GPIO5 como linea de SCL
    gpio_set_function(5, GPIO_FUNC_I2C);
    // Activo pull-up en ambos GPIO, son debiles por lo que
    // es recomendable usar pull-ups externas
    gpio_pull_up(4);
    gpio_pull_up(5);
    // Inicializacion del LCD
    lcd_init();
    // Inicializo ADC
    adc_init();
    // Inicializo GPIO26 como entrada analogica
    adc_gpio_init(26);
    // Selecciono canal analogico
    adc_select_input(0);
    while(true) {
        // Leer NTC
        uint16_t val = adc_read();
        float V_val = val*3.3/4095;
        float R_ntc = 4700.0/((3.3/V_val)-1);
        //pongo la variable del promedio en 0
        float Ptemp = 0.0;
        for (int i = 0; i < 11; ++i)
        {
           // Calculo temperatura
            float T = 1 / (log(R_ntc/10000.0)/3900.0+1/298.15);
            float Temp = T - 273.15;
            //sumamos las 10 temperaturas
            Ptemp += Temp
        }
        //saco el promedio de las 10 temperaturas leidas
        Ptemp = Ptemp/10;
        //imprimo el valor de las variable
        printf("El valor leido por el ADC es de %d\n", val);
        printf("La temperatura es de %.2f\n", Ptemp); 
        // Espero 500 ms
        sleep_ms(500);
    }
    return 0;
}