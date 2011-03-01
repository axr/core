	.section __DWARF,__debug_frame,regular,debug
Lsection__debug_frame:
	.section __DWARF,__debug_info,regular,debug
Lsection__debug_info:
	.section __DWARF,__debug_abbrev,regular,debug
Lsection__debug_abbrev:
	.section __DWARF,__debug_aranges,regular,debug
Lsection__debug_aranges:
	.section __DWARF,__debug_macinfo,regular,debug
Lsection__debug_macinfo:
	.section __DWARF,__debug_line,regular,debug
Lsection__debug_line:
	.section __DWARF,__debug_loc,regular,debug
Lsection__debug_loc:
	.section __DWARF,__debug_pubnames,regular,debug
Lsection__debug_pubnames:
	.section __DWARF,__debug_pubtypes,regular,debug
Lsection__debug_pubtypes:
	.section __DWARF,__debug_inlined,regular,debug
Lsection__debug_inlined:
	.section __DWARF,__debug_str,regular,debug
Lsection__debug_str:
	.section __DWARF,__debug_ranges,regular,debug
Lsection__debug_ranges:
	.section __DWARF,__debug_abbrev,regular,debug
Ldebug_abbrev0:
	.section __DWARF,__debug_info,regular,debug
Ldebug_info0:
	.section __DWARF,__debug_line,regular,debug
Ldebug_line0:
	.text
Ltext0:
	.align 1,0x90
.globl __ZN12AXRXmlParserC2Ev
__ZN12AXRXmlParserC2Ev:
LFB10:
	.file 1 "/Users/miro/AXR/prototype/core/xml/AXRXmlParser.cpp"
	.loc 1 56 0
LVL0:
	pushq	%rbp
LCFI0:
	movq	%rsp, %rbp
LCFI1:
	.loc 1 59 0
	leave
	ret
LFE10:
	.align 1,0x90
.globl __ZN12AXRXmlParserC1Ev
__ZN12AXRXmlParserC1Ev:
LFB11:
	.loc 1 56 0
LVL1:
	pushq	%rbp
LCFI2:
	movq	%rsp, %rbp
LCFI3:
	.loc 1 59 0
	leave
	ret
LFE11:
	.cstring
	.align 3
LC0:
	.ascii "Couldn't allocate memory for parser\12\0"
LC1:
	.ascii "r\0"
LC2:
	.ascii "Parse error at line %i u:\12%s\12\0"
	.text
	.align 1,0x90
.globl __ZN12AXRXmlParser5parseEPKc
__ZN12AXRXmlParser5parseEPKc:
LFB12:
	.loc 1 61 0
LVL2:
	pushq	%rbp
LCFI4:
	movq	%rsp, %rbp
LCFI5:
	movq	%rbx, -24(%rbp)
LCFI6:
	movq	%r12, -16(%rbp)
LCFI7:
	movq	%r13, -8(%rbp)
LCFI8:
	subq	$32, %rsp
LCFI9:
	movq	%rsi, %rbx
LBB2:
	.loc 1 65 0
	xorl	%edi, %edi
LVL3:
	call	_XML_ParserCreate
LVL4:
	movq	%rax, %r12
LVL5:
	.loc 1 66 0
	testq	%rax, %rax
	jne	L6
LVL6:
	.loc 1 67 0
	movq	___stderrp@GOTPCREL(%rip), %rax
LVL7:
	movq	(%rax), %rcx
	movl	$36, %edx
	movl	$1, %esi
	leaq	LC0(%rip), %rdi
	call	_fwrite
	jmp	L11
LVL8:
L6:
	.loc 1 71 0
	leaq	_elementEndCallback(%rip), %rdx
	leaq	_elementStartCallback(%rip), %rsi
	movq	%rax, %rdi
	call	_XML_SetElementHandler
LVL9:
	.loc 1 72 0
	leaq	_elementContentTextCallback(%rip), %rsi
	movq	%r12, %rdi
	call	_XML_SetCharacterDataHandler
	.loc 1 74 0
	leaq	LC1(%rip), %rsi
	movq	%rbx, %rdi
	call	_fopen
	movq	%rax, %r13
LVL10:
LBB3:
	.loc 1 78 0
	movq	%rax, %rcx
	movl	$8192, %edx
	movl	$1, %esi
	leaq	_Buff(%rip), %rdi
	call	_fread
	movq	%rax, %rbx
LVL11:
	.loc 1 79 0
	movq	%r13, %rdi
	call	_feof
	movl	%eax, %ecx
	.loc 1 81 0
	movl	%ebx, %edx
	leaq	_Buff(%rip), %rsi
	movq	%r12, %rdi
	call	_XML_Parse
	testl	%eax, %eax
	jne	L8
	.loc 1 82 0
	movq	%r12, %rdi
	call	_XML_GetErrorCode
	movl	%eax, %edi
	call	_XML_ErrorString
	movq	%rax, %rbx
	movq	%r12, %rdi
	call	_XML_GetCurrentLineNumber
	movq	%rbx, %rcx
	movl	%eax, %edx
	leaq	LC2(%rip), %rsi
	movq	___stderrp@GOTPCREL(%rip), %rax
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	_fprintf
LVL12:
L11:
	.loc 1 83 0
	movl	$-1, %edi
	call	_exit
LVL13:
L8:
LBE3:
	.loc 1 89 0
	movq	%r13, %rdi
	call	_fclose
	.loc 1 91 0
	movq	%r12, %rdi
LBE2:
	.loc 1 92 0
	movq	-24(%rbp), %rbx
	movq	-16(%rbp), %r12
LVL14:
	movq	-8(%rbp), %r13
LVL15:
	leave
LBB4:
	.loc 1 91 0
	jmp	_XML_ParserFree
LVL16:
LBE4:
LFE12:
	.cstring
LC3:
	.ascii " /%s\12\0"
	.text
_elementEndCallback:
LFB8:
	.loc 1 49 0
LVL17:
	pushq	%rbp
LCFI10:
	movq	%rsp, %rbp
LCFI11:
	.loc 1 51 0
	leaq	LC3(%rip), %rdi
LVL18:
	xorl	%eax, %eax
	call	_printf
LVL19:
	.loc 1 52 0
	decl	_Depth(%rip)
	.loc 1 53 0
	leave
	ret
LFE8:
	.cstring
LC4:
	.ascii "  \0"
	.text
_elementContentTextCallback:
LFB7:
	.loc 1 37 0
LVL20:
	pushq	%rbp
LCFI12:
	movq	%rsp, %rbp
LCFI13:
	pushq	%r13
LCFI14:
	pushq	%r12
LCFI15:
	pushq	%rbx
LCFI16:
	subq	$8, %rsp
LCFI17:
	movq	%rsi, %r12
	movl	%edx, %r13d
LBB5:
	.loc 1 39 0
	cmpq	$10, %rsi
	je	L23
LVL21:
	xorl	%ebx, %ebx
LVL22:
	cmpl	$1, %edx
	jne	L18
LVL23:
	jmp	L23
L19:
LBB6:
	.loc 1 42 0
	leaq	LC4(%rip), %rdi
