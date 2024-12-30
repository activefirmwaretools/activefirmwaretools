
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
// Generated on 11/22/2024 at 13:40
// Component: swapdetect
module swapdetect (
	output reg swapout,
	input   sig1,
	input   sig2,
    input   clock
);

//`#start body` -- edit after this line, do not edit this line
reg toggle1;
reg toggle2;

    always @ (posedge sig1)
    begin
        toggle1 <= !toggle1;
    end
    
    always @ (posedge sig2)
    begin
        toggle2 <= !toggle2;
    end
    
reg old_sig11;
reg old_sig21;
reg toggled1a;
reg toggled1b;
reg toggled1c;
reg toggled2a;
reg toggled2b;
reg toggled2c;

    // Count the edges of each signal.  Swap if the second line has more toggles
    always @ (posedge clock)
    begin
    
        old_sig11 <= toggle1;
        old_sig21 <= toggle2;
        
        if ((old_sig11 != toggle1) && (old_sig21 == toggle2))
        begin
            toggled2a <= 0;
            toggled2b <= 0;
            toggled2c <= 0;
            toggled1a <= 1;
            if (toggled1a) toggled1b <= 1;
            if (toggled1b) toggled1c <= 1;
            if (toggled1c)   // Channel 1 is the clock
                swapout <= 0;
        end
        else if ((old_sig11 == toggle1) && (old_sig21 != toggle2))
        begin
            toggled1a <= 0;
            toggled1b <= 0;
            toggled1c <= 0;
            toggled2a <= 1;
            if (toggled2a) toggled2b <= 1;
            if (toggled2b) toggled2c <= 1;
            if (toggled2c)   // Channel 2 is the clock
                swapout <= 1;
        end
        else if ((old_sig11 != toggle1) && (old_sig21 != toggle2))
        begin
            toggled1a <= 0;
            toggled1b <= 0;
            toggled1c <= 0;
            toggled2a <= 0;
            toggled2b <= 0;
            toggled2c <= 0;

        end

    end    
//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
