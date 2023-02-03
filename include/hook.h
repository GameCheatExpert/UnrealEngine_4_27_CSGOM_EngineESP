#pragma once
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <log.h>
#include <dobby.h>

void *main_thread(void *);
uintptr_t get_module(const char* module_name);
