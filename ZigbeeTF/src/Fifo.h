#ifndef _FIFO_h
#define _FIFO_h

template<typename T>
struct Fifo {
	const uint8_t length;
	uint8_t size;
	T* data;
	T* head;
	T* tail;
	Fifo(const uint8_t _length) : length(_length) {
		data = new T[_length];		
		head = data[0];
		tail = data[0];
		size = length * sizeof(T);
	};

	~Fifo() {
		delete[] data;
	}

	T* pop() {
		if (head == tail) {
			return nullptr;
		}
		T* d = tail;
		tail = (tail + 1) % size;
	}

	uint8_t push(T* d) {
		if (((head + 1) % size) == tail) {
			return -1;
		}

		*head = *d;
		head = (head + 1) % size;
		return 0;
	}


};


#endif