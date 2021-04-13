// PractiveAndreev.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";
	argc > 1 ? printf("%s", argv[1]) : printf("%s", "No argv");
	int arg = atoi(argv[1]);

	return 0;
}

