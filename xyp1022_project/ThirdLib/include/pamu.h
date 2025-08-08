#ifndef PAMU_H
#define PAMU_H

#define PAMU_CIR0_BASE_ADDR 0xE8007000
#define PAMU_CIR1_BASE_ADDR 0xE8009000
#define PAMU_CIP0_BASE_ADDR 0xE800B000
#define PAMU_CIP1_BASE_ADDR 0xE800D000

#define PAMU_NORMINT_NUM   0

/////CIR0
#define PAMU_CIR0_PPBAH                (0x0  + PAMU_CIR0_BASE_ADDR)   //primary
#define PAMU_CIR0_PPBAL                (0x4  + PAMU_CIR0_BASE_ADDR)
#define PAMU_CIR0_PPLAH                (0x8  + PAMU_CIR0_BASE_ADDR)
#define PAMU_CIR0_PPLAL                (0xC  + PAMU_CIR0_BASE_ADDR)
#define PAMU_CIR0_SPBAH                (0x10 + PAMU_CIR0_BASE_ADDR)   //secondary
#define PAMU_CIR0_SPBAL                (0x14 + PAMU_CIR0_BASE_ADDR)
#define PAMU_CIR0_SPLAH                (0x18 + PAMU_CIR0_BASE_ADDR)
#define PAMU_CIR0_SPLAL                (0x1C + PAMU_CIR0_BASE_ADDR)

#define PAMU_CIR0_OBAH                 (0x20 + PAMU_CIR0_BASE_ADDR)   //omt
#define PAMU_CIR0_OBAL                 (0x24 + PAMU_CIR0_BASE_ADDR)
#define PAMU_CIR0_OLAH                 (0x28 + PAMU_CIR0_BASE_ADDR)
#define PAMU_CIR0_OLAL                 (0x2C + PAMU_CIR0_BASE_ADDR)

#define PAMU_CIR0_PAC1                 (0x30 + PAMU_CIR0_BASE_ADDR)   //address capabilities
#define PAMU_CIR0_PAC2                 (0x34 + PAMU_CIR0_BASE_ADDR)

#define PAMU_CIR0_POES1                (0x40 + PAMU_CIR0_BASE_ADDR)   //operation error status
#define PAMU_CIR0_POES2                (0x44 + PAMU_CIR0_BASE_ADDR)
#define PAMU_CIR0_POEAH                (0x48 + PAMU_CIR0_BASE_ADDR)   //operation error address
#define PAMU_CIR0_POEAL                (0x4C + PAMU_CIR0_BASE_ADDR)
#define PAMU_CIR0_AVS1                 (0x50 + PAMU_CIR0_BASE_ADDR)   //access violaton status
#define PAMU_CIR0_AVS2                 (0x54 + PAMU_CIR0_BASE_ADDR)
#define PAMU_CIR0_AVSH                 (0x58 + PAMU_CIR0_BASE_ADDR)   //access violaton address
#define PAMU_CIR0_AVSL                 (0x5C + PAMU_CIR0_BASE_ADDR)

#define PAMU_CIR0_EECTL                (0x60 + PAMU_CIR0_BASE_ADDR)   //ecc error control
#define PAMU_CIR0_EEDIS                (0x64 + PAMU_CIR0_BASE_ADDR)   //ecc error disable
#define PAMU_CIR0_EEINTEN              (0x68 + PAMU_CIR0_BASE_ADDR)   //ecc error interrupt enable
#define PAMU_CIR0_EEDET                (0x6C + PAMU_CIR0_BASE_ADDR)   //ecc error detect
#define PAMU_CIR0_EEATTR               (0x70 + PAMU_CIR0_BASE_ADDR)   //ecc error attribute
#define PAMU_CIR0_EEAHI                (0x74 + PAMU_CIR0_BASE_ADDR)   //ecc error address
#define PAMU_CIR0_EEALO                (0x78 + PAMU_CIR0_BASE_ADDR)   //ecc error address
#define PAMU_CIR0_EEDHI                (0x7C + PAMU_CIR0_BASE_ADDR)   //ecc error data  
#define PAMU_CIR0_EEDLO                (0x80 + PAMU_CIR0_BASE_ADDR)   //ecc error data

