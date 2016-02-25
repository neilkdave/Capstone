import com.leapmotion.leap.*;


boolean fullScreenApp = false;
Controller controller = new Controller();
Communicator communicator = new Communicator();
InputHandler inputHandler = new InputHandler(communicator);

void settings() {
  if (fullScreenApp) {
    fullScreen(P3D);
  } 
  else {
    size(800, 450, P3D);
  }
}

void setup() {
  // Setup things here
}

void keyPressed() {
  inputHandler.handleKeystroke(key);
}

//*
void draw() {
  ambientLight(102, 102, 102);
  directionalLight(126, 126, 126, 0, height / 2, height / 2);
  background(0);
  
  Frame frame = controller.frame();
  
  float cameraY = height / 2.0;
  float fov = PI/4;
  float cameraZ = cameraY / tan(fov / 2.0);
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
  
  // proportional to window width so we scale hands the same amount based on window size
  // height doesn't matter because gravity will bring objects down as needed
  // default interaction box is 235 mm wide
  float handScaler = width / 470;


  int handSphereSize = width / 100;
  int handSphereDetail = 12;
  int handCylinderSize = (handSphereSize * 2) / 3;
  int handCylinderDetail = 12;
  

  // offsets are in millimeters
  // do not move x axis
  // move y axis down so we can reach the floor of the box
  // move z axis forward so the zero is in the center of the box
  PVector leapOffset = new PVector(0, -150, 100);

  PVector lastHandJoint = null;
  PVector firstHandJoint = null;
  sphereDetail(handSphereDetail);
  noStroke();
  fill(255);
  HandList hands = frame.hands();
  for (Hand hand : hands) {
    lastHandJoint = null;
    for (Finger finger : hand.fingers()) {
      for(Bone.Type boneType : Bone.Type.values()) {
        Bone bone = finger.bone(boneType);
        PVector prevBone = PVector.mult(PVector.add(vectorToPVector(bone.prevJoint()), leapOffset), handScaler);
        PVector nextBone = PVector.mult(PVector.add(vectorToPVector(bone.nextJoint()), leapOffset), handScaler);
        if ((boneTypeToInt(boneType) - parseInt(fingerTypeToInt(finger.type()) == 0)) <= 2) {
          // TODO: change to something related to window width
          drawCylinder(prevBone, nextBone, handCylinderSize, handCylinderDetail); // p1, p2, radius, detail
        }
        if ((boneTypeToInt(boneType) - parseInt(fingerTypeToInt(finger.type()) == 0)) <= 3) {
          pushMatrix();
          translate(nextBone.x, nextBone.y, nextBone.z);
          sphere(handSphereSize); // TODO: change to something related to window width
          popMatrix();
        }
        if ((boneTypeToInt(boneType)) == 3) {
          if (lastHandJoint != null) {
            drawCylinder(lastHandJoint, nextBone, handCylinderSize, handCylinderDetail);
          }
          if (fingerTypeToInt(finger.type()) == 0) {
            firstHandJoint = nextBone;
          }
          lastHandJoint = nextBone;
        }
        if (((fingerTypeToInt(finger.type())) == 4) && (boneTypeToInt(boneType) == 3)) {
          drawCylinder(prevBone, nextBone, handCylinderSize, handCylinderDetail);
          drawCylinder(firstHandJoint, prevBone, handCylinderSize, handCylinderDetail);pushMatrix();
          translate(prevBone.x, prevBone.y, prevBone.z);
          sphere(handSphereSize); // TODO: change to something related to window width
          popMatrix();
        }
      }
    }
  }

  popMatrix();
}

PVector vectorToPVector(Vector vector) {
  return new PVector(vector.getX(), vector.getY(), vector.getZ());
}

int boneTypeToInt(Bone.Type boneType) {
  switch (boneType.toString()) {
    case "TYPE_DISTAL":
        return 0;
    case "TYPE_INTERMEDIATE":
        return 1;
    case "TYPE_PROXIMAL":
        return 2;
    case "TYPE_METACARPAL":
        return 3;
  }
  return -1;
}

int fingerTypeToInt(Finger.Type fingerType) {
  switch (fingerType.toString()) {
    case "TYPE_THUMB":
        return 0;
    case "TYPE_INDEX":
        return 1;
    case "TYPE_MIDDLE":
        return 2;
    case "TYPE_RING":
        return 3;
    case "TYPE_PINKY":
        return 4;
  }
  return -1;
}

void drawCylinder(PVector p1, PVector p2, int cylinderRadius, int cylinderDetail) {
  PVector pTranslated = PVector.sub(p2, p1);
  float cylinderLength = pTranslated.mag();
  float stepSize = TWO_PI / cylinderDetail;
  pushMatrix();
  
  translate(p1.x, p1.y, p1.z);
  rotateY(-atan2((pTranslated.z), (pTranslated.x)));
  rotateZ(atan2((pTranslated.y), sqrt(pow(pTranslated.x, 2) + pow(pTranslated.z, 2))));
  // stroke(0);
  // line(0, 0, 0, cylinderLength, 0, 0);
  
  beginShape(TRIANGLE_STRIP);
  for (int i = 0; i <= cylinderDetail; i++) {
    float y = cos(i * stepSize) * cylinderRadius;
    float z = sin(i * stepSize) * cylinderRadius;
    vertex(0, y, z);
    vertex(cylinderLength, y, z);
  }
  endShape(CLOSE);
  
  popMatrix();
  // noStroke();
}