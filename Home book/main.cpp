#include <windows.h>
#include <string>
#include <deque>
#include <fstream>
#include <algorithm>
#include "Definitions.h"
#include "Thing.h"
#include "resource.h"

Thing thing;
deque <Thing> list_of_things;
deque<Thing> search_result;

char file_name[260];

string path = "list_of_things.txt";
fstream file;

ostream& operator << (ostream& os, Thing& object);
istream& operator >> (istream& is, Thing& object);

void sort_search(deque<Thing>& dk, int type_of_sorting);
void make_text(char* buffer_text, const deque<Thing>& search_result);
void search_thing(deque<Thing>& const list_of_things, deque<Thing>& search_result);
bool find_maker(string const& qeschion, string const& str);
string make_whitespase(char* str, int length);

void record_thing(deque<Thing>& list_of_things);
void MainWndAddMenue(HWND hWnd);
void MainWndAddWidgets(HWND hWnd);
void buffer_cleaner();

int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR args, _In_ int ncmdshow)
{

	file.open(path, fstream::in | fstream::out);

	WNDCLASS softWearMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON2)),
		L"MainWindowClass", softwareMainProcedure);
	if (!RegisterClassW(&softWearMainClass)) return -1;
	MSG SoftwareMainMessage = { 0 };

	CreateWindow(L"MainWindowClass", L"Книга вещей", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 800, 400, 635, 500, NULL, NULL, NULL, NULL);

	if (!file.is_open())
	{
		SetWindowTextA(hEdit_for_save_control, "База не загружена!");
	}
	else
	{
		while (!file.eof())
		{
			file >> thing;
			list_of_things.push_back(thing);
		}
		SetWindowTextA(hEdit_for_save_control, "База загружена");
		file.close();
	}

	while (GetMessage(&SoftwareMainMessage, NULL, NULL, NULL))
	{
		TranslateMessage(&SoftwareMainMessage);
		DispatchMessage(&SoftwareMainMessage);
	}
	return 0;
}

WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure)
{
	WNDCLASS NWC = { 0 };

	NWC.hIcon = Icon;
	NWC.hCursor = Cursor;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}

