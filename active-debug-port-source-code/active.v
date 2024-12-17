//===================================================================================
// Active-Pro Debug Output Module
//
// This module takes a byte array input that starts and ends with a 0 (zero), 
// computes the length, and clocks out the Active Debug Port packet to send the 
// text message to the requested channel.
//===================================================================================

module ACTIVEPRO (

    // Reset and Clock
    input       RESET,
    input       SYS_CLOCK,               // Output clock is half this clock
    
    // Output - ACTIVE Clock and Data
    output reg  ACTIVE_DATA ,            // Active Debug Port Output Data
    output reg  ACTIVE_CLOCK,            // Active Debug Port Output Clock
    
    // Input - ACTIVE Debug Message
    input [63:0][7:0] ACTIVE_MESSAGE,    // 0 (zero) surrounded debug text message
    input [5:0] ACTIVE_CHANNEL,          // Active Debug Port channel to use
    input  ACTIVE_WR                     // Write strobe to send the above message to the above channel
    
    );
    
    reg [63:0][7:0] ACTIVE_MESSAGE_LATCHED; 
    reg [7:0] MESSAGE_LENGTH;
    
    reg [14:0] state;
    wire clockstate;
    wire [2:0] bitcount;
    wire [7:0] bytecount;
    
    assign clockstate = state[0];
    assign bitcount[2:0] = state[3:1];
    assign bytecount[7:0] = state[11:4];
    
    wire [7:0] FIRST_BYTE;
    assign FIRST_BYTE = 8'h7F;
    wire [7:0] SECOND_BYTE;
    assign SECOND_BYTE = (ACTIVE_CHANNEL & 8'h3F) | 8'h40;
    reg [7:0] clock_divider;
    
    always @ (posedge SYS_CLOCK)
    begin
    
        if (RESET)
        begin
            state <= 0;
            ACTIVE_CLOCK <= 0;
            ACTIVE_DATA <= 0;
        end
        else
        begin

            // Find out the length of this message by counting the bytes until we see a 0
            for (int x = 1; x < 64; x++)  // The first byte is a 0, used to terminate the string on the Active Bus
            begin
              if (ACTIVE_MESSAGE_LATCHED[x] == 8'h0)
                begin
                    MESSAGE_LENGTH <= x + 2;        // Include the 2 header bytes
                    break;
                end
            end
            
            if (state)
            begin
                state <= state + 1;
                
                if (clockstate == 1)   // Send out the bit
                begin
                    ACTIVE_CLOCK <= 0;
                        
                    if (bytecount == 0)
                        ACTIVE_DATA <= FIRST_BYTE[7-bitcount];
                    else if (bytecount == 1)
                        ACTIVE_DATA <= SECOND_BYTE[7-bitcount];
                    else
                        ACTIVE_DATA <= ACTIVE_MESSAGE_LATCHED[MESSAGE_LENGTH - bytecount - 1][7-bitcount];
                end
                else if (clockstate == 0)  ACTIVE_CLOCK <= 1;

                if (bytecount >= MESSAGE_LENGTH) state <= 0;

            end
            else        // Not sending out a message, see if we should send a new one
            begin
                if (ACTIVE_WR) 
                begin
                    state <= 1;     // Start clocking out the data
                    ACTIVE_MESSAGE_LATCHED <= ACTIVE_MESSAGE;
                end
            end
        end
        
    end
endmodule 
