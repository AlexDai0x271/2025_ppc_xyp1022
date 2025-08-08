#define LD_BASE      0xEB005000

#define LD_SR        (LD_BASE    )
#define LD_IER       (LD_BASE + 0x4  )
#define LD_CR        (LD_BASE + 0x8  )


#define LD_HW_SR      (LD_BASE    )
#define LD_HW_IER     (LD_BASE + 0x4  )
#define LD_HW_CR      (LD_BASE + 0x8  )
#define LD_HW1_SR     (LD_BASE + 0x2  )
#define LD_HW1_IER    (LD_BASE + 0x6  )
#define LD_HW1_CR     (LD_BASE + 0xa  )

#define LD_BY_SR      (LD_BASE    )
#define LD_BY_IER     (LD_BASE + 0x4  )
#define LD_BY_CR      (LD_BASE + 0x8  )
#define LD_BY1_SR     (LD_BASE + 0x1  )
#define LD_BY1_IER    (LD_BASE + 0x5  )
#define LD_BY1_CR     (LD_BASE + 0x9  )
#define LD_BY2_SR     (LD_BASE + 0x2  )
#define LD_BY2_IER    (LD_BASE + 0x6  )
#define LD_BY2_CR     (LD_BASE + 0xa  )
#define LD_BY3_SR     (LD_BASE + 0x3  )
#define LD_BY3_IER    (LD_BASE + 0x7  )
#define LD_BY3_CR     (LD_BASE + 0xb  )

#define LD_LDDET      (0x1<<0)     // LD_SR[0]
#define LD_LDOUT      (0x1<<1)     // LD_SR[1]
#define LD_TESTOK     (0x1<<7)     // LD_SR[7]

#define LD_IE         (0x1<<0)     // LD_IER[0]
#define LD_IESEL      (0x1<<1)     // LD_IER[1]

#define LD_EN         (0x1<<0)     // LD_CR[0]

#define LD_TEST       (0x1<<7)     // LD_CR[7]
