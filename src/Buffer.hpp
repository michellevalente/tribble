#include <system_error>
#include <cstring>

/** \brief Encapsulates a buffer (byte array).
  */
class Buffer {
private:
    void* buffer;
    int capacity;
    int sz;
public:
    Buffer(int capacity = 1024):
        capacity(capacity)
    {
        buffer = new char[capacity];
    }
    void* getArray() const
    {
        return buffer;
    }
    int getCapacity() const
    {
        return capacity;
    }
    int setSize(int sz)
    {
        this->sz = sz;
    }
    void copyTo(void* otherBuffer) const;
    // void addData(void* data, int capacity) const;
    // void copyTo(Buffer buffer) const;
};

/** \brief Copies to whole buffer to the given buffer
  *
  */
void Buffer::copyTo(void* otherBuffer) const {
    memcpy(otherBuffer, buffer, capacity);
}