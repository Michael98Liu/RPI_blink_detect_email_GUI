
def email_sender(gmail_user,gmail_pwd,to,message):
#takes in user email adress and password as strings, to as a list of strings of recipient email addresses
  import smtplib

  
  
  smtpserver = smtplib.SMTP("smtp.gmail.com", 587)
  

  smtpserver.ehlo()
  smtpserver.starttls()
  smtpserver.login(gmail_user, gmail_pwd)
  smtpserver.sendmail(gmail_user, to, message)
  print("Successfully sent!")
  smtpserver.close()

def email_constructor(from_,to,subject,content):
  message=["From: "+from_]
  message.append("To: "+to)
  message.append("Subject: "+subject)
  message.append("")
  message.append(content)
  result="\r\n".join(message)
  return result