LRESULT CALLBACK softwareMainProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		switch (wp)
		{
		case ON_SHOW_ALL:
			SetWindowTextA(hEdit_for_save_control, "Все записи");
			SetWindowTextA(read_name, "");
			SetWindowTextA(read_located, "");
			SetWindowTextA(read_tag, "");
			SetWindowTextA(hEdit_ShowResult, "");
			search_result.clear();
			copy(list_of_things.begin(), list_of_things.end(), back_inserter(search_result));
			make_text(buffer_text, search_result);
			SetWindowTextA(hEdit_ShowResult, buffer_text);
			break;
		case ON_Menue_DELETE_ALL:
			list_of_things.clear();
			file.open(path, fstream::in | fstream::out);
			file << "";
			file.close();
			MessageBoxA(hwnd, "Все данные удалены.", "Удаление базы", MB_OK);
			SetWindowTextA(hEdit_for_save_control, "Удалено");
			SetWindowTextA(read_name, "");
			SetWindowTextA(read_located, "");
			SetWindowTextA(read_tag, "");
			SetWindowTextA(hEdit_ShowResult, "");
			buffer_cleaner();
			break;
		case ON_Menue_DELETE_SELECTION:
			if (search_result.empty())
			{
				MessageBoxA(hwnd, "Нет выборки для удаления!", "Удаление базы", MB_OK);
			}
			else
			{
				for (int i = 0; i < search_result.size(); i++)
				{
					Thing temp = search_result[i];
					list_of_things.erase(remove_if(list_of_things.begin(), list_of_things.end(), [&temp](Thing l)
						{
							return l == temp;
						}), list_of_things.end());
				}
				file.open(path, fstream::in | fstream::out);
				for (auto ex : list_of_things)
				{
					file << ex;
				}
				file.close();
				MessageBoxA(hwnd, "Выбранные данные удалены.", "Удаление базы", MB_OK);
				SetWindowTextA(hEdit_for_save_control, "Удалено");
				SetWindowTextA(read_name, "");
				SetWindowTextA(read_located, "");
				SetWindowTextA(read_tag, "");
				SetWindowTextA(hEdit_ShowResult, "");
				buffer_cleaner();
			}
			break;
		case ON_Menue_REFERENSE:
			MessageBoxA(hwnd, "\n  - Программа предназначена для хранения информации о предметах и их расположении в помещении\n\n  - Кнопка НОВАЯ ЗАПИСЬ записывает текущее состояние полей НАИМЕНОВАНИЕ, РАСПОЛОЖЕНИЕ и ПРИЗНАК (под признаком подразумевается любая характеристика предмета на усмотрение пользователя)\n\n  - Кнопка ПОИСК запускает поиск совпадений введенного набора символов с ранее сделанными записями.\n\n  - Результат поиска будет показан ниже, под кнопками сортировки.", "Справка", MB_OK);
			break;
		case ON_Exit_Cliced:
			file.close();
			PostQuitMessage(0);
			break;
		case ON_BUTTON_RECORD:
			file.open(path, fstream::in | fstream::out);
			record_thing(list_of_things);
			for (auto ex : list_of_things)
			{
				file << ex;
			}
			file.close();
			SetWindowTextA(read_name, "");
			SetWindowTextA(read_located, "");
			SetWindowTextA(read_tag, "");
			break;
		case ON_BUTTON_SEARCH:
			SetWindowTextA(hEdit_ShowResult, "");
			search_thing(list_of_things, search_result);
			if (search_result.empty())
			{
				SetWindowTextA(hEdit_for_save_control, "Ничего не найдено");
			}
			else
			{
				SetWindowTextA(hEdit_for_save_control, "Найдено");
				make_text(buffer_text, search_result);
				SetWindowTextA(hEdit_ShowResult, buffer_text);
				buffer_cleaner();
			}
			break;
		case ON_BUTTON_NAME_SORT:
			if (search_result.empty())
			{
				break;
			}
			else
			{
				SetWindowTextA(hEdit_for_save_control, "Найдено");
				SetWindowTextA(hEdit_ShowResult, "");
				sort_search(search_result, 1);
				make_text(buffer_text, search_result);
				SetWindowTextA(hEdit_ShowResult, buffer_text);
				buffer_cleaner();
				break;
			}
		case ON_BUTTON_LOCATED_SORT:
			if (search_result.empty())
			{
				break;
			}
			else
			{
				SetWindowTextA(hEdit_for_save_control, "Найдено");
				SetWindowTextA(hEdit_ShowResult, "");
				sort_search(search_result, 2);
				make_text(buffer_text, search_result);
				SetWindowTextA(hEdit_ShowResult, buffer_text);
				buffer_cleaner();
				break;
			}
			break;
		case ON_BUTTON_TAG_SORT:
			if (search_result.empty())
			{
				break;
			}
			else
			{
				SetWindowTextA(hEdit_for_save_control, "Найдено");
				SetWindowTextA(hEdit_ShowResult, "");
				sort_search(search_result, 3);
				make_text(buffer_text, search_result);
				SetWindowTextA(hEdit_ShowResult, buffer_text);
				buffer_cleaner();
				break;
			}
			break;
		default: break;
		}
		break;
	}

	case WM_CREATE:
		MainWndAddMenue(hwnd);
		MainWndAddWidgets(hwnd);
		break;
	case WM_DESTROY:
		file.close();
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wp, lp);
	}
}

void MainWndAddMenue(HWND hWnd)
{
	HMENU RootMenu = CreateMenu();
	HMENU SubMenu = CreateMenu();
	HMENU SubActionMenu = CreateMenu();

	AppendMenu(SubActionMenu, MF_STRING, ON_Menue_DELETE_SELECTION, L"Найденную выборку");
	AppendMenu(SubActionMenu, MF_STRING, ON_Menue_DELETE_ALL, L"Удалить всё");

	AppendMenu(SubMenu, MF_STRING, ON_SHOW_ALL, L"Показать всё");
	AppendMenu(SubMenu, MF_POPUP, (UINT_PTR)SubActionMenu, L"Удалить");
	AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);

	AppendMenu(SubMenu, MF_STRING, ON_Exit_Cliced, L"Выход");
	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"Меню");
	AppendMenu(RootMenu, MF_STRING, ON_Menue_REFERENSE, L"Справка");
	SetMenu(hWnd, RootMenu);
};

