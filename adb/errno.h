#pragma once

#include <errno.h>
#include <string.h>



// errno values differ between operating systems and between Linux architectures.
// Arbitrarily select the Linux asm-generic values to use in the wire protocol.
int errno_to_wire(int error);
int errno_from_wire(int error);