LVL24:
	xorl	%eax, %eax
	call	_printf
LVL25:
	.loc 1 41 0
	incl	%ebx
LVL26:
L18:
	cmpl	_Depth(%rip), %ebx
	jl	L19
LVL27:
	movq	%r12, %rbx
LVL28:
	xorl	%r12d, %r12d
LVL29:
	jmp	L21
LVL30:
L22:
	.loc 1 44 0
	movsbl	-1(%rbx),%edi
LVL31:
	call	_putchar
LVL32:
	.loc 1 43 0
	incl	%r12d
LVL33:
L21:
	incq	%rbx
	cmpl	%r13d, %r12d
	jl	L22
LVL34:
L23:
LBE6:
LBE5:
	.loc 1 47 0
	addq	$8, %rsp
	popq	%rbx
	popq	%r12
LVL35:
	popq	%r13
LVL36:
	leave
	ret
LFE7:
	.cstring
LC5:
	.ascii " [%s='%s] '\0"
	.text
_elementStartCallback:
LFB6:
	.loc 1 22 0
LVL37:
	pushq	%rbp
LCFI18:
	movq	%rsp, %rbp
LCFI19:
	pushq	%r13
LCFI20:
	pushq	%r12
LCFI21:
	pushq	%rbx
LCFI22:
	subq	$8, %rsp
LCFI23:
	movq	%rsi, %r12
	movq	%rdx, %r13
	.loc 1 22 0
	xorl	%ebx, %ebx
LVL38:
	jmp	L25
LVL39:
L26:
LBB7:
	.loc 1 27 0
	leaq	LC4(%rip), %rdi
LVL40:
	xorl	%eax, %eax
	call	_printf
	.loc 1 26 0
	incl	%ebx
LVL41:
L25:
	cmpl	_Depth(%rip), %ebx
	jl	L26
	.loc 1 28 0
	movq	%r12, %rdi
LVL42:
	call	_puts
	movq	%r13, %rbx
LVL43:
	jmp	L28
L29:
	.loc 1 32 0
	movq	-8(%rbx), %rdx
	leaq	LC5(%rip), %rdi
	xorl	%eax, %eax
	call	_printf
L28:
	.loc 1 31 0
	movq	(%rbx), %rsi
	addq	$16, %rbx
	testq	%rsi, %rsi
	jne	L29
	.loc 1 34 0
	incl	_Depth(%rip)
LBE7:
	.loc 1 35 0
	addq	$8, %rsp
	popq	%rbx
	popq	%r12
LVL44:
	popq	%r13
LVL45:
	leave
	ret
LFE6:
.globl _Buff
.zerofill __DATA, __common, _Buff, 8192, 5
.globl _Depth
.zerofill __DATA, __common, _Depth, 4, 2
	.section __DWARF,__debug_frame,regular,debug
Lframe0:
	.set L$set$0,LECIE0-LSCIE0
	.long L$set$0
LSCIE0:
	.long	0xffffffff
	.byte	0x1
	.ascii "\0"
	.byte	0x1
	.byte	0x78
	.byte	0x10
	.byte	0xc
	.byte	0x7
	.byte	0x8
	.byte	0x90
	.byte	0x1
	.align 3
LECIE0:
LSFDE0:
	.set L$set$1,LEFDE0-LASFDE0
	.long L$set$1
LASFDE0:
	.set L$set$2,Lframe0-Lsection__debug_frame
	.long L$set$2
	.quad	LFB10
	.set L$set$3,LFE10-LFB10
	.quad L$set$3
	.byte	0x4
	.set L$set$4,LCFI0-LFB10
	.long L$set$4
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$5,LCFI1-LCFI0
	.long L$set$5
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE0:
LSFDE2:
	.set L$set$6,LEFDE2-LASFDE2
	.long L$set$6
LASFDE2:
	.set L$set$7,Lframe0-Lsection__debug_frame
	.long L$set$7
	.quad	LFB11
	.set L$set$8,LFE11-LFB11
	.quad L$set$8
	.byte	0x4
	.set L$set$9,LCFI2-LFB11
	.long L$set$9
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$10,LCFI3-LCFI2
	.long L$set$10
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE2:
LSFDE4:
	.set L$set$11,LEFDE4-LASFDE4
	.long L$set$11
LASFDE4:
	.set L$set$12,Lframe0-Lsection__debug_frame
	.long L$set$12
	.quad	LFB12
	.set L$set$13,LFE12-LFB12
	.quad L$set$13
	.byte	0x4
	.set L$set$14,LCFI4-LFB12
	.long L$set$14
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$15,LCFI5-LCFI4
	.long L$set$15
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$16,LCFI9-LCFI5
	.long L$set$16
	.byte	0x8d
	.byte	0x3
	.byte	0x8c
	.byte	0x4
	.byte	0x83
	.byte	0x5
	.align 3
LEFDE4:
LSFDE6:
	.set L$set$17,LEFDE6-LASFDE6
	.long L$set$17
LASFDE6:
	.set L$set$18,Lframe0-Lsection__debug_frame
	.long L$set$18
	.quad	LFB8
	.set L$set$19,LFE8-LFB8
	.quad L$set$19
	.byte	0x4
	.set L$set$20,LCFI10-LFB8
	.long L$set$20
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$21,LCFI11-LCFI10
	.long L$set$21
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE6:
LSFDE8:
	.set L$set$22,LEFDE8-LASFDE8
	.long L$set$22
LASFDE8:
	.set L$set$23,Lframe0-Lsection__debug_frame
	.long L$set$23
	.quad	LFB7
	.set L$set$24,LFE7-LFB7
	.quad L$set$24
	.byte	0x4
	.set L$set$25,LCFI12-LFB7
	.long L$set$25
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$26,LCFI13-LCFI12
	.long L$set$26
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$27,LCFI17-LCFI13
	.long L$set$27
	.byte	0x83
	.byte	0x5
	.byte	0x8c
	.byte	0x4
	.byte	0x8d
	.byte	0x3
	.align 3
LEFDE8:
LSFDE10:
	.set L$set$28,LEFDE10-LASFDE10
	.long L$set$28
LASFDE10:
	.set L$set$29,Lframe0-Lsection__debug_frame
	.long L$set$29
	.quad	LFB6
	.set L$set$30,LFE6-LFB6
	.quad L$set$30
	.byte	0x4
	.set L$set$31,LCFI18-LFB6
	.long L$set$31
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$32,LCFI19-LCFI18
	.long L$set$32
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$33,LCFI23-LCFI19
	.long L$set$33
	.byte	0x83
	.byte	0x5
	.byte	0x8c
	.byte	0x4
	.byte	0x8d
	.byte	0x3
	.align 3
LEFDE10:
	.section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame1:
	.set L$set$34,LECIE1-LSCIE1
	.long L$set$34
LSCIE1:
	.long	0x0
	.byte	0x1
	.ascii "zPR\0"
	.byte	0x1
	.byte	0x78
	.byte	0x10
	.byte	0x6
	.byte	0x9b
	.long	___gxx_personality_v0+4@GOTPCREL
	.byte	0x10
	.byte	0xc
	.byte	0x7
	.byte	0x8
	.byte	0x90
	.byte	0x1
	.align 3
