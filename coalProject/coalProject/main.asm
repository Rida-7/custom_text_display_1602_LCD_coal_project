.include "m328pdef.inc"
.include "delay.inc"
.include "lcd_Macros.inc"
.include "UART_Macros.inc"

.dseg
.org SRAM_START
    countdown: .byte 1

.cseg
.org 0x0000
LCD_init ; initilize the 16x2 LCD
LCD_backlight_OFF
delay 500
LCD_backlight_ON
Serial_begin        ; Initialize UART Communication

; Initialize countdown value
LDI r16, 9
STS countdown, r16

loop:
	countdown_loop:
    ; Display countdown value on LCD
    LCD_clear        ; Clear the LCD
    LCD_home       ; Move cursor to the beginning of the first line
    LDS r16, countdown
    LCD_send_a_register r16

    ; Decrement countdown
    LDS r16, countdown ; Load value from memory into register
    DEC r16            ; Decrement the register
    STS countdown, r16 ; Store the updated value back to memory

    ; Delay for 1 second
    delay 1000

    ; Check if countdown is complete
    LDS r16, countdown ; Load value from memory into register
    CPI r16, 0         ; Compare the register with immediate value 0
    BREQ countdown_complete

    RJMP countdown_loop

countdown_complete:
	LDI r16, 0
	
	LCD_send_a_command 0x01 ; clear the LCD
	delay 500
	
	; Sending Hello World to LCD character-by-character
	
	

receive_text:
    Serial_read
    CPI r16, 0
    BREQ loop2
	rjmp display_text
	loop2:
		rjmp countdown_complete

display_text:
		LCD_send_a_reg_ASCII r16
		RJMP receive_text ; Continue to receive more text
; Add an endless loop at the end if needed
endless_loop:
    RJMP endless_loop
