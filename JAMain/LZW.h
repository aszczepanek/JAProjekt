#ifndef LZW_H
#define LZW_H
#include <Windows.h>
#include <string>
#include <iostream>

using namespace std;

DWORD WINAPI CompressThread(LPVOID lpParameter);

// strukrua zawieraj鉍a parametry dla w靖ku
struct CompressParams {
	char* srcData; // dane do skompresowania
	int srcDataSize; // rozmiar danych srcData
	char* compressedData; // miejsce na skompresowane dane
	int compressedDataSize; // rozmiar danych skompresowanych (w s這wach kodowych, nie bajtach)

	char* dictData; // miejsce na dane s這wnika
	int dictSize; // rozmiar s這wnika

	CompressParams() :
		srcData(NULL),
		srcDataSize(0),
		compressedData(NULL),
		compressedDataSize(0),
		dictData(NULL),
		dictSize(0)
	{}
};

// pojedynczy element s這wnika (jedno s這wo kodowe, jako cz��� listy jednokierunkowej)
struct Element {
	char* value;
	int size; // ile bajt闚 ma value
	Element* next;

	Element(char* pWord, int length) {
		value = new char[length];
		memcpy(value, pWord, length);
		next = NULL;
		size = length;
	}

	~Element() {
		delete value;
	}
};

// klasa obs逝guj鉍a s這wnik
class Dictionary {
private:
	// lista jednokierunkowa
	Element* head;
	Element* tail; 

	int count; // ilo�� s堯w w s這wniku
	int size; // rozmiar s這wnika w bajtach


public:
	// inicjalizacja domy�lnymi warto�ciami
	Dictionary() : head(NULL), tail(NULL), count(0), size(0) {}

	// zwolnienie zasob闚
	~Dictionary() {
		if(head == NULL) return;
		Element* tmp = head;
		while(tmp->next != NULL) {
			Element* tmp_prev = tmp;
			tmp = tmp->next;
			delete tmp_prev;
		}
		delete tmp;
	}

	// dodawanie s堯w kodowych do s這wnika
	int addCodeword(char* pWord, int length) {
		if(head == NULL) {
			head = new Element(pWord, length);
			tail = head;
			count++;
			size += length;
			return count - 1; // zwraca kod nowego s這wa w s這wniku
		}

		tail->next = new Element(pWord, length);
		tail = tail->next;
		count++;
		size += length;
		return count - 1; // zwraca kod nowego s這wa w s這wniku
	}

	// zwraca kod danego s這wa w s這wniku; je�li brak w s這wniku to zwraca -1
	int getCodewordId(char* pWord, int length) {
		Element* tmp = head;
		int result = -1; // domy�lnie nie znaleziono elementu
		for (int i = 0; i < count; i++) {
			if(length != tmp->size) {// nie ma sensu por闚nywa� o ci鉚闚 o r騜nych d逝go�ciach
				tmp = tmp->next;
				continue;
			}
			if(memcmp(pWord, tmp->value, length) == 0) {
				result = i; // znaleziono dopasowanie
				break; 
			}
			tmp = tmp->next;
		}
		return result;
	}

	int getSize() {
		return size;
	}

	int getCount() {
		return count;
	}

	Element* operator[](int index) {
		if(index >= count) return NULL;

		Element* tmp = head;
		for (int i = 0; i < index; i++) {
			head = head->next;
		}
		
		return head;
	}

	void printDebugInfo() {
		cout << "Count: " << count << endl;
		cout << "Size: " << size << endl;
		cout << "Words: " << endl;
		Element* tmp = head;
		for(int i = 0; i < count; i++) {
			cout << i << ": " << tmp->value << endl;
			tmp = tmp->next;
		}
	}
};

#endif