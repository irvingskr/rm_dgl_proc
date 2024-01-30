//
// Created by irving on 24-1-28.
//

#ifndef RM_DGL_PROC_RM_VECTOR_H
#define RM_DGL_PROC_RM_VECTOR_H

#endif //RM_DGL_PROC_RM_VECTOR_H
namespace RmVector {

    template <typename T>
    class Vector {
    private:
        T* data;
        unsigned int capacity;


        void expandCapacity() {
            capacity = (capacity == 0) ? 1 : 2 * capacity;
            T* newData = new T[capacity];
            for (unsigned int i = 0; i < size; ++i) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
        }

    public:
        Vector() : data(nullptr), capacity(0), size(0) {}

        // 拷贝构造函数
        Vector(const Vector& other) : data(nullptr), capacity(0), size(0) {
            if (other.size > 0) {
                data = new T[other.capacity];
                for (unsigned int i = 0; i < other.size; ++i) {
                    data[i] = other.data[i];
                }
                size = other.size;
                capacity = other.capacity;
            }
        }

        // 赋值运算符
        Vector& operator=(const Vector& other) {
            if (this != &other) {
                delete[] data;

                data = new T[other.capacity];
                for (unsigned int i = 0; i < other.size; ++i) {
                    data[i] = other.data[i];
                }
                size = other.size;
                capacity = other.capacity;
            }
            return *this;
        }

        ~Vector() {
            delete[] data;
        }

        void push_back(const T& value) {
            if (size == capacity) {
                expandCapacity();
            }
            data[size++] = value;
        }

        T& operator[](unsigned int index) {
            if (index >= size) {
                throw "Index out of range";
            }
            return data[index];
        }

        void clear() {
            size = 0;
        }

        unsigned int count() const {
            return size;
        }

        unsigned int size;
    };

}