The latest Teensyduino update brakes this durty patch.
So better way to apply patch:
1. Find Serial1.c in Teensyduino foldier
2. Open it via text editor and find following line:
   ```
   #define C2_TX_INACTIVE		C2_ENABLE
   ```
   Change it to the following:
   ```
   #define C2_TX_INACTIVE		UART_C2_RE | UART_C2_RIE | UART_C2_ILIE
   ```
4. Save the file

