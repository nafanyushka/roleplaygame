#pragma once

#include <iostream>
#include <iterator>
namespace std {
	template <class T>
	struct Node
	{
		T data;
		Node<T>* next;
		Node<T>* prev;
		Node(T data = T(), Node* next = nullptr, Node* prev = nullptr) : data(data), next(next), prev(prev) {};
		~Node() {}
	};

	template <class T>
	class list
	{
	private:
		Node<T>* head = nullptr;
		Node<T>* last = nullptr;
		size_t size = 0;

		bool deleteUnique(Node<T>* node)
		{
			if (node->next == nullptr) return false;
			if (node->data == node->next->data)
			{
				Node<T>* next = node->next;
				node->next = node->next->next;
				delete next;
				if (node->next != nullptr)
					node->next->prev = node;
				else
					node->next = nullptr;
				size--;
				return true;
			}
			return false;
		}

	public:
		class Iterator
		{
		public:
			Node<T>* cur;
			inline Iterator(Node<T>* first) : cur(first) {}

			inline Iterator& operator++ (int)
			{
				cur = cur->next;
				return *this;
			}

			inline Iterator& operator-- (int)
			{
				cur = cur->prev;
				return *this;
			}

			inline Node<T>*& operator->()
			{
				return cur;
			}
			inline T& operator*()
			{
				return cur->data;
			}

			inline friend bool operator == (Iterator it, Iterator oth)
			{
				return (oth).cur == (it).cur;
			}

			inline friend bool operator != (Iterator it, Iterator oth)
			{
				return (it).cur != (oth).cur;
			}
		};

		size_t getSize() const { return size; }

		list() : head(nullptr), last(nullptr), size(0)
		{

		}

		~list()
		{
			clear();
		}

		void push_back(T data)
		{
			switch (size)
			{
			case 0:
				head = last = new Node<T>(data, nullptr, nullptr);
				size++;
				break;
			default:
				last->next = new Node<T>(data, nullptr, last);
				size++;
				last = last->next;
				break;
			}
		}

		void push_front(T data)
		{
			switch (size)
			{
			case 0:
				head = last = new Node<T>(data, nullptr, nullptr);
				size++;
				break;
			default:
				head->prev = new Node<T>(data, head, nullptr);
				size++;
				head = head->prev;
				break;
			}
		}

		T& front() { return head; }

		T& back() { return last; }

		void insert(Iterator iterator, T data)
		{
			if (iterator == end())
			{
				push_back(data);
				return;
			}
			else if (iterator == begin())
			{
				push_front(data);
				return;
			}
			iterator.cur->prev = new Node<T>(data, iterator.cur, iterator.cur->prev);
			Node<T>* node = iterator.cur->prev->prev;
			node->next = iterator.cur->prev;
			size++;
		}

		void pop_front()
		{
			switch (size)
			{
			case 0:
				break;
			case 1:
				delete head;
				head = last = nullptr;
				size--;
				break;
			default:
				Node<T>* node = head->next;
				delete head;
				node->prev = nullptr;
				head = node;
				size--;
				break;
			}
		}

		void pop_back()
		{
			switch (size)
			{
			case 0:
				break;
			case 1:
				delete head;
				head = last = nullptr;
				size--;
				break;
			default:
				Node<T>* node = last->prev;
				delete last;
				node->next = nullptr;
				last = node;
				size--;
				break;
			}
		}

		void clear()
		{
			for (auto i = begin(); i != end();)
			{
				auto b = i;
				i++;
				delete (b).cur;
			}
			size = 0;
		}

		void unique()
		{
			Node<T>* cur = head;
			while (cur != nullptr)
			{
				while (deleteUnique(cur)) {}
				cur = cur->next;
			}
		}

		void merge(list otherList)
		{
			for (auto i = otherList.begin(); i != otherList.end(); i++)
			{
				push_back(*i);
			}
		}

		Iterator begin()
		{
			return Iterator(head);
		}

		Iterator end()
		{
			return Iterator(nullptr);
		}

	};
}