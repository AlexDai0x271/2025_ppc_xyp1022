
#include "quicc_uart.h"
#include "quicc_uart_reg.h"

#define UARTx   UART1

/**
* uart_Initialize
* 
* @param[in] none
* @return none
*
* @brief uart 
*/
void uart_Initialize(void)
{
    UART_ConfigType  uartConfig = {0};
    
    //set pin mux
    qe_uart_pinio_init();

    uartConfig.baudrate = BaudRate_57600bps;
    uartConfig.dataBits = UART_WORD_LEN_8BIT;
    uartConfig.stopBits = UART_STOP_1BIT;
    uartConfig.parity = UART_PARI_ODD;
    uartConfig.fifoByteEn = ENABLE; 
    uartConfig.dmaEn = UART_DMA_TXRX_NONE;
    UART_Init(UARTx, &uartConfig);    
}

int quicc_uart_testmain(void)
{
    uint8_t data = 0,recv = 0,cnt = 0;
    uart_Initialize();
    
    while (1)
    {
    	//for(cnt=0;cnt<32;cnt++)
    	{
    		while (!UART_TxIsEmpty(UARTx));
    		UART_SendData(UARTx, data);
    	}
    	
        while(!UART_RxIsDataReady(UARTx));    /* when rx data is ready */
        recv = UART_ReceiveData(UARTx);

        if( recv != data )
        {
        	break;
        }
        
        UART_SetRST(UARTx);
        
        data++;
        
        UART_ClearRST(UARTx);
    }
    
    while(1);
}

