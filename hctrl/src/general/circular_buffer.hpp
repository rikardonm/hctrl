#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <tuple>


template<size_t SIZE>
class ScanCursor
{
public:
    using _Base = ScanCursor<SIZE>;

    /**
     * @brief Construct a new Scan Cursor object
     *
     * @param cursor
     */
    ScanCursor(size_t cursor)
    : _cursor(cursor)
    {
    };

    /**
     * @brief
     *
     * @param length
     */
    void AdvanceCursor(size_t length = 1)
    {
        const auto to_go = (SIZE - _cursor);
        if (to_go > length)
        {
            /* Has space, just advance */
            _cursor += length;
        }
        else
        {
            /* Wrap around */
            _cursor = length - to_go;
        }
    }

    _Base& operator+=(const size_t value)
    {
        AdvanceCursor(value);
        return *this;
    }

    _Base& operator++()
    {
        AdvanceCursor(1);
        return *this;
    }

    /**
     * @brief
     *
     * @param length
     */
    void RecedeCursor(size_t length = 1)
    {
        if (length > _cursor)
        {
            _cursor = SIZE - (length - _cursor);
        }
        else
        {
            _cursor -= length;
        }
    }

    _Base& operator-=(const size_t value)
    {
        RecedeCursor(value);
        return *this;
    }

    _Base& operator--()
    {
        RecedeCursor(1);
        return *this;
    }

    bool operator==(const _Base& other) const
    {
        return _cursor == other._cursor;
    }

    bool operator!=(const _Base& other) const
    {
        return not (*this == other);
    }

    void operator=(const _Base& other)
    {
        _cursor = other._cursor;
    }

    /**
     * @brief Calculates distance from current object cursor to another trailing cursor.
     *
     * @param trailer
     * @return size_t
     */
    size_t TrailingDistance(const _Base& trailer) const
    {
        if (_cursor >= trailer._cursor)
        {
            return _cursor - trailer._cursor;
        }
        else
        {
            return (SIZE - trailer._cursor) + _cursor;
        }
    }

    /**
     * @brief Calculates distance from current object to another leading cursor.
     *
     * @param leader
     * @return size_t
     */
    size_t LeadingDistance(const _Base& leader) const
    {
        if (_cursor <= leader._cursor)
        {
            return leader._cursor - _cursor;
        }
        else
        {
            return (SIZE - _cursor) + leader._cursor;
        }
    }

    /**
     * @brief
     *
     * @return size_t
     */
    size_t Index() const
    {
        return _cursor;
    }

private:
    size_t _cursor;
};


/**
 * @brief
 *
 * @tparam SIZE
 */
template<size_t SIZE>
class BoundedCursor : public ScanCursor<SIZE>
{
public:
    /**
     * @brief Construct a new Bounded Cursor object
     *
     * @param lower
     * @param upper
     */
    BoundedCursor(const ScanCursor<SIZE>& lower, const ScanCursor<SIZE>& upper)
    : ScanCursor<SIZE>(lower.Index()), _lower(lower), _upper(upper)
    {
    }

    /**
     * @brief
     *
     * @return true
     * @return false
     */
    bool IsValid() const
    {
        return _upper.TrailingDistance(_lower) > this->TrailingDistance(_lower);
    }

    /**
     * @brief
     *
     * @return size_t
     */
    size_t LowerDistance()
    {
        return this->TrailingDistance(_lower);
    }

    /**
     * @brief
     *
     * @return size_t
     */
    size_t UpperDistance()
    {
        return this->LeadingDistance(_upper);
    }

protected:
    const ScanCursor<SIZE>& _lower;
    const ScanCursor<SIZE>& _upper;
};


