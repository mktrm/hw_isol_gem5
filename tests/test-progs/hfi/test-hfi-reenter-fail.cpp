#include <assert.h>
#include <cstring>
#include <iostream>
#include <limits>

#include "hfi.h"

int main(int argc, char* argv[])
{
    hfi_sandbox sandbox;
    std::memset(&sandbox, 0, sizeof(hfi_sandbox));

    // initialize ranges
    for(uint64_t i = 0; i < LINEAR_RANGE_COUNT; i++) {
        sandbox.ranges[i].readable = 1;
        sandbox.ranges[i].writeable = 1;
        sandbox.ranges[i].executable = 1;
        sandbox.ranges[i].upper_bound = std::numeric_limits<uint64_t>::max();
    }

    hfi_enter_sandbox(&sandbox);
    // enter sandbox while in a sandbox should fail
    hfi_enter_sandbox(&sandbox);
}