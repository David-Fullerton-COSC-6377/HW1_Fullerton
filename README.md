# HW1_Fullerton
(03-02-2015 Mon @ 9.42 AM) 

David Fullerton
Homework #1
COSC 6377:  Networking class
Spring 2015

This is a readme to explain real quick what is happening in this lab.
There are two different programs:  Client.c and Server.c.  They are
written in C (not C++) and are large files that have similar code.
But, the initial slice of the project is an end-user based application 
that is called the "Client.c".  The client is set up to be the 
communicator and interactor with the end-user.  It will act like a 
menu-driven application.  However, the other side of the project
is the server-like application called "Server.c".

These two different applications are communicating using the 
ATOP protocol that we developed in class.  Both use the four
(or so) different messages to communicate.  The Client asks
the user what they want to do through a menu, then the Client
will translate that into the Protocol.  Then, the Client will
send the message that is expressed through the protocol's
specifics to the Server.  After the Server receives it, the 
Server will attempt to perform what the Client asks.  If it
does, then a successful result will be visible.  If not, then
an error message will be shown and sent back to the client.

To compile these, I have created a quick compiler file called
"mc" which is a batch file to compile both applications.  If you
run it with "./mc" and don't see any errors, then the two programs
compiled without issue.


