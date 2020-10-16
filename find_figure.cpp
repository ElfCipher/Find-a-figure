#include "find_figure.h"


using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");

	char file_name[500];
	cout << "Введите путь к файлу:" << endl;
	cin >> file_name;

	FILE *out = fopen(file_name, "r");
	if (!out)
	{
		cout << "Ошибка открытия файла" << endl;
		system("pause");
		return 0;
	}

	int temp_arr[figure_size*(figure_size + 1)] = { 0 };
	int tmp = 0;
	size_t i = 0;

	while ((tmp = fgetc(out)) != EOF) // в массив temp_arr записываем все символы из файла
	{
		temp_arr[i] = tmp;
		i++;
	}

	int data[figure_size][figure_size] = { 0 };
	size_t k = 1;

	for (size_t i = 1; i < figure_size - 1; i++) // теперь в двумерный массив data конвертируем символы "0" и "1" и инт
	{
		size_t j = 0;
		while (temp_arr[i * figure_size + j + k] != 10)
		{
			if (temp_arr[i*figure_size + j + k] == 48)
				data[i][j] = 0;
			else
				data[i][j] = 1;

			j++;
		}
		k++;
	}

	figure undefined_figure(data); // создаём объект класса, в конструктор которому передаём массив data
	undefined_figure.PrintFigure(); // просто выводит этот массив
	undefined_figure.DefineFigure(); // определяет, есть ли в этом массиве квадрат, круг или неопределённая фигура
	undefined_figure.GetData(); // выводит информацию о найденной фигуре

	fclose(out);

	system("pause");
	return 0;
}

void figure::PrintFigure()
{
	for (size_t i = 0; i < figure_size; i++)
	{
		for (size_t j = 0; j < figure_size; j++)
			cout << this->picture[i][j] << " ";

		cout << endl;
	}
}

figure::figure_type figure::DefineSquare()
{
	size_t counter = 0;
	
	for (size_t i = 1; i < figure_size - 1; i++)
	{
		for (size_t j = 1; j < figure_size - 1; j++)
		{
			if (picture[i][j] == 1)				// проходим по всему массиву, пока не найдётся первая 1
			{
				column = j;						// запоминаем её координаты
				string = i;
				while (picture[i][j] != 0) { counter++; j++; length_of_side++; } //нашли длину стороны
				break;
			}
		}
		if (column + string >= 2)   // выходим из цикла если какая-то точка найдена
			break;
	}

	if (length_of_side < 5 || length_of_side > 10) // если длина стороны меньше 5 или больше 10, то такого квадрата быть не может
		return undefined;

	for (size_t i = string + 1; i < string + length_of_side; i++) // зная границы квадрата, считаем, все ли символы это 1
	{
		for (size_t j = column; j < column + length_of_side; j++)
		{
			if (picture[i][j] == 1)
				counter++;
		}
	}

	if (counter == length_of_side*length_of_side && counter == GetNumberOfCharacters()) // если квадрат заполненм 1, а за его пределами нет 1,
		return square;																	// то квадрат найден
	else
		return undefined;
}

figure::figure_type figure::DefineCircle()
{
	column = string = 0;
	for (size_t i = 1; i < figure_size - 1; i++) // тот же принцип
	{
		for (size_t j = 1; j < figure_size - 1; j++)
		{
			if (picture[i][j] == 1)
			{
				string = i;
				while (picture[i][j] != 0) { i++; diameter++; } //нашли димаетр
				column = j;
				i--;
				string = (string + i) / 2; // координаты центра
				break;
			}
		}
		if (column + string >= 2)
			break;
	}

	if (diameter < 5 || diameter > 10)
		return undefined;

	size_t add_symbols = 3; // 1 по сторонам
	size_t counter = 0;
	if (diameter % 2 != 0) //диаметр нечётный
	{
		size_t step = diameter / 2;
		if (picture[string][column + step]		+
			picture[string + step][column]		+
			picture[string][column - step]		!= add_symbols) // проверяем, со всех ли сторон симметрично есть 1
			return undefined;

		for (size_t i = string - step + 1; i < string + step; i++) // зная границы квадрата, считаем, все ли символы это 1
		{
			for (size_t j = column - step + 1; j < column + step; j++)
			{
				if (picture[i][j] == 1)
					++counter;
			}
		}
	}
	else // диаметр чётный. принцип тот же, только по сторонам больше 1
	{
		add_symbols = 7;
		size_t step = diameter / 2;
		if (picture[string - step + 1][column + 1]										+
			picture[string + step][column] + picture[string + step][column + 1]			+
			picture[string][column - step + 1] + picture[string][column + step]			+
			picture[string + 1][column - step + 1] + picture[string + 1][column + step] != add_symbols)
			return undefined;

		for (size_t i = string - step + 2; i < string + step; i++)
		{
			for (size_t j = column - step + 2; j < column + step; j++)
			{
				if (picture[i][j] == 1)
					counter++;
			}
		}
	}

	if (counter == (diameter - 2)*(diameter - 2) && counter + ++add_symbols == GetNumberOfCharacters()) // как и с поиском квадрата
		return circle;																				// проверяем сумму 1 и круг найден
	else
		return undefined;
}

void figure::GetData() // выводит данные о фигурах
{
	switch (test)
	{
		case undefined:
			cout << "Неопределённая фигура" << endl;
			break;
		case square:
			cout << "Квадрат" << endl;
			cout << "Левая вернхняя координата: " << ++string << "," << ++column << " Длина стороны = " << length_of_side << endl;
			break;
		case circle:
			cout << "Круг" << endl;
			cout << "Координата центра круга: " << ++string << "," << ++column << " Диаметр = " << diameter << endl;
			break;
		default:
			break;
	}
}

void figure::DefineFigure() //вызываем по очереди все функции, которые созданы для поиска фигуры, если фигура будет определена, то 
{								// переменная test сохранит наименованием фигуры
	test = DefineSquare();
	if (test == square)
		return;
	
	test = DefineCircle();
	if (test == circle)	
		return;
}

size_t figure::GetNumberOfCharacters() // считаем кол-во 1
{
	size_t counter = 0;
	for (size_t i = 1; i < figure_size - 1; i++)
	{
		for (size_t j = 1; j < figure_size - 1; j++)
		{
			if (picture[i][j] == 1)
				counter++;
		}
	}

	return counter;
}
