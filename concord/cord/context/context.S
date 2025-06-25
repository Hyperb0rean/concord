.text
.globl make_context
.type make_context,@function
.align 16
make_context:
    # rdi - stack_bottom
    # rsi - thunk
    # rdx - runnable
    movq %rsp, %r11
    movq %rdi, %rsp
    
    andq $-16, %rsp              
    subq $8, %rsp
    
    movq %rdx, 8(%rsp)  

    pushq %rsi

    pushq %rbp
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15   

    subq $8, %rsp
    stmxcsr  (%rsp)     # MMX control- and status-word
    fnstcw   0x4(%rsp)  # save x87 control-word 
     
    movq %rsp, %rax
    
    movq %r11, %rsp
    retq   


.globl context_switch
.type context_switch,@function
.align 16
context_switch:
    pushq %rbp
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    subq $8, %rsp
    stmxcsr  (%rsp)     # MMX control- and status-word
    fnstcw   0x4(%rsp)  # save x87 control-word 
    
    movq %rsp, (%rdi)
    
    movq (%rsi), %rsp

    ldmxcsr  (%rsp)     # MMX control- and status-word
    fldcw   0x4(%rsp)  # save x87 control-word 
    leaq  0x8(%rsp), %rsp

    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
    popq %rbp
    
    retq

.section .note.GNU-stack,"",%progbits
