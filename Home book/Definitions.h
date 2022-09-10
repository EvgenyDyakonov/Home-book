#pragma once

#define ON_Menue_DELETE_ALL 1
#define ON_Menue_DELETE_SELECTION 2
#define ON_Menue_REFERENSE 3
#define ON_Exit_Cliced 4
#define ON_BUTTON_RECORD 5
#define ON_BUTTON_SEARCH 6
#define ON_BUTTON_NAME_SORT 7
#define ON_BUTTON_LOCATED_SORT 8
#define ON_BUTTON_TAG_SORT 9
#define ON_SHOW_ALL 10

#define Index_for_numbers_win 200

#define BUFFER_TEXT_SIZE 4000

char buffer_text[BUFFER_TEXT_SIZE];

int readChars;

unsigned buffer_number = NULL;
unsigned temp_number = 0;

LRESULT CALLBACK softwareMainProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM ip);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);

HWND hEdit_for_save_control;
HWND hEdit_ShowResult;

HWND read_name;
HWND read_located;
HWND read_tag;
HWND read_search;
