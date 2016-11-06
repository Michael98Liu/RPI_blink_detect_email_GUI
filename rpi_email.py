import smtplib

gmail_user = "michael.liu916@gmail.com"
gmail_pwd = "liufengyuan1998"
to = ["fl1092@nyu.edu", 'cx473@nyu.edu']
smtpserver = smtplib.SMTP("smtp.gmail.com", 587)
message = "\r\n".join([
"From: michael", "To: cx473", "Subject: Second email from raspberry Pi", "", "Sorry, the first email is exclusively for me."])

smtpserver.ehlo()
smtpserver.starttls()
smtpserver.login(gmail_user, gmail_pwd)
smtpserver.sendmail(gmail_user, to, message)
print "Successfully sent!"
smtpserver.close()
