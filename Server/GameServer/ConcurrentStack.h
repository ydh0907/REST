#pragma once

#include <pch.h>
#include <mutex>

template<typename T>
class LockStack
{
public:
	LockStack() {}

	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
		_condVar.notify_one();
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;

		// empty -> top -> pop
		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] { return _stack.empty() == false; });
		value = std::move(_stack.top());
		_stack.pop();
	}

private:
	stack<T> _stack;
	mutex _mutex;
	condition_variable _condVar;
};

template<typename T>
class LockFreeStack {
	struct Node
	{
		Node(const T& value) : data(make_shared<T>(value)), next(nullptr) {

		}

		shared_ptr<T> data;
		shared_ptr<Node> next;
	};

public:
	void Push(const T& value) {
		shared_ptr<Node> node = make_shared<Node>(value);
		node->next = atomic_load(&_head);

		while (atomic_compare_exchange_weak(&_head, &node->next, node) == false) {

		}
	}

	shared_ptr<T> TryPop() {
		shared_ptr<Node> oldHead = atomic_load(&_head);

		while (oldHead && atomic_compare_exchange_weak(&_head, &oldHead, oldHead->next) == false) {

		}

		if (oldHead == nullptr)
			return shared_ptr<T>();

		return oldHead->data;
	}

private:
	shared_ptr<Node> _head;
};