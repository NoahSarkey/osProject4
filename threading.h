// Sam Mustipher, Noah Sarkey
// CSE 30341 - 02
// Project 4
// threading.h

////////////////////////////////// TO DO ///////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

#include <queue>
#include <pthread.h>
#include <string>

//template <typename T>
class Threading {
	public:
    		Threading() {
    			pthread_mutex_init(&queueMutex, NULL);
    			pthread_cond_init(&emptyCondVar, NULL);
    			//Stopped = false;
		}

    		void Enqueue(string data) {
    			pthread_mutex_lock(&queueMutex);
    			_queue.push(data);
    			pthread_cond_signal(&emptyCondVar);
			pthread_mutex_unlock(&queueMutex);
		}

    		string Dequeue() {
			pthread_mutex_lock(&queueMutex);
    			if (_queue.empty()) {
        			pthread_cond_wait(&emptyCondVar, &queueMutex);
    			}
    			//if (Stopped) {
        		//	pthread_mutex_unlock(&queueMutex);
        		//	return NULL;
    			//}

    			string elem = _queue.front();
    			_queue.pop();
    			pthread_mutex_unlock(&queueMutex);
    			return elem;
		}

		queue<string> _queue;
		pthread_mutex_t queueMutex;
		pthread_cond_t emptyCondVar;
};
