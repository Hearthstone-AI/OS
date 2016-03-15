import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;

public class RoundRobin {
	int tolJob;
	int tolWait;
	int tolRun;
	String file;
	ArrayList<Job> jobTrace;
	
	public RoundRobin(){
		tolJob = 0;
		tolWait = 0;
		tolRun = 0;
		jobTrace = new ArrayList<Job>();
	}

	public void run(){
		int time = 0; //total running time
		int position = 0; //current position in the trace
		int waitTime = 0; //total waiting time
		ArrayList<Integer> waiting = new ArrayList<Integer>(); //array of currently waiting jobs
		while((position != jobTrace.size()) || !waiting.isEmpty()){
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
			//while there is jobs in the queue
			if(!waiting.isEmpty()){
				waiting.set(0, waiting.get(0) - 1); //decrement the job in queue
				waitTime += (waiting.size() - 1); //add the time waited for all the jobs -1(the worked job)
				if(waiting.get(0) ==  0) //remove the job if its done
					waiting.remove(0);
				else
					waiting.add(waiting.remove(0)); //move job to back if not done
			}
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
		System.out.println("RR " + file + ": [" + (int)Math.ceil(((double)tolWait)/tolJob) + "]");
	}
}