LECIE1:
.globl __ZN12AXRXmlParserC2Ev.eh
__ZN12AXRXmlParserC2Ev.eh:
LSFDE1:
	.set L$set$35,LEFDE1-LASFDE1
	.long L$set$35
LASFDE1:
	.long	LASFDE1-EH_frame1
	.quad	LFB10-.
	.set L$set$36,LFE10-LFB10
	.quad L$set$36
	.byte	0x0
	.byte	0x4
	.set L$set$37,LCFI0-LFB10
	.long L$set$37
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$38,LCFI1-LCFI0
	.long L$set$38
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE1:
.globl __ZN12AXRXmlParserC1Ev.eh
__ZN12AXRXmlParserC1Ev.eh:
LSFDE3:
	.set L$set$39,LEFDE3-LASFDE3
	.long L$set$39
LASFDE3:
	.long	LASFDE3-EH_frame1
	.quad	LFB11-.
	.set L$set$40,LFE11-LFB11
	.quad L$set$40
	.byte	0x0
	.byte	0x4
	.set L$set$41,LCFI2-LFB11
	.long L$set$41
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$42,LCFI3-LCFI2
	.long L$set$42
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE3:
.globl __ZN12AXRXmlParser5parseEPKc.eh
__ZN12AXRXmlParser5parseEPKc.eh:
LSFDE5:
	.set L$set$43,LEFDE5-LASFDE5
	.long L$set$43
LASFDE5:
	.long	LASFDE5-EH_frame1
	.quad	LFB12-.
	.set L$set$44,LFE12-LFB12
	.quad L$set$44
	.byte	0x0
	.byte	0x4
	.set L$set$45,LCFI4-LFB12
	.long L$set$45
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$46,LCFI5-LCFI4
	.long L$set$46
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$47,LCFI9-LCFI5
	.long L$set$47
	.byte	0x8d
	.byte	0x3
	.byte	0x8c
	.byte	0x4
	.byte	0x83
	.byte	0x5
	.align 3
LEFDE5:
_elementEndCallback.eh:
LSFDE7:
	.set L$set$48,LEFDE7-LASFDE7
	.long L$set$48
LASFDE7:
	.long	LASFDE7-EH_frame1
	.quad	LFB8-.
	.set L$set$49,LFE8-LFB8
	.quad L$set$49
	.byte	0x0
	.byte	0x4
	.set L$set$50,LCFI10-LFB8
	.long L$set$50
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$51,LCFI11-LCFI10
	.long L$set$51
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE7:
_elementContentTextCallback.eh:
LSFDE9:
	.set L$set$52,LEFDE9-LASFDE9
	.long L$set$52
LASFDE9:
	.long	LASFDE9-EH_frame1
	.quad	LFB7-.
	.set L$set$53,LFE7-LFB7
	.quad L$set$53
	.byte	0x0
	.byte	0x4
	.set L$set$54,LCFI12-LFB7
	.long L$set$54
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$55,LCFI13-LCFI12
	.long L$set$55
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$56,LCFI17-LCFI13
	.long L$set$56
	.byte	0x83
	.byte	0x5
	.byte	0x8c
	.byte	0x4
	.byte	0x8d
	.byte	0x3
	.align 3
LEFDE9:
_elementStartCallback.eh:
LSFDE11:
	.set L$set$57,LEFDE11-LASFDE11
	.long L$set$57
LASFDE11:
	.long	LASFDE11-EH_frame1
	.quad	LFB6-.
	.set L$set$58,LFE6-LFB6
	.quad L$set$58
	.byte	0x0
	.byte	0x4
	.set L$set$59,LCFI18-LFB6
	.long L$set$59
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$60,LCFI19-LCFI18
	.long L$set$60
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$61,LCFI23-LCFI19
	.long L$set$61
	.byte	0x83
	.byte	0x5
	.byte	0x8c
	.byte	0x4
	.byte	0x8d
	.byte	0x3
	.align 3
LEFDE11:
	.text
Letext0:
	.section __DWARF,__debug_loc,regular,debug
Ldebug_loc0:
LLST0:
	.set L$set$62,LFB10-Ltext0
	.quad L$set$62
	.set L$set$63,LCFI0-Ltext0
	.quad L$set$63
	.word	0x2
	.byte	0x77
	.byte	0x8
	.set L$set$64,LCFI0-Ltext0
	.quad L$set$64
	.set L$set$65,LCFI1-Ltext0
	.quad L$set$65
	.word	0x2
	.byte	0x77
	.byte	0x10
	.set L$set$66,LCFI1-Ltext0
	.quad L$set$66
	.set L$set$67,LFE10-Ltext0
	.quad L$set$67
	.word	0x2
	.byte	0x76
	.byte	0x10
	.quad	0x0
	.quad	0x0
LLST1:
	.set L$set$68,LFB11-Ltext0
	.quad L$set$68
	.set L$set$69,LCFI2-Ltext0
	.quad L$set$69
	.word	0x2
	.byte	0x77
	.byte	0x8
	.set L$set$70,LCFI2-Ltext0
	.quad L$set$70
	.set L$set$71,LCFI3-Ltext0
	.quad L$set$71
	.word	0x2
	.byte	0x77
	.byte	0x10
	.set L$set$72,LCFI3-Ltext0
	.quad L$set$72
	.set L$set$73,LFE11-Ltext0
	.quad L$set$73
	.word	0x2
	.byte	0x76
	.byte	0x10
	.quad	0x0
	.quad	0x0
LLST2:
	.set L$set$74,LFB12-Ltext0
	.quad L$set$74
	.set L$set$75,LCFI4-Ltext0
	.quad L$set$75
	.word	0x2
	.byte	0x77
	.byte	0x8
	.set L$set$76,LCFI4-Ltext0
	.quad L$set$76
	.set L$set$77,LCFI5-Ltext0
	.quad L$set$77
	.word	0x2
	.byte	0x77
	.byte	0x10
	.set L$set$78,LCFI5-Ltext0
	.quad L$set$78
	.set L$set$79,LFE12-Ltext0
	.quad L$set$79
	.word	0x2
	.byte	0x76
	.byte	0x10
	.quad	0x0
	.quad	0x0
LLST3:
	.set L$set$80,LVL2-Ltext0
	.quad L$set$80
	.set L$set$81,LVL3-Ltext0
	.quad L$set$81
	.word	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
LLST4:
	.set L$set$82,LVL2-Ltext0
	.quad L$set$82
	.set L$set$83,LVL4-Ltext0
	.quad L$set$83
	.word	0x1
	.byte	0x54
	.set L$set$84,LVL4-Ltext0
	.quad L$set$84
	.set L$set$85,LVL11-Ltext0
	.quad L$set$85
	.word	0x1
	.byte	0x53
	.set L$set$86,LVL12-Ltext0
	.quad L$set$86
	.set L$set$87,LVL13-Ltext0
	.quad L$set$87
	.word	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
