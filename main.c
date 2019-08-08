#include "mcc_generated_files/mcc.h"
#include "mrubyc.h"
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"

extern const uint8_t sample1[];
#define MEMORY_SIZE (1024*20)
static uint8_t memory_pool[MEMORY_SIZE];

/*
                         Main application
 */
static void c_ra0(mrb_vm *vm, mrb_value *v, int argc) {
    SET_INT_RETURN(PORTBbits.RB6);
}
static void c_rb5(mrb_vm *vm, mrb_value *v, int argc) {
    int set_value = GET_INT_ARG(1);
    PORTBbits.RB5 = set_value;
}
static void c_rb4(mrb_vm *vm, mrb_value *v, int argc) {
    int set_value = GET_INT_ARG(1);
    PORTBbits.RB4 = set_value;
}
static void c_ra4(mrb_vm *vm, mrb_value *v, int argc) {
    int set_value = GET_INT_ARG(1);
    PORTAbits.RA4 = set_value;
}

static void c_puts(mrb_vm *vm, mrb_value *v, int argc) {
    char *mo = mrbc_string_cstr(&v[1]);
    int i = 0;
    while(i<mrbc_string_size(&v[1])){
        UART1_Write(mo[i]);
        i++;
    }
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

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB6 = 1;
    TRISAbits.TRISA4 = 0;
    

    mrbc_init(memory_pool, MEMORY_SIZE);
    mrbc_define_method(0, mrbc_class_object, "rb5", c_rb5);
    mrbc_define_method(0, mrbc_class_object, "ra0", c_ra0);
    mrbc_define_method(0, mrbc_class_object, "rb4", c_rb4);
    mrbc_define_method(0, mrbc_class_object, "ra4", c_ra4);
    mrbc_define_method(0, mrbc_class_object, "pri", c_puts);
    mrbc_create_task(sample1, 0);
    mrbc_run();
    return 1;
}
