#ifndef CONTAINER_HEADER
#define CONTAINER_HEADER
#pragma once

#include <cassert>

template <typename T>
class Container
{
private:
    struct Element {
        T data;
        Element* next;
		Element()
		{
			next = nullptr;
		}
		Element(T* newData)
			: data(*newData)
		{
			next = nullptr;
		}
		Element(const Element& el)
		{
			data = el.data;
			next = el.next;
		}
		~Element()
		{
			delete data;
		}
    };
	
    Element* m_head;
    Element* m_tail;

public:
	Container();
	~Container();

	void PushFront(T newData);
	void PushBack(T newData);
	
	void PopFront();
	void PopBack();
	
	T& PeekFront() const;
	T& PeekBack() const;

	class Iterator {
	private:
		Element* m_ptr;
	public:
		Iterator();
		Iterator(Element* startPtr);
		Iterator& operator ++();
		bool operator !=(const Iterator& anotherItr);
		T operator *() const;
		T* operator ->();
	};

    Iterator Begin() const;
    Iterator End() const;

	size_t GetSize() const;
	bool IsEmpty() const;
	void Clear();
};

template <typename T>
Container<T>::Iterator::Iterator()
{
	m_ptr = nullptr;
}

template <typename T>
Container<T>::Iterator::Iterator(Element* startPtr)
{
	assert (startPtr);
	m_ptr = startPtr;
}

template <typename T>
typename Container<T>::Iterator& Container<T>::Iterator::operator ++()
{
	m_ptr = m_ptr->next;
	return *this;
}

template <typename T>
bool Container<T>::Iterator::operator !=(const typename Container<T>::Iterator& anotherItr)
{
	return (m_ptr != anotherItr.m_ptr);
}

template <typename T>
T Container<T>::Iterator::operator *() const
{
	return m_ptr->data;
}

template <typename T>
T* Container<T>::Iterator::operator ->()
{
	return &(m_ptr->data);
}


template <typename T>
Container<T>::Container(void)
{
    m_head = m_tail = nullptr;
}

template <typename T>
Container<T>::~Container(void)
{
	Clear();
}

template <typename T>
void Container<T>::PushFront(T newData)
{
    if (IsEmpty()) {
		m_head = new Element(&newData);
        m_tail = m_head;
        m_tail->next = nullptr;
    }
	else {
		Element* newHead = new Element(&newData);
        newHead->next = m_head;
        m_head = newHead;
    }
}

template <typename T>
void Container<T>::PushBack(T newData)
{
    if (IsEmpty()) {
        this->PushFront(newData);
    }
	else {
		Element* newTail = new Element(&newData);
        newTail->next = m_tail->next;
        m_tail->next = newTail;
        m_tail = newTail;
    }
}

template <typename T>
void Container<T>::PopFront()
{
    assert (!IsEmpty());

    if (m_head == m_tail) {
        delete m_head;
        m_head = m_tail = nullptr;
    }
    else {
        Element* second = m_head->next;
        delete m_head;
        m_head = second;
    }
}

template <typename T>
void Container<T>::PopBack()
{
    assert (!IsEmpty());

    if (m_head == m_tail) {
        delete m_tail;
		m_head = m_tail = nullptr;
    }
	else {
		Element* previous = m_head;
		while (previous->next != m_tail) {
			previous = previous->next;
		}
		previous->next = nullptr;
		delete m_tail;
		m_tail = previous;
	}
}

template <typename T>
T& Container<T>::PeekFront() const
{
    assert (m_head);
	return m_head->data;
}

template <typename T>
T& Container<T>::PeekBack() const
{
    assert (m_tail);
	return m_tail->data;
}

template <typename T>
typename Container<T>::Iterator Container<T>::Begin() const
{
    if (!m_head) return Iterator();
	return Iterator(m_head);
}

template <typename T>
typename Container<T>::Iterator Container<T>::End() const
{
	return Iterator();
}

template <typename T>
size_t Container<T>::GetSize() const
{
	return ;
}

template <typename T>
bool Container<T>::IsEmpty() const
{
	return ((m_head == nullptr) || (m_tail == nullptr));
}

template <typename T>
void Container<T>::Clear()
{
    while (!IsEmpty())
        this->PopFront();
}

#endif