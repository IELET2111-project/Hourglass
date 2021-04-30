/*
 * project.c
 *
 * Created: 19.04.2021 15:09:55
 * Author : espen
 */

#define F_CPU 16000000UL
#define BAUD  9600UL

#include <avr/io.h>
#include "serial.h"


int main(void) {
    initSerial();

    while (1) {

    }
}