LLST5:
	.set L$set$88,LVL10-Ltext0
	.quad L$set$88
	.set L$set$89,LVL15-Ltext0
	.quad L$set$89
	.word	0x1
	.byte	0x5d
	.quad	0x0
	.quad	0x0
LLST6:
	.set L$set$90,LVL5-Ltext0
	.quad L$set$90
	.set L$set$91,LVL6-Ltext0
	.quad L$set$91
	.word	0x1
	.byte	0x5c
	.set L$set$92,LVL6-Ltext0
	.quad L$set$92
	.set L$set$93,LVL7-Ltext0
	.quad L$set$93
	.word	0x2
	.byte	0x50
	.byte	0xf0
	.set L$set$94,LVL7-Ltext0
	.quad L$set$94
	.set L$set$95,LVL8-Ltext0
	.quad L$set$95
	.word	0x1
	.byte	0x5c
	.set L$set$96,LVL8-Ltext0
	.quad L$set$96
	.set L$set$97,LVL9-Ltext0
	.quad L$set$97
	.word	0x2
	.byte	0x50
	.byte	0xf0
	.set L$set$98,LVL9-Ltext0
	.quad L$set$98
	.set L$set$99,LVL14-Ltext0
	.quad L$set$99
	.word	0x1
	.byte	0x5c
	.set L$set$100,LVL14-Ltext0
	.quad L$set$100
	.set L$set$101,LVL16-Ltext0
	.quad L$set$101
	.word	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
LLST7:
	.set L$set$102,LFB8-Ltext0
	.quad L$set$102
	.set L$set$103,LCFI10-Ltext0
	.quad L$set$103
	.word	0x2
	.byte	0x77
	.byte	0x8
	.set L$set$104,LCFI10-Ltext0
	.quad L$set$104
	.set L$set$105,LCFI11-Ltext0
	.quad L$set$105
	.word	0x2
	.byte	0x77
	.byte	0x10
	.set L$set$106,LCFI11-Ltext0
	.quad L$set$106
	.set L$set$107,LFE8-Ltext0
	.quad L$set$107
	.word	0x2
	.byte	0x76
	.byte	0x10
	.quad	0x0
	.quad	0x0
LLST8:
	.set L$set$108,LVL17-Ltext0
	.quad L$set$108
	.set L$set$109,LVL18-Ltext0
	.quad L$set$109
	.word	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
LLST9:
	.set L$set$110,LVL17-Ltext0
	.quad L$set$110
	.set L$set$111,LVL19-Ltext0
	.quad L$set$111
	.word	0x1
	.byte	0x54
	.quad	0x0
	.quad	0x0
LLST10:
	.set L$set$112,LFB7-Ltext0
	.quad L$set$112
	.set L$set$113,LCFI12-Ltext0
	.quad L$set$113
	.word	0x2
	.byte	0x77
	.byte	0x8
	.set L$set$114,LCFI12-Ltext0
	.quad L$set$114
	.set L$set$115,LCFI13-Ltext0
	.quad L$set$115
	.word	0x2
	.byte	0x77
	.byte	0x10
	.set L$set$116,LCFI13-Ltext0
	.quad L$set$116
	.set L$set$117,LFE7-Ltext0
	.quad L$set$117
	.word	0x2
	.byte	0x76
	.byte	0x10
	.quad	0x0
	.quad	0x0
LLST11:
	.set L$set$118,LVL20-Ltext0
	.quad L$set$118
	.set L$set$119,LVL24-Ltext0
	.quad L$set$119
	.word	0x1
	.byte	0x55
	.set L$set$120,LVL26-Ltext0
	.quad L$set$120
	.set L$set$121,LVL31-Ltext0
	.quad L$set$121
	.word	0x1
	.byte	0x55
	.set L$set$122,LVL33-Ltext0
	.quad L$set$122
	.set L$set$123,LFE7-Ltext0
	.quad L$set$123
	.word	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
LLST12:
	.set L$set$124,LVL20-Ltext0
	.quad L$set$124
	.set L$set$125,LVL25-Ltext0
	.quad L$set$125
	.word	0x1
	.byte	0x54
	.set L$set$126,LVL25-Ltext0
	.quad L$set$126
	.set L$set$127,LVL27-Ltext0
	.quad L$set$127
	.word	0x1
	.byte	0x5c
	.set L$set$128,LVL27-Ltext0
	.quad L$set$128
	.set L$set$129,LVL32-Ltext0
	.quad L$set$129
	.word	0x1
	.byte	0x54
	.set L$set$130,LVL33-Ltext0
	.quad L$set$130
	.set L$set$131,LFE7-Ltext0
	.quad L$set$131
	.word	0x1
	.byte	0x54
	.quad	0x0
	.quad	0x0
LLST13:
	.set L$set$132,LVL20-Ltext0
	.quad L$set$132
	.set L$set$133,LVL21-Ltext0
	.quad L$set$133
	.word	0x1
	.byte	0x51
	.set L$set$134,LVL21-Ltext0
	.quad L$set$134
	.set L$set$135,LVL23-Ltext0
	.quad L$set$135
	.word	0x1
	.byte	0x5d
	.set L$set$136,LVL23-Ltext0
	.quad L$set$136
	.set L$set$137,LVL25-Ltext0
	.quad L$set$137
	.word	0x1
	.byte	0x51
	.set L$set$138,LVL25-Ltext0
	.quad L$set$138
	.set L$set$139,LVL27-Ltext0
	.quad L$set$139
	.word	0x1
	.byte	0x5d
	.set L$set$140,LVL27-Ltext0
	.quad L$set$140
	.set L$set$141,LVL30-Ltext0
	.quad L$set$141
	.word	0x1
	.byte	0x51
	.set L$set$142,LVL30-Ltext0
	.quad L$set$142
	.set L$set$143,LVL36-Ltext0
	.quad L$set$143
	.word	0x1
	.byte	0x5d
	.set L$set$144,LVL36-Ltext0
	.quad L$set$144
	.set L$set$145,LFE7-Ltext0
	.quad L$set$145
	.word	0x1
	.byte	0x51
	.quad	0x0
	.quad	0x0
LLST14:
	.set L$set$146,LVL22-Ltext0
	.quad L$set$146
	.set L$set$147,LVL26-Ltext0
	.quad L$set$147
	.word	0x1
	.byte	0x53
	.set L$set$148,LVL26-Ltext0
	.quad L$set$148
	.set L$set$149,LVL28-Ltext0
	.quad L$set$149
	.word	0x1
	.byte	0x53
	.set L$set$150,LVL29-Ltext0
	.quad L$set$150
	.set L$set$151,LVL33-Ltext0
	.quad L$set$151
	.word	0x1
	.byte	0x5c
	.set L$set$152,LVL33-Ltext0
	.quad L$set$152
	.set L$set$153,LVL34-Ltext0
	.quad L$set$153
	.word	0x1
	.byte	0x5c
	.set L$set$154,LVL34-Ltext0
	.quad L$set$154
	.set L$set$155,LVL35-Ltext0
	.quad L$set$155
	.word	0x1
	.byte	0x5c
	.quad	0x0
	.quad	0x0
