
//`#start header` -- edit after this line, do not edit this line
// ========================================
//
// Copyright YOUR COMPANY, THE YEAR
// All Rights Reserved
// UNPUBLISHED, LICENSED SOFTWARE.
//
// CONFIDENTIAL AND PROPRIETARY INFORMATION
// WHICH IS THE PROPERTY OF your company.
//
// ========================================
`include "cypress.v"
//`#end` -- edit above this line, do not edit this line
// Generated on 08/26/2016 at 13:01
// Component: ACTIVEInput_v1_3
module ACTIVEInput_v1_3 (
	output reg [7:0] DATAOUT,
	output reg TIMESTAMP,
	output reg OE,
	input   CLK,
	input   RESET,
	input   clock,
	input   DAT,
    output  reg DEBUG
);

//'#start body` -- edit after this line, do not edit this line
    reg iOE;
    reg iOE1;
    reg iOE2;
    reg OE2;
    reg OE1;
    reg [7:0] tDATAOUT;
    reg [7:0] newDATAOUT;
    reg was7F;
    
    reg [3:0] count;
    
    always @ (posedge clock)
    begin
        iOE2 <= iOE1;
        iOE1 <= iOE;
        
        if (count)
        begin
            count <= count + 1;
            if (count == 4'h8)
            begin
                OE <= 01;
                DATAOUT <= newDATAOUT;
            end
            if (count == 4'hF)
                count <= 0;
        end
        else if (!iOE2 & iOE1)
        begin
            count <= 4'h1;
        end
        else
            OE <= 0;
    end
    
    reg [2:0] bits;
    
    always @ (posedge CLK)
    begin
                
        TIMESTAMP <= 0;

        if (RESET)
        begin
        
        end
        else 
        begin
            iOE <= 0;   
        
            if (bits == 7)
            begin
                iOE <= 1;
                newDATAOUT <= { tDATAOUT[6:0], DAT };
                
                if ({ tDATAOUT[6:0], DAT } == 8'h7F)
                begin
                    was7F <= 1;
                end
                else
                begin
                    was7F <= 0;
                    if (was7F)
                    begin
                        TIMESTAMP <= 1;
                        DEBUG <= !DEBUG;
                    end
                end
                    
            end
            
            tDATAOUT[7:0] <= { tDATAOUT[6:0], DAT };
            if ({ tDATAOUT[6:0], DAT } == 8'h7F)     // Timestamp this byte.  Also sets bit alignment
            begin
                bits <= 0;      // Next clock is bit 0
                iOE <= 0;        // Don't write the 7F
            end
            else
                bits <= bits + 1;

        end

    end

 
 
 
 
//    reg [2:0] bits;
//    reg old_CLK;
//    
//    reg DAT1;
//    reg DAT2;
//    reg CLK1;
//    reg CLK2;
//    
//    always @ (posedge clock)
//    begin
//                
//        DAT1 <= DAT;
//        DAT2 <= DAT1;
//          
//        CLK1 <= CLK;
//        CLK2 <= CLK1;
//          
//        TIMESTAMP <= 0;
//
//        if (RESET)
//        begin
//        
//        end
//        else if (!CLK2 & CLK1)    // Rising Edge of clock
//        begin
//            OE <= 0;   
//        
//            if (bits == 7)
//                OE <= 1;
//            
//            DATAOUT[7:0] <= { DATAOUT[6:0], DAT1 };
//            if ({ DATAOUT[6:0], DAT1 } == 8'h7F)     // Timestamp this byte.  Also sets bit alignment
//            begin
//                bits <= 0;      // Next clock is bit 0
//                TIMESTAMP <= 1;
//                OE <= 0;        // Don't write the 7F
//            end
//            else
//                bits <= bits + 1;
//
//        end
//
//    end


//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
