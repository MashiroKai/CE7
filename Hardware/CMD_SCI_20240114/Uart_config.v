/*

Uart_config config(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.wen(wen)
    ,.din(din)//8bits
    ,.parity(parity)
    ,.stopbit(stopbit)//2bits
)

*/

module Uart_config(
    input       clk,
    input       rst_n,
    input       wen,
    input       [7:0]din,
    output      reg  parity,
    output      reg  [1:0]stopbit,
    output      reg  [31:0]INTERVAL
);
reg [71:0]  shift;// 3 heads 2 config 4 interval =  9bytes 
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        shift <= 72'b0;
    end
    else begin
        if (wen) begin
            shift <= {shift [63:0],din};
        end
        else begin
            shift <= shift;
        end
    end
end
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        parity <= 1'b0;//odd
        stopbit <= 2'b11;//2 stop bit(s)
        INTERVAL <= 32'b0;
    end
    else begin
        case (shift[71:32])
        {8'hEE,8'hDD,8'hCC,8'h00,8'h1}    :begin
            parity <= 1'b0;//odd
            stopbit <= 2'b11;//2 stop bit(s)
            INTERVAL <= shift[31:0];
        end
        {8'hEE,8'hDD,8'hCC,8'h00,8'h2}    :begin
            parity <= 1'b0;
            stopbit <= 2'b10;
            INTERVAL <= shift[31:0];
        end
        {8'hEE,8'hDD,8'hCC,8'h00,8'h3}    :begin
            parity <= 1'b0;
            stopbit <= 2'b01;
            INTERVAL <= shift[31:0];
        end
        {8'hEE,8'hDD,8'hCC,8'h00,8'h4}    :begin
            parity <= 1'b0;
            stopbit <= 2'b00;
            INTERVAL <= shift[31:0];
        end 
        {8'hEE,8'hDD,8'hCC,8'h01,8'h1}    :begin
            parity <= 1'b1;
            stopbit <= 2'b11;
            INTERVAL <= shift[31:0];
        end
        {8'hEE,8'hDD,8'hCC,8'h01,8'h2}    :begin
            parity <= 1'b1;
            stopbit <= 2'b10;
            INTERVAL <= shift[31:0];
        end
        {8'hEE,8'hDD,8'hCC,8'h01,8'h3}    :begin
            parity <= 1'b1;
            stopbit <= 2'b01;
            INTERVAL <= shift[31:0];
        end
        {8'hEE,8'hDD,8'hCC,8'h01,8'h4}    :begin
            parity <= 1'b1;
            stopbit <= 2'b00;
            INTERVAL <= shift[31:0];
        end 
        default:begin
            parity <= parity;
            stopbit <= stopbit;
            INTERVAL <= INTERVAL;
        end
        endcase
    end
end
endmodule