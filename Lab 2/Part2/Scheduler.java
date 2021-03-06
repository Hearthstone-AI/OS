import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Vector;

//Scheduler contains the main method, but also extends Thread and represents the producer with its run method.
class Scheduler extends Thread {

	static int buff_size = 10; //Size of the buffer.
	static int numCons = 10; //Number of consumers.
	int id = 0; //Request ID, starts at 0.
	static int conID = 0; //ID of the consumer.
	private Vector<Request> requests = new Vector<Request>(); //The queue, vector works with syncronization.
	DateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");

	public static void main(String args[]) {

		// Creates a new producer that handles the creation of new requests.
		Scheduler producer = new Scheduler();

		// Creates numCons consumers, then runs them.
		for (int i = 0; i < numCons; i++) {
			new Consumer(producer, ++conID).start();
		}
		// The producer is then ran.
		producer.start();
	}

	//Run method for the producer.
	public void run() {
		try {
			while (true) {
				// Sleep for a random time.
				Thread.sleep((int) Math.random() * 100);
				// Generate request and add it to queue.
				addRequest();

				// Generate a new random time, and sleep for it.
				int s = (int) Math.floor(Math.random() * 400);
				System.out.println("Sleeping for " + s + " milliseconds");
				Thread.sleep(s);
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	//Add a request to the queue and prints the production message.
	private synchronized void addRequest() throws InterruptedException {
		// Waits if queue is full.
		while (requests.size() == buff_size) {
			wait();
		}
		int s = (int) Math.floor(Math.random() * 500);
		// Generate new request and add it to the queue.
		Request r = new Request(++id, s);
		System.out.println("Producer: Produced request ID " + r.getReqID() + ", length of " + r.getReqLength()
				+ " milliseconds at time " + dateFormat.format(Calendar.getInstance().getTime()));
		requests.add(r);

		// Let any waiting consumers know that there's something in the queue.
		notify();

	}
	//Removes a request from the queue and returns it.
	public synchronized Request removeRequest() throws InterruptedException {

		// If the list is empty, then it waits.
		while (requests.size() == 0) {
			wait();
		}
		// Remove a request from the queue.
		Request r = requests.remove(0);
		// Lets the producer know that it's not full anymore, then return it.
		notify();

		return r;

	}
}

//Class for handling consumers.
class Consumer extends Thread {
	
	Scheduler producer; //Reference to the producer
	int id; //Id of the consumer
	DateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
	
	//Constructor for creating a new consumer
	Consumer(Scheduler prod, int id) {
		this.id = id;
		this.producer = producer;
	}

	public void run() {
		try {
			while (true) {
				// Gets the first request and processes it.
				Request r = producer.removeRequest();
				System.out.println("Consumer: " + id + " assigned request ID " + r.getReqID()
						+ " processing request for the next " + r.getReqLength() + " milliseconds, current time is "
						+ dateFormat.format(Calendar.getInstance().getTime()));
				// Consumes the request by sleeping for its length.
				Thread.sleep((int) r.getReqLength());
				System.out.println("Consumer: " + id + " completed request ID " + r.getReqID()
						+ " at time "
						+ dateFormat.format(Calendar.getInstance().getTime()));
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}