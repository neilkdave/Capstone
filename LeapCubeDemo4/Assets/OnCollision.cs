using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.IO.Ports;

public class OnCollision : MonoBehaviour {
    public Text grabbedText;

   

    void OnCollisionEnter(Collision collision)
    {
        SetText(1);
       // grabbedText.text = "Grabbed: 1";
    }
	
	void OnCollisionExit(Collision collision)
    {
        SetText(0);
    }

    void SetText(int input)
    {
        
         if(input ==1)
         { grabbedText.text = "Grabbed: Yes"; }
         else
         { grabbedText.text = "Grabbed: No"; }
   
    }

}
