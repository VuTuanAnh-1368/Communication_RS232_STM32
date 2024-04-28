#include "stm32f10x.h"

void USART1_Init(void);
void USART1_SendChar(char c);
void USART1_SendString(const char* str);
char USART1_ReceiveChar(void);

int main(void) {
    // Enable clock for GPIOA and USART1
    RCC->APB2ENR |= (1 << 2) | (1 << 14);

    // Configure PA9 (USART1_TX) as alternate function push-pull
    GPIOA->CRH &= ~((0x3 << 4) | (0x3 << 0)); // Clear CNF9 and MODE9
    GPIOA->CRH |= (0x2 << 4) | (0x3 << 0); // CNF9[1] = 1, MODE9[1:0] = 11

    // Configure PA10 (USART1_RX) as input floating
    GPIOA->CRH &= ~((0x3 << 8) | (0x3 << 4)); // Clear CNF10 and MODE10
    GPIOA->CRH |= (0x1 << 8); // CNF10[0] = 1

    USART1_Init();

    // Main loop
    while (1) {
        // Send "Hello Esp"
        USART1_SendString("Hello Esp");
        for (volatile int i = 0; i < 1000000; i++);

        // Check if data received and echo back
        if (USART1->SR & (1 << 5)) { // Check if the receive buffer is not empty
             char receivedChar = USART1_ReceiveChar(); // Receive char
             USART1_SendChar(receivedChar); // Echo received char
        }
    }
}

void USART1_Init(void) {
    // Baud rate: Assuming 8MHz clock, 115200 baud rate
    USART1->BRR = 72000000 / 115200;  // Mantissa: , Fraction: 

    // Enable USART, transmitter, and receiver
    USART1->CR1 |= (1 << 13) | (1 << 3) | (1 << 2); // UE = bit 13, TE = bit 3, RE = bit 2
}

void USART1_SendChar(char c) {
    while (!(USART1->SR & (1 << 7))); // Wait until TX is empty
    USART1->DR = (uint16_t)c; // Transmit char
}

void USART1_SendString(const char* str) {
    while (*str) {
        USART1_SendChar(*str++);
    }
}

char USART1_ReceiveChar(void) {
    while (!(USART1->SR & (1 << 5))); // Wait until RX is not empty
    return (char)(USART1->DR & 0xFF); // Read the received data
}
