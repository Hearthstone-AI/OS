
public class Main {

	public static void main(String[] args) {
		FCFS f = new FCFS();
		f.readFile("Trace\\100-100-6.txt");
		f.run();
		f.printStats();
		f.readFile("Trace\\100-100-8.txt");
		f.run();
		f.printStats();
		f.readFile("Trace\\100-100-11.txt");
		f.run();
		f.printStats();
		f.readFile("Trace\\100-200-6.txt");
		f.run();
		f.printStats();
		f.readFile("Trace\\100-200-8.txt");
		f.run();
		f.printStats();
		f.readFile("Trace\\100-200-11.txt");
		f.run();
		f.printStats();
		f.readFile("Trace\\100-600-6.txt");
		f.run();
		f.printStats();
		f.readFile("Trace\\100-600-8.txt");
		f.run();
		f.printStats();
		f.readFile("Trace\\100-600-11.txt");
		f.run();
		f.printStats();
		RoundRobin rr = new RoundRobin();
		rr.readFile("Trace\\100-100-6.txt");
		rr.run();
		rr.printStats();
		rr.readFile("Trace\\100-100-8.txt");
		rr.run();
		rr.printStats();
		rr.readFile("Trace\\100-100-11.txt");
		rr.run();
		rr.printStats();
		rr.readFile("Trace\\100-200-6.txt");
		rr.run();
		rr.printStats();
		rr.readFile("Trace\\100-200-8.txt");
		rr.run();
		rr.printStats();
		rr.readFile("Trace\\100-200-11.txt");
		rr.run();
		rr.printStats();
		rr.readFile("Trace\\100-600-6.txt");
		rr.run();
		rr.printStats();
		rr.readFile("Trace\\100-600-8.txt");
		rr.run();
		rr.printStats();
		rr.readFile("Trace\\100-600-11.txt");
		rr.run();
		rr.printStats();
	}
}