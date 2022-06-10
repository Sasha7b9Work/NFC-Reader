#pragma once
#include <cstring>


template<class T, int size_buffer>
class Buffer
{
public:

    Buffer() : size(size_buffer) {}; //-V730

    Buffer(int _size) : size(size_buffer) //-V730
    {
        Realloc(_size);
    }

    Buffer(int _size, T value) : size(size_buffer)
    {
        Realloc(_size);
        Fill(value);
    }

    void Fill(T value)
    {
        for (int i = 0; i < size; i++)
        {
            buffer[i] = value;
        }
    }

    T *Data() { return buffer; }

    const T *DataConst() const { return buffer; }

    T *Last()
    {
        return buffer + Size();
    }

    void Realloc(int _size)
    {
        size = _size;

        if (size > size_buffer)
        {
            size = size_buffer;
        }
    }

    void ReallocAndFill(int _size, T value)
    {
        Realloc(_size);

        for (int i = 0; i < size; i++)
        {
            buffer[i] = value;
        }
    }

    // Перевыделить память и заполнить её из buffer
    void ReallocFromBuffer(const T *in, int _size)
    {
        size = _size;

        if (size > size_buffer)
        {
            size = size_buffer;
        }

        for (int i = 0; i < size; i++)
        {
            buffer[i] = in[i];
        }
    }

    // Записать в буфер size байт из buffer. Если памяти выделено меньше, заполнить только выделенную память
    void FillFromBuffer(const T *in, int _size)
    {
        if (_size > Size())
        {
            _size = Size();
        }

        for (int i = 0; i < _size; i++)
        {
            buffer[i] = in[i];
        }
    }

    // Возвращает количество элементов в буфере
    int Size() const
    {
        return size;
    }

    int Capacity() const
    {
        return size_buffer;
    }

    void Append(const void *data, int _size)
    {
        if (Size() + _size > Capacity())
        {
        }
        else
        {
            std::memcpy(&buffer[size], data, (uint)_size);
            size += _size;
        }
    }

    T &operator[](uint i)
    {
        if ((int)i >= 0 && (int)i < Size())
        {
            return buffer[i];
        }

        static T null(0);

        return null;
    }

    T &operator[](int i)
    {
        if (i >= 0 && i < Size())
        {
            return buffer[i];
        }

        static T null(0);

        return null;
    }

protected:

    int size;

    T buffer[size_buffer];
};


template<class T>
class Buffer1024 : public Buffer<T, 1024>
{
public:
    Buffer1024() : Buffer<T, 1024>() { };
    Buffer1024(int size) : Buffer<T, 1024>(size) { };
    Buffer1024(int size, T value) : Buffer<T, 1024>(size, value) { };
};


template<class T>
class Buffer2048 : public Buffer<T, 2048>
{
public:
    Buffer2048() : Buffer<T, 2048>() { };
    Buffer2048(int size) : Buffer<T, 2048>(size) { };
    Buffer2048(int size, T value) : Buffer<T, 2048>(size, value) { };
};
