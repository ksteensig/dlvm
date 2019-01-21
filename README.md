# DLVM: Dynamic Language Virtual Machine
## Work in progress

A simple virtual machine designed for dynamic-typed languages.

DLVM is a stack based virtual machine that uses a bytecode format that is inspired by JVM's.

# TODO

* Implement table-based mark-compact garbage collector (remove the page table), this will make the GC go from O(n^2) to O(n*log(n))
* Interpreter loop
* Write down a detailed (enough) specification
* Profile functions on run-time
* JIT compiler
* "Frontend" language like CLR has