using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.IO.Ports;
using Leap;

public class OnCollision : MonoBehaviour {

    public int digitValue;
    
    void OnCollisionEnter(Collision collision)
    {
        Serial_Comm.Instance.collisionArray[digitValue + 1] = 8;
    }

    void OnCollisionStay(Collision collision)
    {
        Serial_Comm.Instance.collisionArray[digitValue + 1] = 8;
    }

    void OnCollisionExit(Collision collision)
    {
        Serial_Comm.Instance.collisionArray[digitValue + 1] = 0;
    }

}
