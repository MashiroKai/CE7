/*
PINGPONGCTRL P1(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.RECORD(RECORD)
    ,.New_instruction(New_instruction)
    ,.VALID(VALID)
    ,.COMPLE(COMPLE)
    ,.RX_DATA(RX_DATA)
    ,.EN(EN)
    ,.ERR_VALID(ERR_VALID)
    ,.ERR_DIN(ERR_DIN)
    ,.FIFO_VALID_IN_1(FIFO_VALID_IN_1)
    ,.DOUT1(DOUT1)
    ,.FIFO_VALID_IN_2(FIFO_VALID_IN_2)
    ,.DOUT2(DOUT2)
    ,.W_FLAG(W_FLAG)
);
*/
module PINGPONGCTRL (
    input       clk,
    input       rst_n,
    input       RECORD,
    input       New_instruction,
    input       End_instruction,
    input       VALID,
    input       COMPLE,
    input       [7:0]RX_DATA,
    output      reg EN,
    input       ERR_VALID,
    input       [7:0]ERR_DIN,
    //input       COMPLE,
    output       FIFO_VALID_IN_1,
    output       [7:0]DOUT1,
    output       FIFO_VALID_IN_2,
    output       [7:0]DOUT2,
    output      reg W_FLAG
);
reg [3:0]STATE;
localparam  ST01 = 5'b00001;
localparam  ST02 = 5'b00010;
localparam  ST03 = 5'b00100;
localparam  ST04 = 5'b01000;
localparam  ST05 = 5'b10000;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        STATE <= ST01;
        W_FLAG <= 1'b0;
    end
    else begin
        case (STATE)
        ST01    :begin
            if (RECORD) begin
                STATE <= ST02;
            end
        end 
        ST02    :begin //
            if (RECORD) begin
                STATE <= ST03;
            end
        end
        ST03    :begin //W1_R2
            if(New_instruction||End_instruction)begin
                STATE <= ST04;
                W_FLAG <= W_FLAG + 1'b1;
                EN <= 1'b1;
            end
        end
        ST04    :begin  
            EN <= 1'b0;
           // if (COMPLE) begin
                STATE <= ST02;
           // end
        end
        endcase
    end
end
assign  DOUT1 = W_FLAG ? ERR_DIN : RX_DATA;
assign  DOUT2 = W_FLAG ? RX_DATA : ERR_DIN;
assign  FIFO_VALID_IN_1 = W_FLAG ? ERR_VALID : VALID;
assign  FIFO_VALID_IN_2 = W_FLAG ? VALID : ERR_VALID;


endmodule