import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;

public class FCFS {
	
	int tolJob;
	int tolWait;
	int tolRun;
	String file;
	ArrayList<Job> jobTrace;
	
	public FCFS(){
		tolJob = 0;
		tolWait = 0;
		tolRun = 0;
		file = "";
		jobTrace = new ArrayList<Job>();
	}
	
	public void run(){
		int time = 0; //total running time
		int working = 0; //time left on currently active job
		int position = 0; //current position in the trace
		int waitTime = 0; //total waiting time
		ArrayList<Integer> waiting = new ArrayList<Integer>(); //array of currently waiting jobs
		while((position != jobTrace.size()) || !waiting.isEmpty() || (working != 0)){
			//For loop to check on every iteration to scan for any arrived jobs, add those to the queue
			for(int i = position; i < jobTrace.size(); i++){
				if(jobTrace.get(i).getStart() == time){
					position++;
					waiting.add(jobTrace.get(i).getLenth());
				}
				else{
					break;
				}
			}
			//if the active job is done and there is another to load, do it
			if((working == 0) && !waiting.isEmpty()){
				working = waiting.remove(0);
			}
			//if the job isnt done, decrement it
			if(working > 0)
				working--;
			waitTime += waiting.size(); //add the waiting time for all waiting jobs
			time++;
		}
		tolWait = waitTime;
		tolRun = time;
		return;
	}
	
	//Parsing method to parse the file(s) into memory
	public void readFile(String s){
		jobTrace.clear();
		file = s.substring(6, s.length() - 4);
		String line = "";
		String spLine[] = new String[2];
		Path path = Paths.get(s);
		int count = 0;
		try {
			BufferedReader r = Files.newBufferedReader(path);
			while((line = r.readLine()) != null){
				if(count == 0)
					tolJob = Integer.parseInt(line);
				if(count > 2){
					spLine = line.split(" ");
					jobTrace.add(new Job(Integer.parseInt(spLine[0]), Integer.parseInt(spLine[1])));
				}
				count++;
			}
		}
		catch(IOException e) {
			System.out.println("Doc Open Error");
		}
	}
	
	public void printStats(){
		System.out.println("FCFS " + file + ": [" + (int)Math.ceil(((double)tolWait)/tolJob) + "]");
	}
}