#define PAMU_CIR0_UDAD                 (0x90 + PAMU_CIR0_BASE_ADDR)   //unauthorized device access detection

#define PAMU_CIR0_PR1                  (0xBF8+ PAMU_CIR0_BASE_ADDR)   //revision 1
#define PAMU_CIR0_PR2                  (0xBFC+ PAMU_CIR0_BASE_ADDR)   //revision 2

#define PAMU_CIR0_PC1                  (0xC00+ PAMU_CIR0_BASE_ADDR)   //capabilities
#define PAMU_CIR0_PC2                  (0xC04+ PAMU_CIR0_BASE_ADDR)   //capabilities
#define PAMU_CIR0_PC3                  (0xC08+ PAMU_CIR0_BASE_ADDR)   //capabilities
#define PAMU_CIR0_PC4                  (0xC0C+ PAMU_CIR0_BASE_ADDR)   //capabilities

#define PAMU_CIR0_PC                   (0xC10+ PAMU_CIR0_BASE_ADDR)   //control reg
#define PAMU_CIR0_FATT1                (0xC14+ PAMU_CIR0_BASE_ADDR)   //fetch Attribute1
#define PAMU_CIR0_PICS                 (0xC1C+ PAMU_CIR0_BASE_ADDR)   //interrupt control and status

/////CIR1
#define PAMU_CIR1_PPBAH                (0x0  + PAMU_CIR1_BASE_ADDR)   //primary
#define PAMU_CIR1_PPBAL                (0x4  + PAMU_CIR1_BASE_ADDR)
#define PAMU_CIR1_PPLAH                (0x8  + PAMU_CIR1_BASE_ADDR)
#define PAMU_CIR1_PPLAL                (0xC  + PAMU_CIR1_BASE_ADDR)
#define PAMU_CIR1_SPBAH                (0x10 + PAMU_CIR1_BASE_ADDR)   //secondary
#define PAMU_CIR1_SPBAL                (0x14 + PAMU_CIR1_BASE_ADDR)
#define PAMU_CIR1_SPLAH                (0x18 + PAMU_CIR1_BASE_ADDR)
#define PAMU_CIR1_SPLAL                (0x1C + PAMU_CIR1_BASE_ADDR)

#define PAMU_CIR1_OBAH                 (0x20 + PAMU_CIR1_BASE_ADDR)   //omt
#define PAMU_CIR1_OBAL                 (0x24 + PAMU_CIR1_BASE_ADDR)
#define PAMU_CIR1_OLAH                 (0x28 + PAMU_CIR1_BASE_ADDR)
#define PAMU_CIR1_OLAL                 (0x2C + PAMU_CIR1_BASE_ADDR)

#define PAMU_CIR1_PAC1                 (0x30 + PAMU_CIR1_BASE_ADDR)   //address capabilities
#define PAMU_CIR1_PAC2                 (0x34 + PAMU_CIR1_BASE_ADDR)

#define PAMU_CIR1_POES1                (0x40 + PAMU_CIR1_BASE_ADDR)   //operation error status
#define PAMU_CIR1_POES2                (0x44 + PAMU_CIR1_BASE_ADDR)
#define PAMU_CIR1_POEAH                (0x48 + PAMU_CIR1_BASE_ADDR)   //operation error address
#define PAMU_CIR1_POEAL                (0x4C + PAMU_CIR1_BASE_ADDR)
#define PAMU_CIR1_AVS1                 (0x50 + PAMU_CIR1_BASE_ADDR)   //access violaton status
#define PAMU_CIR1_AVS2                 (0x54 + PAMU_CIR1_BASE_ADDR)
#define PAMU_CIR1_AVSH                 (0x58 + PAMU_CIR1_BASE_ADDR)   //access violaton address
#define PAMU_CIR1_AVSL                 (0x5C + PAMU_CIR1_BASE_ADDR)