LLST15:
	.set L$set$156,LFB6-Ltext0
	.quad L$set$156
	.set L$set$157,LCFI18-Ltext0
	.quad L$set$157
	.word	0x2
	.byte	0x77
	.byte	0x8
	.set L$set$158,LCFI18-Ltext0
	.quad L$set$158
	.set L$set$159,LCFI19-Ltext0
	.quad L$set$159
	.word	0x2
	.byte	0x77
	.byte	0x10
	.set L$set$160,LCFI19-Ltext0
	.quad L$set$160
	.set L$set$161,LFE6-Ltext0
	.quad L$set$161
	.word	0x2
	.byte	0x76
	.byte	0x10
	.quad	0x0
	.quad	0x0
LLST16:
	.set L$set$162,LVL37-Ltext0
	.quad L$set$162
	.set L$set$163,LVL40-Ltext0
	.quad L$set$163
	.word	0x1
	.byte	0x55
	.set L$set$164,LVL41-Ltext0
	.quad L$set$164
	.set L$set$165,LVL42-Ltext0
	.quad L$set$165
	.word	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
LLST17:
	.set L$set$166,LVL37-Ltext0
	.quad L$set$166
	.set L$set$167,LVL39-Ltext0
	.quad L$set$167
	.word	0x1
	.byte	0x54
	.set L$set$168,LVL39-Ltext0
	.quad L$set$168
	.set L$set$169,LVL44-Ltext0
	.quad L$set$169
	.word	0x1
	.byte	0x5c
	.quad	0x0
	.quad	0x0
LLST18:
	.set L$set$170,LVL37-Ltext0
	.quad L$set$170
	.set L$set$171,LVL39-Ltext0
	.quad L$set$171
	.word	0x1
	.byte	0x51
	.set L$set$172,LVL39-Ltext0
	.quad L$set$172
	.set L$set$173,LVL45-Ltext0
	.quad L$set$173
	.word	0x1
	.byte	0x5d
	.quad	0x0
	.quad	0x0
