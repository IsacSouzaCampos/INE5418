import java.lang.SecurityManager;
import java.rmi.Naming;
// import java.rmi.RemoteException;

//Executar:
// rmiregistry &
// java -Djava.security.policy=server.policy Server

public class Server {
	
    public Server() {  
		if(System.getSecurityManager() == null) {
		     System.setSecurityManager(new SecurityManager());
		}
		try {  
	        //System.setProperty("java.rmi.server.hostname", "127.0.0.1");
	        //Naming.rebind("rmi://127.0.0.1/Lista1", new List()); 
	        Naming.rebind("Lista1", new List());  
	    }  
	    catch( Exception e ) {  
	        System.out.println( "Nao registrou o objeto: " + e );  
	    }	    
	}
	
	public static void main(String[] args) {		
		// Thread t = new Thread(new Client());
		// t.start();

		// Thread t2 = Thread.currentThread();
		// System.out.println(t2.getName());

		new Server();
	}

}
