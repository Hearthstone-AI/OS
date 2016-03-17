public class Request {
	private int ID; //ID of the request.
	private double length; //Duration (length) of the request.

	//Creates a new request.
	public Request(int id, double length) {
		this.ID = id;
		this.length = length;
	}

	//Get the ID of the request.
	public int getReqID() {
		return ID;
	}
	
	//Set the ID of the request.
	public void setReqID(int i) {
		this.ID = i;
	}

	//Get the length of the request.
	public double getReqLength() {
		return length;
	}
	
	//Set the length of the request.
	public void setReqLength(int l) {
		this.length = l;
	}
}