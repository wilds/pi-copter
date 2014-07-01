package org.wilds.quadcontroller.app;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.VideoView;

import org.wilds.quadcontroller.app.communication.OnReceiveListener;
import org.wilds.quadcontroller.app.communication.Protocol;
import org.wilds.quadcontroller.app.communication.UDPProtocol;
import org.wilds.quadcontroller.app.communication.packet.MotionPacket;
import org.wilds.quadcontroller.app.communication.packet.Packet;
import org.wilds.quadcontroller.app.communication.packet.TestMotorPacket;
import org.wilds.quadcontroller.app.joystick.DualJoystickView;
import org.wilds.quadcontroller.app.joystick.JoystickMovedListener;

import java.net.InetAddress;

/**
 * Created by Wilds on 13/04/2014.
 */
public class QuadControllerActivity extends Activity {

    protected static Protocol protocol = new UDPProtocol(58000 /*, 58100*/);

    TextView txtX1, txtY1;
    TextView txtX2, txtY2;
    DualJoystickView joystick;

    protected int throttle = 6;
    protected int yaw = 0;
    protected int roll = 0;
    protected int pitch = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.dualjoystick);

        VideoView v = (VideoView) findViewById(R.id.surface_view);
        // TEST!!!
        /*
        v.setVideoPath("http://daily3gp.com/vids/747.3gp");
        v.start();
        v.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mediaPlayer) {
                mediaPlayer.seekTo(1);
                mediaPlayer.start();
            }
        });
        */
        // END TEST

        txtX1 = (TextView) findViewById(R.id.TextViewX1);
        txtY1 = (TextView) findViewById(R.id.TextViewY1);

        txtX2 = (TextView) findViewById(R.id.TextViewX2);
        txtY2 = (TextView) findViewById(R.id.TextViewY2);

        joystick = (DualJoystickView) findViewById(R.id.dualjoystickView);

        joystick.setOnJostickMovedListener(_listenerLeft, _listenerRight);
        joystick.getLeftStick().setDisableAutoReturnToCenterY(true);
        joystick.getLeftStick().setYAxisInverted(false);

        Button connect = (Button) findViewById(R.id.buttonConnect);
        connect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                protocol.searchForQuadcopter();
                //protocol.connectToQuadcopter("192.168.11.133");
            }
        });

        Button test0 = (Button) findViewById(R.id.buttonTestMotor0);
        test0.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                protocol.sendPacket(new TestMotorPacket(2500, 0, 0, 0));
            }
        });

        protocol.setOnReceiveListener(new OnReceiveListener() {
            @Override
            public void onReceive(String type, Object message) {
                if (type == Packet.TYPE_HELLO && message instanceof InetAddress) {
                    // TODO show list and choose
                    InetAddress quadcopter = (InetAddress) message;
                    Log.d("QuadController", quadcopter.getHostAddress());
                    protocol.connectToQuadcopter(quadcopter.getHostAddress());
                }
            }
        });

        /*
        PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        PowerManager.WakeLock wl = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "My Tag");
        wl.acquire();
        ..screen will stay on during this section..
        wl.release();
        */
    }

    private JoystickMovedListener _listenerLeft = new JoystickMovedListener() {

        @Override
        public void OnMoved(int pan, int tilt) {
            throttle = tilt + 50;
            yaw = pan;

            txtX1.setText(Integer.toString(yaw));
            txtY1.setText(Integer.toString(throttle));

            protocol.sendPacket(new MotionPacket(throttle, pitch, roll, yaw));  //TODO
        }

        @Override
        public void OnReleased() {
            txtX1.setText("released");
            txtY1.setText("released");
        }

        public void OnReturnedToCenter() {
            yaw = 0;
            //txtX1.setText("stopped");
            //txtY1.setText("stopped");
            protocol.sendPacket(new MotionPacket(throttle, pitch, roll, yaw));  //TODO
        }

        ;
    };

    private JoystickMovedListener _listenerRight = new JoystickMovedListener() {

        @Override
        public void OnMoved(int pan, int tilt) {
            pitch = tilt;
            roll = pan;

            txtX2.setText(Integer.toString(pan));
            txtY2.setText(Integer.toString(tilt));

            protocol.sendPacket(new MotionPacket(throttle, pitch, roll, yaw));  //TODO
        }

        @Override
        public void OnReleased() {
            txtX2.setText("released");
            txtY2.setText("released");
        }

        public void OnReturnedToCenter() {
            pitch = 0;
            roll = 0;
            //txtX2.setText("stopped");
            //txtY2.setText("stopped");
            protocol.sendPacket(new MotionPacket(throttle, pitch, roll, yaw));  //TODO
        }

        ;
    };

    @Override
    protected void onDestroy() {
        super.onDestroy();
        //protocol.close();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        super.onCreateOptionsMenu(menu);

        MenuItem Item = menu.add("PID");
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getTitle() == "PID") {
            Intent intent = new Intent(this, PIDActivity.class);
            startActivity(intent);
        }
        return true;
    }
}
