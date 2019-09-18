#include "mcc_generated_files/mcc.h"
#include "mrubyc.h"
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"
#include "string.h"
#include <math.h>

extern const uint8_t sample1[];
#define MEMORY_SIZE (1024*20)
static uint8_t memory_pool[MEMORY_SIZE];


uint8_t sample[255] = {
0x52,0x49,0x54,0x45,0x30,0x30,0x30,0x34,0x9d,0x71,0x00,0x00,0x00,0x92,0x4d,0x41,
0x54,0x5a,0x30,0x30,0x30,0x30,0x49,0x52,0x45,0x50,0x00,0x00,0x00,0x74,0x30,0x30,
0x30,0x30,0x00,0x00,0x00,0x6c,0x00,0x01,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x11,
0x00,0x40,0x06,0x17,0x00,0x80,0x00,0x06,0x01,0x40,0x00,0x03,0x00,0x80,0x00,0xa0,
0x00,0x80,0x00,0x06,0x01,0x40,0x00,0x03,0x00,0x80,0x40,0xa0,0x00,0x80,0x00,0x06,
0x01,0x3f,0xff,0x83,0x00,0x80,0x00,0xa0,0x00,0x80,0x00,0x06,0x01,0x40,0x00,0x03,
0x00,0x80,0x40,0xa0,0x00,0x80,0x00,0x07,0x00,0xbf,0xf9,0x18,0x00,0x80,0x00,0x05,
0x00,0x00,0x00,0x4a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x03,0x72,0x62,
0x35,0x00,0x00,0x05,0x73,0x6c,0x65,0x65,0x70,0x00,0x45,0x4e,0x44,0x00,0x00,0x00,
0x00,0x08,
};

/* UART_1 mesods */
static void u_puts(char *mo, int size){
    int i = 0;
    if(size == 0){
        size = strlen(mo);
    }
    while (U1STAbits.TRMT == 0);
    while(i<size){
        if(mo[i]==0){break;}
        while (U1STAbits.TRMT == 0);
        U1TXREG= mo[i];
        i++;
    }
}

int u_read(char *addr){
    int i = 0;
    while (1) {
        if ( UART1_ReceiveBufferIsEmpty() == false ) { 
            addr[i] = UART1_Read();
            if(addr[i] == '\n'){
                break;
            }
            i++;
        }
    }
    return i;
}

/* Digital IO mesods */
static void c_btn(mrb_vm *vm, mrb_value *v, int argc) {
    SET_INT_RETURN(PORTBbits.RB7);
}
static void c_leds(mrb_vm *vm, mrb_value *v, int argc) {
    int set_value = GET_INT_ARG(1);
    PORTB = set_value<<10;
}

static void c_puts(mrb_vm *vm, mrb_value *v, int argc) {
    char *mo = mrbc_string_cstr(&v[1]);
    u_puts(mo,0);
}

/* ADC for mruby/c */

static void c_adc_get(mrb_vm *vm, mrb_value *v, int argc) {
    AD1CHS = 0x90000;
    ADC1_Start();
    SET_FLOAT_RETURN(3.3*ADC1_ConversionResultGet()/1023);
    ADC1_Stop();
}
static void c_adc_get2(mrb_vm *vm, mrb_value *v, int argc) {
    AD1CHS = 0xA0000;
    ADC1_Start();
    SET_FLOAT_RETURN(3.3*ADC1_ConversionResultGet()/1023);
    ADC1_Stop();
}


int hal_write(int fd, const void *buf, int nbytes) {
    int i;
    while (U1STAbits.TRMT == 0);
    for (i = nbytes; i; --i) {
        while (U1STAbits.TRMT == 0);
        U1TXREG= *(char*) buf++;
    }
    return (nbytes);
}

int hal_flush(int fd) {
    return 0;
}

#define FLASH_SAVE_ADDR (0xBD000000)
#define PAGE_SIZE (512)
static uint8_t flashBuffer[PAGE_SIZE / sizeof(uint8_t)]

static uint8_t loadFlush() {
    memset(flashBuffer, 0, sizeof(flashBuffer));
    NVM_WriteRow((void* )flashBuffer, (void *)FLASH_SAVE_ADDR);
    return flashBuffer[0];
}

static int saveFlush(const uint8_t* writeData, uint16_t size) {
    if(size > sizeof(flashBuffer)) {
        return -1;
    }

    memset(flashBuffer, 0, sizeof(flashBuffer));
    memcpy(flashBuffer, writeData, size);

    NVM_ErasePage((void *)FLASH_SAVE_ADDR);
    NVM_WriteRow((void *)FLASH_SAVE_ADDR, (void *)flashBuffer);
}

/* mruby/c writer */

void add_code(void){
    char txt[255];
    char *txt_addr;
    int txt_len;
    txt_addr = txt;
    // [crlf] read waite
    txt_len = u_read(txt_addr);
    u_puts("+OK mruby/c\r\n",0);
    memset(txt, 0, sizeof(txt));
    txt_len = u_read(txt_addr);
    u_puts("+OK mruby/c PSoC_5LP v1.00 (2018/09/04)\r\n",0);
    memset(txt, 0, sizeof(txt));
    
    // bytecode length receive
    txt_len = u_read(txt_addr);
    u_puts("+OK Write bytecode\r\n",0);
    int size = 0;
    txt_len = txt_len-2;
    int j = 0;
    while(txt_len>5){
        size = (txt[txt_len] - 0x30) * pow(10,j) + size;
        j++;
        txt_len--;
    }
    
    // mruby/c code write
    int i = 0;
    memset(sample, 0, sizeof(sample));
    while (size > 0) {
        if ( UART1_ReceiveBufferIsEmpty() == false ) { 
            //UART1_Write(UART1_Read());
            sample[i] = UART1_Read();
            size--;
            i++;
        }
    }
    
    // write success => execut
    u_puts("+DONE\r\n",0);
    memset(txt, 0, sizeof(txt));
    txt_len = u_read(txt_addr);
    u_puts("+OK Execute mruby/c.\r\n",0);
}

/* main program */

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    TRISBbits.TRISB7 = 1;
    TRISBbits.TRISB10 = 0;
    TRISBbits.TRISB11 = 0;
    TRISBbits.TRISB12 = 0;
    TRISBbits.TRISB13 = 0;
    ADC1_Initialize();
    add_code();
    

    mrbc_init(memory_pool, MEMORY_SIZE);
    mrbc_define_method(0, mrbc_class_object, "leds_write", c_leds);
    mrbc_define_method(0, mrbc_class_object, "SW", c_btn);
    mrbc_define_method(0, mrbc_class_object, "adc", c_adc_get);
    mrbc_define_method(0, mrbc_class_object, "adc2", c_adc_get2);
    mrbc_define_method(0, mrbc_class_object, "UART_puts", c_puts);
    mrbc_create_task(sample, 0);
    mrbc_run();
    return 1;
}
