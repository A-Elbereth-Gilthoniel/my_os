.global switch_context
.type switch_context, @function
switch_context:
    pusha
    movl 36(%esp), %eax
    movl %esp, (%eax)

    movl 40(%esp), %eax
    movl %eax, %esp

    popa
    ret
