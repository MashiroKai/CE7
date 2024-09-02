/*
SENDCTRL S1(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.W_FLAG(W_FLAG)
    ,.DAQ(DAQ)
    ,.COMPLE(COMPLE)
    ,.READY(READY)
    ,.EMPTY1(EMPTY1)
    ,.EMPTY2(EMPTY2)
    ,.LOAD1(LOAD1)
    ,.LOAD2(LOAD2)
    ,.SEL(SEL)
);

*/
module SENDCTRL(
    input       clk
    ,input      rst_n
    ,input      W_FLAG
    ,input      DAQ
    ,input      COMPLE
    ,input      READY
    ,input      EMPTY1
    ,input      EMPTY2
    ,output     reg LOAD1
    ,output     reg LOAD2
    ,output     SEL
);
assign SEL = ~W_FLAG;
reg [4:0]STATE;
localparam  ST01 = 5'b00001;
localparam  ST02 = 5'b00010;
localparam  ST03 = 5'b00100;
localparam  ST04 = 5'b01000;
localparam  ST05 = 5'b10000;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        STATE <= ST01;
    end
    else begin
        case (STATE)
        ST01    :begin
                if (DAQ) begin
                    STATE <= ST02;
                end
        end 
        ST02    :begin
                if (COMPLE) begin
                    STATE <= ST03;
                end
        end
        ST03    :begin
                if (SEL) begin
                    if (LOAD2) begin
                        LOAD2 <= 1'b0;
                    end
                    else begin
                    if (!EMPTY2&&READY) begin
                        LOAD2 <= 1'b1;
                    end
                    end
                    if (EMPTY2) begin
                        STATE <= ST01;
                    end
                end
                else begin
                    if (LOAD1) begin
                        LOAD1 <= 1'b0;
                    end
                    else begin
                    if (!EMPTY1&&READY) begin
                        LOAD1 <= 1'b1;
                    end
                    end
                    if (EMPTY1) begin
                        STATE <= ST01;
                    end
                end
        end
        endcase
    end
end
endmodule