
#include "e500mc.h"
#include "e500mc_asm.h"
#include "iomacros.h"
#include "types_local.h"
#include "mpic.h"
#include "cpr.h"
#include "cpc.h"
#include "ccm.h"
#include "ccm_axi.h"
#include "ccm_cc.h"
#include "axi_law.h"
#include "pamu.h"
#include "axidma.h"
#include "ebc.h"
#include "acu.h"
#include "xgmii.h"
#include "pcie.h"
#include "usb30.h"
#include "rio.h"
#include "uart.h"
#include "iic.h"

#include "sfm.h"
#include "gpio.h"
#include "ssi.h"
#include "mpu.h"

#include "secmon.h"
#include "vd.h"
#include "td.h"
#include "ld_reg.h"
#include "pgd.h"
#include "fd.h"
#include "mesh.h"
#include "pcix_regs.h"

#include "sd_mmc_card.h"
#include "dwc_mmc.h"

#define ISYNC               asm volatile("isync")

#define FLASH_BASE          (0xf8000000)
#define FLASH_BOOT_BASE     (0xfbce0000)
#define FLASH_SECTOR_SIZE   (0x20000)
#define FLASH_PAGE_SIZE     (0x200)
