#include <iostream>
#include <iterator>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <queue>
#include <math.h>
#include <stdio.h>
#include <atomic>

#define SEQ_CST std::memory_order_seq_cst
#define ACQUIRE std::memory_order_acquire
#define RELAXED std::memory_order_relaxed
#define RELEASE std::memory_order_release
