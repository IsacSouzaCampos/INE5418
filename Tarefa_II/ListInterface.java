import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ListInterface extends Remote {
	public void add(int key, String value) throws RemoteException;
	public String get(int key) throws RemoteException;
	public String remove(int key) throws RemoteException;
}