LLST19:
	.set L$set$174,LVL38-Ltext0
	.quad L$set$174
	.set L$set$175,LVL41-Ltext0
	.quad L$set$175
	.word	0x1
	.byte	0x53
	.set L$set$176,LVL41-Ltext0
	.quad L$set$176
	.set L$set$177,LVL43-Ltext0
	.quad L$set$177
	.word	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
	.file 2 "/Developer/SDKs/MacOSX10.6.sdk/usr/include/i386/_types.h"
	.file 3 "/Developer/SDKs/MacOSX10.6.sdk/usr/include/sys/_types.h"
	.file 4 "/Developer/SDKs/MacOSX10.6.sdk/usr/include/expat_external.h"
	.file 5 "/Developer/SDKs/MacOSX10.6.sdk/usr/include/expat.h"
	.file 6 "/Users/miro/AXR/prototype/core/xml/AXRXmlParser.h"
	.file 7 "/Developer/SDKs/MacOSX10.6.sdk/usr/include/stdio.h"
	.section __DWARF,__debug_info,regular,debug
	.long	0x6e1
	.word	0x2
	.set L$set$178,Ldebug_abbrev0-Lsection__debug_abbrev
	.long L$set$178
	.byte	0x8
	.byte	0x1
	.ascii "GNU C++ 4.2.1 (Apple Inc. build 5664)\0"
	.byte	0x4
	.ascii "/Users/miro/AXR/prototype/core/xml/AXRXmlParser.cpp\0"
	.byte	0x1
	.quad	Ltext0
	.quad	Letext0
	.set L$set$179,Ldebug_line0-Lsection__debug_line
	.long L$set$179
	.byte	0x2
	.byte	0x1
	.byte	0x6
	.ascii "signed char\0"
	.byte	0x2
	.byte	0x1
	.byte	0x8
	.ascii "unsigned char\0"
	.byte	0x2
	.byte	0x2
	.byte	0x5
	.ascii "short int\0"
	.byte	0x2
	.byte	0x2
	.byte	0x7
	.ascii "short unsigned int\0"
	.byte	0x2
	.byte	0x4
	.byte	0x5
	.ascii "int\0"
	.byte	0x2
	.byte	0x4
	.byte	0x7
	.ascii "unsigned int\0"
	.byte	0x3
	.ascii "__int64_t\0"
	.byte	0x2
	.byte	0x2e
	.long	0xe7
	.byte	0x2
	.byte	0x8
	.byte	0x5
	.ascii "long long int\0"
	.byte	0x2
	.byte	0x8
	.byte	0x7
	.ascii "long long unsigned int\0"
	.byte	0x2
	.byte	0x8
	.byte	0x5
	.ascii "long int\0"
	.byte	0x4
	.byte	0x8
	.byte	0x7
	.byte	0x2
	.byte	0x1
	.byte	0x6
	.ascii "char\0"
	.byte	0x2
	.byte	0x8
	.byte	0x7
	.ascii "long unsigned int\0"
	.byte	0x5
	.byte	0x8
	.byte	0x3
	.ascii "__darwin_off_t\0"
	.byte	0x3
	.byte	0x6e
	.long	0xd6
	.byte	0x6
	.byte	0x8
	.long	0x121
	.byte	0x3
	.ascii "XML_Char\0"
	.byte	0x4
	.byte	0x5e
	.long	0x121
	.byte	0x3
	.ascii "XML_Parser\0"
	.byte	0x5
	.byte	0x19
	.long	0x17e
	.byte	0x6
	.byte	0x8
	.long	0x184
	.byte	0x7
	.ascii "XML_ParserStruct\0"
	.byte	0x1
	.byte	0x8
	.ascii "XML_Status\0"
	.byte	0x4
	.byte	0x5
	.byte	0x2d
	.long	0x1e5
	.byte	0x9
	.ascii "XML_STATUS_ERROR\0"
	.byte	0x0
	.byte	0x9
	.ascii "XML_STATUS_OK\0"
	.byte	0x1
	.byte	0x9
	.ascii "XML_STATUS_SUSPENDED\0"
	.byte	0x2
	.byte	0x0
	.byte	0x6
	.byte	0x8
	.long	0x1eb
	.byte	0xa
	.long	0x15c
	.byte	0x6
	.byte	0x8
	.long	0x1f6
	.byte	0xb
	.long	0xbf
	.long	0x205
	.byte	0xc
	.long	0x13e
	.byte	0x0
	.byte	0x6
	.byte	0x8
	.long	0x20b
	.byte	0xa
	.long	0x121
	.byte	0xd
	.set L$set$180,LASF0-Lsection__debug_str
	.long L$set$180
	.byte	0x1
	.byte	0x6
	.byte	0x11
	.long	0x24c
	.byte	0xe
	.byte	0x1
	.set L$set$181,LASF0-Lsection__debug_str
	.long L$set$181
	.byte	0x6
	.byte	0x13
	.byte	0x1
	.byte	0x1
	.long	0x231
	.byte	0xf
	.long	0x24c
	.byte	0x1
	.byte	0x0
	.byte	0x10
	.byte	0x1
	.set L$set$182,LASF1-Lsection__debug_str
	.long L$set$182
	.byte	0x6
	.byte	0x14
	.set L$set$183,LASF7-Lsection__debug_str
	.long L$set$183
	.byte	0x1
	.byte	0x1
	.byte	0xf
	.long	0x24c
	.byte	0x1
	.byte	0xc
	.long	0x205
	.byte	0x0
	.byte	0x0
	.byte	0x6
	.byte	0x8
	.long	0x210
	.byte	0x3
	.ascii "fpos_t\0"
	.byte	0x7
	.byte	0x57
	.long	0x140
	.byte	0x11
	.ascii "__sbuf\0"
	.byte	0x10
	.byte	0x7
	.byte	0x62
	.long	0x290
	.byte	0x12
	.ascii "_base\0"
	.byte	0x7
	.byte	0x63
	.long	0x290
	.byte	0x2
	.byte	0x23
	.byte	0x0
	.byte	0x12
	.ascii "_size\0"
	.byte	0x7
	.byte	0x64
	.long	0xbf
	.byte	0x2
	.byte	0x23
	.byte	0x8
	.byte	0x0
	.byte	0x6
	.byte	0x8
	.long	0x8b
	.byte	0x11
	.ascii "__sFILE\0"
	.byte	0x98
	.byte	0x7
	.byte	0x84
	.long	0x3e6
	.byte	0x12
	.ascii "_p\0"
	.byte	0x7
	.byte	0x85
	.long	0x290
	.byte	0x2
	.byte	0x23
	.byte	0x0
	.byte	0x12
	.ascii "_r\0"
	.byte	0x7
	.byte	0x86
	.long	0xbf
	.byte	0x2
	.byte	0x23
	.byte	0x8
	.byte	0x12
	.ascii "_w\0"
	.byte	0x7
	.byte	0x87
	.long	0xbf
	.byte	0x2
	.byte	0x23
	.byte	0xc
	.byte	0x12
	.ascii "_flags\0"
	.byte	0x7
	.byte	0x88
	.long	0x9c
	.byte	0x2
	.byte	0x23
	.byte	0x10
	.byte	0x12
	.ascii "_file\0"
	.byte	0x7
	.byte	0x89
	.long	0x9c
	.byte	0x2
	.byte	0x23
	.byte	0x12
	.byte	0x12
	.ascii "_bf\0"
	.byte	0x7
	.byte	0x8a
	.long	0x260
	.byte	0x2
	.byte	0x23
	.byte	0x18
	.byte	0x12
	.ascii "_lbfsize\0"
	.byte	0x7
	.byte	0x8b
	.long	0xbf
	.byte	0x2
	.byte	0x23
	.byte	0x28
	.byte	0x12
	.ascii "_cookie\0"
	.byte	0x7
	.byte	0x8e
	.long	0x13e
	.byte	0x2
	.byte	0x23
	.byte	0x30
	.byte	0x12
	.ascii "_close\0"
	.byte	0x7
	.byte	0x8f
	.long	0x1f0
	.byte	0x2
	.byte	0x23
	.byte	0x38
	.byte	0x12
	.ascii "_read\0"
	.byte	0x7
	.byte	0x90
	.long	0x3ff
	.byte	0x2
	.byte	0x23
	.byte	0x40
	.byte	0x12
	.ascii "_seek\0"
	.byte	0x7
	.byte	0x91
	.long	0x41e
	.byte	0x2
	.byte	0x23
	.byte	0x48
	.byte	0x12
	.ascii "_write\0"
	.byte	0x7
	.byte	0x92
	.long	0x43d
	.byte	0x2
	.byte	0x23
	.byte	0x50
	.byte	0x12
	.ascii "_ub\0"
	.byte	0x7
	.byte	0x95
	.long	0x260
	.byte	0x2
	.byte	0x23
	.byte	0x58
	.byte	0x12
	.ascii "_extra\0"
	.byte	0x7
	.byte	0x96
	.long	0x44e
	.byte	0x2
	.byte	0x23
	.byte	0x68
	.byte	0x12
	.ascii "_ur\0"
	.byte	0x7
	.byte	0x97
	.long	0xbf
	.byte	0x2
	.byte	0x23
	.byte	0x70
	.byte	0x12
	.ascii "_ubuf\0"
	.byte	0x7
	.byte	0x9a
	.long	0x454
	.byte	0x2
	.byte	0x23
	.byte	0x74
	.byte	0x12
	.ascii "_nbuf\0"
	.byte	0x7
	.byte	0x9b
	.long	0x464
	.byte	0x2
	.byte	0x23
	.byte	0x77
	.byte	0x12
	.ascii "_lb\0"
	.byte	0x7
	.byte	0x9e
	.long	0x260
	.byte	0x2
	.byte	0x23
	.byte	0x78
	.byte	0x12
	.ascii "_blksize\0"
	.byte	0x7
	.byte	0xa1
	.long	0xbf
	.byte	0x3
	.byte	0x23
	.byte	0x88,0x1
	.byte	0x12
	.ascii "_offset\0"
	.byte	0x7
	.byte	0xa2
	.long	0x252
	.byte	0x3
	.byte	0x23
	.byte	0x90,0x1
	.byte	0x0
	.byte	0xb
	.long	0xbf
	.long	0x3ff
	.byte	0xc
	.long	0x13e
	.byte	0xc
	.long	0x156
	.byte	0xc
	.long	0xbf
	.byte	0x0
	.byte	0x6
	.byte	0x8
	.long	0x3e6
	.byte	0xb
	.long	0x252
	.long	0x41e
	.byte	0xc
	.long	0x13e
	.byte	0xc
	.long	0x252
	.byte	0xc
	.long	0xbf
	.byte	0x0
	.byte	0x6
	.byte	0x8
	.long	0x405
	.byte	0xb
	.long	0xbf
	.long	0x43d
	.byte	0xc
	.long	0x13e
	.byte	0xc
	.long	0x205
	.byte	0xc
	.long	0xbf
	.byte	0x0
	.byte	0x6
	.byte	0x8
	.long	0x424
	.byte	0x7
	.ascii "__sFILEX\0"
	.byte	0x1
	.byte	0x6
	.byte	0x8
	.long	0x443
	.byte	0x13
	.long	0x8b
	.long	0x464
	.byte	0x14
	.long	0x11e
	.byte	0x2
	.byte	0x0
	.byte	0x13
	.long	0x8b
	.long	0x474
	.byte	0x14
	.long	0x11e
	.byte	0x0
	.byte	0x0
	.byte	0x3
	.ascii "FILE\0"
	.byte	0x7
	.byte	0xa3
	.long	0x296
	.byte	0x15
	.long	0x21c
	.byte	0x1
	.byte	0x38
	.byte	0x0
	.byte	0x1
	.long	0x499
	.byte	0x16
	.ascii "this\0"
	.long	0x499
	.byte	0x1
	.byte	0x0
	.byte	0xa
	.long	0x24c
	.byte	0x17
	.long	0x480
	.set L$set$184,LASF2-Lsection__debug_str
	.long L$set$184
	.quad	LFB10
	.quad	LFE10
	.set L$set$185,LLST0-Lsection__debug_loc
	.long L$set$185
	.byte	0x1
	.long	0x4c8
	.byte	0x18
	.long	0x48d
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.byte	0x17
	.long	0x480
	.set L$set$186,LASF3-Lsection__debug_str
	.long L$set$186
	.quad	LFB11
	.quad	LFE11
	.set L$set$187,LLST1-Lsection__debug_loc
	.long L$set$187
	.byte	0x1
	.long	0x4f2
	.byte	0x18
	.long	0x48d
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.byte	0x19
	.long	0x231
	.byte	0x1
	.byte	0x3d
	.quad	LFB12
	.quad	LFE12
	.set L$set$188,LLST2-Lsection__debug_loc
	.long L$set$188
	.byte	0x1
	.long	0x57b
	.byte	0x1a
	.ascii "this\0"
	.long	0x499
	.byte	0x1
	.set L$set$189,LLST3-Lsection__debug_loc
	.long L$set$189
	.byte	0x1b
	.ascii "path\0"
	.byte	0x1
	.byte	0x3d
	.long	0x205
	.set L$set$190,LLST4-Lsection__debug_loc
	.long L$set$190
	.byte	0x1c
	.ascii "xmlfile\0"
	.byte	0x1
	.byte	0x3f
	.long	0x57b
	.set L$set$191,LLST5-Lsection__debug_loc
	.long L$set$191
	.byte	0x1c
	.ascii "p\0"
	.byte	0x1
	.byte	0x41
	.long	0x16c
	.set L$set$192,LLST6-Lsection__debug_loc
	.long L$set$192
	.byte	0x1d
	.ascii "done\0"
	.byte	0x1
	.byte	0x4b
	.long	0xc6
	.byte	0x1e
	.quad	LBB3
	.quad	LBE3
	.byte	0x1d
	.ascii "len\0"
	.byte	0x1
	.byte	0x4d
	.long	0xbf
	.byte	0x0
	.byte	0x0
	.byte	0x6
	.byte	0x8
	.long	0x474
	.byte	0x1f
	.set L$set$193,LASF4-Lsection__debug_str
	.long L$set$193
	.byte	0x1
	.byte	0x31
	.quad	LFB8
	.quad	LFE8
	.set L$set$194,LLST7-Lsection__debug_loc
	.long L$set$194
	.byte	0x1
	.long	0x5c0
	.byte	0x1b
	.ascii "data\0"
	.byte	0x1
	.byte	0x31
	.long	0x13e
	.set L$set$195,LLST8-Lsection__debug_loc
	.long L$set$195
	.byte	0x1b
	.ascii "el\0"
	.byte	0x1
	.byte	0x31
	.long	0x205
	.set L$set$196,LLST9-Lsection__debug_loc
	.long L$set$196
	.byte	0x0
	.byte	0x1f
	.set L$set$197,LASF5-Lsection__debug_str
	.long L$set$197
	.byte	0x1
	.byte	0x25
	.quad	LFB7
	.quad	LFE7
	.set L$set$198,LLST10-Lsection__debug_loc
	.long L$set$198
	.byte	0x1
	.long	0x62f
	.byte	0x1b
	.ascii "data\0"
	.byte	0x1
	.byte	0x25
	.long	0x13e
	.set L$set$199,LLST11-Lsection__debug_loc
	.long L$set$199
	.byte	0x1b
	.ascii "text\0"
	.byte	0x1
	.byte	0x25
	.long	0x1e5
	.set L$set$200,LLST12-Lsection__debug_loc
	.long L$set$200
	.byte	0x1b
	.ascii "len\0"
	.byte	0x1
	.byte	0x25
	.long	0xbf
	.set L$set$201,LLST13-Lsection__debug_loc
	.long L$set$201
	.byte	0x1e
	.quad	LBB6
	.quad	LBE6
	.byte	0x1c
	.ascii "i\0"
	.byte	0x1
	.byte	0x28
	.long	0xbf
	.set L$set$202,LLST14-Lsection__debug_loc
	.long L$set$202
	.byte	0x0
	.byte	0x0
	.byte	0x1f
	.set L$set$203,LASF6-Lsection__debug_str
	.long L$set$203
	.byte	0x1
	.byte	0x16
	.quad	LFB6
	.quad	LFE6
	.set L$set$204,LLST15-Lsection__debug_loc
	.long L$set$204
	.byte	0x1
	.long	0x68b
	.byte	0x1b
	.ascii "data\0"
	.byte	0x1
	.byte	0x16
	.long	0x13e
	.set L$set$205,LLST16-Lsection__debug_loc
	.long L$set$205
	.byte	0x1b
	.ascii "el\0"
	.byte	0x1
	.byte	0x16
	.long	0x205
	.set L$set$206,LLST17-Lsection__debug_loc
	.long L$set$206
	.byte	0x1b
	.ascii "attr\0"
	.byte	0x1
	.byte	0x16
	.long	0x68b
	.set L$set$207,LLST18-Lsection__debug_loc
	.long L$set$207
	.byte	0x1c
	.ascii "i\0"
	.byte	0x1
	.byte	0x18
	.long	0xbf
	.set L$set$208,LLST19-Lsection__debug_loc
	.long L$set$208
	.byte	0x0
	.byte	0x6
	.byte	0x8
	.long	0x205
	.byte	0x13
	.long	0x121
	.long	0x6a2
	.byte	0x20
	.long	0x11e
	.word	0x1fff
	.byte	0x0
	.byte	0x21
	.ascii "Buff\0"
	.byte	0x1
	.byte	0xd
	.long	0x691
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	_Buff
	.byte	0x22
	.ascii "__stderrp\0"
	.byte	0x7
	.byte	0xa9
	.long	0x57b
	.byte	0x1
	.byte	0x1
	.byte	0x21
	.ascii "Depth\0"
	.byte	0x1
	.byte	0x14
	.long	0xbf
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	_Depth
	.byte	0x0
	.section __DWARF,__debug_abbrev,regular,debug
	.byte	0x1
	.byte	0x11
	.byte	0x1
	.byte	0x25
	.byte	0x8
	.byte	0x13
	.byte	0xb
	.byte	0x3
	.byte	0x8
	.byte	0xe1,0x7f
	.byte	0xc
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x1
	.byte	0x10
	.byte	0x6
	.byte	0x0
	.byte	0x0
	.byte	0x2
	.byte	0x24
	.byte	0x0
	.byte	0xb
	.byte	0xb
	.byte	0x3e
	.byte	0xb
	.byte	0x3
	.byte	0x8
	.byte	0x0
	.byte	0x0
	.byte	0x3
	.byte	0x16
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x4
	.byte	0x24
	.byte	0x0
	.byte	0xb
	.byte	0xb
	.byte	0x3e
	.byte	0xb
	.byte	0x0
	.byte	0x0
	.byte	0x5
	.byte	0xf
	.byte	0x0
	.byte	0xb
	.byte	0xb
	.byte	0x0
	.byte	0x0
	.byte	0x6
	.byte	0xf
	.byte	0x0
	.byte	0xb
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x7
	.byte	0x13
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3c
	.byte	0xc
	.byte	0x0
	.byte	0x0
	.byte	0x8
	.byte	0x4
	.byte	0x1
	.byte	0x3
	.byte	0x8
	.byte	0xb
	.byte	0xb
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x9
	.byte	0x28
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x1c
	.byte	0xd
	.byte	0x0
	.byte	0x0
	.byte	0xa
	.byte	0x26
	.byte	0x0
	.byte	0x49
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0xb
	.byte	0x15
	.byte	0x1
	.byte	0x49
	.byte	0x13
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0xc
	.byte	0x5
	.byte	0x0
	.byte	0x49
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0xd
	.byte	0x13
	.byte	0x1
	.byte	0x3
	.byte	0xe
	.byte	0xb
	.byte	0xb
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0xe
	.byte	0x2e
	.byte	0x1
	.byte	0x3f
	.byte	0xc
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x3c
	.byte	0xc
	.byte	0xe1,0x7f
	.byte	0xc
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0xf
	.byte	0x5
	.byte	0x0
	.byte	0x49
	.byte	0x13
	.byte	0x34
	.byte	0xc
	.byte	0x0
	.byte	0x0
	.byte	0x10
	.byte	0x2e
	.byte	0x1
	.byte	0x3f
	.byte	0xc
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x87,0x40
	.byte	0xe
	.byte	0x3c
	.byte	0xc
	.byte	0xe1,0x7f
	.byte	0xc
	.byte	0x0
	.byte	0x0
	.byte	0x11
	.byte	0x13
	.byte	0x1
	.byte	0x3
	.byte	0x8
	.byte	0xb
	.byte	0xb
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x12
	.byte	0xd
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x38
	.byte	0xa
	.byte	0x0
	.byte	0x0
	.byte	0x13
	.byte	0x1
	.byte	0x1
	.byte	0x49
	.byte	0x13
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x14
	.byte	0x21
	.byte	0x0
	.byte	0x49
	.byte	0x13
	.byte	0x2f
	.byte	0xb
	.byte	0x0
	.byte	0x0
	.byte	0x15
	.byte	0x2e
	.byte	0x1
	.byte	0x47
	.byte	0x13
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x20
	.byte	0xb
	.byte	0xe1,0x7f
	.byte	0xc
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x16
	.byte	0x5
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x49
	.byte	0x13
	.byte	0x34
	.byte	0xc
	.byte	0x0
	.byte	0x0
	.byte	0x17
	.byte	0x2e
	.byte	0x1
	.byte	0x31
	.byte	0x13
	.byte	0x87,0x40
	.byte	0xe
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x1
	.byte	0x40
	.byte	0x6
	.byte	0xe1,0x7f
	.byte	0xc
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x18
	.byte	0x5
	.byte	0x0
	.byte	0x31
	.byte	0x13
	.byte	0x2
	.byte	0xa
	.byte	0x0
	.byte	0x0
	.byte	0x19
	.byte	0x2e
	.byte	0x1
	.byte	0x47
	.byte	0x13
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x1
	.byte	0x40
	.byte	0x6
	.byte	0xe1,0x7f
	.byte	0xc
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x1a
	.byte	0x5
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x49
	.byte	0x13
	.byte	0x34
	.byte	0xc
	.byte	0x2
	.byte	0x6
	.byte	0x0
	.byte	0x0
	.byte	0x1b
	.byte	0x5
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x2
	.byte	0x6
	.byte	0x0
	.byte	0x0
	.byte	0x1c
	.byte	0x34
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x2
	.byte	0x6
	.byte	0x0
	.byte	0x0
	.byte	0x1d
	.byte	0x34
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x1e
	.byte	0xb
	.byte	0x1
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.byte	0x1f
	.byte	0x2e
	.byte	0x1
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x1
	.byte	0x40
	.byte	0x6
	.byte	0xe1,0x7f
	.byte	0xc
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x20
	.byte	0x21
	.byte	0x0
	.byte	0x49
	.byte	0x13
	.byte	0x2f
	.byte	0x5
	.byte	0x0
	.byte	0x0
	.byte	0x21
	.byte	0x34
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x3f
	.byte	0xc
	.byte	0x2
	.byte	0xa
	.byte	0x0
	.byte	0x0
	.byte	0x22
	.byte	0x34
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x3f
	.byte	0xc
	.byte	0x3c
	.byte	0xc
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section __DWARF,__debug_pubnames,regular,debug
	.long	0x77
	.word	0x2
	.set L$set$209,Ldebug_info0-Lsection__debug_info
	.long L$set$209
	.long	0x6e5
	.long	0x49e
	.ascii "AXRXmlParser::AXRXmlParser\0"
	.long	0x4c8
	.ascii "AXRXmlParser::AXRXmlParser\0"
	.long	0x4f2
	.ascii "AXRXmlParser::parse\0"
	.long	0x6a2
	.ascii "Buff\0"
	.long	0x6cc
	.ascii "Depth\0"
	.long	0x0
	.section __DWARF,__debug_pubtypes,regular,debug
	.long	0x96
	.word	0x2
	.set L$set$210,Ldebug_info0-Lsection__debug_info
	.long L$set$210
	.long	0x6e5
	.long	0xd6
	.ascii "__int64_t\0"
	.long	0x140
	.ascii "__darwin_off_t\0"
	.long	0x15c
	.ascii "XML_Char\0"
	.long	0x16c
	.ascii "XML_Parser\0"
	.long	0x197
	.ascii "XML_Status\0"
	.long	0x210
	.ascii "AXRXmlParser\0"
	.long	0x252
	.ascii "fpos_t\0"
	.long	0x260
	.ascii "__sbuf\0"
	.long	0x296
	.ascii "__sFILE\0"
	.long	0x474
	.ascii "FILE\0"
	.long	0x0
	.section __DWARF,__debug_aranges,regular,debug
	.long	0x2c
	.word	0x2
	.set L$set$211,Ldebug_info0-Lsection__debug_info
	.long L$set$211
	.byte	0x8
	.byte	0x0
	.word	0x0
	.word	0x0
	.quad	Ltext0
	.set L$set$212,Letext0-Ltext0
	.quad L$set$212
	.quad	0x0
	.quad	0x0
	.section __DWARF,__debug_str,regular,debug
LASF0:
	.ascii "AXRXmlParser\0"
LASF1:
	.ascii "parse\0"
LASF2:
	.ascii "_ZN12AXRXmlParserC2Ev\0"
LASF5:
	.ascii "elementContentTextCallback\0"
LASF3:
	.ascii "_ZN12AXRXmlParserC1Ev\0"
LASF6:
	.ascii "elementStartCallback\0"
LASF7:
	.ascii "_ZN12AXRXmlParser5parseEPKc\0"
LASF4:
	.ascii "elementEndCallback\0"
	.constructor
	.destructor
	.align 1
	.subsections_via_symbols
