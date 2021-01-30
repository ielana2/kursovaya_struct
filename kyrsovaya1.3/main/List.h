//this file was taken from laboratory work ¹1 and modified
#pragma once

class List { //description of class for lists
private:
	void reset_list(); //function to clear and reset all list

	class Element //description of class for list's elements 
	{
	public:
		Element(float inf, Element* next = nullptr, Element* prev = nullptr) { //constructor of element's parameters
			this->information = inf;
			this->next = next;
			this->prev = prev;
		};
		~Element() {}; //destructor of element's parameters

		float information; //element's parameters
		Element* prev, * next; 
	};

	Element* head, * tail; //list's parameters
	size_t list_size;

public:
	List(); //list-constructor
	~List(); //list-destructor

	//list's methods
	void push_back(float);
	void push_front(float);
	void pop_back();
	void pop_front();
	void insert(float, size_t);
	float at(size_t);
	void remove(size_t);
	size_t get_size();
	void print_to_console();
	void clear();
	void set(size_t, float);
	bool isEmpty();

	void push_front(List);
};