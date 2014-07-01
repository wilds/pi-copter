package org.wilds.quadcontroller.app.communication;

import org.wilds.quadcontroller.app.communication.packet.Packet;

/**
 * Created by Wilds on 14/04/2014.
 */
public interface Protocol {
    public boolean isConnected();
    public boolean searchForQuadcopter();  // send broadcast udp packet
    public boolean connectToQuadcopter(String id);   // connect to quadcopter
    public boolean sendPacket(Packet packet);
    public void setOnReceiveListener(OnReceiveListener listener);
    public void close();
}
