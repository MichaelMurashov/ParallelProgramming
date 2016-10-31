#include <atomic>
#include <concurrent_queue.h>

using std::atomic;
using concurrency::Context;
using concurrency::concurrent_queue;

class Semaphore {
 public:
  explicit Semaphore(long long capacity) : _semaphore_count(capacity)
  { }

  void wait() {
    // The capacity of the semaphore is exceeded when the semaphore count
    // falls below zero. When this happens, add the current context to the
    // back of the wait queue and block the current context.
    if (--_semaphore_count < 0) {
      _waiting_contexts.push(Context::CurrentContext());
      Context::Block();
    }
  }

  void signal() {
    // If the semaphore count is negative, unblock the first waiting context.
    if (++_semaphore_count <= 0) {
      // A call to acquire might have decremented the counter, but has not
      // yet finished adding the context to the queue.
      // Create a spin loop that waits for the context to become available.
      Context* waiting = nullptr;
      while (!_waiting_contexts.try_pop(waiting))
        Context::Yield();

      // Unblock the context.
      waiting->Unblock();
    }
  }

 private:
  atomic<long long> _semaphore_count;

  // Ñoncurrency-safe queue of contexts that must wait to acquire the semaphore
  concurrent_queue<Context*> _waiting_contexts;
};
