#Members
William Young: 27569845

John Niland: 27266650

##Part 1

Program contains a main loop that forks a process fron the parent. When inside of the child, a new child, the grandchild,
is created. This child calls sleep for 10 seconds and then exits, printing its id. Outside of that the child waits for the grandchild
to print and exit before doing so itself. The parent then ends by doing the same thing.

##Part 2

User is prompted to enter a process ID to kill. This is done using the kill system call. A check is made on the result
of kill; if it is less than zero then an error is printed. This would happen if an invalid ID was entered, otherwise
a kill message is printed.

##Part 3

The shell runs programs in respect to the current directory, otherwise it is a fairly straight-forward implementation
####References Used
linux.die.net man pages as an alternative for built in linux man
