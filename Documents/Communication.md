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
#### OpCodes
1. Set Inflation State
  * Payload Length: 9 Characters
  * Payload: Each bite represents the inflation state of a single pouch. 0 is deflated F is fully inflated.
```
   1  2  3  4
   |  |  |  |
   |  |  |  |
   5        6
0
 \
   7        8
```

2. System Reset
  * Payload Length: 0 Characters

3. Initialize System
  * Payload Length: 0 Characters

4. Set Parameter
  * Payload Length: ? Characters
  * Payload: Will contain parameter to set and value to set it to. Bad values handled on Glove