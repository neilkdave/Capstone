import processing.serial.*;

public class Communicator {

  // Protocol Version
  private static final String version = "1";

  // OpCodes
  private static final String inflationStateOpCode = "1";
  private static final String resetSystemOpCode = "2";
  private static final String initializeSystemOpCode = "3";
  private static final String setParameterOpCode = "4";

  public Communicator() {
    
  }

  public void sendInflationState(GloveState gs) {
    println("Sending: Inflation State");
    String message = inflationStateOpCode;
    // TODO: make this not hardcoded
    message += "09"; // number of pouches (hex);
    message += gs.getStateString();
    sendMessage(message);
  }

  public void sendResetSystem() {
    println("Sending: Reset System");
    String message = resetSystemOpCode;
    sendMessage(message);
  }

  public void sendInitializeSystem() {
    println("Sending: Initialize System");
    String message = initializeSystemOpCode;
    sendMessage(message);
  }

  public void setParameter() {
    println("Sending: Set Parameter");
    String message = setParameterOpCode;
    sendMessage(message);
  }

  private void sendMessage(String message) {
    message = version + message;
    println("Message: \"" + message + "\"");
  }

}