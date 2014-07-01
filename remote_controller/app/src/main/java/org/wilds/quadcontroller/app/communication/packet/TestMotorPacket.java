package org.wilds.quadcontroller.app.communication.packet;

/**
 * Created by Wilds on 14/04/2014.
 */
public class TestMotorPacket extends Packet {

    protected int m0;
    protected int m1;
    protected int m2;
    protected int m3;

    public TestMotorPacket(int m0, int m1, int m2, int m3) {
        this.setType(TYPE_TEST_MOTOR);
        this.m0 = m0;
        this.m1 = m1;
        this.m2 = m2;
        this.m3 = m3;
    }

    public int getM0() {
        return m0;
    }

    public void setM0(int m0) {
        this.m0 = m0;
    }

    public int getM1() {
        return m1;
    }

    public void setM1(int m1) {
        this.m1 = m1;
    }

    public int getM2() {
        return m2;
    }

    public void setM2(int m2) {
        this.m2 = m2;
    }

    public int getM3() {
        return m3;
    }

    public void setM3(int m3) {
        this.m3 = m3;
    }

    @Override
    public boolean waitReply() {
        return true;
    }

    @Override
    public String toString() {
        return TYPE_TEST_MOTOR +" "+ m0 + " "+ m1 + " "+ m2 + " "+ m3;
    }
}
