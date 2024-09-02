/*
DPRAM #(
    .WIDTH(4'd8),
    .DEPTH(10'd512)
)
ram1(
    .clk(clk),
    .ADDR(),
    .W_R(),
    .DIN(),
    .DOUT()
);
*/


module DPRAM(
    input       clk,
    input       [A-1:0]ADDR,
    input       W_R,//1 for wirte , 0 for read
    input       [WIDTH-1:0]DIN,
    output  reg [WIDTH-1:0]DOUT   
);
parameter    A = logb2(DEPTH);
parameter    WIDTH = 4'd8;
parameter    DEPTH = 10'd512;

reg [WIDTH-1:0]buffer[DEPTH-1:0];

always @(posedge clk) begin
    if (W_R) begin
        buffer[ADDR] <= DIN;
    end
    else
        DOUT <= buffer[ADDR];
end

function integer logb2;
    input [31:0]            value;
    reg   [31:0]            tmp;
begin
    tmp = value - 1;
    for (logb2 = 0; tmp > 0; logb2 = logb2 + 1) 
        tmp = tmp >> 1;
end
endfunction
endmodule