/**
 * @brief Templated circular buffer implementation with scan/edit cursor
 *
 *
 * Data is pushed (stored) and read back (pop) from left to right, wrapping around the SIZE boundary.
 *
 *
 * State 1: write cursor is in front of read cursor; data is contiguous
 *      Used cells: (Write Next) - (Read Next)
 *      Free cells: SIZE - Used
 *      Is Cursor valid: if cursor value falls in range:
 *                          [Read Next, Write Next)
 *
 *
 *                                                                  /---- SIZE
 *                                               Write             /
 *                                               Next = 6         |
 *                                                 |              |
 * Cursor/                                         |              |
 * Index:   0     1     2     3     4     5     6  |   7    8     9
 *           _____ _____ _____ _____ _____ _____ __+__ _____ _____
 *          |     |     |     |     |     |     |     |     |     |
 *          |     |  X  |  X  |  X  |  X  |  X  |     |     |     |
 *          |_____|_____|_____|_____|_____|_____|_____|_____|_____|
 *                   +
 *                   |
 *                   |
 *                   |
 *                 Read
 *                 Next = 1
 *
 *
 *
 * State 2: write cursor is behind of read cursor; data wraps around buffer boundary
 *      Used cells: SIZE - (Read Next) + (Write Next)
 *      Free cells: SIZE - Used
 *      Is Cursor valid: if cursor falls into either range:
 *                          [Read Next, SIZE)
 *                          [0, Write Next)
 *
 *
 *                                                                  /---- SIZE
 *                       Write                                     /
 *                       Next = 2                                 |
 *                         |                                      |
 * Cursor/                 |                                      |
 * Index:   0     1     2  |  3     4     5     6     7     8     9
 *           _____ _____ __+__ _____ _____ _____ _____ _____ _____
 *          |     |     |     |     |     |     |     |     |     |
 *          |  X  |  X  |     |     |     |     |  X  |  X  |  X  |
 *          |_____|_____|_____|_____|_____|_____|_____|_____|_____|
 *                                                 +
 *                                                 |
 *                                                 |
 *                                                 |
 *                                               Read
 *                                               Next = 6
 *
 *
 *
 * @tparam T Type of elements to store
 * @tparam SIZE Maximum amount of elements capable of storing at any given moment
 * @tparam CLEAN Fill buffer cells with default value at every removal or discard operation
 */
template<typename T, size_t SIZE, bool CLEAN>
class CircularBuffer
{
public:
    using _Base = CircularBuffer<T, SIZE, CLEAN>;

    CircularBuffer(bool clean = false)
    : _write_cursor(0), _read_cursor(0)
    {
        if (CLEAN or clean)
        {
            for(auto& x : _buffer)
            {
                x = {};
            }
        }
    }

    bool Push(const T value)
    {
        if (Free() >= 1)
        {
            _Push(value);
            return true;
        }
        return false;
    }

    _Base& operator<<(const T value)
    {
        Push(value);
        return *this;
    }

    template<size_t ASIZE>
    bool Push(const T (&array)[ASIZE])
    {
        if (Free() >= ASIZE)
        {
            for(const auto& value : array)
            {
                _Push(value);
            }
            return true;
        }
        return false;
    }

    template<size_t ASIZE>
    _Base& operator<<(const T (&array)[ASIZE])
    {
        Push(array);
        return *this;
    }

    template<size_t ASIZE, bool ACLEAN>
    bool Push(CircularBuffer<T, ASIZE, ACLEAN>& other, size_t length)
    {
        if (length > Free())
        {
            return false;
        }
        for (auto i = 0; i < length; ++i)
        {
            _Push(other._Pop(ACLEAN));
        }
        return true;
    }

    template<size_t ASIZE, bool ACLEAN>
    bool Push(CircularBuffer<T, ASIZE, ACLEAN>& other)
    {
        return Push(other, other.Used());
    }

    template<size_t ASIZE, bool ACLEAN>
    _Base& operator<<(CircularBuffer<T, ASIZE, ACLEAN>& other)
    {
        Push(other);
        return *this;
    }

    bool RevertPush(size_t length = 1, bool clean = false)
    {
        if (Used() < length)
        {
            return false;
        }
        _RevertPush(length, clean);
        return true;
    }

    std::tuple<bool, T> Pop(bool clean = false)
    {
        if (Used() >= 1)
        {
            return {true, _Pop(clean)};
        }
        return {false, {}};
    }

