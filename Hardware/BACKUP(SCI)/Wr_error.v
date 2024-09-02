/*
Wr_error w1(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.HCE_SUM(HCE_SUM)
    ,.PIE_SUM(PIE_SUM)
    ,.ITE_SUM(ITE_SUM)
    ,.CSE_SUM(CSE_SUM)
    ,.LCE_SUM(LCE_SUM)
    ,.BIE_SUM(BIE_SUM)
    ,.IIE_SUM(IIE_SUM)
    ,.RIE_SUM(RIE_SUM)
    ,.ECE_SUM(ECE_SUM)
    ,.CHE_SUM(CHE_SUM)
    ,.STP_SUM(STP_SUM)
    ,.EN(EN)
    ,.VALID(VALID)
    ,.DOUT(DOUT)
    ,.COMPLE(COMPLE)
);
*/


module Wr_error (
    input       clk,
    input       rst_n,
    input   [7:0]HCE_SUM,PIE_SUM,ITE_SUM,CSE_SUM,LCE_SUM,BIE_SUM,IIE_SUM,RIE_SUM,ECE_SUM,CHE_SUM,STP_SUM,
    input   EN,
    output  reg VALID,
    output  reg [7:0]DOUT,
    output  reg COMPLE
);
reg [87:0]SHIFT;
reg [4:0]STATE;
reg [3:0]CNT;
reg [31:0]PKGHEAD;
localparam  ST01 = 5'b00001;
localparam  ST02 = 5'b00010;
localparam  ST03 = 5'b00100;
localparam  ST04 = 5'b01000;
localparam  ST05 = 5'b10000;

initial begin
    PKGHEAD = {8'h55,8'h04,8'h90,8'hEB};
end
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        SHIFT <= 88'b0;
        STATE <= ST01;
        CNT <= 4'b0;
        VALID <= 1'b0;
        COMPLE <= 1'b0;
    end
    else begin
        case (STATE)
        ST01    :begin
            COMPLE <= 1'b0;
            SHIFT <= {HCE_SUM,PIE_SUM,ITE_SUM,CSE_SUM,LCE_SUM,BIE_SUM,IIE_SUM,RIE_SUM,ECE_SUM,CHE_SUM,STP_SUM};
            if(EN) STATE <= ST02;
            VALID <= 1'b0;
        end 
        ST02    :begin
            PKGHEAD <= {PKGHEAD[7:0],PKGHEAD[31:8]};
            DOUT  <= PKGHEAD[7:0];
            VALID <= 1'b1;
            CNT <= CNT+1'b1;
            if(CNT == 4'D3) STATE <= ST03;
        end
        ST03    :begin
            SHIFT <= {SHIFT[7:0],SHIFT[87:8]};
            DOUT <= SHIFT[7:0];
            VALID <= 1'b1;
            CNT <= CNT+1'b1;
            if(CNT == 4'D14) begin
                COMPLE <= 1'b1;
                STATE <= ST01;
                CNT   <= 4'b0;
            end
        end
        endcase
    end
end   
endmodule