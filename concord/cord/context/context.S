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

    subq $64, %rsp
    
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
    
    movq %rsp, (%rdi)
    
    movq (%rsi), %rsp
    
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx
    popq %rbp
    
    retq

.section .note.GNU-stack,"",%progbits
