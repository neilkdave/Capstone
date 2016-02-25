using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.IO.Ports;
using Leap;

public class Serial_Comm : MonoBehaviour {
   public Text countText;
  
   public SerialPort stream;
     
   private int count;
   
   
    // Use this for initialization
	void Start () {
        stream = new SerialPort("COM3", 9600);
        stream.ReadTimeout = 50;
        stream.Open();
     
        count = 0;
        SetText();
     //   SetGrabbedText();
	}
	
	// Update is called once per frame
	void Update () {
        
        count = count + 1;
        SetText();
        //stream.WriteLine(countText.text);
        if (count == 400)
        {
            WriteToArduino("PING");
        }
    }

    void SetText()
    {

        countText.text = "Count: " + count.ToString();
      
    }

    public void WriteToArduino(string message)
    {
        // Send the request
        stream.WriteLine(message);
        stream.BaseStream.Flush();
    }
}
