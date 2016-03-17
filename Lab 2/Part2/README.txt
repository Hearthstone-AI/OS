John Niland
William Young

Part 2 Report

Scheduler in C

The class consists of 4 methods:
Producer : The producer thread, creates a request and uses the insertRequestmethod to add it to the queue.
Consumer: The consumer thread, removes a request from the queue using removeRequestand process it.
InsertRequest : Inserts a request into the queue.
RemoveRequest: Removes a request from the queue.

Besides this, there is a struct, request, representing a request that has an id field (tid) and a duration (secs) field.
The main method works by initializing the semaphores, starting numCons amount of consumers, and then
starting the producer thread. FInally, the program will run for duration seconds before terminating.

How to change the program:

To change the number of consumers, change the numCons variable. Default is 10.
To change the duration of the program, change the duration variable. Default is 20.
To change the max runtime of a request, change the #define variable at the top, MAX_RUNTIME. Default is 10.
To change the max size of the buffer, change the #define variable at the top, BUFFER_SIZE. Default is 10.


Running the program:

First, compile the program with:

gcc main.c -o main -lpthread

To run it, type:

./main

Scheduler in Java

There are two files, Scheduler and Request. 
Request is a class used to represent a request, it has two fields, ID and 
length, as well as getter and setter methods. 
Scheduler has a main method that creates numCons consumers, starts them, and
then starts the producer. The producer is just an instance of the Scheduler class, whos run method calls addRequest,
which creates a new request, adds it to the queue, and notifies any waiting consumers that the queue can be accessed.
A java Vector was used for the queue since it works with syncronization.
The consumer thread is represented by an inner class, Consumer, whos run method calls the removeRequest method in Scheduler.
this method removes the first request in the queue and processes it.

How to change the program:

To change the number of consumers, change the numCons variable. Default is 10.
To change the max size of the buffer, change the variable buff_size. Default is 10.

Running the program:

First, compile both programs by doing:

javac Scheduler.java
javac Request.java

To run it, type:

java Scheduler