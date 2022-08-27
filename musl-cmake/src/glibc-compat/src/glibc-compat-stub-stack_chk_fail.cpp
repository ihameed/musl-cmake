extern "C" {

void
__stack_chk_fail(void) {
    __builtin_trap();
}

}
