import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.util.Scanner;


public class Client {

	Scanner in; 
	ListInterface list; 
	
	public Client() {
		in = new Scanner(System.in);

		if(System.getSecurityManager() == null) {
			System.setSecurityManager(new SecurityManager());
		}	    	 
		try {  
			list =(ListInterface)Naming.lookup( "rmi://127.0.0.1/Lista1");  
		}
		catch(RemoteException e ) {  
			System.out.println();  
			System.out.println( "RemoteException: " + e.toString() );  
		}  
		catch(NotBoundException e ) {  
			System.out.println();  
			System.out.println( "NotBoundException: " + e.toString() );  
		}  
		catch(Exception e ) {  
			System.out.println();  
			System.out.println( "Exception: " + e.toString() );  
		}
	}
	
	public void execute() {
		// String entrada;
		String comando;
		int key;
		String value;

		// System.out.println(Thread.currentThread().getName());
		
		try {
			System.out.println("Entre com um dos comandos a seguir:\n \t\tput <key> <valor>\n\t\tget <key>\n\t\tremove <key>\n\t\tsair\n\n");
			while(true) {
				comando = in.next();
				if(comando.equalsIgnoreCase("put")) {
					key = in.nextInt();
					value = in.next();
					list.add(key,value);
					System.out.println("Adicionou à linha " + String.valueOf(key) + ": " + value);
				} else if(comando.equalsIgnoreCase("get")) {
					key = in.nextInt();
					value = list.get(key);
					System.out.println("Resultado: " + value);
				} else if(comando.equalsIgnoreCase("remove")) {
					key = in.nextInt();
					value = list.remove(key);
					System.out.println("Removeu linha " + String.valueOf(key) + ": " + value);
				} else {
					System.out.println("Saindo do programa");
					break;
				}
			}
			in.close();
		} catch(Exception e) {		
			System.out.println( "Exception: " + e.toString()); 
		}
	}
	
	public static void main(String[] args) {
		Client c = new Client();
		c.execute();
	}
}
