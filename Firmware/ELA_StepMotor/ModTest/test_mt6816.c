#include "test_mt6816.h"
#include "ela_mt6816.h"
#include "ela_uart.h"

void test_mt6816(void)
{
	MT6816_Init();
	Uart_PrintfInit();
	while (1)
	{
		MT6816_GetAngle();
		HAL_Delay(100);
		printf("raw_angle:%d,\
				data_valid:%d,\
				magnet_valid:%d",
			   g_mt6816_st.raw_angle,
			   g_mt6816_st.data_valid,
			   g_mt6816_st.magnet_valid);
	}
}