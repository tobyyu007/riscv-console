.section .text, "ax"
.global _interrupt_handler, GetTicks, GetController, InitThread, SwitchThread, checkControllerStatus, DirectionPadLeft, DirectionPadUp, DirectionPadDown, DirectionPadRight, ToggleButtonsUp, ToggleButtonsRight, ToggleButtonsLeft, ToggleButtonsDown
_interrupt_handler:
    addi	sp,sp,-40
    sw	    ra,36(sp)
    sw	    t0,32(sp)
    sw	    t1,28(sp)
    sw	    t2,24(sp)
    sw	    a0,20(sp)
    sw	    a1,16(sp)
    sw	    a2,12(sp)
    sw	    a3,8(sp)
    sw	    a4,4(sp)
    sw	    a5,0(sp)
    call    c_interrupt_handler
    lw	    ra,36(sp)
    lw	    t0,32(sp)
    lw	    t1,28(sp)
    lw	    t2,24(sp)
    lw	    a0,20(sp)
    lw	    a1,16(sp)
    lw	    a2,12(sp)
    lw	    a3,8(sp)
    lw	    a4,4(sp)
    lw	    a5,0(sp)
    addi    sp,sp,40
    mret

GetTicks:
    li a5, 1
    ecall
GetController:
    li a5, 2
    ecall
InitThread:
    li a5, 3
    ecall
SwitchThread:
    li a5, 4
    ecall
checkControllerStatus:
    li a5, 5
    ecall
DirectionPadLeft:
    li a5, 6
    ecall
DirectionPadUp:
    li a5, 7
    ecall
DirectionPadDown:
    li a5, 8
    ecall
DirectionPadRight:
    li a5, 9
    ecall
ToggleButtonsUp:
    li a5, 10
    ecall
ToggleButtonsRight:
    li a5, 11
    ecall
ToggleButtonsLeft:
    li a5, 12
    ecall
ToggleButtonsDown:
    li a5, 13
    ecall