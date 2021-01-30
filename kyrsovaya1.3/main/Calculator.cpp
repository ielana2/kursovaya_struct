#include <iostream>
#include <fstream>
#include "Calculator.h"
#include <math.h>
#include <cmath>

const float PI = 3.14159265358979323846; //value for const pi
const float E = 2.718281828459045235360; //value for const e

/*list contains only float elements, so operations and constants which are in the same list with operands, but they need to be coded, so this is codes for them
(cause numbers could only be positive, operations and constants are coded as negative numbers. Here are their codes:
			'+' = -1
			unary '-' = -2
			binary '-' = -3
			'*' = -4
			'/' = -5
			'cos' = -6
			'sin' = -7
			'tg' = -8
			'ctg' = -9
			'ln' = -10
			'log' = -11
			'sqrt' = -12
			'mod' = -13
			'^' = -14

			'pi' = -15
			'e' = -16*/

Calculator::Calculator() { //constructer of the calculator
	prefix_phrase = new List(); //creating list for numbers after transforming line to prefix form
	phrase_size = 0;
}

Calculator::~Calculator() { //destructor of the calculator
	prefix_phrase->~List();
	phrase_size = 0;
}

void Calculator::ReadPhrase() { //get the input line from the file
	char symbol; //variable for checking for the end of the string
	ifstream file("Input.txt");
	if (!file.is_open()) //checking if the file is open
		throw "Can't open an input file";
	file >> symbol;
	while (!file.eof() && symbol != '\n') { //cycle for writing symbols down to the string
		phrase.push_back(symbol); //adding symbol from the file to the phrase string
		file >> symbol; //geting new symbol
		phrase_size++; //increasing size of the string
	}
	file.close();

	cout << "Original phrase: " << endl;//output of the taken phrase
	cout << phrase;
	cout << endl;

	CheckPhrase(0, phrase_size - 1); //checking if the frase was inputed correct

	TransformPhrase(0, phrase_size - 1); //transform it to prefix_form
}

