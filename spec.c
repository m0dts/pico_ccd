/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/adc.h"
#include "spec.pio.h"
#include <stdlib.h>


int main() {
    stdio_init_all();
    adc_init();

    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    // Select ADC input 0 (GPIO26)
    adc_select_input(0);

    PIO pio = pio0;
    uint offset = pio_add_program(pio, &spec_program);
    uint sm = pio_claim_unused_sm(pio, true);
    spec_program_init(pio, sm, offset, 0);

    //set default pin values
    int val = 0;
    val = (val |(0x2 | 0x4 | 0x8) ) &  ~0x10;    //ph2,RB,CLB high, TG low
    pio_sm_put_blocking(pio, sm, val);


    bool newline=true;
    int counter=0;
    unsigned char ccd_trace[10600];
    char inbuf[4] = "200\0";

    for (int i=0;i<10600;i++){
            ccd_trace[i]=1;
    }

    while (true) {
        
        //new line start
        if (newline){
            counter=0;
            val |=0x10;     //TG high
            pio_sm_put_blocking(pio, sm, val);
            val =(val |0x1) & ~0x2;     //ph high
            pio_sm_put_blocking(pio, sm, val);
            sleep_us(1);
            for (int i=0;i<5;i++){
                val &=~0x4;     //RB low
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val |=0x4;     //RB high
                pio_sm_put_blocking(pio, sm, val);
                val &=~0x8;     //CLB low
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val |=0x8;     //CLB high
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(2);
            }
            val =(val |0x2) & ~0x1;     //ph low
            pio_sm_put_blocking(pio, sm, val);
            val &=~0x10;     //TG low
            pio_sm_put_blocking(pio, sm, val);
            sleep_us(1);

            //sleep_ms(50);
            newline=false;

        }else{

            for (int i=0;i<13;i++){
                val =(val |0x1) & ~0x2;     //ph high
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val &=~0x4;     //RB low
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val |=0x4;     //RB high
                pio_sm_put_blocking(pio, sm, val);
                val &=~0x8;     //CLB low
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val |=0x8;     //CLB high
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val =(val |0x2) & ~0x1;     //ph low
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
            }

            for (int i=0;i<50;i++){
                val =(val |0x1) & ~0x2;     //ph high
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val &=~0x4;     //RB low
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val |=0x4;     //RB high
                pio_sm_put_blocking(pio, sm, val);
                val &=~0x8;     //CLB low
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val |=0x8;     //CLB high
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val =(val |0x2) & ~0x1;     //ph low
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
            }

            for (long int i=0;i<10600;i++){
                val =(val |0x1) & ~0x2;     //ph high
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val &=~0x4;     //RB low
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val |=0x4;     //RB high
                pio_sm_put_blocking(pio, sm, val);
                val &=~0x8;     //CLB low
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val |=0x8;     //CLB high
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val =(val |0x2) & ~0x1;     //ph low
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                ccd_trace[i]=adc_read()>>4;
                
            }


            for (int i=0;i<10;i++){
                val =(val |0x1) & ~0x2;     //ph high
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val &=~0x4;     //RB low
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val |=0x4;     //RB high
                pio_sm_put_blocking(pio, sm, val);
                val &=~0x8;     //CLB low
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val |=0x8;     //CLB high
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
                val =(val |0x2) & ~0x1;     //ph low
                pio_sm_put_blocking(pio, sm, val);
                sleep_us(1);
            }

            newline=true;
            //printf("s");
  //          for (int i=0;i<10600;i++){
          //      if(i==10000)
                   // printf("%i\n",ccd_trace[i]);
  //                  putchar(ccd_trace[i]);//,1,1,stdout);
                    //sleep_us(5000);
  //          }

            //sleep_ms(5000);
            //printf("%i\n",sizeof(ccd_trace));
            ccd_trace[0]=255;
            fwrite(ccd_trace,1,sizeof(ccd_trace),stdout);
            //fwrite(0,1,1,stdout);
            //fflush(stdout);
            //fwrite(&ccd_trace,4001,4000,stdout);
            //fwrite(&ccd_trace,8001,2600,stdout);
            //fwrite(255,1,1,stdout);
            //for(;;){}
            int count=0;
            int c = getchar_timeout_us(100);
            while (c != PICO_ERROR_TIMEOUT  && (c & 0xFF)!='\n') {
                inbuf[count] = (c & 0xFF);
                c = getchar_timeout_us(100);
                count++;
            }
            sleep_ms(atoi(inbuf));
           // printf("Trace done...\n");
        }
    }

}
