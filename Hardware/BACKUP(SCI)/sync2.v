// sync signal to different clock domain
module sync2 (
output reg q,
input d, clk, rst_n);
reg q1; // 1st stage ff output
always@(posedge clk or negedge rst_n)
if (!rst_n)
{q,q1} <= 2'b0;
else
{q,q1} <= {q1,d};
endmodule