#define PAMU_CIR1_EECTL                (0x60 + PAMU_CIR1_BASE_ADDR)   //ecc error control
#define PAMU_CIR1_EEDIS                (0x64 + PAMU_CIR1_BASE_ADDR)   //ecc error control
#define PAMU_CIR1_EEINTEN              (0x68 + PAMU_CIR1_BASE_ADDR)   //ecc error interrupt enable
#define PAMU_CIR1_EEDET                (0x6C + PAMU_CIR1_BASE_ADDR)   //ecc error detect
#define PAMU_CIR1_EEATTR               (0x70 + PAMU_CIR1_BASE_ADDR)   //ecc error attribute
#define PAMU_CIR1_EEAHI                (0x74 + PAMU_CIR1_BASE_ADDR)   //ecc error address
#define PAMU_CIR1_EEALO                (0x78 + PAMU_CIR1_BASE_ADDR)   //ecc error address
#define PAMU_CIR1_EEDHI                (0x7C + PAMU_CIR1_BASE_ADDR)   //ecc error data  
#define PAMU_CIR1_EEDLO                (0x80 + PAMU_CIR1_BASE_ADDR)   //ecc error data

#define PAMU_CIR1_UDAD                 (0x90 + PAMU_CIR1_BASE_ADDR)   //unauthorized device access detection

#define PAMU_CIR1_PR1                  (0xBF8+ PAMU_CIR1_BASE_ADDR)   //revision 1
#define PAMU_CIR1_PR2                  (0xBFC+ PAMU_CIR1_BASE_ADDR)   //revision 2

#define PAMU_CIR1_PC1                  (0xC00+ PAMU_CIR1_BASE_ADDR)   //capabilities
#define PAMU_CIR1_PC2                  (0xC04+ PAMU_CIR1_BASE_ADDR)   //capabilities
#define PAMU_CIR1_PC3                  (0xC08+ PAMU_CIR1_BASE_ADDR)   //capabilities
#define PAMU_CIR1_PC4                  (0xC0C+ PAMU_CIR1_BASE_ADDR)   //capabilities

#define PAMU_CIR1_PC                   (0xC10+ PAMU_CIR1_BASE_ADDR)   //control reg
#define PAMU_CIR1_FATT1                (0xC14+ PAMU_CIR0_BASE_ADDR)   //fetch Attribute1
#define PAMU_CIR1_PICS                 (0xC1C+ PAMU_CIR1_BASE_ADDR)   //interrupt control and status

/////CIP0
#define PAMU_CIP0_PPBAH                (0x0  + PAMU_CIP0_BASE_ADDR)   //primary
#define PAMU_CIP0_PPBAL                (0x4  + PAMU_CIP0_BASE_ADDR)
#define PAMU_CIP0_PPLAH                (0x8  + PAMU_CIP0_BASE_ADDR)
#define PAMU_CIP0_PPLAL                (0xC  + PAMU_CIP0_BASE_ADDR)
#define PAMU_CIP0_SPBAH                (0x10 + PAMU_CIP0_BASE_ADDR)   //secondary
#define PAMU_CIP0_SPBAL                (0x14 + PAMU_CIP0_BASE_ADDR)
#define PAMU_CIP0_SPLAH                (0x18 + PAMU_CIP0_BASE_ADDR)
#define PAMU_CIP0_SPLAL                (0x1C + PAMU_CIP0_BASE_ADDR)

#define PAMU_CIP0_OBAH                 (0x20 + PAMU_CIP0_BASE_ADDR)   //omt
#define PAMU_CIP0_OBAL                 (0x24 + PAMU_CIP0_BASE_ADDR)
#define PAMU_CIP0_OLAH                 (0x28 + PAMU_CIP0_BASE_ADDR)
#define PAMU_CIP0_OLAL                 (0x2C + PAMU_CIP0_BASE_ADDR)

#define PAMU_CIP0_PAC1                 (0x30 + PAMU_CIP0_BASE_ADDR)   //address capabilities
#define PAMU_CIP0_PAC2                 (0x34 + PAMU_CIP0_BASE_ADDR)

