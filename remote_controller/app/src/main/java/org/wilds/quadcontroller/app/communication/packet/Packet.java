package org.wilds.quadcontroller.app.communication.packet;

/**
 * Created by Wilds on 14/04/2014.
 */
//TODO remove json and send string message

public abstract class Packet {

    public static final String TYPE_HELLO = "hello";
    public static final String TYPE_MOTION = "rcinput";
    public static final String TYPE_HEARTBEAT = "heartbeat";
    public static final String TYPE_TEST_MOTOR = "tm";

    protected String type;

    public String getType() {
        return type;
    }

    protected void setType(String type) {
        this.type = type;
    }

    public boolean waitReply() {
        return false;
    }

    @Override
    public String toString() {
        return type;
    }

    public byte[] getByte() { return this.toString().getBytes(); }

    public int getLength() {
        return this.toString().length();
    }
}
