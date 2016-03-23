using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.IO.Ports;
using Leap;

public class Serial_Comm : MonoBehaviour {

    public static Serial_Comm Instance;
    public int[] collisionArray = new int[17];
   
    public Text digitCollision;
    public SerialPort stream;

    string transmit;
    int count; 
    void Start () {
        Instance = this;
        count = 0;
        this.collisionArray[0] = 1;
        this.collisionArray[1] = 7;

        stream = new SerialPort("COM6", 9600);
        stream.ReadTimeout = 50;
        stream.Open();
	}
	
	// Update is called once per frame
	void Update () {
        for (int i = 0; i < 5/*collisionArray.Length*/; i++)
        {
            transmit +=  collisionArray[i].ToString();
        }
        count++;
     
      print(transmit);
        WriteToArduino(transmit);
        transmit = "";
       
        
    }
   

     void WriteToArduino(string message)
    {
        // Send the request
        stream.WriteLine(message);
        stream.BaseStream.Flush();
    }
}