#define PAMU_CIP0_POES1                (0x40 + PAMU_CIP0_BASE_ADDR)   //operation error status
#define PAMU_CIP0_POES2                (0x44 + PAMU_CIP0_BASE_ADDR)
#define PAMU_CIP0_POEAH                (0x48 + PAMU_CIP0_BASE_ADDR)   //operation error address
#define PAMU_CIP0_POEAL                (0x4C + PAMU_CIP0_BASE_ADDR)
#define PAMU_CIP0_AVS1                 (0x50 + PAMU_CIP0_BASE_ADDR)   //access violaton status
#define PAMU_CIP0_AVS2                 (0x54 + PAMU_CIP0_BASE_ADDR)
#define PAMU_CIP0_AVSH                 (0x58 + PAMU_CIP0_BASE_ADDR)   //access violaton address
#define PAMU_CIP0_AVSL                 (0x5C + PAMU_CIP0_BASE_ADDR)

#define PAMU_CIP0_EECTL                (0x60 + PAMU_CIP0_BASE_ADDR)   //ecc error control
#define PAMU_CIP0_EEDIS                (0x64 + PAMU_CIP0_BASE_ADDR)   //ecc error control
#define PAMU_CIP0_EEINTEN              (0x68 + PAMU_CIP0_BASE_ADDR)   //ecc error interrupt enable
#define PAMU_CIP0_EEDET                (0x6C + PAMU_CIP0_BASE_ADDR)   //ecc error detect
#define PAMU_CIP0_EEATTR               (0x70 + PAMU_CIP0_BASE_ADDR)   //ecc error attribute
#define PAMU_CIP0_EEAHI                (0x74 + PAMU_CIP0_BASE_ADDR)   //ecc error address
#define PAMU_CIP0_EEALO                (0x78 + PAMU_CIP0_BASE_ADDR)   //ecc error address
#define PAMU_CIP0_EEDHI                (0x7C + PAMU_CIP0_BASE_ADDR)   //ecc error data  
#define PAMU_CIP0_EEDLO                (0x80 + PAMU_CIP0_BASE_ADDR)   //ecc error data

#define PAMU_CIP0_UDAD                 (0x90 + PAMU_CIP0_BASE_ADDR)   //unauthorized device access detection

#define PAMU_CIP0_PR1                  (0xBF8+ PAMU_CIP0_BASE_ADDR)   //revision 1
#define PAMU_CIP0_PR2                  (0xBFC+ PAMU_CIP0_BASE_ADDR)   //revision 2

#define PAMU_CIP0_PC1                  (0xC00+ PAMU_CIP0_BASE_ADDR)   //capabilities
#define PAMU_CIP0_PC2                  (0xC04+ PAMU_CIP0_BASE_ADDR)   //capabilities
#define PAMU_CIP0_PC3                  (0xC08+ PAMU_CIP0_BASE_ADDR)   //capabilities
#define PAMU_CIP0_PC4                  (0xC0C+ PAMU_CIP0_BASE_ADDR)   //capabilities

#define PAMU_CIP0_PC                   (0xC10+ PAMU_CIP0_BASE_ADDR)   //control reg
#define PAMU_CIP0_FATT1                (0xC14+ PAMU_CIR0_BASE_ADDR)   //fetch Attribute1
#define PAMU_CIP0_PICS                 (0xC1C+ PAMU_CIP0_BASE_ADDR)   //interrupt control and status

/////CIP1
#define PAMU_CIP1_PPBAH                (0x0  + PAMU_CIP1_BASE_ADDR)   //primary
#define PAMU_CIP1_PPBAL                (0x4  + PAMU_CIP1_BASE_ADDR)
#define PAMU_CIP1_PPLAH                (0x8  + PAMU_CIP1_BASE_ADDR)
#define PAMU_CIP1_PPLAL                (0xC  + PAMU_CIP1_BASE_ADDR)
#define PAMU_CIP1_SPBAH                (0x10 + PAMU_CIP1_BASE_ADDR)   //secondary
#define PAMU_CIP1_SPBAL                (0x14 + PAMU_CIP1_BASE_ADDR)
#define PAMU_CIP1_SPLAH                (0x18 + PAMU_CIP1_BASE_ADDR)
#define PAMU_CIP1_SPLAL                (0x1C + PAMU_CIP1_BASE_ADDR)

