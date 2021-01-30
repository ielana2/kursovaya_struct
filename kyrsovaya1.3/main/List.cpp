//this file was taken from laboratory work ¹1 and modified
#include <iostream>
#include "List.h"
#include <stdexcept>

using namespace std;

void List::reset_list() {//function to reset list's parameters
	head = nullptr;
	tail = nullptr;
	list_size = 0;
}

List::List() { //list's constructor
	reset_list();
}

List::~List() { //list's destructor
	if (head) { //if there id head pointer
		while (head->next) { //while there is min two elements
			head = head->next;
			delete head->prev;
		}
		reset_list();
	}
}

void List::push_back(float inf) { //add element in the end of list
	Element* current = new Element(inf);
	current->next = nullptr;

	if (list_size == 0) {
		head = tail = current;
		current->prev = nullptr;
	}
	else {
		tail->next = current;
		current->prev = tail;
		tail = current;
	}
	list_size++;
}

void List::push_front(float inf) { //add element in the front of the list
	Element* current = new Element(inf);
	current->prev = nullptr;
	if (list_size == 0) {
		head = tail = current;
		current->next = nullptr;
	}
	else {
		head->prev = current;
		current->next = head;
		head = current;
	}
	list_size++;
}

void List::pop_back() { //delete last element in the list
	if (list_size == 0)
		throw out_of_range("List is empty");

	if (list_size == 1) {
		delete head;
		reset_list();
	}
	else {
		Element* current = tail->prev;
		current->next = nullptr;
		delete tail;
		tail = current;
		list_size--;
	}
}

void List::pop_front() { //delete first element of the list
	if (list_size == 0)
		throw out_of_range("List is empty");

	if (list_size == 1) {
		delete head;
		reset_list();
	}
	else {
		Element* current = head->next;
		current->prev = nullptr;
		delete head;
		head = current;
		list_size--;
	}
}

void List::insert(float inf, size_t index) { //insert element before element with chosen index
	if (index > list_size - 1)
		throw out_of_range("Index is greater than size of the list");

	if (index == 0)
		push_front(inf);

	else {
		Element* add_element = new Element(inf);
		Element* current = head;

		for (int i = 0; i < index - 1; i++) {
			current = current->next;
		}

		add_element->next = current->next;
		current->next->prev = add_element;
		add_element->prev = current;
		current->next = add_element;



		list_size++;
	}
}

float List::at(size_t index) { //get element with chosen index
	if (index > list_size - 1)
		throw out_of_range("Index is greater than size of the list");

	Element* current = head;
	for (int i = 0; i < index; i++) {
		current = current->next;
	}

	return current->information;
}

void List::remove(size_t index) { //delete element with chosen index
	if (index > list_size - 1)
		throw out_of_range("Index is greater than size of the list");

	if (index == 0)
		pop_front();

	else if (index == list_size - 1)
		pop_back();
	else {
		Element* current = head;
		for (int i = 0; i < index; i++) {
			current = current->next;
		}

		current->prev->next = current->next;
		current->next->prev = current->prev;
		delete current;
		list_size--;
	}
}

size_t List::get_size() { //get the size of the list
	return list_size;
}

void List::print_to_console() { //output of the list
	if (list_size == 0) {
		cout << "List is empty";
	}
	else {
		Element* current = head;
		for (int i = 0; i < list_size; i++) {
			cout << current->information << " ";
			current = current->next;
		}
	}
}

void List::clear() { //clear the list
	if (head) {
		head->next = head;
		delete head;
	}

	reset_list();
}

void List::set(size_t index, float change_inf) { //change element with chosen index with new element
	if (index > list_size - 1)
		throw out_of_range("Index is greater than size of the list");

	Element* current = head;
	for (int i = 0; i < index; i++) {
		current = current->next;
	}

	current->information = change_inf;
}

bool List::isEmpty() { //check if the list is empty
	if (list_size == 0)
		return true;
	else
		return false;
}

void List::push_front(List Another_List) { //adding another list in front of chosen list
	head->prev = Another_List.tail;
	Another_List.tail->next = head;
	head = Another_List.head;

	list_size += Another_List.get_size();
	Another_List.reset_list();
}