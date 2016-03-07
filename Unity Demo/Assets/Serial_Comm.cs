using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.IO.Ports;
using Leap;

//public class Serial_Comm : MonoBehaviour {
    public class Serial_Comm : SkeletalHand
{

        public Text countDisplay;
       public Text digitCollision;
   public SerialPort stream;
     
   private int count;
   
   
    // Use this for initialization
	void Start () {
        stream = new SerialPort("COM3", 9600);
        stream.ReadTimeout = 50;
        stream.Open();
     
        count = 0;
        SetText();

	}
	
	// Update is called once per frame
	/*void Update () {
        UpdateCollisions();
        count = count + 1;
        SetText();
        //stream.WriteLine(countText.text);
        if (count == 400)
        {
            WriteToArduino("PING");
        }
    }*/
    void Update()
    {
        UpdateHand();
    }
    void SetText()
    {

        countDisplay.text = "Count: " + count.ToString();
      
    }

    public void WriteToArduino(string message)
    {
        // Send the request
        stream.WriteLine(message);
        stream.BaseStream.Flush();
    }
    public override void UpdateHand()
    {
        count = count + 1;
        SetText();
        int grabbedValue = 0;
        int index = 1;
        for (int f = 0; f < fingers.Length; ++f)
        {
            for (int i = 0; i < fingers[f].bones.Length; ++i)
            {
                if (fingers[f].bones[i] != null)
                {
                    //  CapsuleCollider capsule = fingers[f].bones[i].GetComponent<CapsuleCollider>();
                   Collider capsule = fingers[f].bones[i].GetComponent<Collider>();
                    if (capsule != null)
                   {

                    grabbedValue = grabbedValue + index;
                        /* // Initialization
                         capsule.direction = 2;
                         fingers[f].bones[i].localScale = new Vector3(1f, 1f, 1f);

                         // Update
                         capsule.radius = fingers[f].GetBoneWidth(i) / 2f;
                         capsule.height = fingers[f].GetBoneLength(i) + fingers[f].GetBoneWidth(i);*/
                   }

                    index++;
                
                }
            }
            digitCollision.text = "Collision:" + grabbedValue.ToString();
        }
       
    }
}
