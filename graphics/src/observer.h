#include <iostream>
#include <vector>
#include <memory>
#include <functional>


template <typename T>
class Observer {
private:
	std::function<void(T)> action;
public:
	Observer(std::function<void(T)> act) : action(act) {}

	void onNotify(T& param) {
		action(param);
	}
};

template <>
class Observer<void> {
private:
	std::function<void()> action;
public:
	Observer(std::function<void()> act) : action(act) {}

	void onNotify() {
		action();
	}
};

template <typename T>
class Event {
private:
		std::vector<std::shared_ptr<Observer<T>>> observers;

public:
	Event() {};

	void addObserver(std::shared_ptr<Observer<T>> observer) {
		observers.push_back(observer);
	}

	void notifyObservers(T param) {
		for (std::shared_ptr<Observer<T>> observer : observers) {
			observer->onNotify(param);
		}
	}
};

template <>
class Event<void> {
private:
	std::vector<std::shared_ptr<Observer<void>>> observers;

public:
	Event() {};

	void addObserver(std::shared_ptr<Observer<void>> observer) {
		observers.push_back(observer);
	}

	void notifyObservers() {
		for (std::shared_ptr<Observer<void>> observer : observers) {
			observer->onNotify();
		}
	}
};