void Calculator::CheckPhrase(int begining, int ending) {
	char symbol_type = 'a';
	int i = begining; //begining of the phrase that needs to be checked
	while (i <= ending){ //while phrase isn't finished
		int dots_counter = 0; //counter for dots in numbers (cause one number can have only one dot)
		symbol_type = GetSymbolType(phrase, i, dots_counter, ending, symbol_type); //checking symbol type
		if (symbol_type == 'e') { //if there is error
			cout << "Input Error, position " << i + 1 << endl; //informing about error
			throw "Input error"; //throwing error exeption
		}


		if (symbol_type == 'n') { //if this symbol type is number
			bool last_dot = false; //to know if last checked symbol is dot
			for (int j = i + 1; j <= ending + 1; j++) { //checking next symbols to see if it's still that number
				symbol_type = GetSymbolType(phrase, j, dots_counter, ending, symbol_type); //getting next symbol
				switch (symbol_type) {
				case'L': //if the phrase is fininshed
					if (last_dot) { //phrase can't be finished on dot
						cout << "Input Error, position " << j + 1 << endl; //error message
						throw "Input error"; //error execption
					}
					i = j;
					j = ending + 1; //to get out of cycle checking same type symbols
					break;
				case'd': //if this symbol is dot
					last_dot = true; //now last symbol of the phrase is dot
					if (dots_counter >= 2) { //if there are two and more dots in one number
						cout << "Input Error, position " << j + 1 << endl;
						throw "Input error";
					}
					break;
				case 'n': //this symbol is number
					last_dot = false; //if there was a dot, it's now not last
					break;
				case'o': //if symbol is operation
					if (last_dot) { //if last symbol before operation was dot (which is incorrect)
						cout << "Input Error, position " << j + 1 << endl;
						throw "Input error";
					}
					i = j; //to skip part of the phrase that has already been checked and get new values
					j = ending + 1;//to get out of cycle
					break;
				case')': //this symbol is closing bracket
					if (last_dot) { //if last symbol before bracket was dot (still incorrect)
						cout << "Input Error, position " << j + 1 << endl;
						throw "Input error";
					}
					i = j;
					j = ending + 1; //to get out of cycle if it wasn't dot
					break;
				default: //all other varians of what can be after number are incorrect, so error
					cout << "Input Error, position " << j + 1 << endl;
					throw "Input error";
					break;
				}
			}
		}

		if (symbol_type == 'o') { //this symbol is operation
			symbol_type = GetSymbolType(phrase, i + 1, dots_counter, ending, symbol_type); //get next symbol
			switch (symbol_type) {
			case'n': //next is number
				i++;
				break;
			case'E': //next is e constant
				i++;
				break;
			case'P': //next id pi constant
				i++;
				break;
			case '(': //next is opening bracket
				i++;
				break;
			case'2': //next is function which has two char symbols ("ln" and "tg")
				i++;
				break;
			case'3': //next is function wich has three char symbols ("cos", "sin", etc.)
				i++;
				break;
			case'4': //next is function which has four char symbols ("sqrt")
				i++;
				break;
			default: //all other variants are incorrect, so error
				cout << "Input Error, position " << i + 2 << endl;
				throw "Input error";
				break;
			}
		}

		if (symbol_type == 'E') { //this symbol is e constant
			symbol_type = GetSymbolType(phrase, i + 1, dots_counter, ending, symbol_type); //getting next symbol
			switch (symbol_type) {
			case'L': //end of the phrase
				i ++;
				break;
			case'o': //operation
				i++;
				break;
			case')': //closing bracket
				i ++;
				break;
			default: //all others are incorrent
				cout << "Input Error, position " << i + 2 << endl;
				throw "Input error";
				break;
			}
		}

		if (symbol_type == 'P') { //this symbol is pi constant (same as "e", but pi has two chars, but e has one, so only difference is where to search for next symbol. One more char should be skipped)
			symbol_type = GetSymbolType(phrase, i + 2, dots_counter, ending, symbol_type);
			switch (symbol_type) {
			case'L':
				i +=2;
				break;
			case'o':
				i += 2;
				break;
			case')':
				i += 2;
				break;
			default:
				cout << "Input Error, position " << i + 3 << endl;
				throw "Input error";
				break;
			}
		}

		if (symbol_type == '2') { //this symbol belongs to two chars functions ("ln" and "tg"
			symbol_type = GetSymbolType(phrase, i + 2, dots_counter, ending, symbol_type);
			switch (symbol_type) {
			case'(': //next is opening bracket
				i += 2;
				break;
			case'n': //next is number
				i += 2;
				break;
			default: //all others are incorrect
				cout << "Input Error, position " << i + 3 << endl;
				throw "Input error";
				break;
			}
		}

		if (symbol_type == '3') { //this symbol belongs to three char functions ("cos", "sin", etc.) (Everything is ame with two chars functions, exept the quantity of the skiiped symbols)
			symbol_type = GetSymbolType(phrase, i + 3, dots_counter, ending, symbol_type);
			switch (symbol_type) {
			case'(':
				i += 3;
				break;
			case'n':
				i += 3;
				break;
			default:
				cout << "Input Error, position " << i + 4 << endl;
				throw "Input error";
				break;
			}
		}

		if (symbol_type == '4') { //this belongs to "sqrt"/ Same as previous, but more symbols skipped
			symbol_type = GetSymbolType(phrase, i + 4, dots_counter, ending, symbol_type);
			switch (symbol_type) {
			case'(':
				i += 4;
				break;
			case'n':
				i += 4;
				break;
			default:
				cout << "Input Error, position " << i + 5 << endl;
				throw "Input error";
				break;
			}
		}

		if (symbol_type == '(') { //this symbol is opening bracket
			symbol_type = GetSymbolType(phrase, i + 1, dots_counter, ending, symbol_type);
			switch (symbol_type) {
			case'u'://next symbol is unary minus
				i++;
				break;
			case'('://next symbol is opening brackets
				i ++;
				break;
			case'2': //next symbol is two char operation
				i++;
				break;
			case'3'://next symbol is three char operation
				i ++;
				break;
			case'4': //next symbol is four char operation
				i ++;
				break;
			case'E': //next symbol is "e" constant
				i ++;
				break;
			case 'P': //next symbol is "pi" constant
				i ++;
				break;
			case'n': //next symbol is number
				i ++;
				break;
			default: //all others are incorrect
				cout << "Input Error, position " << i + 2 << endl;
				throw "Input error";
				break;
			}
		}

		if (symbol_type == ')') { //this symbol is closing bracket
			symbol_type = GetSymbolType(phrase, i + 1, dots_counter, ending, symbol_type);
			switch (symbol_type) {
			case'L': //next is end of the phrase
				i ++;
				break;
			case'o': //next is operation
				i ++;
				break;
			case')': //next is another closing bracket
				i ++;
				break;
			default: //all others are incorrect
				cout << "Input Error, position " << i + 2 << endl;
				throw "Input error";
				break;
			}
		}
	}

	CheckBrackets(begining, ending); //to check if every opening bracket has closing bracket
}

void Calculator::CheckBrackets(int begining, int ending) {//to check if every opening bracket has closing bracket
	int first_bracket_index = -1; //index of the first opened and not closed bracket
	int last_bracket_index = -1; //index of the first closed but not opened bracket
	int bracket_counter = 0;

	for (int i = begining; i <= ending; i++) { //going through the phrase
		if (phrase[i] == '(') { //found opening bracket
			if (bracket_counter == 0) //there are no opened and unclosed or closed and unopened brackets yet
				first_bracket_index = i; //remembering index of this opening bracket
			bracket_counter++; //increasing bracket counter
		}
		if (phrase[i] == ')') { //found closing bracket
			if (bracket_counter == 0) //there are no opened and unclosed or closed and unopened brackets yet
				last_bracket_index = i; //rememberibg index of this closing bracket
			bracket_counter--; //decreasing bracket counter
		}
	}

	if (bracket_counter > 0) { //if there are opened and not closed brackets
		cout << "Bracket isn't closed. Position " << first_bracket_index + 1 << endl; //error message
		throw "Input error"; //exeption
	}
	if (bracket_counter < 0) { //if there are closed and not opened brackets
		cout << "Bracket isn't open. Position " << last_bracket_index + 1 << endl;
		throw "Input error";
	}
}

