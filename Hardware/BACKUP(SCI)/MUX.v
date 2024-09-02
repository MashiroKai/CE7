module MUX (
    input   SEL
    ,input  FIFO_VALID_IN_1
    ,input  FIFO_VALID_IN_2
    ,input [7:0]FIFO_OUT1
    ,input [7:0]FIFO_OUT2
    ,output VALID
    ,output [7:0]TX_DATA
);
assign VALID = SEL?FIFO_VALID_IN_2:FIFO_VALID_IN_1;
assign TX_DATA = SEL?FIFO_OUT2:FIFO_OUT1;
endmodule