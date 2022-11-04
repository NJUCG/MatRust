import com.rust.model.RustThread;

public class ThreadTest implements Runnable{

    public void run(){
        int count = 0;
        for(int i=0;i<10000;i++){
            count++;
        }
        System.out.println(count);
    }

    public static void main(String[] args) {
        Thread t = new RustThread();
        t.start();
        System.out.println("?");
    }
}
