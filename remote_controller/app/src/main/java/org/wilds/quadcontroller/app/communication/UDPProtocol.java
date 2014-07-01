package org.wilds.quadcontroller.app.communication;

import android.util.Log;

import org.wilds.quadcontroller.app.communication.packet.HelloPacket;
import org.wilds.quadcontroller.app.communication.packet.Packet;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

/**
 * Created by Wilds on 14/04/2014.
 */
public class UDPProtocol implements Protocol {

    //
    protected DatagramSocket sendSocked;
    protected int sendPort = 58000;
    protected InetAddress sendTo;

    // Listening vars
    /*
    protected Thread listenThread;
    protected int listenPort = 57001;
    protected boolean listening = true;
    */

    protected OnReceiveListener onReceiveListener;

    public UDPProtocol(int sendPort /*, int listenPort*/) {
        this.sendPort = sendPort;
        //this.listenPort = listenPort;

        try {
            sendSocked = new DatagramSocket(sendPort);
        } catch (SocketException e) {
            e.printStackTrace();
        }

//        startListening();
    }

    @Override
    public void setOnReceiveListener(OnReceiveListener listener) {
        this.onReceiveListener = listener;
    }


    /*
    protected void startListening() {
        listenThread = new Thread(new Runnable() {
            @Override
            public void run() {

                try {
                    DatagramSocket sock = new DatagramSocket(listenPort);
                    while (listening) {
                        byte[] message = new byte[1500];
                        DatagramPacket p = new DatagramPacket(message, message.length);
                        sock.receive(p);
                        String text = new String(message, 0, p.getLength());

                        //TODO handle quadcopter response at hello packet

                        onReceiveListener.onReceive(text);
                    }
                    sock.close();

                } catch (SocketException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }

        });
        listenThread.start();
    }
*/

    @Override
    public boolean sendPacket(final Packet packet) { // TODO convertire in async task
        if (!isConnected())
            return false;
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    sendSocked.send(new DatagramPacket(packet.getByte(), packet.getLength(), sendTo, sendPort));

                    if (packet.waitReply()) {
                        byte[] message = new byte[1024];
                        DatagramPacket p = new DatagramPacket(message, message.length);
                        sendSocked.receive(p);
                        String text = new String(message, 0, p.getLength());

                        //TODO handle quadcopter response
                        if (onReceiveListener != null)
                            onReceiveListener.onReceive(packet.getType(), text);
                    }
                    //return true;
                } catch (IOException e) {
                    e.printStackTrace();
                    //return false;
                }
            }
        }).start();

        return true;
    }


    @Override
    public boolean searchForQuadcopter() {  // TODO convertire in async task
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    Packet packet = new HelloPacket(InetAddress.getLocalHost().getHostAddress());
                    DatagramPacket broadcast = new DatagramPacket(packet.getByte(), packet.getLength(), InetAddress.getByName(Utils.getBroadcast()), sendPort);
                    sendSocked.setBroadcast(true);
                    sendSocked.send(broadcast);
                    long t = System.currentTimeMillis();
                    while (System.currentTimeMillis() - t < 5000) {
                        byte[] message = new byte[1024];
                        DatagramPacket p = new DatagramPacket(message, message.length);
                        sendSocked.receive(p);
                        String text = new String(message, 0, p.getLength());
                        Log.e("LOGGER", "RECEIVED: "+text);
                        // TODO
                        if (text.contains("OK")) {
                            if (onReceiveListener != null)
                                onReceiveListener.onReceive(packet.getType(), p.getAddress());
                        }
                    }
                    //return true;
                } catch (IOException e) {
                    e.printStackTrace();
                    //return false;
                }
            }
        }).start();

        return true;
    }

    @Override
    public boolean connectToQuadcopter(String id) {
        try {
            this.sendTo = InetAddress.getByName(id);
            return true;
        } catch (UnknownHostException e) {
            e.printStackTrace();
            return false;
        }
    }

    public void close() {
        //listening = false;
        sendSocked.close();
    }

    @Override
    public boolean isConnected() {
        return sendTo != null;
    }
}
