#include "pch.h"
#include "CppUnitTest.h"
#include "../main/Calculator.cpp"
#include "../main/Calculator.h"
#include "../main/List.h"
#include "../main/List.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		Calculator* test;

		TEST_METHOD_INITIALIZE(setUp) //inizialization list's test
		{
			test = new Calculator();

		}

		TEST_METHOD_CLEANUP(cleanUp) //deleting list's test
		{
			delete test;
		}

		TEST_METHOD(setPhrase_getPhraseSize) //setting phrase to phrase in class
		{
			string phrase;
			phrase = "1+1";
			test->SetPhrase(phrase);
			test->TransformPhrase(0, 2);

			Assert::AreEqual(test->GetPhraseSize(), 3);
		}

		TEST_METHOD(checkPhrase_default_case) {
			string phrase;

			phrase = "1+1/1";
			test->SetPhrase(phrase);
			test->CheckPhrase(0, 4);
			test->TransformPhrase(0, 4);
			Assert::AreEqual(test->GetPhraseSize(), 5);

		}

		TEST_METHOD(checkPhrase_begining_of_the_phrase) { //checknig error in the begining of the phrase
			try {
				string phrase;
				phrase = ".1+1";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 3);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(checkPhrase_ending_of_the_phrase) {//checknig error in the ending of the phrase (last point)
			try {
				string phrase;
				phrase = "1+1.";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 3);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(checkPhrase_two_points) { //checknig error two and more point in a number
			try {
				string phrase;
				phrase = "1.1.1+1";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 6);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(checkPhrase_last_point_before_operation) { // last symbol in number before operation is point
			try {
				string phrase;
				phrase = "1.+1";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 3);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(checkPhrase_last_point_before_bracket) { // last symbol before operation is point closing bracket was point
			try {
				string phrase;
				phrase = "(1+1.)";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 5);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(checkPhrase_other_number_errors) { // other cases if symbol was number
			try {
				string phrase;
				phrase = "1+1(";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 3);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(checkPhrase_after_operation) { // errors after operation symbol
			try {
				string phrase;
				phrase = "1+)";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 2);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(checkPhrase_e_errors) { // errors after "e" constant
			try {
				string phrase;
				phrase = "1+esin";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 6);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(checkPhrase_pi_errors) { // errors after "pi" constant
			try {
				string phrase;
				phrase = "1+pisin";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 6);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(checkPhrase_two_chars_errors) { // errors after functions of two chars
			try {
				string phrase;
				phrase = "1+ln)";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 4);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(checkPhrase_three_chars_errors) { // errors after functions of three chars
			try {
				string phrase;
				phrase = "1+mod)";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 5);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(checkPhrase_sqrt_errors) { // errors after "sqrt"
			try {
				string phrase;
				phrase = "1+sqrt)";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 6);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(checkPhrase_opening_bracket_errors) { // errors after opening bracket
			try {
				string phrase;
				phrase = "(+1";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 2);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(checkPhrase_closing_bracket_errors) { // errors after closing bracket
			try {
				string phrase;
				phrase = "(1+1)(";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 5);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(checkPhrase_quantity_of_opening_bracket_errors) { //more opening brackets than closing ones
			try {
				string phrase;
				phrase = "((1+1)";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 5);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(checkPhrase_quantity_of_closing_bracket_errors) { //more opening brackets than closing ones
			try {
				string phrase;
				phrase = "(1+1))";
				test->SetPhrase(phrase);
				test->CheckPhrase(0, 5);
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Input error");
			}
		}

		TEST_METHOD(readPhrase) { //inputing phrase
			try {
				test->ReadPhrase();
			}
			catch (const char* error) {
				Assert::AreEqual(error, "Can't open an input file");
			}
		}

		TEST_METHOD(transformPhrase_getPrefixPhrase) { //transforming check
			string phrase = "1+1+1";
			test->SetPhrase(phrase);
			test->TransformPhrase(0, 4);
			List* test_list = test->GetPrefixPhrase();
			Assert::AreEqual(test_list->get_size(), size_t(5));
		}

		TEST_METHOD(prefixOutput) {

		}
	};
}
