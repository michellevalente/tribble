#include <system_error>
#include <cstring>

#ifndef BUFFER_H
#define BUFFER_H

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

    /** \brief Return buffer as a void *
     *
     */
    void* getArray() const
    {
        return buffer;
    }

    /** \brief Return buffer's size.
     *
     */
    int getCapacity() const
    {
        return capacity;
    }

    /** \brief Set the buffer size.
     *
     */
    void setSize(int sz)
    {
        this->sz = sz;
    }

    /** \brief Return buffer's size.
     *
     */
    int getSize() const
    {
        return sz;
    }
    void copyTo(void* otherBuffer) const;

};

/** \brief Copies to whole buffer to the given buffer
  *
  */
void Buffer::copyTo(void* otherBuffer) const {
    memcpy(otherBuffer, buffer, capacity);
}

#endif