void MainWndAddWidgets(HWND hWnd)
{
	CreateWindowA("static", "Наименование:", WS_VISIBLE | WS_CHILD, 5, 35, 150, 20, hWnd, NULL, NULL, NULL);
	CreateWindowA("static", "Признак:", WS_VISIBLE | WS_CHILD, 170, 35, 70, 20, hWnd, NULL, NULL, NULL);
	CreateWindowA("static", "Расположение:", WS_VISIBLE | WS_CHILD, 335, 35, 110, 20, hWnd, NULL, NULL, NULL);

	read_name = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | WS_TABSTOP, 5, 60, 150, 50, hWnd, NULL, NULL, NULL);
	read_tag = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | WS_TABSTOP, 170, 60, 150, 50, hWnd, NULL, NULL, NULL);
	read_located = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_MULTILINE | WS_TABSTOP, 335, 60, 150, 50, hWnd, NULL, NULL, NULL);

	CreateWindowA("button", "Новая запись", WS_VISIBLE | WS_CHILD | ES_CENTER, 490, 60, 110, 50, hWnd, (HMENU)ON_BUTTON_RECORD, NULL, NULL);
	hEdit_for_save_control = CreateWindowA("static", " ", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 5, 600, 30, hWnd, NULL, NULL, NULL);

	read_search = CreateWindowA("edit", "", WS_VISIBLE | WS_CHILD | ES_MULTILINE, 5, 120, 480, 30, hWnd, NULL, NULL, NULL);
	CreateWindowA("button", "Поиск", WS_VISIBLE | WS_CHILD | ES_CENTER, 490, 120, 110, 30, hWnd, (HMENU)ON_BUTTON_SEARCH, NULL, NULL);

	CreateWindowA("static", "Сортировать по:", WS_VISIBLE | WS_CHILD, 5, 165, 130, 20, hWnd, NULL, NULL, NULL);
	CreateWindowA("button", "Расположению", WS_VISIBLE | WS_CHILD | ES_CENTER, 275, 190, 130, 30, hWnd, (HMENU)ON_BUTTON_LOCATED_SORT, NULL, NULL);

	CreateWindowA("button", "Признаку", WS_VISIBLE | WS_CHILD | ES_CENTER, 140, 190, 130, 30, hWnd, (HMENU)ON_BUTTON_TAG_SORT, NULL, NULL);
	CreateWindowA("button", "Наименованию", WS_VISIBLE | WS_CHILD | ES_CENTER, 5, 190, 130, 30, hWnd, (HMENU)ON_BUTTON_NAME_SORT, NULL, NULL);
	CreateWindowA("button", "Расположению", WS_VISIBLE | WS_CHILD | ES_CENTER, 275, 190, 130, 30, hWnd, (HMENU)ON_BUTTON_LOCATED_SORT, NULL, NULL);

	hEdit_ShowResult = CreateWindowA("static", "Результат поиска", WS_VISIBLE | WS_CHILD | WS_VSCROLL, 5, 230, 600, 600, hWnd, NULL, NULL, NULL);
}

string make_whitespase(char* str, int length)
{
	for (int i = 0; i < length; i++)
	{
		if (str[i] == ' ')
		{
			str[i] = '_';
		}
	}
	return string(str);
}

char* make_whitespase(string str)
{
	const int length = str.size();
	const char* arr_str = str.c_str();
	char* new_array = new char[length + 1];

	for (int i = 0; i < length; i++)
	{
		new_array[i] = arr_str[i];
		if (arr_str[i] == '_')
		{
			new_array[i] = ' ';
		}
	}
	new_array[length] = '\0';
	return *&new_array;
}

void make_text(char* buffer_text, const deque<Thing>& search_result)
{
	int length, string_numbers = 1, i = 0;
	char* str;
	for (auto ex : search_result)
	{
		buffer_text[i] = (char)string_numbers + 48;
		i++;
		buffer_text[i] = ' ';
		i++;
		str = make_whitespase(ex.GetName());
		length = strlen(str);

		for (int j = 0; j < length; j++)
		{
			buffer_text[i] = str[j];
			i++;
		}

		buffer_text[i] = ' ';
		i++;
		buffer_text[i] = '(';
		i++;
		str = make_whitespase(ex.GetType());
		length = strlen(str);

		for (int j = 0; j < length; j++)
		{
			buffer_text[i] = str[j];
			i++;
		}
		buffer_text[i] = ')';
		i++;
		buffer_text[i] = ' ';
		i++;
		str = make_whitespase(ex.GetAdress());
		length = strlen(str);
		buffer_text[i] = '-';
		i++;
		buffer_text[i] = ' ';
		i++;
		for (int j = 0; j < length; j++)
		{
			buffer_text[i] = str[j];
			i++;
		}
		buffer_text[i] = ';';
		i++;
		if (string_numbers - 1 < search_result.size())
		{
			buffer_text[i] = '\n';
			i++;
			string_numbers++;
		}
	}
	buffer_text[i] = '\0';
}

