import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject; 
import java.util.HashMap;

public class List extends UnicastRemoteObject implements ListInterface{
	HashMap<Integer,String> list;
	
	public List() throws RemoteException {
		list = new HashMap<Integer,String>();
	}

	@Override
	public void add(int key, String value) throws RemoteException {
		list.put(key, value);		
	}
	
	@Override
	public String get(int key) throws RemoteException {
		return list.get(key);
	}

	@Override
	public String remove(int key) throws RemoteException {
		String value = this.get(key);
		list.remove(key);
		return value;
	}
}
