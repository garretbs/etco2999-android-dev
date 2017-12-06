
package sfml.com.sfml_dummy;

import java.util.Random;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

public class Test
{
    private static Test test = null;
    private String message;

    private static int size = 44100;
    private static short[] samples = new short[size];


    public Test(String message)
    {
        this.message = message;
    }

    private String printMessage()
    {
        System.out.println(message);
        return message;
    }

    public static String print()
    {
        assure();
        return test.printMessage();
    }

    private static void assure()
    {
        if (test == null)
        {
            test = new Test("test");
        }
    }

    public static short[] randomSound(){
        Random randy = new Random();
        /*short[] choice = {0, 2500, 5000, 7500, 10000, 12500, 15000, 17500, 20000, 22500, 25000,
                27500, 30000, 32500, (short) 35000, (short) 37500, (short) 44100, (short) 37500,
                (short) 35000, 32500, 30000, 27500, 25000, 22500, 20000, 17500, 15000, 12500, 10000,
                7500, 5000, 2500};*/

        for(int i=0;i<size;i++){
            samples[i] = (short) randy.nextInt();
            //samples[i] = choice[i%31];
        }
        getNetworkSound();
        return samples;
    }

    private static short[] getNetworkSound(){
        short[] result = new short[size];

        final int port = 1101;
        final String address = "206.21.94.172";
        try {
            SocketAddress clientAddress = new InetSocketAddress(address, port);

            Socket host = new Socket();
            host.connect(clientAddress, port);

            //get data from host
            for(int i=0;i<size;i++){
                samples[i] = (short) host.getInputStream().read();
            }

            host.close();
        }catch(IOException e){
            System.out.println("getNetworkSound() ain't working");
        }

        return result;
    }
}
