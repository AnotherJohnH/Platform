@-------------------------------------------------------------------------------
@
@ NOTE: Adapted from disassemly of code generated from source in the official
@       pico-sdk which has the following copyright...
@
@          Copyright (c) 2019-2021 Raspberry Pi (Trading) Ltd.
@          SPDX-License-Identifier: BSD-3-Clause
@
@-------------------------------------------------------------------------------

.cpu cortex-m0
.fpu softvfp

.section .boot2

.align 8
.global boot2

@-------------------------------------------------------------------------------
@ Entry point from boot ROM

boot2:
    @ Save LR used to determine entry type
    push    {lr}

@-------------------------------------------------------------------------------
@ Configure PADS_QSPI

    ldr     r3, =0x40020000    @ PADS_QSPI_BASE

    @ GPIO_QSPI_SCLK => 8mA drive, fast slew
    movs    r0, #0b00100001
    str     r0, [r3, #0x04]    @ GPIO_QSPI_SCLK = 0b00100001

    @ GPIO_QSPI_SDx => disable input Schmitt to reduce delay
    ldr     r0, [r3, #0x08]    @ ctrl = GPIO_QSPI_SD0
    movs    r1, #0b00000010
    bic     r0, r1             @ ctrl &= ~0b00000010
    str     r0, [r3, #0x08]    @ GPIO_QSPI_SD0 = ctrl
    str     r0, [r3, #0x0C]    @ GPIO_QSPI_SD1 = ctrl
    str     r0, [r3, #0x10]    @ GPIO_QSPI_SD2 = ctrl
    str     r0, [r3, #0x14]    @ GPIO_QSPI_SD3 = ctrl

@-------------------------------------------------------------------------------
@ Configure XIP.SSI

    ldr     r3, =0x18000000  @ XIP_SSI_BASE

    @ Disable SSI for config
    movs    r1, #0
    str     r1, [r3, #8]     @ SSIENR = 0

    @ Set SSI clock divider
    movs    r1, #2
    str     r1, [r3, #0x14]  @ BAUDR = 2

    @ Set 1-cycle sample delay. If PICO_FLASH_SPI_CLKDIV == 2 then this means,
    @ if the flash launches data on SCLK posedge, we capture it at the time that
    @ the next SCLK posedge is launched. This is shortly before that posedge
    @ arrives at the flash, so data hold time should be ok. For
    @ PICO_FLASH_SPI_CLKDIV > 2 this pretty much has no effect.

    movs    r1, #1
    movs    r2, #0xf0
    str     r1, [r3, r2]     @ RX_SAMPLE_DLY = 1

    ldr     r1, =0x00070000
    str     r1, [r3, #0]     @ CTRLR0.DFS_32 = 7

    @ Enable SSI and select slave 0
    movs    r1, #1
    str     r1, [r3, #8]     @ SSIENR = 1

@-------------------------------------------------------------------------------
@ Check whether FLASH SREG needs programming

    movs    r0, #0x35         @ CMD_READ_STATUS2
    bl      read_flash_sreg
    movs    r2, #2            @ SREG_DATA
    cmp     r0, r2
    beq     skip_sreg_programming

@-------------------------------------------------------------------------------
@ Program FLASH SREG

    movs    r1, #6
    str     r1, [r3, #0x60]   @ Send 6
    bl      wait_ssi_ready
    ldr     r1, [r3, #0x60]

    @ Send status write command followed by data bytes
    movs    r1, #1
    str     r1, [r3, #0x60]   @ Send 0x01, 0x00, 0x02
    movs    r0, #0
    str     r0, [r3, #0x60]   
    str     r2, [r3, #0x60]

    bl      wait_ssi_ready
    ldr     r1, [r3, #0x60]
    ldr     r1, [r3, #0x60]
    ldr     r1, [r3, #0x60]

poll_for_write_completion:
    movs    r0, #5
    bl      read_flash_sreg
    movs    r1, #1
    tst     r0, r1
    bne     poll_for_write_completion

skip_sreg_programming:

@-------------------------------------------------------------------------------
@ Further configure XIP.SSI

    @ Disable SSI for config
    movs    r1, #0
    str     r1, [r3, #8]     @ SSIENR = 0

    ldr     r1, =0x5f0300
    str     r1, [r3, #0]     @ SSI_CTRLR0 = 0x5F0300
    movs    r1, #0
    str     r1, [r3, #4]     @ SSI_CTRLR1 = 0x000000

    ldr     r1, =0x2221
    ldr     r0, =0x180000f4
    str     r1, [r0, #0]     @ SPI_CTRL0 = 0x2221

    @ Enable SSI and select slave 0
    movs    r1, #1
    str     r1, [r3, #8]     @ SSIENR = 1

@-------------------------------------------------------------------------------

    movs    r1, #0xeb        @ Send CMD_READ
    str     r1, [r3, #0x60]
    movs    r1, #0xa0        @ Send MODE_CONTINUOUS_READ
    str     r1, [r3, #0x60]

    bl      wait_ssi_ready

@-------------------------------------------------------------------------------
@ More XIP.SSI configuration

    @ Disable SSI for config
    movs    r1, #0
    str     r1, [r3, #8]     @ SSIENR = 0

    ldr     r1, =0xa0002022
    ldr     r0, =0x180000f4
    str     r1, [r0, #0]     @ SPI_CTRL0 = 0xA0002022

    @ Enable SSI and select slave 0
    movs    r1, #1
    str     r1, [r3, #8]     @ SSIENR = 1

@ Bus accesses to the XIP window will now be transparently serviced by the
@ external flash on cache miss. We are ready to run code from flash

@-------------------------------------------------------------------------------
@ Check LR on entry 0 => vector into flash

    pop     {r0}
    cmp     r0, #0
    beq     vector_into_flash
    bx      r0

vector_into_flash:
    ldr     r0, =vector_table_core0
    ldr     r1, =0xe000ed08
    str     r0, [r1, #0]
    ldmia   r0, {r0, r1}
    msr     MSP, r0
    bx      r1

@-------------------------------------------------------------------------------
@ Wait for XIP SSI to have transmitted a command and returned a response
@ XXX expects XIP_SSI base in r3

wait_ssi_ready:
    push    {r0, r1, lr}

wait_ssi_ready__loop:
    ldr     r1, [r3, #0x28]       @ Read SR

    movs    r0, #0b100
    tst     r1, r0
    beq     wait_ssi_ready__loop  @ Loop until SR.TFE == 1

    movs    r0, #1
    tst     r1, r0
    bne     wait_ssi_ready__loop  @ Loop until SR.BUSY == 0

    pop     {r0, r1, pc}

@-------------------------------------------------------------------------------
@ Set command to FLASH and return response
@ XXX expects XIP_SSI base in r3

read_flash_sreg:
    push    {r1, lr}
    str     r0, [r3, #0x60]
    str     r0, [r3, #0x60]

    bl      wait_ssi_ready

    ldr     r0, [r3, #0x60]
    ldr     r0, [r3, #0x60]
    pop     {r1, pc}

@-------------------------------------------------------------------------------

.global literals
literals:
.ltorg

.end
