#include "common.h"
#include <atomic>

class node{
    public:
        std::atomic<node*> next = {NULL};
        std::atomic<bool> flag = {false};
};

class locks {
    public:
        virtual void lock(){};
        virtual void unlock(){};
        virtual void lock(node* my_node){};
        virtual void unlock(node* my_node){};
};

class test_and_set: public locks {
   public:
        std::atomic_flag flag = ATOMIC_FLAG_INIT;

        void lock();
        void unlock();
};

class test_and_test_and_set: public locks {
    public:
        std::atomic<bool> flag{false};

        void lock();
        void unlock();
};

class ticket_lock: public locks {
    public:
        std::atomic<int> next_num{0};
        std::atomic<int> now_serving{0};

        void lock();
        void unlock();
};

class mcs: public locks {
    public:
        std::atomic<node*> tail{NULL};

        void lock(node* my_node);
        void unlock(node* my_node);
};

class barrier {
    std::atomic<int> cnt = {0};
    std::atomic<bool> sense = {0};
    int NUM_THREADS;

    public:
        barrier(int N):NUM_THREADS(N){}

        void wait();

};