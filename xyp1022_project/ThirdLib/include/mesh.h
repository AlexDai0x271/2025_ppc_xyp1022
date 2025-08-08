#ifndef MESH_H
#define MESH_H

// Reg Address {{{
#define SHIELD_REG_BASE 0xEB002000
#define SHIELD_CFG        (SHIELD_REG_BASE+0x0)
#define SHIELD_DATA       (SHIELD_REG_BASE+0x4)
#define SHIELD_DIV        (SHIELD_REG_BASE+0x8)
#define SHIELD_EN_STATUS  (SHIELD_REG_BASE+0xc)
// }}}
// Reg Valid Bits {{{
#define WR_BITS_SHIELD_CFG        0x00000003
#define RD_BITS_SHIELD_CFG        0x00000003
#define WR_BITS_SHIELD_DATA       0x0000ffff
#define RD_BITS_SHIELD_DATA       0x0000ffff
#define WR_BITS_SHIELD_DIV        0xffffffff
#define RD_BITS_SHIELD_DIV        0xffffffff
#define WR_BITS_SHIELD_EN_STATUS  0x00000000
#define RD_BITS_SHIELD_EN_STATUS  0x00000001
#define RW_BITS_SHIELD_CFG        (RD_BITS_SHIELD_CFG       & WR_BITS_SHIELD_CFG      )
#define RW_BITS_SHIELD_DATA       (RD_BITS_SHIELD_DATA      & WR_BITS_SHIELD_DATA     )
#define RW_BITS_SHIELD_DIV        (RD_BITS_SHIELD_DIV       & WR_BITS_SHIELD_DIV      )
#define RW_BITS_SHIELD_EN_STATUS  (RD_BITS_SHIELD_EN_STATUS & WR_BITS_SHIELD_EN_STATUS)
// }}}
// Reg Reset Value {{{
#define RST_VAL_SHIELD_CFG        0x00000000
#define RST_VAL_SHIELD_DATA       0x00000000
#define RST_VAL_SHIELD_DIV        0x000000ff
#define RST_VAL_SHIELD_EN_STATUS  0x00000001
// }}}
// Reg Value {{{
#define SHIELD_CFG_DISABLE        0x00000000
#define SHIELD_CFG_ENABLE         0x00000001
#define SHIELD_CFG_RST_ON_ATTACK  0x00000002
// }}}
#define REG_READ(addr)        ( *(volatile UINT32 *)(addr)         )
#define REG_WRITE(addr,wdata) ( *(volatile UINT32 *)(addr) = wdata )

#endif
