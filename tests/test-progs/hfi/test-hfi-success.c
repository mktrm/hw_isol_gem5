#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "hfi.h"

typedef void(*void_void_ptr_t)();

void hfi_load_store_test(hfi_sandbox* sandbox, void* load_address, void* store_address);
void hfi_load_store_ret_test(hfi_sandbox* sandbox, void* load_address, void* store_address);
void hfi_load_store_push_pop_test(hfi_sandbox* sandbox, void* load_address, void* store_address);
void hfi_ur_load_store_test(hfi_sandbox* sandbox, void* load_address, void* store_address);
uint64_t hfi_load_test(hfi_sandbox* sandbox, void* load_address);
void hfi_store_test(hfi_sandbox* sandbox, void* store_address, uint64_t store_value);
void noop_func();
void hfi_call_test(hfi_sandbox* sandbox, void* call_address);
void hfi_loadsavecontext_test(hfi_sandbox* sandbox, hfi_thread_context* save_context,
    hfi_thread_context* load_context, hfi_thread_context* save_context2);
void hfi_loadsavemetadata_test(hfi_sandbox* sandbox, hfi_sandbox* save_metadata,
    hfi_sandbox* load_metadata, hfi_sandbox* save_metadata2);
void hfi_exit_handler_test(hfi_sandbox* sandbox);
void hfi_test_exit_location();

hfi_sandbox get_full_access_sandbox() {
    hfi_sandbox sandbox;
    memset(&sandbox, 0, sizeof(hfi_sandbox));
    sandbox.ranges[0].readable = 1;
    sandbox.ranges[0].writeable = 1;
    sandbox.ranges[0].executable = 1;
    sandbox.ranges[0].upper_bound = UINT64_MAX;
    return sandbox;
}

void test_entry_exit() {
    hfi_sandbox sandbox = get_full_access_sandbox();
    printf("test_entry_exit\n");
    hfi_set_sandbox_metadata(&sandbox);
    hfi_enter_sandbox();
    hfi_exit_sandbox();
}

void test_load_store() {
    hfi_sandbox sandbox = get_full_access_sandbox();
    uint64_t array[] = {0,1,2,3,4,5,6,7};
    sandbox.ranges[0].lower_bound = (uintptr_t) array;
    sandbox.ranges[0].upper_bound = (uintptr_t) &(array[5]);

    // check load and store
    printf("test_load_store\n");
    hfi_load_store_test(&sandbox, &(array[3]), &(array[4]));
    assert(array[4] == array[3]);
    array[4] = 4;

    // check load and store and returns
    printf("test_load_store + ret\n");
    hfi_load_store_ret_test(&sandbox, &(array[3]), &(array[4]));
    assert(array[4] == array[3]);
    array[4] = 4;

    // check load and store with a push pop
    printf("test_load_store + push pop\n");
    hfi_load_store_push_pop_test(&sandbox, &(array[3]), &(array[4]));
    assert(array[4] == array[3]);
    array[4] = 4;

    // check urmov load and store
    printf("test_load_store unrestricted\n");
    hfi_ur_load_store_test(&sandbox, &(array[6]), &(array[7]));
    assert(array[7] == array[6]);
    array[7] = 7;
}

void test_load_store_with_base() {
    hfi_sandbox sandbox = get_full_access_sandbox();
    uint64_t array[] = {0,1,2,3,4,5,6,7};

    // check load and store with a base address
    sandbox.ranges[0].base_address = (uintptr_t) array;
    sandbox.ranges[0].lower_bound = 0;
    sandbox.ranges[0].upper_bound = ((uintptr_t) &(array[5])) - ((uintptr_t) &(array[0]));
    printf("test_load_store_with_base\n");
    hfi_load_store_test(&sandbox,
        (void*) (((uintptr_t) &(array[3])) - ((uintptr_t) &(array[0]))),
        (void*) (((uintptr_t) &(array[4])) - ((uintptr_t) &(array[0])))
    );
    assert(array[4] == array[3]);
    array[4] = 4;
}

