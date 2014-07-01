package org.wilds.quadcontroller.app.communication.packet;

/**
 * Created by Wilds on 14/04/2014.
 */
public class MotionPacket extends Packet {

    protected int thr;
    protected int pitch;
    protected int roll;
    protected int yaw;

    public MotionPacket() {
        this.setType(TYPE_MOTION);
    }

    public MotionPacket(int throttle, int pitch, int roll,  int yaw) {
        this.setType(TYPE_MOTION);
        setThrottle(throttle);
        setPitch(pitch);
        setRoll(roll);
        setYaw(yaw);
    }

    public int getThrottle() {
        return thr;
    }

    protected void setThrottle(int throttle) {
        this.thr = throttle;
    }


    public int getYaw() {
        return yaw;
    }

    protected void setYaw(int yaw) {
        this.yaw = yaw;
    }

    public int getPitch() {
        return pitch;
    }

    public void setPitch(int pitch) {
        this.pitch = pitch;
    }

    public int getRoll() {
        return roll;
    }

    public void setRoll(int roll) {
        this.roll = roll;
    }

    @Override
    public String toString() {
        return TYPE_MOTION + " " + thr + " " + pitch + " " + roll + " " + yaw;
    }
}
