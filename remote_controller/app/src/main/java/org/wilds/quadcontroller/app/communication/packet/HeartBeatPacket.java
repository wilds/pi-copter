package org.wilds.quadcontroller.app.communication.packet;

/**
 * Created by Wilds on 14/04/2014.
 */
public class HeartBeatPacket extends Packet {

    protected long timestamp;

    public HeartBeatPacket() {
        this.setType(TYPE_HEARTBEAT);
        setTimestamp(System.currentTimeMillis());
    }

    public long getTimestamp() {
        return timestamp;
    }

    protected void setTimestamp(long timestamp) {
        this.timestamp = timestamp;
    }

    @Override
    public boolean waitReply() {
        return true;
    }

    @Override
    public String toString() {
        return TYPE_HEARTBEAT + " " + timestamp;
    }
}
