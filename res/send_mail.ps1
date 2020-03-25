Param(
        [String]$Att,
        [String]$Subj,
        [String]$Bod
)

Function Send-Email
{
    Param(
            [Parameter(Mandatory=$true)][String]$To,
            [Parameter(Mandatory=$true)][String]$From,
            [Parameter(Mandatory=$true)][String]$Password,
            [Parameter(Mandatory=$true)][String]$Subject,
            [Parameter(Mandatory=$true)][String]$Body,
            [Parameter(Mandatory=$true)][String]$Attachment)

    try
    {
        $Msg = New-Object System.Net.Mail.MailMessage($From, $To, $Subject, $Body)
        $Srv = "smtp.gmail.com"
        if ($Attachment -ne $null)
        {
			Write-Output "Attachment -ne null"
            try
            {
				$Attachments = $Attachment -split ("\:\:");
				Write-Output "right after split of attachments"

                ForEach ($val in $Attachments)
                {
                    $attach = New-Object System.Net.Mail.Attachment($val)
                    $Msg.Attachments.Add($attach)
				}
				Write-Output "right after foreach on attachments"
            }
            catch
            {
				Write-Output "attachment error"
                exit 2;
            }

			Write-Output "before create client object"
            $Client = New-Object Net.Mail.SmtpClient($Srv, 587)
			Write-Output "after create client object"
            $Client.EnableSsl = $true
			Write-Output "before create cred object"
            $Client.Credentials = New-Object System.Net.NetworkCredential($From.Split("@")[0], $Password)
			Write-Output "after create cred object"
			Write-Output "before send mail"
            $Client.Send($Msg)
			Write-Output "after send mail"

            Remove-Variable -Name Client
            Remove-Variable -Name Password

			Write-Output "good0"
            exit 7;
		}
		Write-Output "out of attachments block"
    }
    catch
    {
		Write-Output "main error"
        exit 3;
    }
}

try
{
	Write-Output "start"
    Send-Email `
        -Attachment $Att `
        -To "@gmail.com" `
        -Subject $Subj `
        -Body $Bod `
        -Password "" `
        -From "@gmail.com"
	Write-Output "good"
}
catch
{
	Write-Output "end error"
    exit 4;
}