    /**
     * @brief
     *
     * @param clean
     */
    void Discard(bool clean = false)
    {
        if (CLEAN or clean)
        {
            while(Used())
            {
                _Pop(true);
            }
        }
        _read_cursor = 0;
        _write_cursor = 0;
    }

    /**
     * @brief
     *
     * @param size
     * @param clean
     * @return true
     * @return false
     */
    bool Discard(const size_t size, bool clean = false)
    {
        if (Used() < size)
        {
            return false;
        }
        if (CLEAN or clean)
        {
            for (auto counter = 0; counter < size; ++counter)
            {
                _Pop(true);
            }
        }
        else
        {
            _read_cursor += size;
        }
        return true;
    }

    size_t Free()
    {
        return SIZE - Used();
    }

    size_t Used()
    {
        return _write_cursor.TrailingDistance(_read_cursor);
    }

    /**
     * @brief
     *
     */
    class CircularBufferCursor : public BoundedCursor<SIZE>
    {
    public:

        /**
         * @brief Construct a new Circular Buffer Cursor object
         *
         * @param circular_buffer
         */
        CircularBufferCursor(_Base& circular_buffer)
        : BoundedCursor<SIZE>(circular_buffer._read_cursor, circular_buffer._write_cursor), _buffer(circular_buffer)
        {
        }

        ~CircularBufferCursor() = default;
        CircularBufferCursor(const CircularBufferCursor& other) = default;
        CircularBufferCursor& operator=(const CircularBufferCursor& other) = default;

        /**
         * @brief
         *
         * @param value
         * @return CircularBufferCursor&
         */
        CircularBufferCursor& operator=(const T& value)
        {
            _buffer[this->Index()] = value;
            return *this;
        }


        /**
         * @brief
         *
         * @return T&
         */
        T& Item()
        {
            return _buffer._buffer[this->Index()];
        }

        /**
         * @brief
         *
         * @return T
         */
        operator T()
        {
            return _buffer._buffer[this->Index()];
        }

        /**
         * @brief Advance object's cursor until chartest returns true or cursor is invalid.
         *
         * @param chartest Function pointer to test if character meets stopping criteria.
         */
        void Next(bool (*chartest)(const T))
        {
            for (; this->IsValid() ; ++(*this))
            {
                if (chartest(this->Item()))
                {
                    return;
                }
            }
        }

    private:
        _Base& _buffer;
    };

    CircularBufferCursor GetCursor()
    {
        return CircularBufferCursor(*this);
    }

    // can't really be used... as the contents are not always contiguous
    // perhaps use some sort of iterator here
    // well, it kind of can... it just needs to be called twice!!
    std::tuple<size_t, const T*> PeekUsed()
    {
        if (_write_cursor.Index() >= _read_cursor.Index())
        {
            return {_write_cursor.TrailingDistance(_read_cursor), &_buffer[_read_cursor.Index()]};
        }
        else
        {
            return {SIZE - _read_cursor.Index(), &_buffer[_read_cursor.Index()]};
        }
    }

protected:
    void _Push(const T& value)
    {
        _buffer[_write_cursor.Index()] = value;
        ++_write_cursor;
    }

    void _RevertPush(size_t length, bool clean)
    {
        if (CLEAN or clean)
        {
            for(auto count = 0; count < length; ++count)
            {
                --_write_cursor;
                _buffer[_write_cursor.Index()] = {};
            }
        }
        else
        {
            _write_cursor -= length;
        }
    }

    T _Pop(bool clean)
    {
        auto ret = _buffer[_read_cursor.Index()];
        if (CLEAN or clean)
        {
            _buffer[_read_cursor.Index()] = {};
        }
        ++_read_cursor;
        return ret;
    }

private:
    T _buffer[SIZE];
    /* _buffer based offset cursors; both point to the NEXT operation location */
    ScanCursor<SIZE> _write_cursor;
    ScanCursor<SIZE> _read_cursor;

    /* Declare sibling classes as friends */
    template<typename, size_t, bool> friend class CircularBuffer;


};
