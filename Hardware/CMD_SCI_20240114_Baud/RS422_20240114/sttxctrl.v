module sttxctrl(
    input   empty
    ,input  ready
    ,output ren
);
assign ren = (!empty&&ready)?1'b1:1'b0; 
endmodule