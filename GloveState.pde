public class GloveState {

  private int gloveSize;
  private int[] pouches;

  public GloveState(int gloveSize) {
    this.gloveSize = gloveSize;
    this.pouches = new int[gloveSize];
  }

  public void setPouch(int pouch, int pressure) {
    if ((0 <= pouch) && (pouch < gloveSize)) {
      if (pressure < 0) {
        println("Bad pressure!");
        pressure = 0;
      }
      if (15 < pressure) {
        println("Bad pressure!");
        pressure = 15;
      }
      println("Inflating pouch " + pouch + " to " + pressure);
      pouches[pouch] = pressure;
    }
  }

  public String getStateString() {
    String returnString = "";
    for (int i = 0; i < gloveSize; i++) {
      returnString += intToHexString(pouches[i]);
    }
    return returnString;
  }

  private String intToHexString(int number) {
    String returnString = "";
    int digit;
    if (number == 0) {
      return "0";
    }
    while (0 < number) {
      digit = number % 16;
      if (digit < 10) {
        returnString = digit + returnString;
      }
      else {
        switch (digit) {
          case 10 :
            returnString = "a" + returnString;
          break;
          case 11 :
            returnString = "b" + returnString;
          break;
          case 12 :
            returnString = "c" + returnString;
          break;
          case 13 :
            returnString = "d" + returnString;
          break;
          case 14 :
            returnString = "e" + returnString;
          break;
          case 15 :
            returnString = "f" + returnString;
          break;
        }  
      }
      number = number / 16;
    }
    return returnString;
  }
}