import socket
import sys

#code for this program was adapted from theServer
#lecture 15 slides

#Function to send text through a socket
#Parameters: a socket, text to be sent
#Will return a string if there was an error
def sendtext(s,text):
	try:
		s.sendall(text)
	except socket.error:
		print "Error sending server handle"
		return 'err'	

#function to receive data from a socket connection
#Parameters: socket, text to be prepended to incoming data		
def gettext(s, pre):
	print pre,
	sys.stdout.flush() #force print without newline		
	message = s.recv(500)
	print message
	return message
	
#function to get input from stdin and verify it's length
#paramters: prompt to display 		
def getinput(prompt):
	message = raw_input(prompt)
	while len(message) > 500:
		print"Message exceeds 500 character limit."
		message = raw_input(prompt)
	return message

#main function to create a socket send/recieve messages
def main(arg):
	portno = arg
	name = "theServer> "
	message = ''	
	
	try: #create a socket and start listening
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		sock.bind(('', portno))
		sock.listen(5)
	except socket.error: #catch an errors with making socket
		print"Error opening socket"
		sys.exit(1)
		
	while 1: #loop until sigint
		print("The server is listening on port" + str(portno))
		#accept incoming connection
		newsock, addr = sock.accept()
		#receive client handle
		cliname = newsock.recv(10)
		cliname = cliname + ">"
		#send this handle
		if sendtext(newsock, name) != 'err':
			while 1: #loop until a user wants to quit
				#get message from client
				message = gettext(newsock, cliname)	
				if message == "\quit":
					break
			
				#get input from stdin
				message = getinput(name) #get user input from std in
			
				#send message to client
				#break from loop if error sending
				# or msg sent was quit
				if sendtext(newsock, message) == 'err':
					break
				if message == "\quit":
					break	
		#close non-listening connection
		newsock.close()
		
	#should never reach here
	sock.close()
	
if __name__ == "__main__":
	main(int(sys.argv[1]))
		
		
	
	
