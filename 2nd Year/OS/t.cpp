#include <iostream>
using namespace std;

class Deque {
private:
    int* deque;       // Array to hold elements
    int front;        // Index of the front element
    int rear;         // Index of the rear element
    int size;         // Maximum size of the deque

public:
    // Constructor to initialize the deque
    Deque(int n) {
        size = n;
        deque = new int[size];
        front = -1;
        rear = -1;
    }

    // Check if deque is full
    bool isFull() {
        return ((rear + 1) % size == front);
    }

    // Check if deque is empty
    bool isEmpty() {
        return (front == -1);
    }

    // Insert element at the front
    void insertFront(int value) {
        if (isFull()) {
            cout << "Deque is full" << endl;
            return;
        }
        if (isEmpty()) {
            front = rear = 0;
        } else if (front == 0) {
            front = size - 1;
        } else {
            front--;
        }
        deque[front] = value;
    }

    // Insert element at the rear
    void insertRear(int value) {
        if (isFull()) {
            cout << "Deque is full" << endl;
            return;
        }
        if (isEmpty()) {
            front = rear = 0;
        } else if (rear == size - 1) {
            rear = 0;
        } else {
            rear++;
        }
        deque[rear] = value;
    }

    // Delete element from the front
    void deleteFront() {
        if (isEmpty()) {
            cout << "Deque is empty" << endl;
            return;
        }
        cout << "Deleted from front: " << deque[front] << endl;
        if (front == rear) {
            front = rear = -1; // Deque is now empty
        } else if (front == size - 1) {
            front = 0;
        } else {
            front++;
        }
    }

    // Delete element from the rear
    void deleteRear() {
        if (isEmpty()) {
            cout << "Deque is empty" << endl;
            return;
        }
        cout << "Deleted from rear: " << deque[rear] << endl;
        if (front == rear) {
            front = rear = -1; // Deque is now empty
        } else if (rear == 0) {
            rear = size - 1;
        } else {
            rear--;
        }
    }

    // Get the front element
    int getFront() {
        if (isEmpty()) {
            cout << "Deque is empty" << endl;
            return -1;
        }
        return deque[front];
    }

    // Get the rear element
    int getRear() {
        if (isEmpty()) {
            cout << "Deque is empty" << endl;
            return -1;
        }
        return deque[rear];
    }

    // Destructor to free allocated memory
    ~Deque() {
        delete[] deque;
    }
};

int main() {
    Deque dq(5); // Create a deque of size 5

    dq.insertRear(10);
    dq.insertRear(20);
    dq.insertFront(5);
    dq.insertFront(3);

    cout << "Front element: " << dq.getFront() << endl;
    cout << "Rear element: " << dq.getRear() << endl;

    dq.deleteFront();
    dq.deleteRear();

    dq.insertRear(30);
    cout << "Front element: " << dq.getFront() << endl;
    cout << "Rear element: " << dq.getRear() << endl;

    return 0;
}
