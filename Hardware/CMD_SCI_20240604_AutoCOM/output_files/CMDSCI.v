/*
Uart_interval(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.wen(wen)
    ,.din(din)
    ,.ENABLE(ENABLE)
    ,.INTERVAL(INTERVAL)
);

*/

module Uart_interval(
    input       clk
    ,input      rst_n
    ,input      wen
    ,input      [7:0]din
    ,output     reg ENABLE
    ,output     reg [15:0]INTERVAl
);
reg [39:0]  shift;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        shift <= 40'b0;
    end
    else begin
        if (wen) begin
            shift <= {shift [31:0],din};
        end
        else begin
            shift <= shift;
        end
    end
end
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        ENABLE <= 1'b0;
        INTERVAl <= 16'h0000;
    end
    else begin
        casez (shift)
        {8'hEE,8'hDD,8'hzz,8'hzz,8'hCC}    :begin
            ENABLE <= 1'b1;
            INTERVAl <= shift[23:8];
        end
        {8'hEE,8'hDD,8'hzz,8'hzz,8'hBB}    :begin
            ENABLE <= 1'b0;
            INTERVAl <= 16'h0000;
        end
        default:begin
            ENABLE <= ENABLE;
            INTERVAl <= INTERVAl;
        end
        endcase
    end
end
endmodule