# Communication

## Serial Communication Structure
```
(OpCode)(Version)(Payload)
```
* OpCode: Single Hex Character (1-F, 0 reserved)
* Version: Single Hex Character (0-F)
* Payload: OpCode and Version Specific

## Versions
### Version 1
Our First Version! Likely unstable because we will do most of our development work here. Make sure to update to master for both Arduino and Processing.
#### OpCodes
1. Set Inflation State
  * 2 Bites: number of pouches
  * 1 Bite per pouch: Each bite represents the inflation state of a single pouch. 0 is deflated F is fully inflated.

2. System Reset
  * Payload Length: 0 Characters

3. Initialize System
  * Payload Length: 0 Characters

4. Set Parameter
  * Payload Length: ? Characters
  * Payload: Will contain parameter to set and value to set it to. Bad values handled on Glove

Pouch Numbering
```
    1   2   3   4

    14  15  16  17
   
    10  11  12  13
   
    5           6
0 

  9

    7           8
```