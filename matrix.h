#pragma once

#include <tuple>
#include <unordered_map>

template<typename T, T defaultValue = 0>
class Matrix
{
public:
    template<typename Container, typename Key, typename Element>
    class ImplicitSetter
    {
    public:
        ImplicitSetter(Container& owner, Key key) : m_owner(owner), m_key(key) {}

        ImplicitSetter& operator=(const Element& value)
        {
            m_owner.set(m_key, value);
            return *this;
        }

        operator const Element&() const { return m_owner.get(m_key); }
    private:
        Container& m_owner;
        Key        m_key;
    };

    class Row
    {
        using RowData = std::unordered_map<size_t, T>;
    public:
        using iterator       = typename RowData::iterator;
        using const_iterator = typename RowData::const_iterator;
        using Assigner       = ImplicitSetter<Row, size_t, T>;

        void     set(size_t index, T const& value);
        const T& get(size_t index) const;

        Assigner operator[](size_t index)       { return Assigner(*this, index); }
        const T& operator[](size_t index) const { return get(index); }

        size_t          size()  const { return m_Elements.size(); }
        iterator        begin()       { return m_Elements.begin(); }
        const_iterator  begin() const { return m_Elements.begin(); }
        iterator        end()         { return m_Elements.end(); }
        const_iterator  end()   const { return m_Elements.end(); }

    private:
        RowData m_Elements;
        const T m_DefaultValue = defaultValue;
    };

private:
    using MatrixData = std::unordered_map<size_t, Row>;

public:

    class const_iterator
    {
    public:
        static const_iterator Begin(MatrixData const& data)
        {
            const_iterator I(data);
            I.moveToBegin();
            return I;
        }

        static const_iterator End(MatrixData const& data)
        {
            const_iterator I(data);
            I.moveToEnd();
            return I;
        }

        const_iterator& operator++() { moveToNextValid(); return *this; }
        std::tuple<size_t, size_t, T> operator*() const;

        bool operator==(const const_iterator& other) const
        { return m_RowIt == other.m_RowIt && m_CellIt == other.m_CellIt; }
        bool operator!=(const const_iterator& other) const
        { return !operator==(other); }

    private:
        const_iterator(MatrixData const& data) : m_matrix(data) {}

        bool isEnd()   const { return m_RowIt == m_matrix.end(); }
        bool isValid() const { return !isEnd() && m_CellIt != m_RowIt->second.end(); }

        void moveToBegin();
        void moveToNext();
        void moveToNextValid();
        void moveToEnd() { m_RowIt = m_matrix.end(); }

    private:
        MatrixData const& m_matrix;
        typename MatrixData::const_iterator m_RowIt;
        typename Row::const_iterator        m_CellIt;
    };

    Matrix() {}

    void       set(size_t index, Row const& value);
    const Row& get(size_t index) const;
    size_t     size() const;

    Row&       operator[](size_t index)       { return m_Rows[index]; }
    const Row& operator[](size_t index) const { return get(index); }

    const_iterator begin() const { return const_iterator::Begin(m_Rows); }
    const_iterator end()   const { return const_iterator::End(m_Rows); }


private:
    MatrixData       m_Rows;
    const MatrixData m_EmptyRow;
};

//========================================================================================
// Matrix::Row
//========================================================================================

template<typename T, T defaultValue>
void Matrix<T, defaultValue>::Row::set(size_t index, T const& value)
{
    if (value != defaultValue) {
        m_Elements[index] = value;
    } else {
        m_Elements.erase(index);
    }
}

template<typename T, T defaultValue>
const T& Matrix<T, defaultValue>::Row::get(size_t index) const
{
    const_iterator I = m_Elements.find(index);
    return (I != m_Elements.end()) ? I->second : m_DefaultValue;
}

//========================================================================================
// Matrix::const_iterator
//========================================================================================

template<typename T, T defaultValue>
std::tuple<size_t, size_t, T>
Matrix<T, defaultValue>::const_iterator::operator*() const
{
    return isEnd() ? std::tuple<size_t, size_t, T>()
                   : std::make_tuple(m_RowIt->first, m_CellIt->first, m_CellIt->second);
}

template<typename T, T defaultValue>
void Matrix<T, defaultValue>::const_iterator::moveToBegin()
{
    m_RowIt = m_matrix.begin();
    if (!isEnd()) {
        m_CellIt = m_RowIt->second.begin();
        if (!isValid())
            moveToNextValid();
    }
}

template<typename T, T defaultValue>
void Matrix<T, defaultValue>::const_iterator::moveToNext()
{
    if (isEnd())
        return;
    const Row &currentRow = m_RowIt->second;
    if (m_CellIt != currentRow.end()) {
        ++m_CellIt;
    } else {
        ++m_RowIt;
        if (!isEnd()) {
            const Row &nextRow = m_RowIt->second;
            m_CellIt = nextRow.begin();
        }
    }
}

template<typename T, T defaultValue>
void Matrix<T, defaultValue>::const_iterator::moveToNextValid()
{
    do {
        moveToNext();
    } while(!isValid() && !isEnd());
}

//========================================================================================
// Matrix
//========================================================================================

template<typename T, T defaultValue>
const typename Matrix<T, defaultValue>::Row&
Matrix<T, defaultValue>::get(size_t index) const
{
    typename MatrixData::const_iterator I = m_Rows.find(index);
    return (I != m_Rows.end()) ? I->second : m_EmptyRow;
}

template<typename T, T defaultValue>
size_t Matrix<T, defaultValue>::size() const
{
    size_t nTotal = 0;
    for(typename MatrixData::const_iterator I = m_Rows.begin(); I != m_Rows.end(); ++I)
        nTotal += I->second.size();
    return nTotal;
}