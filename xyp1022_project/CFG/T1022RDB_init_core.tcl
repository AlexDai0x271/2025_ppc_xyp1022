########################################################################################
# Initialization file for T1042 RDB
# Clock Configuration:
#         CPU: 1200/1400 MHz,   CCB: 600 MHz,
#         DDR: 1600 MHz,   IFC: 150 MHz,
#         FMAN1:  600 MHz,  QMAN: 300 MHz,  PME: 300 MHz
########################################################################################


proc init_board {} {


}

proc T1042RDB_init_core {} {

	variable CAM_GROUP "regPPCTLB1/"
	variable SPR_GROUP "e5500 Special Purpose Registers/"
	variable GPR_GROUP "General Purpose Registers/"	

	stop		

	#0x00000000ffff0000-0x00000000FFFFFFFF 0x4FFFF0000-0x4FFFFFFFF   64k 01010 111  111  0  0x0  0x0   0  0  1 
	reg ${CAM_GROUP}L2MMU_CAM0 = 0x3000000afc08000000000004ffff0000ffffffffffff0001

	#0x00000000E0000000-0x00000000E00FFFFF 0x4E0000000-0x4E00FFFFF   1M 01010 111  111  0  0x0  0x0   0  0  1 
	reg ${CAM_GROUP}L2MMU_CAM3 = 0x5000000afc08000000000004e0000000ffffffffe0000001

	#0x00000000B0000000-0x00000000BFFFFFFF 0xC00000000-0xC0FFFFFFF 256M 01010 111  111  0  0x0  0x0   0  0  1     
	#reg ${CAM_GROUP}L2MMU_CAM7 = 0x9000000bfc0800000000000c00000000ffffffffb0000001

	#0x00000000C0000000-0x00000000CFFFFFFF 0xE00000000-0xE0FFFFFFF 256M 01010 111  111  0  0x0  0x0   0  0  1     
	#reg ${CAM_GROUP}L2MMU_CAM8 = 0x9000000bfc0800000000000e00000000ffffffffc0000001


}

proc envsetup {} {
	# Environment Setup
	radix x
	config hexprefix 0x
	config MemIdentifier v
	config MemWidth 32
	config MemAccess 32
	config MemSwap off
}

#-------------------------------------------------------------------------------
# Main                                                                          
#-------------------------------------------------------------------------------

  envsetup
  
  T1042RDB_init_core