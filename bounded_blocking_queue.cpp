#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
using namespace std;
// Romain Garnier
// 30/08/2020
// Compile with
// $ g++ -Wall -Wextra -Wpedantic -Wconversion -pthread bounded_blocking_queue.cpp -o bounded_blocking_queue
// Execute with 
// $ ./bounded_blocking_queue

class bounded_blocking_queue {
public:
  bounded_blocking_queue(int capacity) : _capacity(capacity) {

    _queue.resize(_capacity);
  }

  void enqueue(int element) {
    // Get the lock
    unique_lock<mutex> lck(_mu);
    // Sleep push until notification that the max capacity has been reached
    _push.wait(lck, [=] { return _size < _capacity; });
    _queue[_tail] = element;
    _size++;
    _tail = (_tail + 1) % _capacity;
    cout << "Enqueue element " << element << "\n";
    _pop.notify_all();
  }

  int dequeue() {
    // Get the lock
    unique_lock<mutex> lck(_mu);
    // Sleep pop until notification that the number of elements is positif
    _pop.wait(lck, [=] { return _size > 0; });
    int tmp_head = _head;
    _head = (_head + 1) % _capacity;
    _size--;
    cout << "Dequeue "
         << "\n";
    _push.notify_all();
    return _queue[tmp_head];
  }

  int size() {
    cout << "Size is " << _size << "\n";
    return _size;
  }

private:
  int _tail = 0;
  int _head = 0;
  int _size = 0;
  int _capacity;

  vector<int> _queue;

  mutex _mu;
  condition_variable _push, _pop;
};

int main() {

  cout << "Declaration of pointer to bounded_blocking_queue of max capacity 2\n";
  bounded_blocking_queue *bq_p = new bounded_blocking_queue(2);

  cout << "Executing threads in random order\n";
  // Get current size
  thread t1(&bounded_blocking_queue::size, bq_p);
  thread t2(&bounded_blocking_queue::enqueue, bq_p, 1);
  thread t3(&bounded_blocking_queue::enqueue, bq_p, 2);
  // Maximal capacity has been reached so next
  // enqueue will be executed after releasing one element
  thread t4(&bounded_blocking_queue::enqueue, bq_p, 3);
  // Dequeue (releasing one element)
  thread t5(&bounded_blocking_queue::dequeue, bq_p);
  thread t6(&bounded_blocking_queue::dequeue, bq_p);
  thread t7(&bounded_blocking_queue::dequeue, bq_p);
  // Get current size
  thread t8(&bounded_blocking_queue::size, bq_p);

  
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();
  t7.join();
  t8.join();

  delete bq_p;
}
