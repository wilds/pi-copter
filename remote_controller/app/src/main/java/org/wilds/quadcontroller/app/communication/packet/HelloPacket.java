package org.wilds.quadcontroller.app.communication.packet;

/**
 * Created by Wilds on 14/04/2014.
 */
public class HelloPacket extends Packet {

    protected String from;

    public HelloPacket(String from) {
        this.setType(TYPE_HELLO);
        setFrom(from);
    }

    public String getFrom() {
        return from;
    }

    protected void setFrom(String from) {
        this.from = from;
    }

    @Override
    public boolean waitReply() {
        return true;
    }

    @Override
    public String toString() {
        return TYPE_HELLO + " " + from;
    }
}
