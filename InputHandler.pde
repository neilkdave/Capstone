public class InputHandler  {

  private GloveState keyboard;
  private GloveState virtual;
  private Communicator communicator;

  public InputHandler (Communicator communicator) {
    this.keyboard = new GloveState(9);
    this.virtual = new GloveState(9);
    this.communicator = communicator;
  }

  public void handleKeystroke(char keystroke) {
    switch (key + "") {
      case "1":
        keyboard.setPouch(0, 15);
        communicator.sendInflationState(keyboard);
        break;
      case "2":
        keyboard.setPouch(1, 15);
        communicator.sendInflationState(keyboard);
        break;
      case "3":
        keyboard.setPouch(2, 15);
        communicator.sendInflationState(keyboard);
        break;
      case "4":
        keyboard.setPouch(3, 15);
        communicator.sendInflationState(keyboard);
        break;
      case "5":
        keyboard.setPouch(4, 15);
        communicator.sendInflationState(keyboard);
        break;
      case "6":
        keyboard.setPouch(5, 15);
        communicator.sendInflationState(keyboard);
        break;
      case "7":
        keyboard.setPouch(6, 15);
        communicator.sendInflationState(keyboard);
        break;
      case "8":
        keyboard.setPouch(7, 15);
        communicator.sendInflationState(keyboard);
        break;
      case "9":
        keyboard.setPouch(8, 15);
        communicator.sendInflationState(keyboard);
        break;
      case "q":
        keyboard.setPouch(0, 0);
        communicator.sendInflationState(keyboard);
        break;
      case "w":
        keyboard.setPouch(1, 0);
        communicator.sendInflationState(keyboard);
        break;
      case "e":
        keyboard.setPouch(2, 0);
        communicator.sendInflationState(keyboard);
        break;
      case "r":
        keyboard.setPouch(3, 0);
        communicator.sendInflationState(keyboard);
        break;
      case "t":
        keyboard.setPouch(4, 0);
        communicator.sendInflationState(keyboard);
        break;
      case "y":
        keyboard.setPouch(5, 0);
        communicator.sendInflationState(keyboard);
        break;
      case "u":
        keyboard.setPouch(6, 0);
        communicator.sendInflationState(keyboard);
        break;
      case "i":
        keyboard.setPouch(7, 0);
        communicator.sendInflationState(keyboard);
        break;
      case "o":
        keyboard.setPouch(8, 0);
        communicator.sendInflationState(keyboard);
        break;
    }
  }
}