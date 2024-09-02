/*
Uart_rst_n test(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.valid(valid)
    ,.din(din)
    ,.reset_n(reset_n)
);
*/
//when receive AA BB CC DD EE through UART ,generate negative pulse  reset_n ;
module Uart_rst_n(
            input       clk
            ,input      rst_n
            ,input      valid
            ,input      [7:0]din
            ,output     reset_n // must connect to module "pulsed" 
);
wire neg;
assign reset_n = ~neg;
reg rst_n_r;
reg [39:0]shift;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        shift <= 40'b0;
    end
    else begin
        if (valid) begin
            shift <= {shift[31:0],din};
        end
        else begin
            shift <= shift;
        end
    end
end
always @(*) begin
    if (!rst_n) begin
        rst_n_r = 1'b1;
    end
    else begin
        case (shift)
        {8'hAA,8'hBB,8'hCC,8'hDD,8'hEE}    : begin
            rst_n_r = 1'b0;
        end
            default: rst_n_r = 1'b1;
        endcase
    end
end
pulsed_reg #(
    .WIDTH(1)
)
p1(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.signal(rst_n_r)
    ,.neg(neg)
);
endmodule