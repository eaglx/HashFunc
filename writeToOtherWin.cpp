#include <iostream>
#include <string.h>
#include <sstream>
#include <Windows.h>

HWND handleOtherProc;

struct ObjectAnotherProcess {
    HWND hwnd;
    int id;
    char content[256];
};

ObjectAnotherProcess UserName;
ObjectAnotherProcess InputField;
ObjectAnotherProcess CheckButton;

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) 
{
    TCHAR buff[256];
    GetWindowText(hwnd, (LPSTR)buff, 255);
    std::cout << "	GetWindowText = " << buff << std::endl;
    memset(buff, 0, sizeof(buff));
    
    GetDlgItemText(handleKeyGen, GetDlgCtrlID(hwnd), (LPSTR)buff, 255);
    std::cout << "	GetDlgItemText = " << buff << std::endl;
    memset(buff, 0, sizeof(buff));
    
    SendMessage(hwnd, WM_GETTEXT, sizeof(buff) / sizeof(buff[0]), (LPARAM) buff);
    std::cout << "	SendMessage = " << buff << std::endl;
    if (GetDlgCtrlID(hwnd) == 4) 
    {
        CheckButton.hwnd = hwnd;
        CheckButton.id = GetDlgCtrlID(hwnd);
    }
    else if (GetDlgCtrlID(hwnd) == 5) 
    {
        InputField.hwnd = hwnd;
        InputField.id = GetDlgCtrlID(hwnd);
    }
    else if (GetDlgCtrlID(hwnd) == 6) 
    {
        UserName.hwnd = hwnd;
        UserName.id = GetDlgCtrlID(hwnd);
        for (int i = 0; i < 256; i++) UserName.content[i] = (char) buff[i];
    }
    return TRUE;
}

int main() 
{
  DWORD procID;
  std::string strInputToOtherProc = "";
  
  handleOtherProc = FindWindow(NULL, "< window name >");
  if(handleOtherProc == NULL) return -1;
  GetWindowThreadProcessId(handleOtherProc, &procID);
  
  // Enumerates all the child windows / controls belongs to a specified window.
  // This function invokes a application defined CALLBACK function until the
  // last child control is enumerated or the function returns false.
  EnumChildWindows(handleOtherProc, EnumChildProc, 0);
  
  TCHAR buff[256];
  memset(buff, 0, sizeof(buff));
  strInputToOtherProc.push_back((char)((int)UserName.content[0] + 1));
  
  SendMessage(InputField.hwnd, WM_SETTEXT, sizeof(buff) / sizeof(buff[0]), (LPARAM)buff); // write str to input field
  SendMessage(CheckButton.hwnd, BM_CLICK, 0, 0); // click button
  
  return 0;
}
