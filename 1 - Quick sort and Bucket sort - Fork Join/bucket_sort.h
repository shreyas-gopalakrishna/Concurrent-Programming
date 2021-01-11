using namespace std;
#include <vector>
#include <mutex>
#include <queue>

void multithreaded_bucket_sort(int num_threads, int chunk_size);
void *bucket_sort(void *data);