void test_save_load_context() {
    hfi_sandbox sandbox = get_full_access_sandbox();

    // This test saves the current context, loads the target context and then saves the current context again
    // The test plan here is to enter a sandbox
    // 1) save context and see that it has the expected value
    // 2) load context which is the same as the sandbox context, save it again and see if it has the expected value
    hfi_thread_context contexts[3];
    memset(contexts, 0, sizeof(hfi_thread_context) * 3);

    hfi_thread_context* save_context  = &(contexts[0]);
    hfi_thread_context* load_context  = &(contexts[1]);
    hfi_thread_context* save_context2 = &(contexts[2]);
    sandbox.ranges[0].lower_bound = (uintptr_t) contexts;
    sandbox.ranges[0].upper_bound = (uintptr_t) &(contexts[4]);

    load_context->curr_sandbox_data = sandbox;
    load_context->inside_sandbox = 1;
    load_context->exit_sandbox_reason = hfi_get_exit_reason();
    load_context->exit_instruction_pointer = hfi_get_exit_location();
    printf("test_save_load_context\n");
    hfi_loadsavecontext_test(&sandbox, save_context, load_context, save_context2);

    assert(memcmp(save_context, load_context, sizeof(hfi_thread_context)) == 0);
    assert(memcmp(save_context2, load_context, sizeof(hfi_thread_context)) == 0);
}

void test_save_load_metadata() {
    hfi_sandbox sandbox = get_full_access_sandbox();

    // This test saves the current metadata, loads the target metadata and then saves the current metadata again
    // The test plan here is to enter a sandbox
    // 1) save metadata and see that it has the expected value
    // 2) load metadata which is the same as the sandbox metadata, save it again and see if it has the expected value
    hfi_sandbox metadatas[3];
    memset(metadatas, 0, sizeof(hfi_sandbox) * 3);

    hfi_sandbox* save_metadata  = &(metadatas[0]);
    hfi_sandbox* load_metadata  = &(metadatas[1]);
    hfi_sandbox* save_metadata2 = &(metadatas[2]);
    sandbox.ranges[0].lower_bound = (uintptr_t) metadatas;
    sandbox.ranges[0].upper_bound = (uintptr_t) &(metadatas[4]);

    *load_metadata = sandbox;
    printf("test_save_load_metadata\n");
    hfi_loadsavemetadata_test(&sandbox, save_metadata, load_metadata, save_metadata2);

    assert(memcmp(save_metadata, load_metadata, sizeof(hfi_sandbox)) == 0);
    assert(memcmp(save_metadata2, load_metadata, sizeof(hfi_sandbox)) == 0);
}

void test_call_indirect() {
    hfi_sandbox sandbox = get_full_access_sandbox();

    printf("test_call_indirect\n");
    // check call indirect which does a memory reference
    void_void_ptr_t func_ptr = &noop_func;
    sandbox.ranges[0].lower_bound = (uintptr_t) &func_ptr;
    sandbox.ranges[0].upper_bound = sandbox.ranges[0].lower_bound + sizeof(func_ptr);
    hfi_call_test(&sandbox, &func_ptr);
}

void test_exit_handler() {
    hfi_sandbox sandbox = get_full_access_sandbox();
    sandbox.exit_sandbox_handler = (void*) &noop_func;
    printf("test_exit_handler\n");
    hfi_exit_handler_test(&sandbox);
    enum HFI_EXIT_REASON exit_reason = hfi_get_exit_reason();
    assert(exit_reason == HFI_EXIT_REASON_EXIT);
    void* exit_location = hfi_get_exit_location();
    void* expected = &hfi_test_exit_location;
    assert(exit_location == expected);
}

int main(int argc, char* argv[])
{
    uint64_t version = hfi_get_version();
    printf("HFI version: %"PRIu64"\n", version);
    assert(version >= 2);

    uint64_t hfi_linear_range_count = hfi_get_linear_range_count();
    printf("HFI linear range count: %"PRIu64"\n", hfi_linear_range_count);
    assert(hfi_linear_range_count >= 4);

    test_entry_exit();
    test_load_store();
    test_load_store_with_base();
    test_save_load_context();
    test_save_load_metadata();
    test_call_indirect();
    test_exit_handler();
}