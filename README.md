### thread_safe_queue

Running example of a thread safe blocking queue using [mutex](https://en.cppreference.com/w/cpp/thread/mutex) and [condition variable](https://en.cppreference.com/w/cpp/thread/condition_variable).    

The methods of the calling object are:    

* bounded_blocking_queue(int max_capacity)  
The constructor.    
  
* void enqueue(int element)  
 Adds an element to the front of the queue.  
 If the queue is full, the calling thread is put to sleep until it has been notified not to be full.    

* int dequeue()  
 Returns the element at the rear of the queue and removes it.  
 If the queue is empty, the calling thread is put to sleep until it has been notified not to be empty.    

* int size()  
 Returns the number of elements currently in the queue.    

Compilation  
$ g++ -Wall -Wextra -Wpedantic -Wconversion -pthread bounded_blocking_queue.cpp -o bounded_blocking_queue     
  
Execution  
$ ./bounded_blocking_queue
