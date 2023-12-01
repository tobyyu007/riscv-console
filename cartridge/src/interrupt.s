.section .text, "ax"
.global _interrupt_handler, GetTicks, GetController, InitThread, SwitchThread, ChangeMode, CreateControlSprite, FreeControlSprite, ControlSprite, CreateSprite, FreeSprite, TestSendPointer, checkControllerStatus, DirectionPadLeft, DirectionPadUp, DirectionPadDown, DirectionPadRight, ToggleButtonsUp, ToggleButtonsRight, ToggleButtonsLeft, ToggleButtonsDown, EnableCMDInterrupt, CMDInterrupted, DisableCMDInterrupt, StartTimer, EndTimer, TimeElpased, ResetTimer, GetCurrentTime, ClearTextData, ShowTextToLine, EnableVideoInterrupt, VideoInterrupted, DisableVideoInterrupt, ClearCMDInterrupt, ClearVideoInterrupt

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
EnableCMDInterrupt:
    li a5, 14
    ecall
CMDInterrupted:
    li a5, 15
    ecall
DisableCMDInterrupt:
    li a5, 16
    ecall
StartTimer:
    li a5, 17
    ecall
EndTimer:
    li a5, 18
    ecall
TimeElpased:
    li a5, 19
    ecall
ResetTimer:
    li a5, 20
    ecall
GetCurrentTime:
    li a5, 21
    ecall
EnableVideoInterrupt:
    li a5, 22
    ecall
VideoInterrupted:
    li a5, 23
    ecall
DisableVideoInterrupt:
    li a5, 24
    ecall
ClearCMDInterrupt:
    li a5, 25
    ecall
ClearVideoInterrupt:
    li a5, 26
    ecall
CreateControlSprite:
    li a5, 30
    ecall
FreeControlSprite:
    li a5, 31
    ecall
ControlSprite:
    li a5, 32
    ecall
CreateSprite:
    li a5, 50
    ecall
FreeSprite:
    li a5, 51
    ecall
ClearTextData:
    li a5, 55
    ecall
ShowTextToLine:
    li a5, 56
    ecall
ChangeMode:
    li a5, 60
    ecall
TestSendPointer:
    li a5, 100
    ecall