void record_thing(deque<Thing>& list_of_things)
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	Thing new_thing;
	string EditMessage;
	int text_lenth = GetWindowTextLength(read_name) + 1;
	char* data = new char[text_lenth];
	GetWindowTextA(read_name, data, text_lenth);

	EditMessage = make_whitespase(data, text_lenth);
	new_thing.SetName(EditMessage);
	delete[]data;

	text_lenth = GetWindowTextLength(read_located) + 1;
	data = new char[text_lenth];
	GetWindowTextA(read_located, data, text_lenth);
	EditMessage = make_whitespase(data, text_lenth);
	new_thing.SetAdress(EditMessage);
	delete[]data;

	text_lenth = GetWindowTextLength(read_tag) + 1;
	data = new char[text_lenth];
	GetWindowTextA(read_tag, data, text_lenth);
	EditMessage = make_whitespase(data, text_lenth);
	new_thing.SetType(EditMessage);
	delete[]data;

	if (new_thing.GetAdress() == "" && new_thing.GetName() == "" && new_thing.GetType() == "")
	{

	}
	else
	{
		list_of_things.push_back(new_thing);
		SetWindowTextA(hEdit_for_save_control, "Записано");
	}

	SetConsoleOutputCP(866);
	SetConsoleCP(866);
}



void search_thing(deque<Thing>& const list_of_things, deque<Thing>& search_result)
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	search_result.clear();
	int text_lenth = GetWindowTextLength(read_search) + 1;
	char* data = new char[text_lenth];
	GetWindowTextA(read_search, data, text_lenth);
	string reqest = make_whitespase(data, text_lenth);
	copy_if(list_of_things.begin(), list_of_things.end(), back_inserter(search_result), [&reqest, &list_of_things](Thing& a)
		{
			return find_maker(reqest, a.GetAdress()) || find_maker(reqest, a.GetName()) || find_maker(reqest, a.GetType());
		});

	SetConsoleOutputCP(866);
	SetConsoleCP(866);
}

bool find_maker(string const& qeschion, string const& str)
{
	const int size_qeschion = qeschion.size();
	const char* arr_qeschion = qeschion.c_str();
	const int size_str = str.size();
	const char* arr_str = str.c_str();

	for (int i = 0; i < size_str; i++)
	{
		if (arr_str[i] == arr_qeschion[0])
		{
			int j = 0;
			while (j < size_qeschion)
			{
				i++;
				j++;
				if (arr_str[i] != arr_qeschion[j])
				{
					break;
				}
				if (j == size_qeschion - 1)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void sort_search(deque<Thing>& dk, int type_of_sorting)
{
	switch (type_of_sorting)
	{
	case 1:
		sort(dk.begin(), dk.end(), [&dk](Thing a, Thing b)
			{
				return a.GetName() < b.GetName();
			});
		break;
	case 2:
		sort(dk.begin(), dk.end(), [&dk](Thing a, Thing b)
			{
				return a.GetAdress() < b.GetAdress();
			});
		break;
	case 3:
		sort(dk.begin(), dk.end(), [&dk](Thing a, Thing b)
			{
				return a.GetType() < b.GetType();
			});
		break;
	default:
		break;
	}
}

void buffer_cleaner()
{
	int size = BUFFER_TEXT_SIZE * sizeof(char);
	memset(buffer_text, '\0', size);
}

ostream& operator << (ostream& os, Thing& object)
{
	os << object.GetName() << " " << object.GetAdress() << " " << object.GetType() << '\n';
	return os;
}

istream& operator >> (istream& is, Thing& object)
{
	string try_Name;
	string try_Adress;
	string try_Type;
	is >> try_Name >> try_Adress >> try_Type;
	object.SetName(try_Name);
	object.SetAdress(try_Adress);
	object.SetType(try_Type);
	return is;
}