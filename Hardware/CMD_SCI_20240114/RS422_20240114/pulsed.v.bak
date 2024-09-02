module pulsed #(
    parameter   WIDTH = 5
)
(
    input                clk
    ,input               rst_n
    ,input   [WIDTH-1:0] signal
    ,output  [WIDTH-1:0] pos
    ,output  [WIDTH-1:0] neg
);
reg [WIDTH-1:0]temp_p,temp_n;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        temp_p <= 1'b1;
        temp_n <= 1'b0;
    end
    else begin
        temp_p <= signal;
        temp_n <= signal;
    end
end
assign pos= (~temp_p)&signal;
assign neg= temp_n&(~signal);
endmodule
