// Sam Mustipher, Noah Sarkey
// CSE 30341 - 02
// Project 4
// threading.h

////////////////////////////////// TO DO ///////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

#include <queue>
#include <pthread.h>
#include <string>

template <typename T>
class QueueClass {
	public:
    		QueueClass() {
    			pthread_mutex_init(&queueMutex, NULL);
    			pthread_cond_init(&emptyCondVar, NULL);
		}

    		~QueueClass() {
    			pthread_mutex_destroy(&queueMutex);
    			pthread_cond_destroy(&emptyCondVar);
		}

    		void Enqueue(T data) {
    			pthread_mutex_lock(&queueMutex);
    			_queue.push(data);
    			pthread_cond_signal(&emptyCondVar);
			pthread_mutex_unlock(&queueMutex);
		}

    		T Dequeue() {
			pthread_mutex_lock(&queueMutex);
    			if (_queue.empty()) {
        			pthread_cond_wait(&emptyCondVar, &queueMutex);
    			}

    			string elem = _queue.front();
    			_queue.pop();
    			pthread_mutex_unlock(&queueMutex);
    			return elem;
		}

		int sizeOfQueue() {
			pthread_mutex_lock(&queueMutex);
			int s = _queue.size();
			pthread_mutex_unlock(&queueMutex);
			return s;
		}

		queue<T> _queue;
		pthread_mutex_t queueMutex;
		pthread_cond_t emptyCondVar;
};
