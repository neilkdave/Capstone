import de.voidplus.leapmotion.*;

LeapMotion leap;
PVector leapWorld = new PVector(100, 100, 100);
boolean fullScreenApp = true;

void settings() {
  if (fullScreenApp) {
    fullScreen(P3D);
  } 
  else {
    size(800, 450, P3D);
  }
}

void setup() {
  background(255);
  leap = new LeapMotion(this);
  
  // leap.moveWorld(-360, -360, -60); // need to move the world proportionally to the window size
  // 800x500 (-360,-360,-60)

  leap.moveWorld(parseInt((-1.0/2.0) * width), parseInt((-3.0/4.0) * height), parseInt((-1.0/15.0) * max(height, width))); // need to move the world proportionally to the window size
  // 1600x900 (-720,-720,-120)
  
  // is roughly -900 to 900 in x 
  // is roughly 
}

// void setup() {
//   size(640, 360, P3D);
//   noStroke();
// }

/*
maxX: 971.0079
minX: -124.62537
maxY: 492.07104
minY: 0.0
maxZ: 106.02615
minZ: -23.31665
//*/

void draw() {
  ambientLight(102, 102, 102);
  directionalLight(126, 126, 126, 0, height / 2, height / 2);
  background(0);
  float cameraY = height / 2.0;
  float fov = PI/4;
  float cameraZ = cameraY / tan(fov / 2.0);
  // System.out.println(cameraZ);
  float aspect = float(width) / float(height);
  perspective(fov, aspect, cameraZ / 10.0, cameraZ * 10.0);
  
  translate(width / 2, height / 3, 0);
  rotateX(-PI/8);
  fill(255);
  stroke(0);
  box(width, height, max(width, height)); // draw world container

  
  translate(0, height / 2, -width / 2); // shift axis to bottom of back wall
  
  // flip y axis
  scale(1, -1, 1);
  
  // draw origin box
  noFill();
  stroke(0);
  box(5);

  // +X axis in Red
  stroke(255, 0, 0);
  line(0, 0, 0, 200, 0, 0);
  // +Y axis in Green
  stroke(0, 255, 0);
  line(0, 0, 0, 0, 200, 0);
  // +Z axis in Blue
  stroke(0, 0, 255);
  line(0, 0, 0, 0, 0, 200);

  stroke(0);
  fill(255);

  pushMatrix();
  scale(1, -1, -1);
  // translate(leapWorld.x / 2, leapWorld.y / 2, leapWorld.z / 2);
  // width height
  // translate(0, 0, 0);

  ArrayList<Hand> hands = leap.getHands();

  // for drawing spheres in space
  sphereDetail(8);
  noStroke();
  fill(255);
  // System.out.println("Frame");
  for (Hand hand : hands) {
    PVector handPosition = hand.getPosition();
    System.out.println("handPosition.x: " + handPosition.x);
    System.out.println("handPosition.y: " + handPosition.y);
    System.out.println("handPosition.z: " + handPosition.z);
    // System.out.println("Hand.x: " + hand.getId());
    // System.out.println("Hand: " + hand.getId());
    for (Finger finger : hand.getFingers()) {
      // System.out.println("Finger: " + finger.getType());
      for (int i = 0; i < 4; i++) {
        // finger 0 is thumb, bone 3 on thumb is not real
        Bone bone = finger.getBone(i);
        // System.out.println("Bone: " + bone.getType());
        PVector prevBone = bone.getPrevJoint();
        PVector nextBone = bone.getNextJoint();
        pushMatrix();
        translate(nextBone.x, nextBone.y, nextBone.z * 6);
        sphere(15);
        popMatrix();
        // System.out.println("prevBone.x: " + prevBone.x);
        // System.out.println("prevBone.y: " + prevBone.y);
        // System.out.println("prevBone.z: " + prevBone.z);
      }
    }
  }
  popMatrix();
  // int fps = leap.getFrameRate();

  // get hands
  
  // collisions

  // physics

  // send updates to glove

  // draw hands

  // draw objects
}
/*
void draw() {
  lights();
  background(0);
  float cameraY = height/2.0;
  float fov = float(width) * PI/2;
  float cameraZ = cameraY / tan(fov / 2.0);
  float aspect = float(width)/float(height);
  perspective(fov, aspect, cameraZ/10.0, cameraZ * 10.0);
  translate(width/2, height/2, 0);
  box(45);
  // ...
  int fps = leap.getFrameRate();


  // ========= HANDS =========

  for (Hand hand : leap.getHands ()) {


    // ----- BASICS -----

    int     hand_id          = hand.getId();
    PVector hand_position    = hand.getPosition();
    PVector hand_stabilized  = hand.getStabilizedPosition();
    PVector hand_direction   = hand.getDirection();
    PVector hand_dynamics    = hand.getDynamics();
    float   hand_roll        = hand.getRoll();
    float   hand_pitch       = hand.getPitch();
    float   hand_yaw         = hand.getYaw();
    boolean hand_is_left     = hand.isLeft();
    boolean hand_is_right    = hand.isRight();
    float   hand_grab        = hand.getGrabStrength();
    float   hand_pinch       = hand.getPinchStrength();
    float   hand_time        = hand.getTimeVisible();
    PVector sphere_position  = hand.getSpherePosition();
    float   sphere_radius    = hand.getSphereRadius();


    // ----- SPECIFIC FINGER -----

    Finger  finger_thumb     = hand.getThumb();
    // or                      hand.getFinger("thumb");
    // or                      hand.getFinger(0);

    Finger  finger_index     = hand.getIndexFinger();
    // or                      hand.getFinger("index");
    // or                      hand.getFinger(1);

    Finger  finger_middle    = hand.getMiddleFinger();
    // or                      hand.getFinger("middle");
    // or                      hand.getFinger(2);

    Finger  finger_ring      = hand.getRingFinger();
    // or                      hand.getFinger("ring");
    // or                      hand.getFinger(3);

    Finger  finger_pink      = hand.getPinkyFinger();
    // or                      hand.getFinger("pinky");
    // or                      hand.getFinger(4);        


    // ----- DRAWING -----

    hand.draw();
    // hand.drawSphere();


    // ========= ARM =========

    if (hand.hasArm()) {
      Arm     arm               = hand.getArm();
      float   arm_width         = arm.getWidth();
      PVector arm_wrist_pos     = arm.getWristPosition();
      PVector arm_elbow_pos     = arm.getElbowPosition();
    }


    // ========= FINGERS =========

    for (Finger finger : hand.getFingers()) {
      // Alternatives:
      // hand.getOutstrechtedFingers();
      // hand.getOutstrechtedFingersByAngle();

      // ----- BASICS -----

      int     finger_id         = finger.getId();
      PVector finger_position   = finger.getPosition();
      PVector finger_stabilized = finger.getStabilizedPosition();
      PVector finger_velocity   = finger.getVelocity();
      PVector finger_direction  = finger.getDirection();
      float   finger_time       = finger.getTimeVisible();


      // ----- SPECIFIC FINGER -----

      switch(finger.getType()) {
      case 0:
        // System.out.println("thumb");
        break;
      case 1:
        // System.out.println("index");
        break;
      case 2:
        // System.out.println("middle");
        break;
      case 3:
        // System.out.println("ring");
        break;
      case 4:
        // System.out.println("pinky");
        break;
      }


      // ----- SPECIFIC BONE -----

      Bone    bone_distal       = finger.getDistalBone();
      // or                       finger.get("distal");
      // or                       finger.getBone(0);

      Bone    bone_intermediate = finger.getIntermediateBone();
      // or                       finger.get("intermediate");
      // or                       finger.getBone(1);

      Bone    bone_proximal     = finger.getProximalBone();
      // or                       finger.get("proximal");
      // or                       finger.getBone(2);

      Bone    bone_metacarpal   = finger.getMetacarpalBone();
      // or                       finger.get("metacarpal");
      // or                       finger.getBone(3);


      // ----- DRAWING -----

      // finger.draw(); // = drawLines()+drawJoints()
      // finger.drawLines();
      // finger.drawJoints();


      // ----- TOUCH EMULATION -----

      int     touch_zone        = finger.getTouchZone();
      float   touch_distance    = finger.getTouchDistance();

      switch(touch_zone) {
      case -1: // None
        break;
      case 0: // Hovering
        // println("Hovering (#"+finger_id+"): "+touch_distance);
        break;
      case 1: // Touching
        // println("Touching (#"+finger_id+")");
        break;
      }
    }


    // ========= TOOLS =========

    for (Tool tool : hand.getTools ()) {


      // ----- BASICS -----

      int     tool_id           = tool.getId();
      PVector tool_position     = tool.getPosition();
      PVector tool_stabilized   = tool.getStabilizedPosition();
      PVector tool_velocity     = tool.getVelocity();
      PVector tool_direction    = tool.getDirection();
      float   tool_time         = tool.getTimeVisible();


      // ----- DRAWING -----

      // tool.draw();


      // ----- TOUCH EMULATION -----

      int     touch_zone        = tool.getTouchZone();
      float   touch_distance    = tool.getTouchDistance();

      switch(touch_zone) {
      case -1: // None
        break;
      case 0: // Hovering
        // println("Hovering (#"+tool_id+"): "+touch_distance);
        break;
      case 1: // Touching
        // println("Touching (#"+tool_id+")");
        break;
      }
    }
  }


  // ========= DEVICES =========

  for (Device device : leap.getDevices ()) {
    float device_horizontal_view_angle = device.getHorizontalViewAngle();
    float device_verical_view_angle = device.getVerticalViewAngle();
    float device_range = device.getRange();
  }
}

// ========= CALLBACKS =========

void leapOnInit() {
  // println("Leap Motion Init");
}
void leapOnConnect() {
  // println("Leap Motion Connect");
}
void leapOnFrame() {
  // println("Leap Motion Frame");
}
void leapOnDisconnect() {
  // println("Leap Motion Disconnect");
}
void leapOnExit() {
  // println("Leap Motion Exit");
}
//*/