#include <common.h>
#include <s3c2410.h>

#define S3C2440_MPLL_400MHZ     ((0x5c<<12)|(0x01<<4)|(0x01))
#define S3C2440_UPLL_48MHZ      ((0x10<<12)|(0x02<<4)|(0x01))
#define S3C2440_CLKDIV          (0x05)


static inline void delay (unsigned long loops){    
	__asm__ volatile ("1:\n"      
	"subs %0, %1, #1\n"      
	"bne 1b":"=r" (loops):"0" (loops));
}

void clock_init(void){
	S3C24X0_CLOCK_POWER *const clk_power=S3C24X0_GetBase_CLOCK_POWER();

     /* FCLK:HCLK:PCLK = 1:4:8 */
     clk_power->CLKDIVN = S3C2440_CLKDIV;
     /* change to asynchronous bus mod */        
	 __asm__(    "mrc    p15, 0, r1, c1, c0, 0\n"    /* read ctrl register   */
	 "orr    r1, r1, #0xc0000000\n"      /* Asynchronous         */
	 "mcr    p15, 0, r1, c1, c0, 0\n"    /* write ctrl register  */
	 :::"r1"
	 );	
	 /* configure UPLL */
	clk_power->UPLLCON = S3C2440_UPLL_48MHZ;
	 
	/* some delay between MPLL and UPLL */
	delay (4000);

	/* to reduce PLL lock time, adjust the LOCKTIME register */
	clk_power->LOCKTIME = 0xFFFFFF;

	/* configure MPLL */
	clk_power->MPLLCON = S3C2440_MPLL_400MHZ;

	/* some delay between MPLL and UPLL */
	delay (8000);

	


}