#define PAMU_CIP1_OBAH                 (0x20 + PAMU_CIP1_BASE_ADDR)   //omt
#define PAMU_CIP1_OBAL                 (0x24 + PAMU_CIP1_BASE_ADDR)
#define PAMU_CIP1_OLAH                 (0x28 + PAMU_CIP1_BASE_ADDR)
#define PAMU_CIP1_OLAL                 (0x2C + PAMU_CIP1_BASE_ADDR)

#define PAMU_CIP1_PAC1                 (0x30 + PAMU_CIP1_BASE_ADDR)   //address capabilities
#define PAMU_CIP1_PAC2                 (0x34 + PAMU_CIP1_BASE_ADDR)

#define PAMU_CIP1_POES1                (0x40 + PAMU_CIP1_BASE_ADDR)   //operation error status
#define PAMU_CIP1_POES2                (0x44 + PAMU_CIP1_BASE_ADDR)
#define PAMU_CIP1_POEAH                (0x48 + PAMU_CIP1_BASE_ADDR)   //operation error address
#define PAMU_CIP1_POEAL                (0x4C + PAMU_CIP1_BASE_ADDR)
#define PAMU_CIP1_AVS1                 (0x50 + PAMU_CIP1_BASE_ADDR)   //access violaton status
#define PAMU_CIP1_AVS2                 (0x54 + PAMU_CIP1_BASE_ADDR)
#define PAMU_CIP1_AVSH                 (0x58 + PAMU_CIP1_BASE_ADDR)   //access violaton address
#define PAMU_CIP1_AVSL                 (0x5C + PAMU_CIP1_BASE_ADDR)

#define PAMU_CIP1_EECTL                (0x60 + PAMU_CIP1_BASE_ADDR)   //ecc error control
#define PAMU_CIP1_EEDIS                (0x64 + PAMU_CIP1_BASE_ADDR)   //ecc error control
#define PAMU_CIP1_EEINTEN              (0x68 + PAMU_CIP1_BASE_ADDR)   //ecc error interrupt enable
#define PAMU_CIP1_EEDET                (0x6C + PAMU_CIP1_BASE_ADDR)   //ecc error detect
#define PAMU_CIP1_EEATTR               (0x70 + PAMU_CIP1_BASE_ADDR)   //ecc error attribute
#define PAMU_CIP1_EEAHI                (0x74 + PAMU_CIP1_BASE_ADDR)   //ecc error address
#define PAMU_CIP1_EEALO                (0x78 + PAMU_CIP1_BASE_ADDR)   //ecc error address
#define PAMU_CIP1_EEDHI                (0x7C + PAMU_CIP1_BASE_ADDR)   //ecc error data  
#define PAMU_CIP1_EEDLO                (0x80 + PAMU_CIP1_BASE_ADDR)   //ecc error data

#define PAMU_CIP1_UDAD                 (0x90 + PAMU_CIP1_BASE_ADDR)   //unauthorized device access detection

#define PAMU_CIP1_PR1                  (0xBF8+ PAMU_CIP1_BASE_ADDR)   //revision 1
#define PAMU_CIP1_PR2                  (0xBFC+ PAMU_CIP1_BASE_ADDR)   //revision 2

#define PAMU_CIP1_PC1                  (0xC00+ PAMU_CIP1_BASE_ADDR)   //capabilities
#define PAMU_CIP1_PC2                  (0xC04+ PAMU_CIP1_BASE_ADDR)   //capabilities
#define PAMU_CIP1_PC3                  (0xC08+ PAMU_CIP1_BASE_ADDR)   //capabilities
#define PAMU_CIP1_PC4                  (0xC0C+ PAMU_CIP1_BASE_ADDR)   //capabilities

#define PAMU_CIP1_PC                   (0xC10+ PAMU_CIP1_BASE_ADDR)   //control reg
#define PAMU_CIP1_FATT1                (0xC14+ PAMU_CIR0_BASE_ADDR)   //fetch Attribute1
#define PAMU_CIP1_PICS                 (0xC1C+ PAMU_CIP1_BASE_ADDR)   //interrupt control and status

#endif
