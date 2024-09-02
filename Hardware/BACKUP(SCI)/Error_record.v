/*
Error_record E1(
    .clk(clk),
    .rst_n(rst_n),
    .HCE(HCE),
    .PIE(PIE),
    .ITE(ITE),
    .CSE(CSE),
    .LCE(LCE),
    .BIE(BIE),
    .IIE(IIE),
    .RIE(RIE),
    .ECE(ECE),
    .HCE_SUM(HCE_SUM),
    .PIE_SUM(PIE_SUM),
    .ITE_SUM(ITE_SUM),
    .CSE_SUM(CSE_SUM),
    .LCE_SUM(LCE_SUM),
    .BIE_SUM(BIE_SUM),
    .IIE_SUM(IIE_SUM),
    .RIE_SUM(RIE_SUM),
    .ECE_SUM(ECE_SUM),
    .CHE_SUM(CHE_SUM),
    .STP_SUM(STP_SUM)
);
wire [7:0]HCE_SUM,PIE_SUM,ITE_SUM,CSE_SUM,LCE_SUM,BIE_SUM,IIE_SUM,RIE_SUM,ECE_SUM;
*/
module Error_record(
    input   clk ,
    input   rst_n,
    input   HCE,PIE,ITE,CSE,LCE,BIE,IIE,RIE,ECE,CHECK,STOP,
    output  [7:0]HCE_SUM,PIE_SUM,ITE_SUM,CSE_SUM,LCE_SUM,BIE_SUM,IIE_SUM,RIE_SUM,ECE_SUM,CHE_SUM,STP_SUM
);
counter #(
    .RST(8'd255),
    .START(8'd0)
)
c11(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(1'b0)
    ,.en(STOP)
    ,.cnt(STP_SUM)
);
counter #(
    .RST(8'd255),
    .START(8'd0)
)
c10(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(1'b0)
    ,.en(CHECK)
    ,.cnt(CHE_SUM)
);
counter #(
    .RST(8'd255),
    .START(8'd0)
)
c9(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(1'b0)
    ,.en(ECE)
    ,.cnt(ECE_SUM)
);
counter #(
    .RST(8'd255),
    .START(8'd0)
)
c8(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(1'b0)
    ,.en(RIE)
    ,.cnt(RIE_SUM)
);
counter #(
    .RST(8'd255),
    .START(8'd0)
)
c7(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(1'b0)
    ,.en(IIE)
    ,.cnt(IIE_SUM)
);
counter #(
    .RST(8'd255),
    .START(8'd0)
)
c6(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(1'b0)
    ,.en(BIE)
    ,.cnt(BIE_SUM)
);
counter #(
    .RST(8'd255),
    .START(8'd0)
)
c5(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(1'b0)
    ,.en(LCE)
    ,.cnt(LCE_SUM)
);
counter #(
    .RST(8'd255),
    .START(8'd0)
)
c4(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(1'b0)
    ,.en(CSE)
    ,.cnt(CSE_SUM)
);
counter #(
    .RST(8'd255),
    .START(8'd0)
)
c3(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(1'b0)
    ,.en(ITE)
    ,.cnt(ITE_SUM)
);
counter #(
    .RST(8'd255),
    .START(8'd0)
)
c2(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(1'b0)
    ,.en(PIE)
    ,.cnt(PIE_SUM)
);
counter #(
    .RST(8'd255),
    .START(8'd0)
)
c1(
    .clk(clk)
    ,.rst_n(rst_n)
    ,.asyn(1'b0)
    ,.en(HCE)
    ,.cnt(HCE_SUM)
);
endmodule