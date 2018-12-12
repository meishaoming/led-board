#include "uart.h"

#define UART_NUM    3

static uart_t *uarts[UART_NUM];

static int get_uart_id(USART_TypeDef *Instance)
{
    if (Instance == USART1) {
        return 0;
    }
    if (Instance == USART2) {
        return 1;
    }
#ifdef USART3
    if (Instance == USART3) {
        return 2;
    }
#endif
    return -1;
}

static void uart_irq(USART_TypeDef *Instance)
{
    int id = get_uart_id(Instance);
    if (id < 0) {
        return;
    }

    uart_t *uart = uarts[id];

    if (uart && uart->isr) {
        uart->isr(uart);
    }
}

void USART1_IRQHandler(void)
{
    uart_irq(USART1);
}

void USART2_IRQHandler(void)
{
    uart_irq(USART2);
}

#if defined(USART3)
void USART3_IRQHandler(void)
{
    uart_irq(USART3);
}
#endif

int uart_init(uart_t *uart, UART_HandleTypeDef *huart, void (*isr)(uart_t *uart))
{
    int id = get_uart_id(huart->Instance);
    if (id < 0) {
        return -1;
    }
    uart->huart = huart;
    uart->isr = isr;
    uarts[id] = uart;
    return 0;
}

void uart_tx_irq_enable(uart_t *uart)
{
    UART_HandleTypeDef *huart = uart->huart;

    __HAL_UART_ENABLE_IT(huart, UART_IT_TXE);
}

void uart_tx_irq_disable(uart_t *uart)
{
    UART_HandleTypeDef *huart = uart->huart;

    __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);
}

void uart_rx_irq_enable(uart_t *uart)
{
    UART_HandleTypeDef *huart = uart->huart;

    __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
}

void uart_rx_irq_disable(uart_t *uart)
{
    UART_HandleTypeDef *huart = uart->huart;

    __HAL_UART_DISABLE_IT(huart, UART_IT_RXNE);
}

int uart_writeable(uart_t *uart)
{
    UART_HandleTypeDef *huart = uart->huart;

    return (__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE) != RESET) ? 1 : 0;
}

int uart_readable(uart_t *uart)
{
    UART_HandleTypeDef *huart = uart->huart;

    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE)) {
        __HAL_UART_CLEAR_OREFLAG(huart);
    }
    return (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) != RESET) ? 1 : 0;
}

int uart_putc(uart_t *uart, int ch)
{
    UART_HandleTypeDef *huart = uart->huart;
    if (uart_writeable(uart)) {
#if defined (STM32F103xx)
        huart->Instance->DR = (uint8_t)(ch & 0xFF);
#else
        huart->Instance->TDR = (uint8_t)(ch & 0xFF);
#endif
        return ch;
    }
    return -1;
}

int uart_getc(uart_t *uart)
{
    UART_HandleTypeDef *huart = uart->huart;
    if (uart_readable(uart)) {
#if defined (STM32F103xx)
        return (uint8_t)(huart->Instance->DR & (uint8_t) 0xFF);
#else
        return (uint8_t)(huart->Instance->RDR & (uint8_t) 0xFF);
#endif
    }
    return -1;
}
