
#ifndef LCD_H_
#define LCD_H_

#define			LCD_BASE			(0xe9570000)

#define			CR1_ADDR				(*(volatile unsigned int *)(LCD_BASE))
#define			HTR_ADDR				(*(volatile unsigned int *)(LCD_BASE + 0x08))
#define			VTR1_ADDR				(*(volatile unsigned int *)(LCD_BASE + 0x0c))
#define			VTR2_ADDR				(*(volatile unsigned int *)(LCD_BASE + 0x10))
#define			PCTR_ADDR				(*(volatile unsigned int *)(LCD_BASE + 0x14))
#define			ISR_ADDR				(*(volatile unsigned int *)(LCD_BASE + 0x18))
#define			IMR_ADDR				(*(volatile unsigned int *)(LCD_BASE + 0x1c))
#define			IVR_ADDR				(*(volatile unsigned int *)(LCD_BASE + 0x20))
#define			ISCR_ADDR				(*(volatile unsigned int *)(LCD_BASE + 0x24))
#define			DBAR_ADDR				(*(volatile unsigned int *)(LCD_BASE + 0x28))
#define			DCAR_ADDR				(*(volatile unsigned int *)(LCD_BASE + 0x2c))
#define			CIR_ADDR				(*(volatile unsigned int *)(LCD_BASE + 0x1FC))
#define			PALT_ADDR				(*(volatile unsigned int *)(LCD_BASE + 0x200))

#define 		EXT_ADDR				(*(volatile unsigned int *)(LCD_BASE + 0x30))

//status flag
#define OFU				(1<<0)
#define OFO				(1<<1)
#define IFU				(1<<2)
#define IFO				(1<<3)
#define FER				(1<<4)
#define MBE				(1<<5)
#define VCT				(1<<6)
#define BAU				(1<<7)
#define LDD				(1<<8)
#define ABL				(1<<9)
#define ARI				(1<<10)
#define ARS				(1<<11)

#endif /* LCD_H_ */
