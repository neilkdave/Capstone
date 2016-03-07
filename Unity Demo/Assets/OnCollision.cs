using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.IO.Ports;
using Leap;

public class OnCollision : MonoBehaviour {
    //blic Text grabbedText;
    public int digitCollision = 0;

    void OnCollisionEnter(Collision collision)
    {
        digitCollision = 1;
       // grabbedText.text = "Grabbed: 1";
    }

    void OnCollisionStay(Collision collision)
    {
        digitCollision = 1;
        
    }

    void OnCollisionExit(Collision collision)
    {
        digitCollision = 0;
    }

 

}
