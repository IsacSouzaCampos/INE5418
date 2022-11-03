import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject; 
import java.util.HashMap;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class List extends UnicastRemoteObject implements ListInterface{
	HashMap<Integer,String> list;
	private static Lock lock = new ReentrantLock();
	
	public List() throws RemoteException {
		list = new HashMap<Integer,String>();
	}

	@Override
	public void add(int key, String value) throws RemoteException {
		lock.lock();
		list.put(key, value);
		lock.unlock();
	}
	
	@Override
	public String get(int key) throws RemoteException {
		String value = list.get(key);
		
		return value;
	}

	@Override
	public String remove(int key) throws RemoteException {
		String value = list.get(key);
		list.remove(key);
		
		return value;
	}
}
