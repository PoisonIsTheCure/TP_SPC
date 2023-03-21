
ENTRY(_start);

SECTIONS
{
	.vectors :
    	{
		*(boot)                 /* Boot values for PC, SP */
        	*(interrupt_vector)     /* Interrupt Vector */
		. = ALIGN(0x04);
    	} >rom0 AT >rom
    	.text . :
    	{
        	*main.o(.text)		/* Main user functions */
           	*startup.o(.text)       /* Boot code helpers */
        	*interrupts.o(.text)    /* Default Handler */
		*(.init)
		*(.fini)
        	*(.text .text.*)        /* Program code */
		. = ALIGN(0x04);
	} >rom0 AT >rom
    	.ARM.exidx . : {
 		__exidx_start = . ;
		*(.ARM.exidx* .gnu.linkonce.armexidx.*) /* exception handling */
		__exidx_end   = . ;
		. = ALIGN(0x04);
	} >rom0 AT >rom
   	.glue . : {
		__preinit_array_start = . ;
 		*(.preinit_array*) 	/* Pointers to preinit code */
		__preinit_array_end   = . ;
		__init_array_start = . ;
 		*(.init_array* .ctors*) /* Pointers to global constructors */
		__init_array_end   = . ;
		__fini_array_start = . ;
 		*(.fini_array* .dtors*) /* Pointers to global destructors */
		__fini_array_end   = . ;
		*(.jcr*)
    	} >rom0 AT >rom	
	.rodata . : {
        	*(.rodata .rodata.*)    /* Read-only data */
 		*(.ARM.extab* .gnu.linkonce.armextab.*) /* exception handling */
		*(.eh_frame*) 		/* exception handling tables */
		*(.eh_frame_hdr*)	/* exception handling tables (headers) */
		. = ALIGN(0x04);
    	} >rom0 AT >rom
   	.data : {
		__data_start__ = . ;
		*(.data .data.*)  /* Read-Write Data (copied to RAM at boot) */
        	. = ALIGN(0x04);
		__data_end__ = . ;
	} >ram AT >rom
    	.bss (NOLOAD) : {
		__bss_start__  = . ;
        	*(.bss .bss.*)    /* 0-initialized memory */
		*(COMMON)
        	. = ALIGN(0x04);
		__bss_end__  = . ;
    	} >ram
   	.crc_info : {
		*(crc_info)
       		. = ALIGN(0x04);
    	} >rom
 }

__stack = ORIGIN(ram)+LENGTH(ram); /* used to initialise SP */

__data_load__  = LOADADDR(.data);

__heap_bottom = __bss_end__ ;

__flash_program_begin = ORIGIN(rom) ;
__flash_program_end   = LOADADDR(.data) + SIZEOF(.data);
