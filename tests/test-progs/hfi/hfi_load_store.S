.text

.globl hfi_load_store_test
.type hfi_load_store_test,@function
.align 16
hfi_load_store_test:
_hfi_load_store_test:
    # rdi: param 1 is the sandbox params
    # rsi: param 2 is the load address
    # rdx: param 3 is the store address
    # enter the sandbox, perform load&store, then exit
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x71, 0x00 # set sandbox metadata
    .byte   0x0F, 0x04, 0x65, 0x00 # enter sandbox
    mov 0x0(%rsi), %rcx
    mov %rcx, 0x0(%rdx)
    .byte   0x0F, 0x04, 0x66, 0x00 # exit sandbox
    ret

.globl hfi_load_store_ret_test
.type hfi_load_store_ret_test,@function
.align 16
hfi_load_store_ret_test:
_hfi_load_store_ret_test:
    # rdi: param 1 is the sandbox params
    # rsi: param 2 is the load address
    # rdx: param 3 is the store address
    # enter the sandbox, perform load&store, then exit
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x71, 0x00 # set sandbox metadata
    .byte   0x0F, 0x04, 0x65, 0x00 # enter sandbox
    mov 0x0(%rsi), %rcx
    mov %rcx, 0x0(%rdx)
    call hfi_exit_sandbox
    // .byte   0x0F, 0x04, 0x66, 0x00 # exit sandbox
    ret

.globl hfi_load_store_push_pop_test
.type hfi_load_store_push_pop_test,@function
.align 16
hfi_load_store_push_pop_test:
_hfi_load_store_push_pop_test:
    # rdi: param 1 is the sandbox params
    # rsi: param 2 is the load address
    # rdx: param 3 is the store address
    # enter the sandbox, perform load&store, push&pop, then exit
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x71, 0x00 # set sandbox metadata
    .byte   0x0F, 0x04, 0x65, 0x00 # enter sandbox
    mov 0x0(%rsi), %rcx
    push %rcx
    pop %r8
    mov %r8, 0x0(%rdx)
    .byte   0x0F, 0x04, 0x66, 0x00 # exit sandbox
    ret

.globl hfi_ur_load_store_test
.type hfi_ur_load_store_test,@function
.align 16
hfi_ur_load_store_test:
_hfi_ur_load_store_test:
    # rdi: param 1 is the sandbox params
    # rsi: param 2 is the load address
    # rdx: param 3 is the store address
    # enter the sandbox, perform load&store, then exit
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x71, 0x00 # set sandbox metadata
    .byte   0x0F, 0x04, 0x65, 0x00 # enter sandbox
    .byte 0x0e # unrestricted mov prefix
    mov 0x0(%rsi), %rcx
    .byte 0x0e # unrestricted mov prefix
    mov %rcx, 0x0(%rdx)
    .byte   0x0F, 0x04, 0x66, 0x00 # exit sandbox
    ret

.globl hfi_load_test
.type hfi_load_test,@function
.align 16
hfi_load_test:
_hfi_load_test:
    # rdi: param 1 is the sandbox params
    # rsi: param 2 is the load address
    # enter the sandbox, perform load, then exit
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x71, 0x00 # set sandbox metadata
    .byte   0x0F, 0x04, 0x65, 0x00 # enter sandbox
    mov 0x0(%rsi), %rax
    .byte   0x0F, 0x04, 0x66, 0x00 # exit sandbox
    ret

.globl hfi_store_test
.type hfi_store_test,@function
.align 16
hfi_store_test:
_hfi_store_test:
    # rdi: param 1 is the sandbox params
    # rsi: param 2 is the store address
    # rdx: param 3 is the store value
    # enter the sandbox, perform store, then exit
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x71, 0x00 # set sandbox metadata
    .byte   0x0F, 0x04, 0x65, 0x00 # enter sandbox
    mov %rdx, 0x0(%rsi)
    .byte   0x0F, 0x04, 0x66, 0x00 # exit sandbox
    ret

.globl noop_func
.type noop_func,@function
.align 16
noop_func:
_noop_func:
    ret

.globl hfi_call_test
.type hfi_call_test,@function
.align 16
hfi_call_test:
_hfi_call_test:
    # rdi: param 1 is the sandbox params
    # rsi: param 2 is the call address
    # enter the sandbox, perform call, then exit
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x71, 0x00 # set sandbox metadata
    .byte   0x0F, 0x04, 0x65, 0x00 # enter sandbox
    call *(%rsi)
    .byte   0x0F, 0x04, 0x66, 0x00 # exit sandbox
    ret

.globl hfi_loadsavecontext_test
.type hfi_loadsavecontext_test,@function
.align 16
hfi_loadsavecontext_test:
_hfi_loadsavecontext_test:
    # rdi: param 1 is the sandbox params
    # rsi: param 2 is the thread_context to save into
    # rdx: param 3 is the thread_context to load from
    # rcx: param 4 is the thread_context to save into the second time
    # enter the sandbox, perform savecontext, loadcontext, savecontext then exit
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x71, 0x00 # set sandbox metadata
    .byte   0x0F, 0x04, 0x65, 0x00 # enter sandbox
    mov %rsi, %rax
    .byte   0x0F, 0x04, 0x67, 0x00 # save context
    mov %rdx, %rax
    .byte   0x0F, 0x04, 0x68, 0x00 # load context
    mov %rcx, %rax
    .byte   0x0F, 0x04, 0x67, 0x00 # save context
    .byte   0x0F, 0x04, 0x66, 0x00 # exit sandbox
    ret

.globl hfi_loadsavemetadata_test
.type hfi_loadsavemetadata_test,@function
.align 16
hfi_loadsavemetadata_test:
_hfi_loadsavemetadata_test:
    # rdi: param 1 is the sandbox params
    # rsi: param 2 is the curr sandbox metadata to save into
    # rdx: param 3 is the curr sandbox metadata to load from
    # rcx: param 4 is the curr sandbox metadata to save into the second time
    # enter the sandbox, perform savemetadata, loadmetadata, savemetadata then exit
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x71, 0x00 # set sandbox metadata
    .byte   0x0F, 0x04, 0x65, 0x00 # enter sandbox
    .byte   0x0F, 0x04, 0x66, 0x00 # exit sandbox
    mov %rsi, %rax
    .byte   0x0F, 0x04, 0x72, 0x00 # save metadata
    mov %rdx, %rax
    .byte   0x0F, 0x04, 0x71, 0x00 # load metadata
    mov %rcx, %rax
    .byte   0x0F, 0x04, 0x72, 0x00 # save metadata
    ret

.globl hfi_exit_handler_test
.type hfi_exit_handler_test,@function
.align 16
hfi_exit_handler_test:
_hfi_exit_handler_test:
    # rdi: param 1 is the sandbox params
    # enter the sandbox, then exit. The exit would invoke the exit handler which will directly return
    mov %rdi, %rax
    .byte   0x0F, 0x04, 0x71, 0x00 # set sandbox metadata
    .byte   0x0F, 0x04, 0x65, 0x00 # enter sandbox

.globl hfi_test_exit_location
.type hfi_test_exit_location,@function
hfi_test_exit_location:
    .byte   0x0F, 0x04, 0x66, 0x00 # exit sandbox
    ud2
