#include "common.h"
#include "locks.h"
#include <atomic>


/*
    Test and set lock
*/
void test_and_set::lock() {
    while(std::atomic_flag_test_and_set_explicit(&flag, ACQUIRE));
}

void test_and_set::unlock(){
    std::atomic_flag_clear_explicit(&flag, RELEASE);
}

/*
    Test and Test and set lock
*/
void test_and_test_and_set::lock() {
    while(flag.load(RELAXED) == true || flag.exchange(true, ACQUIRE)){
    };
}

void test_and_test_and_set::unlock(){
    flag.store(false, RELEASE);
}

/*
    Ticket lock
*/
void ticket_lock::lock() {
    int my_num = next_num++;
    while(now_serving.load(ACQUIRE)!=my_num){};
}

void ticket_lock::unlock(){
    now_serving++;
}

/*
    MCS lock
*/
void mcs::lock(node* my_node) {
    node* old_tail = tail.load(SEQ_CST);
    my_node->next.store(NULL, RELAXED);

    while(!tail.compare_exchange_strong(old_tail, my_node, SEQ_CST)) {
        // old_tail = tail.load(SEQ_CST);
    }

    if(old_tail != NULL) {
        my_node->flag.store(true,RELAXED);
        old_tail->next.store(my_node, SEQ_CST);
        while(my_node->flag.load(SEQ_CST)) {

        }
    }
}

void mcs::unlock(node* my_node){
    node *local_node = my_node;

    if(tail.compare_exchange_strong(local_node, NULL, SEQ_CST)) {

    }
    else{
        while(my_node->next.load(SEQ_CST) == NULL){

        }
        node* temp = my_node->next;
        temp->flag.store(false, SEQ_CST);
    }
    delete my_node;
}

void barrier::wait(){
    thread_local bool my_sense = 0;
    if(my_sense == 0){
        my_sense = 1;
    }else{
        my_sense = 0;
    }
    int cnt_cpy= cnt++;
    if(cnt_cpy == NUM_THREADS){
        cnt.store(0, RELAXED);
        sense.store(my_sense,SEQ_CST);
    }else{
        while(sense.load(ACQUIRE) != my_sense){}
    }
}