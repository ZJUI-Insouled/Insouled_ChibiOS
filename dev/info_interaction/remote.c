//
// Created by Lan Tian on 18/03/2018.
//

#include "remote.h"

void remoteInit(void) {
    uartStart(&REMOTE_UART_PORT, &remoteUartConfig);
    palSetPadMode(GPIOB, 7, PAL_MODE_ALTERNATE(7));
    uartStartReceive(&REMOTE_UART_PORT, REMOTE_DATA_SIZE, remoteData);
}

void remoteReceived(UARTDriver *uartp) {
    (void) uartp;

    remote.ch0 = (((remoteData[0] | remoteData[1] << 8) & 0x07FF) - 1024) / 660.0f;
    ABS_LIMIT(remote.ch0, 1.0f);
    remote.ch1 = (((remoteData[1] >> 3 | remoteData[2] << 5) & 0x07FF) - 1024) / 660.0f;
    ABS_LIMIT(remote.ch1, 1.0f);
    remote.ch2 = (((remoteData[2] >> 6 | remoteData[3] << 2 | remoteData[4] << 10) & 0x07FF) - 1024) / 660.0f;
    ABS_LIMIT(remote.ch2, 1.0f);
    remote.ch3 = (((remoteData[4] >> 1 | remoteData[5] << 7) & 0x07FF) - 1024) / 660.0f;
    ABS_LIMIT(remote.ch3, 1.0f);

    remote.left_lever = ((remoteData[5] >> 4) & 0x000C) >> 2;
    remote.right_lever = (remoteData[5] >> 4) & 0x0003;

    if(remote.ch0 > 0.5f) {
        LED_G_ON();
    } else {
        LED_G_OFF();
    }
    if(remote.ch0 < -0.5f) {
        LED_R_ON();
    } else {
        LED_R_OFF();
    }

    uartStartReceive(&REMOTE_UART_PORT, REMOTE_DATA_SIZE, remoteData);
}
