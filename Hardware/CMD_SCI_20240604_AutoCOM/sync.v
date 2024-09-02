/*
sync#(
    .WIDTH(5)
    ,.CYCLE(2)
) 
s1(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.d(d)//width
    ,.q(q)
);
*/


module sync#(
    parameter   WIDTH = 5,
    parameter   CYCLE = 3
)(
        input       clk
        ,input      rst_n
        ,input      [WIDTH-1:0]d
        ,output     [WIDTH-1:0]q
);
reg [(CYCLE*WIDTH)-1:0]temp;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        temp <= 0;
    end
    else begin
        temp <= {temp[(CYCLE)*WIDTH-1:0],d};
    end
end
assign q = temp[(CYCLE*WIDTH-1):(CYCLE-1)*WIDTH];
endmodule