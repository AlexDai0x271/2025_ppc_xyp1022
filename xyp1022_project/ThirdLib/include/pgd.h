#define PGD_BASE      0xEB001000

#define PGD_SR       (PGD_BASE    )
#define PGD_IER      (PGD_BASE + 0x4  )
#define PGD_CR       (PGD_BASE + 0x8  )
#define PGD_RVD      (PGD_BASE + 0xC  )

#define PGD_HW_SR    (PGD_BASE    )
#define PGD_HW_IER   (PGD_BASE + 0x4  )
#define PGD_HW_CR    (PGD_BASE + 0x8  )
#define PGD_HW1_SR   (PGD_BASE + 0x2  )
#define PGD_HW1_IER  (PGD_BASE + 0x6  )
#define PGD_HW1_CR   (PGD_BASE + 0xa  )

#define PGD_BY_SR    (PGD_BASE    )
#define PGD_BY_IER   (PGD_BASE + 0x4  )
#define PGD_BY_CR    (PGD_BASE + 0x8  )
#define PGD_BY1_SR   (PGD_BASE + 0x1  )
#define PGD_BY1_IER  (PGD_BASE + 0x5  )
#define PGD_BY1_CR   (PGD_BASE + 0x9  )
#define PGD_BY2_SR   (PGD_BASE + 0x2  )
#define PGD_BY2_IER  (PGD_BASE + 0x6  )
#define PGD_BY2_CR   (PGD_BASE + 0xa  )
#define PGD_BY3_SR   (PGD_BASE + 0x3  )
#define PGD_BY3_IER  (PGD_BASE + 0x7  )
#define PGD_BY3_CR   (PGD_BASE + 0xb  )

#define PGD_PGDDET   (0x1<<0)     // PGD_SR[0]
#define PGD_PGDOUT   (0x1<<1)     // PGD_SR[1]
#define PGD_TESTOK   (0x1<<7)     // PGD_SR[7]

#define PGD_IE       (0x1<<0)     // PGD_IER[0]
#define PGD_IESEL    (0x1<<1)     // PGD_IER[1]

#define PGD_EN       (0x1<<0)     // PGD_CR[0]
#define PGD_SL       (0x1<<1)     // PGD_CR[1]
#define PGD_TEST     (0x1<<7)     // PGD_CR[7]