char Calculator::GetSymbolType(string phrase, int index, int& dots_counter, int ending, char symbol_type) { //getting current symbol's type
	char operations[] = { '+', '-', '/', '*', '^' }; //array of operations
	int operations_size = 5; //operations array's size

	string functions_of_two[] = { "tg", "ln" }; //array of two chars functions
	int functions_of_two_size = 2; //functions of two chars array's size

	string functions_of_three[] = { "cos", "sin", "ctg", "log", "mod" }; //array of three chars functions
	int functions_of_three_size = 5; //functions of three chars array's size

	string function_of_four = "sqrt"; //string for sqrt

	string constPI = "pi"; //string for "pi" const
	string constE = "e"; //string for "e" const

	char numbers[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.' }; //array for numbers (with point)
	int numbers_size = 11; //numbers array's size

	char brackets[] = { '(', ')' }; //array for brackets
	int brackets_size = 2; //brackets array's size
	
	if (index == 0) { //checking the begining of the phrase
		if (phrase[index] == operations[1]) //if phrase begins with munis (unary)
			return 'o'; //returning value for operation

		if (phrase[index] == brackets[0]) //if phrase begins with opening bracket
			return '('; //retruns value for opening bracket

		for (int i = 0; i < numbers_size; i++) { //if phrase begins with number
			if (phrase[index] == numbers[i] && i < numbers_size - 1) //if it's actual number
				return 'n'; //returns value for number
			if (phrase[index] == numbers[i] && i == numbers_size - 1) //if it's point
				return 'e'; //returns value for error
		}

		string function; //string for checking functions and constants
		function.push_back(phrase[index]); //adding first checking symbol to string
		if (function == constE) //if phrase begins with "e" constant
			return 'E'; //returs value for "e" constant

		function.push_back(phrase[index + 1]); //adding next symbol to string
		for (int i = 0; i < functions_of_two_size; i++) { //if phrase begins with functions of two chars
			if (function == functions_of_two[i])
				return '2'; //returns value for two chars function
		}
		if (function == constPI) //if phrase begins with "pi" constant
			return 'P'; //returns value for "pi" constant

		function.push_back(phrase[index + 2]); //adding next symbol to string
		for (int i = 0; i < functions_of_three_size; i++) { //if phrase begins with functions of three chars
			if (function == functions_of_three[i])
				return '3'; //returning value for three chars functions
		}

		function.push_back(phrase[index + 3]); //adding nwxt symbol to string
		if (function == function_of_four) //if phrase begins with "sqrt"
			return '4'; //returns value for "sqrt"

		return 'e'; //all other cases return error
	}

	if (index != 0) { //if it's not begining of the phrase
		for (int i = 0; i < numbers_size; i++) {//checking for numbers
			if (phrase[index] == numbers[i]) {
				if (i == numbers_size - 1) {//if it's point
					dots_counter++;//increase of point's counter
					return 'd'; //retruns bvalue for point
				}
				return 'n'; //in another case returns value for number
			}
		}

		for (int i = 0; i < operations_size; i++) {//checking for operations
			if (phrase[index] == operations[i]) {
				if (i == 1 && symbol_type=='(') //this operation is unary minus
					return 'u'; //returns value for unary minus
				return 'o';//returns value for operations
			}
		}

		if (phrase[index] == brackets[0]) //checking for opening bracket
			return '(';//returns value for opening bracket
		if (phrase[index] == brackets[1]) //checking for closing bracket
			return ')'; //returns value for closing bracket

		if (index > ending) //checking if it's the end of phrase
			return 'L'; //returns value for the end of the phrase

		string function; //string for checkin two and more chars values
		function.push_back(phrase[index]); //adding first char to string
		if (function == constE) //if it's "e" constant
			return 'E'; //return value for "e" constant

		function.push_back(phrase[index + 1]); //ading one more symbol to string
		for (int i = 0; i < functions_of_two_size; i++) { //checking if it's functions of two chars
			if (function == functions_of_two[i])
				return '2'; //returns value for two chars functions
		}
		if (function == constPI)//checking if it's "pi" constant
			return 'P';//returns value for "pi" constant

		function.push_back(phrase[index + 2]); //adding another symbol to string
		for (int i = 0; i < functions_of_three_size; i++) { //checking if it's three chars function
			if (function == functions_of_three[i])
				return '3'; //retruns value for three chars function
		}

		function.push_back(phrase[index + 3]); //adding one more symbol to the string
		if (function == function_of_four) //checking if it's "sqrt"
			return '4'; //reurns value for "sqrt"

		return 'e'; //all other cases return error value
	}
}

void Calculator::SetPhrase(string set_string) {//to set phrase into recursin class
	phrase = set_string; //phrase is set from gotten string
}

int Calculator::GetPhraseSize() { //to know size of the input phrase
	return phrase_size; //retruns prefix_phrase size
}

void Calculator::TransformPhrase(int first_element, int last_element) { //transforming phrase to prefix form
	if(phrase_size == 0) //if current phrase size is zero (this phrase came from recursin)
		phrase_size = last_element + 1; //setting phrase size as it's last element's index plus one

	List* Actions = new List(); //list for operators

	bool first_operation = true; //to know if it's first operation
	int next_operation_index = -1; //to know where in phrase is next operation

	bool first_priority = false; //to know were there first priority operations ("+" and binary "-")
	bool second_priority = false; //to know were there second priority operations ("*" and "/")
	bool third_priority = false; //to know was there third priority operation ("^")
	bool fourth_priority = false; //to know was there fourth priority operation (unary "-")
	bool from_fourth_priority = false; //to know if function came from last priority operation's handler

	ActionsSearch(Actions, first_element, last_element); //to know what operations are in this phrase

	if (Actions->get_size() == 0) //if there are no operations
		prefix_phrase->push_back(GetOperand()); //geting number value and adding it to prefix phrase


	for (int i = 1; i < Actions->get_size(); i += 2) { //looking for adding and substacting
		if (Actions->at(i) == -1 || Actions->at(i) == -3) {
			first_priority = true; //there is first priority operation
			prefix_phrase->push_front(Actions->at(i)); //adding this operation to the prefix phrase
			if (first_operation) { //if it's first operation in the phrase
				CreateClassForRecursion(Actions, phrase, prefix_phrase, 0, Actions->at(i - 1) - 1, from_fourth_priority); //passing left operand (from begining of the phrase to operation) to the function
				first_operation = false; //now all operations aren't first
			}
			for (int j = i + 2; j < Actions->get_size(); j += 2) { //looking for next operation with the same priority
				if (Actions->at(j) == -1 || Actions->at(j) == -3) {
					next_operation_index = j - 1; //geting it's index
					j = Actions->get_size(); //to go out of the cycle
				}
			}
			if (next_operation_index == -1) //there is no same priority operatins
				CreateClassForRecursion(Actions, phrase, prefix_phrase, Actions->at(i - 1) + 1, phrase_size - 1, from_fourth_priority); //passing right operand (from this operation to the end of the phrase) to the function
			else { //there is same priority operation
				CreateClassForRecursion(Actions, phrase, prefix_phrase, Actions->at(i - 1) + 1, Actions->at(next_operation_index) - 1, from_fourth_priority); //passing right operand (between the same priority operations) to the function
				next_operation_index = -1; //setting next operation index to -1 because operation at the last index can be last same priority operation in the phrase
			}
		}
	}

	if (!first_priority) { //there are no "+" and binary "-"
		for (int i = 1; i < Actions->get_size(); i += 2) { //looking for multiplying and dividing
			if (Actions->at(i) == -4 || Actions->at(i) == -5) {
				first_priority = true; //there are no "+" and binary "-"
				second_priority = true; //there is second priority operation
				prefix_phrase->push_front(Actions->at(i)); //adding operation to prefix phrase
				//all this is same as first priority operations
				if (first_operation) {
					CreateClassForRecursion(Actions, phrase, prefix_phrase, 0, Actions->at(i - 1) - 1, from_fourth_priority);
					first_operation = false;
				}
				for (int j = i + 2; j < Actions->get_size(); j += 2) {
					if (Actions->at(j) == -4 || Actions->at(j) == -5) {
						next_operation_index = j - 1;
						j = Actions->get_size();
					}
				}
				if (next_operation_index == -1)
					CreateClassForRecursion(Actions, phrase, prefix_phrase, Actions->at(i - 1) + 1, phrase_size - 1, from_fourth_priority);
				else {
					CreateClassForRecursion(Actions, phrase, prefix_phrase, Actions->at(i - 1) + 1, Actions->at(next_operation_index) - 1, from_fourth_priority);
					next_operation_index = -1;
				}
			}
		}
	}

	if (!first_priority && !second_priority) { //looking for elevating
		for (int i = 1; i < Actions->get_size(); i += 2) {
			if (Actions->at(i) == -14) {
				first_priority = true; //there are no "+" and binary "-"
				second_priority = true; //there are no "*" and "/"
				third_priority = true; //there is third priority operation
				prefix_phrase->push_front(Actions->at(i)); //adding elevating to the prefix phrase
				//all this is same as before
				if (first_operation) {
					CreateClassForRecursion(Actions, phrase, prefix_phrase, 0, Actions->at(i - 1) - 1, from_fourth_priority);
					first_operation = false;
				}
				for (int j = i + 2; j < Actions->get_size(); j += 2) {
					if (Actions->at(j) == -14) {
						next_operation_index = j - 1;
						j = Actions->get_size();
					}
				}
				if (next_operation_index == -1)
					CreateClassForRecursion(Actions, phrase, prefix_phrase, Actions->at(i - 1) + 1, phrase_size - 1, from_fourth_priority);
				else {
					CreateClassForRecursion(Actions, phrase, prefix_phrase, Actions->at(i - 1) + 1, Actions->at(next_operation_index) - 1, from_fourth_priority);
					next_operation_index = -1;
				}
			}
		}
	}

	if (!first_priority && !second_priority && !third_priority) { //looking for unary minus
		for (int i = 1; i < Actions->get_size(); i += 2) {
			if (Actions->at(i) == -2) {
				first_priority = true; //there are no "+" and binary "-"
				second_priority = true; //there are no "*" and "/"
				third_priority = true; //there are no "^"
				fourth_priority = true; //there is fourth priority operation
				prefix_phrase->push_front(Actions->at(i)); //adding unary minus to the prefix phrase
				CreateClassForRecursion(Actions, phrase, prefix_phrase, Actions->at(i - 1) + 1, phrase_size - 1, from_fourth_priority); //passing right operand of the unary minus (cuase it has only one operand) to the function (operand is betwwen operation and end of the phrase, because there can't be two unary minuses which need to be counted equally in one phrase. If there are more than one, it is in brackets)
			}
		}
	}

	if (!first_priority && !second_priority && !third_priority && !fourth_priority) { //looking for different functions
		for (int i = 1; i < Actions->get_size(); i += 2) {
			if (Actions->at(i) == -6 || Actions->at(i) == -7 || Actions->at(i) == -8 || Actions->at(i) == -9 || Actions->at(i) == -10 || Actions->at(i) == -11 || Actions->at(i) == -12 || Actions->at(i) == -13) { //if it's "cos", "sin", etc.
				prefix_phrase->push_front(Actions->at(i)); //adding function to the prefix phrase
				from_fourth_priority = true; //this function was from last priority cycle
				CreateClassForRecursion(Actions, phrase, prefix_phrase, Actions->at(i - 1), phrase_size - 1, from_fourth_priority); //passing right operand of the phrase function (cuase it has only one operand) to the function (operand is betwwen operation and end of the phrase, because there can't be functions which need to be counted equally in one phrase. If there are more than one, it is in brackets)
			}
		}
	}
}

void Calculator::CreateClassForRecursion(List* Actions, string phrase, List* prefix_phrase, int begining_of_the_operand, int ending_of_the_operand, bool from_fourth_priority) { //function that cuts part of the phrase and passes it to the new class to get processed
	string string_for_new_class; //inizialization of the string which will be passed to new class
	int string_size = 0; //this string's size 
	int start_Offset = 0; //left indent (to cut off opening bracket and such functions as "cos", "sin", etc.
	int end_Offset = 0;//right indent (to cut off closing bracket)
	int bracket_counter = 0;
	bool many_brackets = false; //to know if there are different but same priority brackets

	if (phrase[begining_of_the_operand] == '(' && phrase[ending_of_the_operand] == ')'){ //if next operation is in brackets
		for (int i = begining_of_the_operand; i <= ending_of_the_operand; i++) {
			if (phrase[i] == '(') //finding opening bracket
				bracket_counter++; //increasing bracket's counter
			if (phrase[i] == ')')//finding closing bracket
				bracket_counter--;//decreasing breaket's counter
			if (bracket_counter == 0 && i < ending_of_the_operand) { //if there are same priority brackets
				i = ending_of_the_operand + 1; //to go out of the cycle
				many_brackets = true; //there are same priority brackets
			}
		}

		if (many_brackets) { //if there are same priority brackets nothing should be changed
			start_Offset = 0;
			end_Offset = 0;
		}
		else { //if there are only one pair of breakets, cutting them off by setting indetns
			start_Offset = 1;
			end_Offset = 1;
		}
	}

	//if next operation is cosinus and there are brackets after cosinus, cosinus and brackets have to be erased be setting indents
	if (phrase[begining_of_the_operand] == 'c' && phrase[begining_of_the_operand + 1] == 'o' && phrase[ending_of_the_operand] == ')' && from_fourth_priority) {
		start_Offset = 4;
		end_Offset = 1;
	}
	//if next operation is cosinus without brackets, cosinus has to be erase by setting left indent
	if (phrase[begining_of_the_operand] == 'c' && phrase[begining_of_the_operand + 1] == 'o' &&  phrase[ending_of_the_operand] <= '9' && phrase[ending_of_the_operand] >= '0' && Actions->get_size() == 2 && from_fourth_priority) {
		start_Offset = 3;
	}

	//if next operation is sinus and there are brackets after sinus, sinus and brackets have to be erased by setting indents
	if (phrase[begining_of_the_operand] == 's' && phrase[begining_of_the_operand + 1] == 'i' && phrase[ending_of_the_operand] == ')' && from_fourth_priority) {
		start_Offset = 4;
		end_Offset = 1;
	}
	//if next operation is sinus without brackets, sinus has to be erase by setting left indent
	if (phrase[begining_of_the_operand] == 's' && phrase[begining_of_the_operand + 1] == 'i' && phrase[ending_of_the_operand] <= '9' && phrase[ending_of_the_operand] >= '0' && Actions->get_size() == 2 && from_fourth_priority) {
		start_Offset = 3;
	}

	//if next operation is tangent and there are brackets after tangens, tangens and brackets have to be erased by setting indents
	if (phrase[begining_of_the_operand] == 't' && phrase[ending_of_the_operand] == ')' && from_fourth_priority) {
		start_Offset = 3;
		end_Offset = 1;
	}
	//if next operation is tangent without brackets, tangens has to be erase by setting left indent
	if (phrase[begining_of_the_operand] == 't' && phrase[ending_of_the_operand] <= '9' && phrase[ending_of_the_operand] >= '0' && Actions->get_size() == 2 && from_fourth_priority) {
		start_Offset = 2;
	}

	//if next operation is cotangent and there are brackets after cotangens, cotangens and brackets have to be erased by setting indents
	if (phrase[begining_of_the_operand] == 'c' && phrase[begining_of_the_operand + 1] == 't' && phrase[ending_of_the_operand] == ')' && from_fourth_priority) {
		start_Offset = 4;
		end_Offset = 1;
	}
	//if next operation is cotangent without brackets, cotangens has to be erase by setting left indent
	if (phrase[begining_of_the_operand] == 'c' && phrase[begining_of_the_operand + 1] == 't' && phrase[ending_of_the_operand] <= '9' && phrase[ending_of_the_operand] >= '0' && Actions->get_size() == 2 && from_fourth_priority) {
		start_Offset = 3;
	}

	//if next operation is ln and there are brackets after ln, ln and brackets have to be erased by setting indents
	if (phrase[begining_of_the_operand] == 'l' && phrase[begining_of_the_operand + 1] == 'n' && phrase[ending_of_the_operand] == ')' && from_fourth_priority) {
		start_Offset = 3;
		end_Offset = 1;
	}
	//if next operation is ln without brackets, ln has to be erase by setting left indent
	if (phrase[begining_of_the_operand] == 'l' && phrase[begining_of_the_operand + 1] == 'n' && phrase[ending_of_the_operand] <= '9' && phrase[ending_of_the_operand] >= '0' && Actions->get_size() == 2 && from_fourth_priority) {
		start_Offset = 2;
	}

	//if next operation is log and there are brackets after log, log and brackets have to be erased by setting indents
	if (phrase[begining_of_the_operand] == 'l' && phrase[begining_of_the_operand + 1] == 'o' && phrase[ending_of_the_operand] == ')' && from_fourth_priority) {
		start_Offset = 4;
		end_Offset = 1;
	}
	//if next operation is log without brackets, log has to be erase by setting left indent
	if (phrase[begining_of_the_operand] == 'l' && phrase[begining_of_the_operand + 1] == 'o' && phrase[ending_of_the_operand] <= '9' && phrase[ending_of_the_operand] >= '0' && Actions->get_size() == 2 && from_fourth_priority) {
		start_Offset = 3;
	}

	//if next operation is sqrt and there are brackets after sqrt, sqrt and brackets have to be erased by setting indents
	if (phrase[begining_of_the_operand] == 's' && phrase[begining_of_the_operand + 1] == 'q' && phrase[ending_of_the_operand] == ')' && from_fourth_priority) {
		start_Offset = 5;
		end_Offset = 1;
	}
	//if next operation is sqrt without brackets, sqrt has to be erase by setting left indent
	if (phrase[begining_of_the_operand] == 's' && phrase[begining_of_the_operand + 1] == 'q' && phrase[ending_of_the_operand] <= '9' && phrase[ending_of_the_operand] >= '0' && Actions->get_size() == 2 && from_fourth_priority) {
		start_Offset = 4;
	}

	//if next operation is module and there are brackets after module, module and brackets have to be erased by setting indents
	if (phrase[begining_of_the_operand] == 'm' && phrase[ending_of_the_operand] == ')' && from_fourth_priority) {
		start_Offset = 4;
		end_Offset = 1;
	}
	//if next operation is module without brackets, module has to be erase by setting left indent
	if (phrase[begining_of_the_operand] == 'm' && phrase[ending_of_the_operand] <= '9' && phrase[ending_of_the_operand] >= '0' && Actions->get_size() == 2 && from_fourth_priority) {
		start_Offset = 3;
	}

	for (int i = begining_of_the_operand + start_Offset; i <= ending_of_the_operand - end_Offset; i++) { //passing part of the phrase to new array
		string_for_new_class.push_back(phrase[i]);
		string_size++;
	}

	Calculator* Operand = new Calculator(); //creating object for new class
	Operand->SetPhrase(string_for_new_class); //setting phrase in new class
	Operand->TransformPhrase(0, string_size - 1); //transforming phrase in the new class
	List* new_prefix = new List(); //creating new list for prefix phrase which will come for new class
	new_prefix = Operand->GetPrefixPhrase(); //passing prefix phrase from new class to new prefix_phrase
	for (int i = 0; i < new_prefix->get_size(); i++) { //adding new prefix phrase to current prefix phrase
		prefix_phrase->push_back(new_prefix->at(i));
	}
}

List* Calculator::GetPrefixPhrase() { //to get prefix phrase
	return prefix_phrase;
}

float Calculator::GetOperand() { //to get operand from char lo float value
	if (phrase[0] == 'p') //if operand is "pi" constant
		return -15; //returns coded "pi" constant
	if (phrase[0] == 'e') //if operand is "e" constant
		return -16; //returns coded "e" constant
	if (phrase[0] <= '9' && phrase[0] >= '0') //if operand is number
		return MakeNumber(); //returns float number operand
}

float Calculator::MakeNumber() { //makes solid number from different chars
	int current = 0; //index from witch it should search the number
	float number = 0; //here will be gotten number
	int point_index = -1; //to know the index of the point in number (if there isn't one, index will be -1)
	float digit_capacity = 0; //to get ten in needed degree to multiply number on
	int capacity_counter = 0; //to know on ten in which degree number should be multiplyed

	bool has_dot = false; //to know if there is a point in number
	while (current < phrase_size) { //going through number
		if (phrase[current] == '.') { //finding the point
			has_dot = true; //now it has a point
			point_index = current; //the index of the point
		}
		current++;
	}

	if (has_dot) //if there is a point
		capacity_counter = point_index - 1; //setting wright degree
	else  //if there isn't a point
		capacity_counter = phrase_size - 1; //setting wright degree

	digit_capacity = pow(10, capacity_counter); //getting ten in needed degree

	for (current = 0; current < phrase_size; current++) { //going through number again
		if (current != point_index) { //while it's not a point
			number += (phrase[current] - '0') * digit_capacity; //multiplying this number on ten in needed degree and adding it to final number
			capacity_counter--; //decreasing the degree
			digit_capacity = pow(10, capacity_counter); //getting ten in new degree
		}
	}

	return number; //returns final number
}

void Calculator::ActionsSearch(List* Actions, int first_element, int last_element) { //to know what operation are in the phrase
	int bracket_counter = 0; //bracket's counter to skip operations in brackets

	for (int i = first_element; i < last_element; i++) { //going through the phrase
		if (phrase[i] == '(') //finding opening bracket
			bracket_counter++; //increase bracket's counter
		if (phrase[i] == ')') //finding closing bracket
			bracket_counter--; //decreasing bracket's counter


		if (bracket_counter == 0) { //if some brackets are open, operations in them will be skipped
			if (phrase[i] == '+') { //if operation is adding, adding this operation to list
				Actions->push_back(i); //writes its position in list
				Actions->push_back(-1); //writes the code number of the operation in the list
			}

			if (phrase[i] == '-') {//if operation is minus
				if (i - 1 < 0 || phrase[i - 1] == '(') { //if it is unary minus
					Actions->push_back(i);
					Actions->push_back(-2);
				}
				else { //if it is binary minus
					Actions->push_back(i);
					Actions->push_back(-3);
				}
			}

			if (phrase[i] == '*') { //if operation is multiplying, adding this operation to list
				Actions->push_back(i);
				Actions->push_back(-4);
			}

			if (phrase[i] == '/') { //if operation is dividing, adding this operation to list
				Actions->push_back(i);
				Actions->push_back(-5);
			}

			if (phrase[i] == 'c' && phrase[i + 1] == 'o') { //if operation is cosinus
				Actions->push_back(i);
				Actions->push_back(-6);
			}

			if (phrase[i] == 's' && phrase[i + 1] == 'i') {//if operation is sinus
				Actions->push_back(i);
				Actions->push_back(-7);
			}

			if (phrase[i] == 't' && phrase[i - 1] != 'c' && phrase[i + 1] == 'g') {//if operation is tangent
				Actions->push_back(i);
				Actions->push_back(-8);
			}

			if (phrase[i] == 'c' && phrase[i + 1] == 't') {//if operation is cotangent
				Actions->push_back(i);
				Actions->push_back(-9);
			}

			if (phrase[i] == 'l' && phrase[i + 1] == 'n') {//if operation is ln
				Actions->push_back(i);
				Actions->push_back(-10);
			}

			if (phrase[i] == 'l' && phrase[i + 1] == 'o') {//if operation is log
				Actions->push_back(i);
				Actions->push_back(-11);
			}

			if (phrase[i] == 's' && phrase[i + 1] == 'q') {//if operation is sqrt
				Actions->push_back(i);
				Actions->push_back(-12);
			}

			if (phrase[i] == 'm') {//if operation is module
				Actions->push_back(i);
				Actions->push_back(-13);
			}

			if (phrase[i] == '^') {//if operation is elevating
				Actions->push_back(i);
				Actions->push_back(-14);
			}
		}
	}
}

void Calculator::ChooseOperation() { //detecting what operation has to be done
	for (int i = prefix_phrase->get_size() - 1; i >= 0; i--) { //checking all list values. If finds code values, calls their functions
		if (prefix_phrase->at(i) == float(-1)) //Add
			Add(i); //calling for needed function
		else if (prefix_phrase->at(i) == float(-2)) //Minus
			Minus(i);
		else if (prefix_phrase->at(i) == float(-3)) //Substract
			Substract(i);
		else if (prefix_phrase->at(i) == float(-4)) //Multiply
			Multiply(i);
		else if (prefix_phrase->at(i) == float(-5)) //Divide
			Divide(i);
		else if (prefix_phrase->at(i) == float(-6)) //Cos
			Cos(i);
		else if (prefix_phrase->at(i) == float(-7)) //Sin
			Sin(i);
		else if (prefix_phrase->at(i) == float(-8)) //Tg
			Tg(i);
		else if (prefix_phrase->at(i) == float(-9)) //Ctg
			Ctg(i);
		else if (prefix_phrase->at(i) == float(-10)) //Ln
			Ln(i);
		else if (prefix_phrase->at(i) == float(-11)) //Log
			Log(i);
		else if (prefix_phrase->at(i) == float(-12)) //Sqrt
			Sqrt(i);
		else if (prefix_phrase->at(i) == float(-13)) //Modele
			Module(i);
		else if (prefix_phrase->at(i) == float(-14)) //Elevate
			Elevate(i);
	}
}


//All binary functions work this way
void Calculator::Add(int index) { //Add function
	float first_operand = prefix_phrase->at(index + 1); //finds first element for operation
	float second_operand = prefix_phrase->at(index + 2); //finds second element for operation

	DecodeConstants(first_operand, second_operand); //checking if there are coded constants
	float result = first_operand + second_operand; //doing operation

	prefix_phrase->set(index, result); //puting operation result wherever the operation coded symbol was
	prefix_phrase->remove(index + 1); //deleting first operand from list
	prefix_phrase->remove(index + 1); //deleting second operand from list
}

//All unary functions work this way
void Calculator::Minus(int index) { //Unary minus function
	float first_operand = prefix_phrase->at(index + 1); //finds the element for operation
	float second_operand = 0; //only needed to decode function, so is zero

	DecodeConstants(first_operand, second_operand); //checking if there is coded constant
	float result = -first_operand; //doing operation

	prefix_phrase->set(index, result); //puting operation result wherever the operation coded symbol was
	prefix_phrase->remove(index + 1); //deleting operand from list
}

void Calculator::Substract(int index) { //Substract (Binary minus) function
	float first_operand = prefix_phrase->at(index + 1);
	float second_operand = prefix_phrase->at(index + 2);

	DecodeConstants(first_operand, second_operand);
	float result = first_operand - second_operand;

	prefix_phrase->set(index, result);
	prefix_phrase->remove(index + 1);
	prefix_phrase->remove(index + 1);
}

void Calculator::Multiply(int index) { //Multiply function
	float first_operand = prefix_phrase->at(index + 1);
	float second_operand = prefix_phrase->at(index + 2);

	DecodeConstants(first_operand, second_operand);
	float result = first_operand * second_operand;

	prefix_phrase->set(index, result);
	prefix_phrase->remove(index + 1);
	prefix_phrase->remove(index + 1);
}

void Calculator::Divide(int index) { //Divide function
	float first_operand = prefix_phrase->at(index + 1);
	float second_operand = prefix_phrase->at(index + 2);

	DecodeConstants(first_operand, second_operand);
	float result = first_operand / second_operand;

	prefix_phrase->set(index, result);
	float a = prefix_phrase->at(index);
	prefix_phrase->remove(index + 1);
	prefix_phrase->remove(index + 1);
}

void Calculator::Cos(int index) { //Cosinus function
	float first_operand = prefix_phrase->at(index + 1);
	float second_operand = 0;

	DecodeConstants(first_operand, second_operand);
	float result = cos(first_operand);

	prefix_phrase->set(index, result);
	prefix_phrase->remove(index + 1);
}

void Calculator::Sin(int index) { //Sinus function
	float first_operand = prefix_phrase->at(index + 1);
	float second_operand = 0;

	DecodeConstants(first_operand, second_operand);
	float result = sin(first_operand);

	prefix_phrase->set(index, result);
	prefix_phrase->remove(index + 1);
}

void Calculator::Tg(int index) { //Tangens function
	float first_operand = prefix_phrase->at(index + 1);
	float second_operand = 0;

	DecodeConstants(first_operand, second_operand);
	float result = tan(first_operand);

	prefix_phrase->set(index, result);
	prefix_phrase->remove(index + 1);
}

void Calculator::Ctg(int index) { //Cotangens function
	float first_operand = prefix_phrase->at(index + 1);
	float second_operand = 0;

	DecodeConstants(first_operand, second_operand);
	float result = 1/tan(first_operand);

	prefix_phrase->set(index, result);
	prefix_phrase->remove(index + 1);
}

void Calculator::Ln(int index) { //Nalural logarithm function
	float first_operand = prefix_phrase->at(index + 1);
	float second_operand = 0;

	DecodeConstants(first_operand, second_operand);
	float result = log(first_operand);

	prefix_phrase->set(index, result);
	prefix_phrase->remove(index + 1);
}

void Calculator::Log(int index) { // function logarithm base 10
	float first_operand = prefix_phrase->at(index + 1);
	float second_operand = 0;

	DecodeConstants(first_operand, second_operand);
	float result = log10(first_operand);

	prefix_phrase->set(index, result);
	prefix_phrase->remove(index + 1);
}

void Calculator::Sqrt(int index) { //Sqrt function
	float first_operand = prefix_phrase->at(index + 1);
	float second_operand = 0;

	DecodeConstants(first_operand, second_operand);
	float result = sqrt(first_operand);

	prefix_phrase->set(index, result);
	prefix_phrase->remove(index + 1);
}

void Calculator::Module(int index) { //Module function
	float first_operand = prefix_phrase->at(index + 1);
	float second_operand = 0;

	DecodeConstants(first_operand, second_operand);
	float result = abs(first_operand);

	prefix_phrase->set(index, result);
	prefix_phrase->remove(index + 1);
}

void Calculator::Elevate(int index) { //Elevate function
	float first_operand = prefix_phrase->at(index + 1);
	float second_operand = prefix_phrase->at(index + 2);

	DecodeConstants(first_operand, second_operand);
	float result = pow(first_operand, second_operand);

	prefix_phrase->set(index, result);
	prefix_phrase->remove(index + 1);
	prefix_phrase->remove(index + 1);
}

void Calculator::DecodeConstants(float& first_operand, float& second_operand) { //Decoder for constants
	if (first_operand == -15) //fisrt operand is 'pi'
		first_operand = PI;
	if (first_operand == -16) //fisrt operand is 'e'
		first_operand = E;
	if (second_operand == -15) //second operand is 'pi'
		second_operand = PI;
	if (second_operand == -16) //second operand is 'e'
		second_operand = E;
}

void Calculator::DecodePrefixPhrase(float current_element) { //decoding elements in the prefix phrase for correct output of the prefix phrase
	switch (int(current_element)) { //checking current element
	case-1: //it's adding
		cout << "+ "; //printing plus
		break;
	case-2: //it's unary minus
		cout << "-(u) "; //ptinting unary minus
		break;
	case-3: //it's binary minus
		cout << "- ";
		break;
	case-4: //it's multiplying
		cout << "* ";
		break;
	case-5: //it's dividing
		cout << "/ ";
		break;
	case-6: //it's cosinus
		cout << "cos ";
		break;
	case-7: //it's sinus
		cout << "sin ";
		break;
	case-8: //it's tangent
		cout << "tg ";
		break;
	case-9: //it's cotangent
		cout << "ctg ";
		break;
	case-10: //it's ln
		cout << "ln ";
		break;
	case-11: //it's log
		cout << "log ";
		break;
	case-12: //it's sqrt
		cout << "sqrt ";
		break;
	case-13: //it's module
		cout << "mod ";
		break;
	case-14: //it's elevating
		cout << "^ ";
		break;
	case-15: //it's "pi" constant
		cout << "pi ";
		break;
	case-16: //it's "e" constant
		cout << "e ";
		break;
	}
}

void Calculator::PrefixOuput() { //outputting of prefix phrase and the resul
	float current_element;
	cout << "Prefix form: " << endl;
	for (int i = 0; i < prefix_phrase->get_size(); i++) { //going through prefix phrase
		if (prefix_phrase->get_size() == 0) //if there are no elements
			cout << "No operations and operands" << endl;
		else { //if there are elements
			current_element = prefix_phrase->at(i); //getting current element
			if (current_element < 0) { //if it is something coded
				DecodePrefixPhrase(current_element); //going to decode ot
			}
			else //if it's not coded
				cout << current_element << " ";//printing it
		}
	}

	cout << endl;
	system("pause");

	ChooseOperation(); //going to do mathematics

	cout << "Result: " << endl;
	prefix_phrase->print_to_console(); //printing result
	cout << endl;

	system("pause");
}